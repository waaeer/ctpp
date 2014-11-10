/*-
 * Copyright (c) 2004 - 2011 CTPP Team
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the CTPP Team nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *      CTPP2Gettext.cpp
 *
 * $CTPP$
 */
#include "CTPP2GetText.hpp"
#include "CTPP2Exception.hpp"
#include "CTPP2SyscallFactory.hpp"
#include "STLFunctional.hpp"
#include "functions/FnGetText.hpp"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

#if defined _MSC_VER
#include "libgen_win32.h"
#else
#include <libgen.h>
#endif

#include <assert.h>

#define LE_MAGIC 0x950412DE
#define BE_MAGIC 0xDE120495

//#define _GETTEXT_PLURAL_DEBUG 1
//#define _GETTEXT_USE_REPORTER 1


/*
TernaryExpr    = OrExpr { "?" TernaryExpr ":" TernaryExpr }

OrExpr         = AndExpr { "||" OrExpr }

AndExpr        = EqExpr { "&&" AndExpr }

EqExpr         = LtOrGtExpr { EqRelation LtOrGtExpr }

LtOrGtExpr     = ModExpr { LtOrGtRelation ModExpr }

ModExpr        = Term { "%" Term }

Term           = number | "n" | "(" TernaryExpr ")" | TernaryExpr

EqRelation     = EQ | NE
LtOrGtRelation = GT | LT | GE | LE
*/

namespace CTPP // C++ Template Engine
{

#ifdef _GETTEXT_USE_REPORTER
/**
  @class Reporter
  @brief Print template parsing syntax tree with indents
*/
class Reporter
{
public:
	/**
	  @brief Constructor
	  @param sIData - method name
	  @param szCode - code to print
	*/
	Reporter(const STLW::string & sIName, const STLW::string & sData, STLW::string::size_type iPos);

	/**
	  @brief Destructor
	*/
	~Reporter() throw();
private:
	// Does not exist
	Reporter(const Reporter & oRhs);
	Reporter & operator=(const Reporter & oRhs);

	/** Method name   */
	STLW::string sName;
	/** Indent lebvel */
	static int iLevel;
};

//
// Constructor
//
Reporter::Reporter(const STLW::string & sIName, const STLW::string & sData, STLW::string::size_type iPos): sName(sIName)
{
	for (INT_32 iI = 0; iI < iLevel; ++iI) { fprintf(stdout, "    "); }
	++iLevel;
	fprintf(stdout, ">> %s: `%s`\n", sName.c_str(), sData.substr(iPos).c_str());
}

//
// Destructor
//
Reporter::~Reporter() throw() { --iLevel; fprintf(stdout, "<< %s\n", sName.c_str());  }

// Indent level
int Reporter::iLevel = 0;

// Fast access macros
#define REPORTER(x, y, z) Reporter oReporter((x), (y), (z));

#else // No reporter

// Fast access macros
#define REPORTER(x, y, z) { ; }

#endif // Reporter class


/**
  @class ScopeGuard
  @brief Deallocate memory of pointer when go out from scope
  @tparam type of pointer
*/
template<typename T>
class ScopeGuard
{
	/** Type of function, that deallocate memory */
	typedef void (*DeleteFuncType)(T *);

public:
	/**
	  @brief Constructor
	  @param p - pointer
	  @param func - function for deleting pointer
	*/
    ScopeGuard(T *p, DeleteFuncType func = DefaultDeleteFunc) :
        pData(p), deleteFunc(func) { ;; }

	/**
	  @brief Destructor
	*/
    ~ScopeGuard() throw() { if (pData) { deleteFunc(pData); } }

	/**
	  @brief Get pointer
	  @return pointer
    */
    T * Get() const { return pData; }

	/**
	  @brief Release pointer
	  @return pointer
	*/
    T * Release()
	{
        T * pTemp = pData;
        pData = NULL;
        return pTemp;
    }

private:
	// Does not exist
    ScopeGuard(const ScopeGuard &);
    ScopeGuard & operator=(const ScopeGuard &);

	/** Default function for deleting pointer */
    static void DefaultDeleteFunc(T * p) { delete p; }

	/** Pointer                               */
	T * pData;
	/** Function for deleting pointer         */
	DeleteFuncType deleteFunc;
};

//
// Deletes array
//
static void DeleteArrFunc(UCHAR_P pData) { delete[] pData; }

//
// Split string into vector of substrings by separator
//
static void Split(const STLW::string & sStr, CHAR_8 cSep, STLW::vector<STLW::string> & vResult)
{
    STLW::string::size_type iPrevPos = 0;
    STLW::string::size_type iPos = 0;

    while ((iPos = sStr.find(cSep, iPos)) != STLW::string::npos)
	{
        STLW::string sSubstring(sStr.substr(iPrevPos, iPos - iPrevPos));
        vResult.push_back(sSubstring);
        iPrevPos = ++iPos;
    }
	STLW::string sSubstr(sStr.substr(iPrevPos, iPos - iPrevPos));
    vResult.push_back(sSubstr);
}

//
// Remove trailing and leading spaces from string
//
static void Trim(STLW::string & s)
{
	s.erase(
		STLW::find_if(s.rbegin(), s.rend(), STLW::not1(STLW::ptr_fun<INT_32, INT_32>(::isspace))).base(),
		s.end());

	s.erase(
		s.begin(),
		STLW::find_if(s.begin(), s.end(), STLW::not1(STLW::ptr_fun<INT_32, INT_32>(::isspace))));
}

//
// A constructor
//
CTPP2GetText::CTPP2GetText() { sDefaultDomain.assign("default"); }

//
// A destructor
//
CTPP2GetText::~CTPP2GetText() throw() { ;; }

//
// Add translation
//
void CTPP2GetText::AddTranslation(const STLW::string & sFileName, const STLW::string & sDomain, const STLW::string & sLang)
{
	UINT_32 iLength = 0;

    ScopeGuard<UCHAR_8> pData(ReadFile(sFileName.c_str(), iLength), DeleteArrFunc);

	sWorkableDomain = sDomain;
	sWorkableLang = sLang;

	CTPP2Catalog & oCatalog = mCatalog[sWorkableLang][sWorkableDomain];

    UINT_32 iMagic = (reinterpret_cast<UINT_32 *>(pData.Get()))[0];

    switch (iMagic)
	{
	case LE_MAGIC:
		oCatalog.b_reversed = false;
		break;
	case BE_MAGIC:
		oCatalog.b_reversed = true;
		break;
	default:
		{
			STLW::string sMsg = sFileName + ": invalid MO file";
			throw CTPPGetTextError(sMsg.c_str());
		}
		break;
	}

    UINT_32 iMsgCount = ReadMOData(pData.Get(), 8);
    UINT_32 iMasteridx = ReadMOData(pData.Get(), 12);
    UINT_32 iTransidx = ReadMOData(pData.Get(), 16);

    for (UINT_32 iI = 0; iI < iMsgCount; ++iI)
	{
        ExtractMessage(pData.Get(), iLength, iMasteridx, iTransidx);
		iMasteridx += 2 * sizeof(UINT_32);
		iTransidx += 2 * sizeof(UINT_32);
    }

	sWorkableDomain.erase();
	sWorkableLang.erase();

#if _GETTEXT_PLURAL_DEBUG
	CatalogMap::const_iterator itmC = mCatalog.begin();
	for (; itmC != mCatalog.end(); ++itmC)
	{
		fprintf(stdout, "============================================\n");
		fprintf(stdout, "Language %s\n", itmC -> first.c_str());
		fprintf(stdout, "============================================\n");
		STLW::map<STLW::string, CTPP2Catalog>::const_iterator itm = itmC -> second;
		for (; itm != itmC -> end(); ++itm)
		{
			fprintf(stdout, "CTPP2Catalog %s\n", itm -> first.c_str());
			fprintf(stdout, "--------------------------------------------\n");
			fprintf(stdout, "Charset: %s\n", itm -> second.s_charset.c_str());
			fprintf(stdout, "Info:\n");
			{
				STLW::map<STLW::string, STLW::string>::const_iterator mit = itm -> second.m_info.begin();
				for (; mit != itm -> second.m_info.end(); ++mit) {
					fprintf(stdout, "\t%s: %s\n", mit -> first.c_str(), mit -> second.c_str());
				}
			}
			fprintf(stdout, "Reversed: %d\n", itm -> second.b_reversed);
			fprintf(stdout, "Messages:\n");
			{
				STLW::map<STLW::string, STLW::vector<STLW::string> >::const_iterator itmM = itm -> second.m_messages.begin();
				++itmM;
				for (; itmM != itm -> second.m_messages.end(); ++itmM) {
					fprintf(stdout, "\t'%s':\n", itmM -> first.c_str());
					STLW::vector<STLW::string>::const_iterator vit = itmM -> second.begin();
					for (; itv != itmM -> second.end(); ++itv) {
						fprintf(stdout, "\t\t'%s' ", itv -> c_str());
					}
					fprintf(stdout, "\n");
				}
			}
			fprintf(stdout, "--------------------------------------------\n");
		}
		fprintf(stdout, "============================================\n");
	}
#endif
}

//
// Find translated message
//
STLW::string CTPP2GetText::FindMessage(const STLW::string & sLang, const STLW::string & sMessage, const STLW::string & sDomain_)
{
	STLW::string sDomain = sDomain_;
	if (sDomain.empty()) { sDomain = sDefaultDomain; }

	if (mCatalog.find(sLang) == mCatalog.end() || mCatalog[sLang].find(sDomain) == mCatalog[sLang].end()) { return sMessage; }

	typedef STLW::map<STLW::string, STLW::vector<STLW::string> > MessageMap;
	MessageMap & oMessages = mCatalog[sLang][sDomain].m_messages;
	MessageMap::const_iterator itM = oMessages.find(sMessage);

	if (itM == oMessages.end()) { return sMessage; }

    return itM -> second[0];
}

//
// Find translated message with correct plural form
//
STLW::string CTPP2GetText::FindPluralMessage(const STLW::string & sLang, const STLW::string & sMessage,
                                            const STLW::string & sPlMessage, UINT_32 iCount, const STLW::string & sDomain_)
{
	STLW::string sDomain = sDomain_;
	if (sDomain.empty()) { sDomain = sDefaultDomain; }

	if (mCatalog.find(sLang) == mCatalog.end() ||
	    mCatalog[sLang].find(sDomain) == mCatalog[sLang].end() ||
	    mCatalog[sLang][sDomain].m_messages.find(sMessage) == mCatalog[sLang][sDomain].m_messages.end())
	{
		if (iCount > 1) { return sPlMessage; }
		else            { return sMessage;   }
	}

    UINT_32 iN = CalculatePluralForm(iCount, sLang, sDomain);

	typedef STLW::map<STLW::string, STLW::vector<STLW::string> > MessageMap;
	MessageMap & oMessages = mCatalog[sLang][sDomain].m_messages;
	MessageMap::const_iterator itM = oMessages.find(sMessage);

	if (iN >= itM -> second.size())
	{
		STLW::string sMsg = STLW::string("i18n domain '") + sDomain + "': failed to found plural form: " + sMessage;
		throw CTPPGetTextError(sMsg.c_str());
	}
    return itM -> second[iN];
}

//
// Set default i18n domain
//
void CTPP2GetText::SetDefaultDomain(const STLW::string & sDomain) { sDefaultDomain = sDomain; }

//
// Initialize system calls FnGetText
//
void CTPP2GetText::InitSTDLibFunction(SyscallFactory & oSyscallFactory)
{
	FnGetText * pTMP = dynamic_cast<FnGetText *>(oSyscallFactory.GetHandlerByName("gettext"));
	pTMP -> SetGetText(this);
	pTMP = dynamic_cast<FnGetText *>(oSyscallFactory.GetHandlerByName("_"));
	pTMP -> SetGetText(this);
}

//
// Set language for system calls FnGetText/FnGetText_
//
void CTPP2GetText::SetLanguage(SyscallFactory & oSyscallFactory, const STLW::string & sLang)
{
	FnGetText * pTMP = dynamic_cast<FnGetText *>(oSyscallFactory.GetHandlerByName("gettext"));
	pTMP -> SetLanguage(sLang);
	pTMP = dynamic_cast<FnGetText *>(oSyscallFactory.GetHandlerByName("_"));
	pTMP -> SetLanguage(sLang);
}

// Parsing of .mo file ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// Parse line of metadata from .mo file
//
void CTPP2GetText::ParseMetadataLine(const STLW::string & sLine, STLW::string & sLastKey)
{
	STLW::string::size_type iLineDelimPos = sLine.find(":");
	STLW::string sKey;
	STLW::string sValue;
	if (iLineDelimPos != STLW::string::npos)
	{
		sKey = sLine.substr(0, iLineDelimPos);
		Trim(sKey);
		STLW::transform(sKey.begin(), sKey.end(), sKey.begin(), ::tolower);
		sValue = sLine.substr(iLineDelimPos + 1);
		Trim(sValue);
		mCatalog[sWorkableLang][sWorkableDomain].m_info[sKey] = sValue;
		sLastKey = sKey;
	}
	else if (!sLastKey.empty())
	{
		mCatalog[sWorkableLang][sWorkableDomain].m_info[sLastKey] += "\n" + sLine;
	}

	if (sKey == "content-type")
	{
		STLW::string::size_type iCharsetPos = sValue.find("charset=");
		if (iCharsetPos != STLW::string::npos)
		{
			mCatalog[sWorkableLang][sWorkableDomain].s_charset = sValue.substr(iCharsetPos + 8);
		}
	}
	else if (sKey == "plural-forms")
	{
		STLW::vector<STLW::string> vPlurals;
		Split(sValue, ';', vPlurals);
		STLW::string sPluralRule = vPlurals[1].substr(vPlurals[1].find("plural=") + 7);
		GeneratePluralRule(sPluralRule);
	}
}

//
// Parse metadata from .mo file
//
void CTPP2GetText::ParseMetadata(const STLW::string & sMeta)
{
	STLW::string sLastKey;

	STLW::vector<STLW::string> vMetaLines;
	Split(sMeta, '\n', vMetaLines);
	STLW::vector<STLW::string>::iterator itvMetaLine = vMetaLines.begin();
	for (; itvMetaLine != vMetaLines.end(); ++itvMetaLine)
	{
		Trim(*itvMetaLine);
		if (!itvMetaLine -> empty()) { ParseMetadataLine(*itvMetaLine, sLastKey); }
	}
}

//
// Read .mo data by 4 bytes
//
UINT_32 CTPP2GetText::ReadMOData(UCCHAR_P pData, INT_32 iOffset)
{
	const UINT_32 *p = reinterpret_cast<const UINT_32 *>(pData + iOffset);

	if (mCatalog[sWorkableLang][sWorkableDomain].b_reversed)
	{
		UINT_32 i0 = (*p >>  0) & 0xFF;
		UINT_32 i1 = (*p >>  8) & 0xFF;
		UINT_32 i2 = (*p >> 16) & 0xFF;
		UINT_32 i3 = (*p >> 24) & 0xFF;

		return (i0 << 24) | (i1 << 16) | (i2 << 8) | i3;
	}

	return *p;
}

//
// Read .mo file
//
UCHAR_P CTPP2GetText::ReadFile(CCHAR_P szFileName, UINT_32 &iLength)
{
    FILE * pFile = ::fopen(szFileName, "rb");
    if (!pFile)
	{
		STLW::string sMsg = STLW::string(szFileName) + ": " + strerror(errno);
		throw CTPPGetTextError(sMsg.c_str());
	}

    struct stat st;
    if (::fstat(fileno(pFile), &st) == -1)
	{
		STLW::string sMsg = STLW::string(szFileName) + ": fstat failed";
        throw CTPPGetTextError(sMsg.c_str());
    }

    ScopeGuard<UCHAR_8> pData(new UCHAR_8[st.st_size], DeleteArrFunc);

    ::fseek(pFile, 0, SEEK_SET);

    if (::fread(reinterpret_cast<void *>(pData.Get()), st.st_size, 1, pFile) < 1)
	{
		STLW::string sMsg = STLW::string(szFileName) + ": fread failed";
        throw CTPPGetTextError(sMsg.c_str());
    }

	iLength = st.st_size;
    return pData.Release();
}

//
// Extract message from .mo data
//
void CTPP2GetText::ExtractMessage(UCCHAR_P pData, UINT_32 iLength, INT_32 iMasteridx, INT_32 iTransidx)
{
    UINT_32 iMsgLen = ReadMOData(pData, iMasteridx);
    UINT_32 iMsgOffset = ReadMOData(pData, iMasteridx + sizeof(UINT_32));
    UINT_32 iTransLen = ReadMOData(pData, iTransidx);
    UINT_32 iTransOffset = ReadMOData(pData, iTransidx + sizeof(UINT_32));
    if (iMsgLen + iMsgOffset >= iLength || iTransLen + iTransOffset >= iLength)
	{
		STLW::string sMsg = STLW::string("i18n domain '") + sWorkableDomain + "': corrupted MO file";
        throw CTPPGetTextError(sMsg.c_str());
    }

    STLW::string sMessage(reinterpret_cast<CCHAR_P>(pData + iMsgOffset), iMsgLen);
    STLW::string sTMessage(reinterpret_cast<CCHAR_P>(pData + iTransOffset), iTransLen);

    if (iMsgLen == 0) { ParseMetadata(sTMessage); }

	CTPP2Catalog & oCatalog = mCatalog[sWorkableLang][sWorkableDomain];
    STLW::string::size_type iPos = sMessage.find("\x00");
    if (iPos != STLW::string::npos)
	{
        STLW::vector<STLW::string> vMsgIds;
        Split(sMessage, '\x00', vMsgIds);

        STLW::vector<STLW::string> vTMsgIds;
        Split(sTMessage, '\x00', vTMsgIds);

		oCatalog.m_messages[vMsgIds[0]] = vTMsgIds;
    }
	else
	{
        STLW::vector<STLW::string> vTMsgIds;
        vTMsgIds.push_back(sTMessage);
		oCatalog.m_messages[sMessage] = vTMsgIds;
    }
}

// Expressions for generating plural form rule ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// Term           = number | "n" | "(" TernaryExpr ")" | TernaryExpr
//
STLW::string::size_type CTPP2GetText::IsTerm(const STLW::string & sData, STLW::string::size_type iPos)
{
	REPORTER("IsTerm", sData, iPos);

	if (iPos == STLW::string::npos) { return iPos; }

	CTPP2Catalog & oCatalog = mCatalog[sWorkableLang][sWorkableDomain];
	if (::isdigit(sData[iPos]))
	{
		STLW::string::size_type iDigitPos = iPos;
		while (::isdigit(sData[iPos])) { ++iPos; }
		STLW::string sDigit = sData.substr(iDigitPos, iPos - iDigitPos);
		INT_32 iDigit = atoi(sDigit.c_str());
#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "PUSH: %d\n", iDigit);
#endif
		oCatalog.v_pl_stack.push_back(CTPP2Data(iDigit));
	}
	else if (sData[iPos] == 'n')
	{
#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "PUSH: n\n");
#endif
		oCatalog.v_pl_stack.push_back(CTPP2Data());
		++iPos;
	}
	else if (sData[iPos] == '(')
	{
		++iPos;
		if (iPos >= sData.size())
		{
			STLW::string sMsg = STLW::string("i18n domain '") + sWorkableDomain + "': plural rule parser error (1 IsTerm)";
			throw CTPPGetTextError(sMsg.c_str());
		}

		iPos = IsTernaryExpr(sData, iPos);

		if (sData[iPos] != ')')
		{
			STLW::string sMsg = STLW::string("i18n domain '") + sWorkableDomain + "': plural rule parser error (2 IsTerm)";
			throw CTPPGetTextError(sMsg.c_str());
		}
		++iPos;
	}
	else
	{
		UINT_32 iNextPos = IsTernaryExpr(sData, iPos);

		if (iNextPos == iPos)
		{
			STLW::string sMsg = STLW::string("i18n domain '") + sWorkableDomain + "': plural rule parser error (3 IsTerm)";
			throw CTPPGetTextError(sMsg.c_str());
		}
		iPos = iNextPos;
	}

	return iPos;
}

//
// ModExpr        = Term { "%" Term }
//
STLW::string::size_type CTPP2GetText::IsModExpr(const STLW::string & sData, STLW::string::size_type iPos)
{
	REPORTER("IsModExr", sData, iPos);

	iPos = IsTerm(sData, iPos);
	if (iPos == STLW::string::npos) { return iPos; }

	if (sData[iPos] == '%')
	{
#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "\t --> OPERATOR '%%'\n");
#endif
		CTPP2Catalog & oCatalog = mCatalog[sWorkableLang][sWorkableDomain];

		oCatalog.v_pl_instructions.push_back(INS_MOD);

		iPos = IsTerm(sData, iPos + 1);
		if (iPos == STLW::string::npos)
		{
			STLW::string sMsg = STLW::string("i18n domain '") + sWorkableDomain + "': plural rule parser error (2 IsModExpr)";
			throw CTPPGetTextError(sMsg.c_str());
		}
	}

	return iPos;
}

//
// LtOrGtExpr     = ModExpr { LtOrGtRelation ModExpr }
//
STLW::string::size_type CTPP2GetText::IsLtOrGtExpr(const STLW::string & sData, STLW::string::size_type iPos)
{
	REPORTER("IsLtOrGtExpr", sData, iPos);

	iPos = IsModExpr(sData, iPos);
	if (iPos == STLW::string::npos) { return iPos; }

	CTPP2Catalog & oCatalog = mCatalog[sWorkableLang][sWorkableDomain];

	bool bHasRelation = true;
	if (sData.find(">=", iPos) == iPos)
	{
#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "\t --> OPERATOR '>='\n");
#endif
		oCatalog.v_pl_instructions.push_back(INS_GE);
		iPos += 2;
	}
	else if (sData.find("<=", iPos) == iPos)
	{
#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "\t --> OPERATOR '<='\n");
#endif
		oCatalog.v_pl_instructions.push_back(INS_LE);
		iPos += 2;
	}
	else if (sData[iPos] == '<')
	{
#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "\t --> OPERATOR '<'\n");
#endif
		oCatalog.v_pl_instructions.push_back(INS_LT);
		++iPos;
	}
	else if (sData[iPos] == '>')
	{
#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "\t --> OPERATOR '>'\n");
#endif
		oCatalog.v_pl_instructions.push_back(INS_GT);
		++iPos;
	}
	else
	{
		bHasRelation = false;
	}

	if (bHasRelation)
	{
		if (iPos >= sData.size())
		{
			STLW::string sMsg = STLW::string("i18n domain '") + sWorkableDomain + "': plural rule parser error (1 IsLtOrGtExpr)";
			throw CTPPGetTextError(sMsg.c_str());
		}
		iPos = IsModExpr(sData, iPos);
	}

	return iPos;
}

//
// EqExpr         = LtOrGtExpr { EqRelation LtOrGtExpr }
//
STLW::string::size_type CTPP2GetText::IsEqExpr(const STLW::string & sData, STLW::string::size_type iPos)
{
	REPORTER("IsEqExpr", sData, iPos);

	iPos = IsLtOrGtExpr(sData, iPos);
	if (iPos == STLW::string::npos) { return iPos; }

	CTPP2Catalog & oCatalog = mCatalog[sWorkableLang][sWorkableDomain];

	bool bHasRelation = true;
	if (sData.find("==", iPos) == iPos)
	{
#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "\t --> OPERATOR '=='\n");
#endif
		oCatalog.v_pl_instructions.push_back(INS_EQ);
		iPos += 2;
	}
	else if (sData.find("!=", iPos) == iPos)
	{
#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "\t --> OPERATOR '!='\n");
#endif
		oCatalog.v_pl_instructions.push_back(INS_NE);
		iPos += 2;
	}
	else
	{
		bHasRelation = false;
	}

	if (bHasRelation)
	{
		if (iPos >= sData.size())
		{
			STLW::string sMsg = STLW::string("i18n domain '") + sWorkableDomain + "': plural rule parser error (1 IsEqExpr)";
			throw CTPPGetTextError(sMsg.c_str());
		}
		iPos = IsLtOrGtExpr(sData, iPos);
	}

	return iPos;
}

//
// AndExpr        = EqExpr { "&&" AndExpr }
//
STLW::string::size_type CTPP2GetText::IsAndExpr(const STLW::string & sData, STLW::string::size_type iPos)
{
	REPORTER("IsAndExpr", sData, iPos);

	iPos = IsEqExpr(sData, iPos);
	if (iPos == STLW::string::npos) { return iPos; }

	if (sData.find("&&", iPos) == iPos)
	{

#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "\t --> JUMP IF '&&'\n");
#endif
		CTPP2Catalog & oCatalog = mCatalog[sWorkableLang][sWorkableDomain];
		oCatalog.v_pl_instructions.push_back(INS_JLAND);

		oCatalog.v_pl_stack.push_back(0);
		oCatalog.v_pl_stack.push_back(0);
		UINT_32 iSIdx = oCatalog.v_pl_stack.size() - 2;
		UINT_32 iIIdx = oCatalog.v_pl_stack.size() - 1;

#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "PUSH: 0 (STACK OFFSET), IDX = %u\n", iSIdx);
fprintf(stdout, "PUSH: 0 (INSTRUCTION OFFSET), IDX = %u\n", iIIdx);
#endif

		UINT_32 iPrevStackSize = oCatalog.v_pl_stack.size();
		UINT_32 iPrevInstrSize = oCatalog.v_pl_instructions.size();
		if (iPos >= sData.size())
		{
			STLW::string sMsg = STLW::string("i18n domain '") + sWorkableDomain + "': plural rule parser error (1 IsAndExpr)";
			throw CTPPGetTextError(sMsg.c_str());
		}
		iPos = IsAndExpr(sData, iPos + 2);

		UINT_32 iStackSize = oCatalog.v_pl_stack.size();
		UINT_32 iInstrSize = oCatalog.v_pl_instructions.size();

#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "REPLACE: %u (STACK OFFSET), IDX = %u\n", iStackSize - iPrevStackSize, iSIdx);
fprintf(stdout, "REPLACE: %u (INSTRUCTION OFFSET), IDX = %u\n", iInstrSize - iPrevInstrSize, iIIdx);
#endif

		oCatalog.v_pl_stack[iSIdx] = iStackSize - iPrevStackSize;
		oCatalog.v_pl_stack[iIIdx] = iInstrSize - iPrevInstrSize;
	}


	return iPos;
}

//
// OrExpr         = AndExpr { "||" OrExpr }
//
STLW::string::size_type CTPP2GetText::IsOrExpr(const STLW::string & sData, STLW::string::size_type iPos)
{
	REPORTER("IsOrExpr", sData, iPos);

	iPos = IsAndExpr(sData, iPos);
	if (iPos == STLW::string::npos) { return iPos; }

	if (sData.find("||", iPos) == iPos)
	{

#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "\t --> JUMP IF '||'\n");
#endif
		CTPP2Catalog & oCatalog = mCatalog[sWorkableLang][sWorkableDomain];
		oCatalog.v_pl_instructions.push_back(INS_JLOR);

		oCatalog.v_pl_stack.push_back(0);
		oCatalog.v_pl_stack.push_back(0);
		UINT_32 iSIdx = oCatalog.v_pl_stack.size() - 2;
		UINT_32 iIIdx = oCatalog.v_pl_stack.size() - 1;

#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "PUSH: 0 (STACK OFFSET), IDX = %u\n", iSIdx);
fprintf(stdout, "PUSH: 0 (INSTRUCTION OFFSET), IDX = %u\n", iIIdx);
#endif

		UINT_32 iPrevStackSize = oCatalog.v_pl_stack.size();
		UINT_32 iPrevInstrSize = oCatalog.v_pl_instructions.size();
		if (iPos >= sData.size())
		{
			STLW::string sMsg = STLW::string("i18n domain '") + sWorkableDomain + "': plural rule parser error (1 IsOrExpr)";
			throw CTPPGetTextError(sMsg.c_str());
		}
		iPos = IsOrExpr(sData, iPos + 2);

		UINT_32 iStackSize = oCatalog.v_pl_stack.size();
		UINT_32 iInstrSize = oCatalog.v_pl_instructions.size();

#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "REPLACE: %u (STACK OFFSET), IDX = %u\n", iStackSize - iPrevStackSize, iSIdx);
fprintf(stdout, "REPLACE: %u (INSTRUCTION OFFSET), IDX = %u\n", iInstrSize - iPrevInstrSize, iIIdx);
#endif

		oCatalog.v_pl_stack[iSIdx] = iStackSize - iPrevStackSize;
		oCatalog.v_pl_stack[iIIdx] = iInstrSize - iPrevInstrSize;
	}

	return iPos;
}

//
// TernaryExpr    = OrExpr { "?" TernaryExpr ":" TernaryExpr }
//
STLW::string::size_type CTPP2GetText::IsTernaryExpr(const STLW::string & sData, STLW::string::size_type iPos)
{
	REPORTER("IsTernaryExpr", sData, iPos);

	iPos = IsOrExpr(sData, iPos);
	if (iPos == STLW::string::npos) { return iPos; }

	if (sData[iPos] == '?')
	{

#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "\t --> JUMP IF '?:'\n");
#endif
		CTPP2Catalog & oCatalog = mCatalog[sWorkableLang][sWorkableDomain];
		oCatalog.v_pl_instructions.push_back(INS_JN);

		oCatalog.v_pl_stack.push_back(0);
		oCatalog.v_pl_stack.push_back(0);
		UINT_32 iJNSIdx = oCatalog.v_pl_stack.size() - 2;
		UINT_32 iJNIIdx = oCatalog.v_pl_stack.size() - 1;

#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "PUSH: 0 (STACK OFFSET), IDX = %u\n", iJNSIdx);
fprintf(stdout, "PUSH: 0 (INSTRUCTION OFFSET), IDX = %u\n", iJNIIdx);
#endif

		UINT_32 iPrevStackSize = oCatalog.v_pl_stack.size();
		UINT_32 iPrevInstrSize = oCatalog.v_pl_instructions.size();
		if (iPos >= sData.size())
		{
			STLW::string sMsg = STLW::string("i18n domain '") + sWorkableDomain + "': plural rule parser error (1 IsTernaryExpr)";
			throw CTPPGetTextError(sMsg.c_str());
		}

		iPos = IsTernaryExpr(sData, iPos + 1);

		UINT_32 iStackSize = oCatalog.v_pl_stack.size();
		UINT_32 iInstrSize = oCatalog.v_pl_instructions.size();
#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "REPLACE: %u (STACK OFFSET), IDX = %u\n", iStackSize - iPrevStackSize + 2, iJNSIdx);
fprintf(stdout, "REPLACE: %u (INSTRUCTION OFFSET), IDX = %u\n", iInstrSize - iPrevInstrSize, iJNIIdx);
#endif
		oCatalog.v_pl_stack[iJNSIdx] = iStackSize - iPrevStackSize + 2;
		oCatalog.v_pl_stack[iJNIIdx] = iInstrSize - iPrevInstrSize;

		if (sData[iPos] == ':')
		{
#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "\t --> JUMP\n");
#endif
			++iPos;
			if (iPos >= sData.size())
			{
				STLW::string sMsg = STLW::string("i18n domain '") + sWorkableDomain + "': plural rule parser error (2 IsTernaryExpr)";
				throw CTPPGetTextError(sMsg.c_str());
			}

			oCatalog.v_pl_instructions.push_back(INS_JMP);

			oCatalog.v_pl_stack.push_back(0);
			oCatalog.v_pl_stack.push_back(0);
			UINT_32 iJMPSIdx = oCatalog.v_pl_stack.size() - 2;
			UINT_32 iJMPIIdx = oCatalog.v_pl_stack.size() - 1;

#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "PUSH: 0 (STACK OFFSET), IDX = %u\n", iJMPSIdx);
fprintf(stdout, "PUSH: 0 (INSTRUCTION OFFSET), IDX = %u\n", iJMPIIdx);
#endif

			iPrevStackSize = oCatalog.v_pl_stack.size();
			iPrevInstrSize = oCatalog.v_pl_instructions.size();
			iPos = IsTernaryExpr(sData, iPos);
			iStackSize = oCatalog.v_pl_stack.size();
			iInstrSize = oCatalog.v_pl_instructions.size();
#ifdef _GETTEXT_PLURAL_DEBUG
fprintf(stdout, "REPLACE: %u (STACK OFFSET), IDX = %u\n", iStackSize - iPrevStackSize, iJMPSIdx);
fprintf(stdout, "REPLACE: %u (INSTRUCTION OFFSET), IDX = %u\n", iInstrSize - iPrevInstrSize, iJMPIIdx);
#endif
			oCatalog.v_pl_stack[iJMPSIdx] = iStackSize - iPrevStackSize;
			oCatalog.v_pl_stack[iJMPIIdx] = iInstrSize - iPrevInstrSize;
		}
		else
		{
			STLW::string sMsg = STLW::string("i18n domain '") + sWorkableDomain + "': plural rule parser error (3 IsTernaryExpr)";
			throw CTPPGetTextError(sMsg.c_str());
		}
	}

	return iPos;
}

// Other stuff ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// Calculate plural form of message
//
UINT_32 CTPP2GetText::CalculatePluralForm(UINT_32 iCount, const STLW::string & sLang, const STLW::string & sDomain)
{
	STLW::vector<UINT_32> vStack;
	CTPP2Catalog & oCatalog = mCatalog[sLang][sDomain];
	vStack.reserve(oCatalog.v_pl_stack.size());

	STLW::vector<CTPP2Data>::reverse_iterator itvSrc = oCatalog.v_pl_stack.rbegin();
	for (; itvSrc != oCatalog.v_pl_stack.rend(); ++itvSrc)
	{
		if (itvSrc -> b_variable) { vStack.push_back(iCount);            }
		else                      { vStack.push_back(itvSrc -> i_value); }
	}

#ifdef _GETTEXT_PLURAL_DEBUG
	fprintf(stdout, "==>");
	for (STLW::vector<eCTPP2Instruction>::iterator it = oCatalog.v_pl_instructions.begin();
		it != oCatalog.v_pl_instructions.end(); ++it)
	{
			fprintf(stdout, " %s", StringifyInstruction(*it));
	}
	fprintf(stdout, " <==\n");
#endif

	STLW::vector<eCTPP2Instruction>::iterator itvPluralInstrs = oCatalog.v_pl_instructions.begin();
	while (itvPluralInstrs != oCatalog.v_pl_instructions.end())
	{
#ifdef _GETTEXT_PLURAL_DEBUG
		fprintf(stdout, "\t --> INSTRUCTION: %s\n", StringifyInstruction(*itvPluralInstrs));
		fprintf(stdout, "\t --> STACK DUMP: ");
		for (INT_32 iN = vStack.size() - 1; iN >= 0; --iN) { fprintf(stdout, "%u, ", vStack[iN]); }
		fprintf(stdout, "\n");
#endif

#ifdef _GETTEXT_PLURAL_DEBUG
#define CALCULATE_EXPR(N)                                      \
	do {                                                       \
		UINT_32 iLeft = vStack.back();                         \
		fprintf(stdout, "POP: %u (iLeft)\n", iLeft);           \
		vStack.pop_back();                                     \
		UINT_32 iRight = vStack.back();                        \
		fprintf(stdout, "POP: %u (iRight)\n", iRight);         \
		vStack.pop_back();			                           \
		vStack.push_back(N);                                   \
		fprintf(stdout, "PUSH: %u (RESULT OF '%s')\n", N, #N); \
	} while(0)
#else
#define CALCULATE_EXPR(N)                                      \
	do {                                                       \
		UINT_32 iLeft = vStack.back();                         \
		vStack.pop_back();                                     \
		UINT_32 iRight = vStack.back();                        \
		vStack.pop_back();			                           \
		vStack.push_back(N);                                   \
	} while(0)
#endif

		switch (*itvPluralInstrs)
		{
		case INS_EQ:
			CALCULATE_EXPR(iLeft == iRight);
			break;
		case INS_LE:
			CALCULATE_EXPR(iLeft <= iRight);
			break;
		case INS_GE:
			CALCULATE_EXPR(iLeft >= iRight);
			break;
		case INS_LT:
			CALCULATE_EXPR(iLeft < iRight);
			break;
		case INS_GT:
			CALCULATE_EXPR(iLeft > iRight);
			break;
		case INS_NE:
			CALCULATE_EXPR(iLeft != iRight);
			break;
		case INS_MOD:
			CALCULATE_EXPR(iLeft % iRight);
			break;
#undef CALCULATE_EXPR
		case INS_JLAND:
			{
				UINT_32 iCond = vStack.back();
				vStack.pop_back();
#ifdef _GETTEXT_PLURAL_DEBUG
				fprintf(stdout, "POP: %u (JLAND CONDITION)\n", iCond);
#endif

				UINT_32 iStackOffset = vStack.back();
#ifdef _GETTEXT_PLURAL_DEBUG
				fprintf(stdout, "POP: %u (STACK OFFSET)\n", iStackOffset);
#endif
				vStack.pop_back();
				UINT_32 iInstrOffset = vStack.back();
#ifdef _GETTEXT_PLURAL_DEBUG
				fprintf(stdout, "POP: %u (INSTRUCTION OFFSET)\n", iInstrOffset);
#endif
				vStack.pop_back();

				if (!iCond)
				{
#ifdef _GETTEXT_PLURAL_DEBUG
					STLW::vector<UINT_32>::const_reverse_iterator itvStack = vStack.rbegin();
					for (INT_32 iN = vStack.size() - iStackOffset; itvStack != vStack.rend() && iN < vStack.size() ; ++iN)
					{
						fprintf(stdout, "POP: %d (JLAND UNUSED)\n", *itvStack);
					}
#endif
					vStack.resize(vStack.size() - iStackOffset);
					for (UINT_32 iI = 0; iI < iInstrOffset; ++iI)
					{
#ifdef _GETTEXT_PLURAL_DEBUG
						assert(itvPluralInstrs != oCatalog.v_pl_instructions.end());
#endif
						++itvPluralInstrs;
#ifdef _GETTEXT_PLURAL_DEBUG
						fprintf(stdout, "\t --> INSTRUCTION (SKIP): %s\n",
						        StringifyInstruction(*itvPluralInstrs));
#endif
					}
#ifdef _GETTEXT_PLURAL_DEBUG
					fprintf(stdout, "PUSH: %u (JLAND CONDITION)\n", iCond);
#endif
					vStack.push_back(iCond);
				}
			}
			break;
		case INS_JLOR:
			{
				UINT_32 iCond = vStack.back();
#ifdef _GETTEXT_PLURAL_DEBUG
				fprintf(stdout, "POP: %u (JLOR CONDITION)\n", iCond);
#endif
				vStack.pop_back();

				UINT_32 iStackOffset = vStack.back();
#ifdef _GETTEXT_PLURAL_DEBUG
				fprintf(stdout, "POP: %u (STACK OFFSET)\n", iStackOffset);
#endif
				vStack.pop_back();
				UINT_32 iInstrOffset = vStack.back();
#ifdef _GETTEXT_PLURAL_DEBUG
				fprintf(stdout, "POP: %u (INSTRUCTION OFFSET)\n", iInstrOffset);
#endif
				vStack.pop_back();

				if (iCond)
				{
#ifdef _GETTEXT_PLURAL_DEBUG
					STLW::vector<UINT_32>::const_reverse_iterator itvStack = vStack.rbegin();
					for (INT_32 iN = vStack.size() - iStackOffset; itvStack != vStack.rend() && iN < vStack.size() ; ++iN)
					{
						fprintf(stdout, "POP: %d (JLOR UNUSED)\n", *itvStack);
					}
#endif
					vStack.resize(vStack.size() - iStackOffset);
					for (UINT_32 iI = 0; iI < iInstrOffset; ++iI)
					{
#ifdef _GETTEXT_PLURAL_DEBUG
						assert(itvPluralInstrs != oCatalog.v_pl_instructions.end());
#endif
						++itvPluralInstrs;
#ifdef _GETTEXT_PLURAL_DEBUG
						fprintf(stdout, "\t --> INSTRUCTION (SKIP): %s\n", StringifyInstruction(*itvPluralInstrs));
#endif
					}
#ifdef _GETTEXT_PLURAL_DEBUG
					fprintf(stdout, "PUSH: %u (JLAND CONDITION)\n", iCond);
#endif
					vStack.push_back(iCond);
				}
			}
			break;
		case INS_JN:
			{
				UINT_32 iCond = vStack.back();
#ifdef _GETTEXT_PLURAL_DEBUG
				fprintf(stdout, "POP: %u (JN CONDITION)\n", iCond);
#endif
				vStack.pop_back();

				UINT_32 iStackOffset = vStack.back();
#ifdef _GETTEXT_PLURAL_DEBUG
				fprintf(stdout, "POP: %u (STACK OFFSET)\n", iStackOffset);
#endif
				vStack.pop_back();
				UINT_32 iInstrOffset = vStack.back();
#ifdef _GETTEXT_PLURAL_DEBUG
				fprintf(stdout, "POP: %u (INSTRUCTION OFFSET)\n", iInstrOffset);
#endif
				vStack.pop_back();

				if (!iCond)
				{
#ifdef _GETTEXT_PLURAL_DEBUG
					STLW::vector<UINT_32>::const_reverse_iterator itvStack = vStack.rbegin();
					for (INT_32 iN = vStack.size() - iStackOffset; itvStack != vStack.rend() && iN < vStack.size() ; ++iN)
					{
						fprintf(stdout, "POP: %d (JN UNUSED)\n", *itvStack);
					}
#endif
					vStack.resize(vStack.size() - iStackOffset);
					for (UINT_32 iI = 0; iI < iInstrOffset + 1; ++iI)
					{
#ifdef _GETTEXT_PLURAL_DEBUG
						assert(itvPluralInstrs != oCatalog.v_pl_instructions.end());
#endif
						++itvPluralInstrs;
#ifdef _GETTEXT_PLURAL_DEBUG
						fprintf(stdout, "\t --> INSTRUCTION (SKIP): %s\n", StringifyInstruction(*itvPluralInstrs));
#endif
					}
				}
			}
			break;
		case INS_JMP:
			{
				INT_32 iLastValue = vStack.back();
				vStack.pop_back();
#ifdef _GETTEXT_PLURAL_DEBUG
				fprintf(stdout, "POP: %u (LAST VALUE)\n", iLastValue);
#endif

				UINT_32 iStackOffset = vStack.back();
#ifdef _GETTEXT_PLURAL_DEBUG
				fprintf(stdout, "POP: %u (STACK OFFSET)\n", iStackOffset);
#endif
				vStack.pop_back();
				UINT_32 iInstrOffset = vStack.back();
#ifdef _GETTEXT_PLURAL_DEBUG
				fprintf(stdout, "POP: %u (INSTRUCTION OFFSET)\n", iInstrOffset);
#endif
				vStack.pop_back();

#ifdef _GETTEXT_PLURAL_DEBUG
				STLW::vector<UINT_32>::const_reverse_iterator itvStack = vStack.rbegin();
				for (INT_32 iN = vStack.size() - iStackOffset; itvStack != vStack.rend() && iN < vStack.size() ; ++iN)
				{
					fprintf(stdout, "POP: %d (JMP UNUSED)\n", *itvStack);
				}
#endif
				vStack.resize(vStack.size() - iStackOffset);
				for (UINT_32 iI = 0; iI < iInstrOffset; ++iI)
				{
#ifdef _GETTEXT_PLURAL_DEBUG
					assert(itvPluralInstrs != oCatalog.v_pl_instructions.end());
#endif
					++itvPluralInstrs;
#ifdef _GETTEXT_PLURAL_DEBUG
					fprintf(stdout, "\t --> INSTRUCTION (SKIP): %s\n", StringifyInstruction(*itvPluralInstrs));
#endif
				}
#ifdef _GETTEXT_PLURAL_DEBUG
				fprintf(stdout, "PUSH: %u (LAST VALUE)\n", iLastValue);
#endif
				vStack.push_back(iLastValue);
			}
			break;
		default:
			break;
		}
		++itvPluralInstrs;
	}

#ifdef _GETTEXT_PLURAL_DEBUG
	fprintf(stdout, "Stack size: %u\n", vStack.size());
	for (UINT_32 idx = 0; idx < vStack.size(); ++idx) { fprintf(stdout, "--> %u\n", vStack[idx]); }
#endif
	return vStack.back();
}

//
// Generate plural form rule
//
void CTPP2GetText::GeneratePluralRule(const STLW::string & sPluralRule)
{
	STLW::string s = sPluralRule;
	s.erase(STLW::remove_if(s.begin(), s.end(), ::isspace), s.end());
	IsTernaryExpr(s, 0);
	mCatalog[sWorkableLang][sWorkableDomain].v_pl_instructions.push_back(INS_NONE);
}

//
// Stringify instruction for determining plural form
//
CCHAR_P CTPP2GetText::StringifyInstruction(eCTPP2Instruction eType)
{
	switch (eType)
	{
	case INS_EQ:
		return "'=='";
	case INS_LE:
		return "'<='";
	case INS_GE:
		return "'>='";
	case INS_LT:
		return "'<'";
	case INS_GT:
		return "'>'";
	case INS_NE:
		return "'!='";
	case INS_MOD:
		return "'%'";
	case INS_JN:
		return "JN";
	case INS_JMP:
		return "JMP";
	case INS_JLAND:
		return "JLAND";
	case INS_JLOR:
		return "JLOR";
	case INS_NONE:
		return "NONE";
	default:
		return "UNKNOWN";
	}
}

}  // namespace CTPP
// End.
