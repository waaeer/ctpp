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
 *      CTPP2BitIndex.cpp
 *
 * $CTPP$
 */
#include <stdio.h>
#include <CTPP2BitIndex.hpp>

#ifdef HAVE_SYSEXITS_H
#include <sysexits.h>
#endif

using namespace CTPP;

#define C_BITS_NUM 13

int main(void)
{
	BitIndex oBitIndex(C_BITS_NUM);

	for (UINT_32 iI = 0; iI < C_BITS_NUM; ++iI)
	{
		oBitIndex.SetBit(iI, iI % 2);
		fprintf(stderr, "%c", (iI % 2) ? '1' : '0');
	}
	fprintf(stderr, "\n");

	for (UINT_32 iI = 0; iI < C_BITS_NUM; ++iI)
	{
		fprintf(stderr, "%c", oBitIndex.GetBit(iI) ? '1' : '0');
	}
	fprintf(stderr, "\n");

	oBitIndex.SetBit(0, 1);
	oBitIndex.SetBit(1, 1);
	oBitIndex.SetBit(2, 1);

	oBitIndex.SetBit(3, 0);
	oBitIndex.SetBit(4, 0);
	oBitIndex.SetBit(5, 0);

	oBitIndex.SetBit(6, 1);
	oBitIndex.SetBit(7, 1);
	oBitIndex.SetBit(8, 1);

	for (UINT_32 iI = 0; iI < C_BITS_NUM; ++iI)
	{
		fprintf(stderr, "%c", oBitIndex.GetBit(iI) ? '1' : '0');
	}
	fprintf(stderr, "\nExpand index:\n");

	for (UINT_32 iI = C_BITS_NUM; iI < C_BITS_NUM * 2; ++iI)
	{
		oBitIndex.SetBit(iI, 0);
		fprintf(stderr, "Bit %d, used bytes: %d\n", iI, oBitIndex.GetUsedSize());
	}
	fprintf(stderr, "\n");

	for (UINT_32 iI = C_BITS_NUM; iI < C_BITS_NUM * 2; ++iI)
	{
		oBitIndex.SetBit(iI, iI % 2);
		fprintf(stderr, "%c", (iI % 2) ? '1' : '0');
	}
	fprintf(stderr, "\n");

	for (UINT_32 iI = 0; iI < C_BITS_NUM * 2; ++iI)
	{
		fprintf(stderr, "%c", oBitIndex.GetBit(iI) ? '1' : '0');
	}
	fprintf(stderr, "\n");

	// make valgrind happy
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.

