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
 *      CDT.hpp
 *
 * $CTPP$
 */
#ifndef _CTPP2_GETTEXT_HPP__
#define _CTPP2_GETTEXT_HPP__ 1

#include "CTPP2Types.h"
#include "STLString.hpp"
#include "STLVector.hpp"
#include "STLMap.hpp"

namespace CTPP // C++ Template Engine
{

// FWD
class SyscallFactory;

/**
  @class CTPP2GetText CTPP2GetText.hpp <CTPP2GetText.hpp>
  @brief CTPP2 gettext support
*/
class CTPP2DECL CTPP2GetText
{
public:
	/**
	  @brief A constructor
	*/
	CTPP2GetText();

	/**
	  @brief A destructor
	*/
	~CTPP2GetText() throw();

	/**
	  @brief Add translation
	  @param sFileName - .mo filename
	  @param sDomain - i18n domain
	  @param sLang - language of translation
	*/
	void AddTranslation(const STLW::string & sFileName, const STLW::string & sDomain, const STLW::string & sLang);

	/**
	  @brief Find translated message
	  @param sLang - language of translation
	  @param sMessage - message
	  @param sDomain - i18n domain (if none, default domain used)
	  @return translated message
	*/
	STLW::string FindMessage(const STLW::string & sLang, const STLW::string & sMessage, const STLW::string & sDomain = "");

	/**
	  @brief Find translated message with correct plural form
	  @param sLang - language of translation
	  @param sMessage - message in singular
	  @param sPlMessage - message in plural
	  @param iCount - determine plural form
	  @param sDomain - i18n domain (if none, default domain used)
	  @return translated message
	*/
	STLW::string FindPluralMessage(const STLW::string & sLang, const STLW::string & sMessage, const STLW::string & sPlMessage,
	                               UINT_32 iCount, const STLW::string & ssDomain = "");

	/**
	  @brief Set default i18n domain
	  @param sDomain - i18n domain
	*/
	void SetDefaultDomain(const STLW::string & sDomain);

	/**
	  @brief Initialize system calls FnGetText/FnGetText_
	  @param oSyscallFactory - factory with system calls
	*/
	void InitSTDLibFunction(SyscallFactory & oSyscallFactory);

	/**
	  @brief Set language for system calls FnGetText/FnGetText_
	  @param oSyscallFactory - factory with system calls
	  @param sLang - language of translation
	*/
	void SetLanguage(SyscallFactory & oSyscallFactory, const STLW::string & sLang);

private:
	enum eCTPP2Instruction { INS_NONE  = 0,
	                         INS_EQ    = 1,
	                         INS_NE    = 2,
	                         INS_LE    = 3,
	                         INS_LT    = 4,
	                         INS_GE    = 5,
	                         INS_GT    = 6,
	                         INS_MOD   = 7,
	                         INS_JN    = 8,
	                         INS_JMP   = 9,
	                         INS_JLAND = 10,
	                         INS_JLOR  = 11 };

	/**
	  @struct CTPP2Data CTPP2GetText.hpp <CTPP2GetText.hpp>
	  @brief Representation of number/placeholder in plural form rule
	*/
	struct CTPP2Data
	{
		/**
		  @brief A constructor for number data
		  @param iValue - number
		*/
		CTPP2Data(UINT_32 iValue) : i_value(iValue), b_variable(false) { ;; }

		/**
		  @brief A constructor for placeholder data
		*/
		CTPP2Data() : i_value(0), b_variable(true) { ;; }

		/** value of number  */
		UINT_32 i_value;
		/** placeholder flag */
		bool b_variable;
	};

	/**
	  @struct CTPP2Data CTPP2GetText.hpp <CTPP2GetText.hpp>
	  @brief i18n catalog
	*/
	struct CTPP2Catalog
	{
		/** stack of data for determining of plural form */
		STLW::vector<CTPP2Data> v_pl_stack;
		/** list of instructions for determining of plural form */
		STLW::vector<eCTPP2Instruction> v_pl_instructions;
		/** charset of messages from .mo file */
		STLW::string s_charset;
		/** messages with translations */
		STLW::map<STLW::string, STLW::vector<STLW::string> > m_messages;
		/** generic information from .mo file */
		STLW::map<STLW::string, STLW::string> m_info;
		/** flag of endiannes of .mo file */
		bool b_reversed;
	};

	/** Type of catalog map */
	typedef STLW::map<STLW::string, STLW::map<STLW::string, CTPP2Catalog> > CatalogMap;

	/** CTPP2Catalog map                                 */
	CatalogMap mCatalog;
	/** Default i18n domain                         */
	STLW::string sDefaultDomain;

	/** i18n domain for generating plural form rule */
	STLW::string sWorkableDomain;
	/** Language for generating plural form rule    */
	STLW::string sWorkableLang;

	// Parsing of .mo file ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	  @brief Parse line of metadata from .mo file
	  @param sLine - line of metadata
	  @param sLastKey - key for multiline value
	*/
	void ParseMetadataLine(const STLW::string & sLine, STLW::string & sLastKey);

	/**
	  @brief Parse metadata from .mo file
	  @param sMeta - metadata
	*/
	void ParseMetadata(const STLW::string & sMeta);

	/**
	  @brief Read .mo data by 4 bytes
	  @param pData - .mo data
	  @param iOffset - offset of reading
	  @return integer value
	*/
	UINT_32 ReadMOData(UCCHAR_P pData, INT_32 iOffset);

	/**
	  @brief Read .mo file
	  @param szFileName - .mo filename
	  @param iLength - length of read file
	  @return data
	*/
	UCHAR_P ReadFile(CCHAR_P szFileName, UINT_32 & iLength);

	/**
	  @brief Extract message from .mo data
	  @param pData - .mo data
	  @param iLength - length of .mo data
	  @param iMasteridx - message index
	  @param iTransidx - translated message index
	*/
	void ExtractMessage(UCCHAR_P pData, UINT_32 iLength, INT_32 iMasteridx, INT_32 iTransidx);

	// Expressions for generating plural form rule ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	  @brief Term           = number | "n" | "(" TernaryExpr ")" | TernaryExpr
	  @param sData - current stream
	  @param iPos - current stream position
	  @return STLW::string::npos if sequence not found or position to end of sequence
	*/
	STLW::string::size_type IsTerm(const STLW::string & sData, STLW::string::size_type iPos);

	/**
	  @brief ModExpr        = Term { "%" Term }
	  @param sData - current stream
	  @param iPos - current stream position
	  @return STLW::string::npos if sequence not found or position to end of sequence
	*/
	STLW::string::size_type IsModExpr(const STLW::string & sData, STLW::string::size_type iPos);

	/**
	  @brief LtOrGtExpr     = ModExpr { LtOrGtRelation ModExpr }
	  @param sData - current stream
	  @param iPos - current stream position
	  @return STLW::string::npos if sequence not found or position to end of sequence
	*/
	STLW::string::size_type IsLtOrGtExpr(const STLW::string & sData, STLW::string::size_type iPos);

	/**
	  @brief EqExpr         = LtOrGtExpr { EqRelation LtOrGtExpr }
	  @param sData - current stream
	  @param iPos - current stream position
	  @return STLW::string::npos if sequence not found or position to end of sequence
	*/
	STLW::string::size_type IsEqExpr(const STLW::string & sData, STLW::string::size_type iPos);

	/**
	  @brief AndExpr        = EqExpr { "&&" AndExpr }
	  @param sData - current stream
	  @param iPos - current stream position
	  @return STLW::string::npos if sequence not found or position to end of sequence
	*/
	STLW::string::size_type IsAndExpr(const STLW::string & sData, STLW::string::size_type iPos);

	/**
	  @brief OrExpr         = AndExpr { "||" OrExpr }
	  @param sData - current stream
	  @param iPos - current stream position
	  @return STLW::string::npos if sequence not found or position to end of sequence
	*/
	STLW::string::size_type IsOrExpr(const STLW::string & sData, STLW::string::size_type iPos);

	/**
	  @brief TernaryExpr    = OrExpr { "?" TernaryExpr ":" TernaryExpr }
	  @param sData - current stream
	  @param iPos - current stream position
	  @return STLW::string::npos if sequence not found or position to end of sequence
	*/
	STLW::string::size_type IsTernaryExpr(const STLW::string & sData, STLW::string::size_type iPos);

	// Other stuff ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	  @brief Calculate plural form of message
	  @param iCount - determine plural form
	  @param sLang - language of translation
	  @param sDomain - i18n domain
	  @return Number of plural form
	*/
	UINT_32 CalculatePluralForm(UINT_32 iCount, const STLW::string & sLang, const STLW::string & sDomain);

	/**
	  @brief Generate plural form rule
	  @param sPluralRule - string of plural form rule from .mo file
	*/
	void GeneratePluralRule(const STLW::string & sPluralRule);

	/**
	  @brief Stringify instruction for determining plural form
	  @param eType - instruction for determining plural form
	  @return string representation of instruction
	*/
	CCHAR_P StringifyInstruction(eCTPP2Instruction eType);

};

} // namespace CTPP
#endif // _CTPP2_GETTEXT_HPP__
// End.
