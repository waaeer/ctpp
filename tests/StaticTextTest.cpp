/*-
 * Copyright (c) 2006, 2007 CTPP Team
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
 *      VMTest.cpp
 *
 * $CTPP$
 */

#include <CTPP2StaticText.hpp>

#include <stdio.h>

#ifdef HAVE_SYSEXITS_H
#include <sysexits.h>
#endif


using namespace CTPP;

int main(void)
{
	StaticText oStaticText;

	oStaticText.StoreData("Test",     4);
	oStaticText.StoreData("really",   6);
	oStaticText.StoreData(" passed?", 8);
	oStaticText.StoreData("1234",     4);

	fprintf(stderr, "Stored text: ");
	UINT_32 iDataSize = 0;
	CCHAR_P sData = oStaticText.GetData(0, iDataSize);
	fwrite(sData, iDataSize, 1, stderr);

	sData = oStaticText.GetData(2, iDataSize);
	fwrite(sData, iDataSize, 1, stderr);

	sData = oStaticText.GetData(6, iDataSize);
	if (sData == NULL) { fprintf(stderr, "\nNon-existent text segment: OK\nYet another stored text: "); }
	else               { return EX_SOFTWARE; }

	sData = oStaticText.GetData(1, iDataSize);
	fwrite(sData, iDataSize, 1, stderr);

	fprintf(stderr, "\n");

	// make valgrind happy
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.

