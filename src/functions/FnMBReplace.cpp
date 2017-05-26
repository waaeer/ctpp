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
 *      FnMBReplace.cpp
 *
 * $CTPP$
 */

#include "CDT.hpp"
#include "CTPP2Logger.hpp"
#include "CTPP2Util.hpp"
#include "FnMBReplace.hpp"
#include <unicode/unistr.h>

namespace CTPP // C++ Template Engine
{

//
// Constructor
//
FnMBReplace::FnMBReplace()
{
	;;
}

//
// Handler
//
INT_32 FnMBReplace::Handler(CDT            * aArguments,
                              const UINT_32    iArgNum,
                              CDT            & oCDTRetVal,
                              Logger         & oLogger)
{
	// substr('foobar', 2, 3) -> 'oba'
	if (iArgNum == 3)
	{

		const UnicodeString sReplacement(aArguments[0].GetString().c_str());
		const UnicodeString sSearch     (aArguments[1].GetString().c_str());
		      UnicodeString sSrc  	    (aArguments[2].GetString().c_str());
	
		UnicodeString res = 
			sSrc.findAndReplace(  0, sSrc.length(), 
					sSearch,      0, sSearch.length(),
					sReplacement, 0, sReplacement.length()
			);

		STLW::string s;
		res.toUTF8String<STLW::string>(s);
		oCDTRetVal =  s.c_str();
		return 0;

	}

	oLogger.Emerg("Usage: MB_REPLACE(source, string, replacement)");
return -1;
}

//
// Get function name
//
CCHAR_P FnMBReplace::GetName() const { return "mb_replace"; }

//
// A destructor
//
FnMBReplace::~FnMBReplace() throw() { ;; }

} // namespace CTPP
// End.
