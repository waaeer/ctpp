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
 *      FnGetText.cpp
 *
 * $CTPP$
 */

#include "CDT.hpp"
#include "CTPP2Logger.hpp"
#include "FnGetText.hpp"

#ifdef WIN32
#include <windows.h>
#else
#include <strings.h>
#endif
#include "CTPP2GetText.hpp"

namespace CTPP // C++ Template Engine
{

//
// Constructor
//
FnGetText::FnGetText(CCHAR_P szAlias) : pGetText(NULL), szFuncName(strdup(szAlias))
{
	;;
}

//
// Handler
//
INT_32 FnGetText::Handler(CDT            * aArguments,
                          const UINT_32    iArgNum,
                          CDT            & oCDTRetVal,
                          Logger         & oLogger)
{
	if (pGetText == NULL)
	{
		oLogger.Emerg("GetText object is NULL");
		return -1;
	}

	if (iArgNum == 1)
	{
		oCDTRetVal = pGetText -> FindMessage(sLanguage, aArguments[0].GetString());
		return 0;
	}
	else if (iArgNum == 2)
	{
		oCDTRetVal = pGetText -> FindMessage(sLanguage, aArguments[1].GetString(), aArguments[0].GetString());
		return 0;
	}
	else if (iArgNum == 3)
	{
		oCDTRetVal = pGetText -> FindPluralMessage(sLanguage, aArguments[2].GetString(),
		                                     aArguments[1].GetString(), (UINT_32)aArguments[0].GetUInt());
		return 0;
	}
	else if (iArgNum == 4)
	{
		oCDTRetVal = pGetText -> FindPluralMessage(sLanguage, aArguments[3].GetString(), aArguments[2].GetString(),
		                                     (UINT_32)aArguments[1].GetUInt(), aArguments[0].GetString());
		return 0;
	}

	STLW::string sTMP(szFuncName);

	for (UINT_32 iI = 0; iI < sTMP.size(); ++iI) { sTMP[iI] = toupper(sTMP[iI]); }
	STLW::string sMsg = "Usage: " + sTMP + "(msgid[, msgid_plural, n][, domain])";

	oLogger.Emerg(sMsg.c_str());
return -1;
}

//
// Get function name
//
CCHAR_P FnGetText::GetName() const { return szFuncName; }

//
// A destructor
//
FnGetText::~FnGetText() throw() { free(szFuncName); }

//
// Set GetText object
//
void FnGetText::SetGetText(CTPP2GetText * pGetText_) { pGetText = pGetText_; }

//
// Set language of translation
//
void FnGetText::SetLanguage(const STLW::string & sLang) { sLanguage = sLang; }

} // namespace CTPP
// End.
