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
 *      FnHashKeys.cpp
 *
 * $CTPP$
 */

#include "CDT.hpp"
#include "CTPP2Logger.hpp"
#include "FnHashKeys.hpp"

namespace CTPP // C++ Template Engine
{

//
// Constructor
//
FnHashKeys::FnHashKeys()
{
	;;
}

//
// Handler
//
INT_32 FnHashKeys::Handler(CDT	    * aArguments,
                           const UINT_32    iArgNum,
                           CDT	    & oCDTRetVal,
                           Logger	 & oLogger)
{
	// Only 1 args allowed
	if (iArgNum != 1)
	{
		oLogger.Emerg("Usage: HASH_KEYS(x)");
		return -1;
	}

	// Second argument *MUST* be an HASH
	if (aArguments[0].GetType() != CDT::HASH_VAL)
	{
		STLW::string msg = STLW::string("Second argument MUST be HASH, no ") + aArguments[0].PrintableType();
		oLogger.Error(msg.c_str());
		return -1;
	}

	oCDTRetVal = aArguments[0].GetHashKeys();
return 0;
}

//
// Get function name
//
CCHAR_P FnHashKeys::GetName() const { return "hash_keys"; }

//
// A destructor
//
FnHashKeys::~FnHashKeys() throw() { ;; }

} // namespace CTPP
// End.
