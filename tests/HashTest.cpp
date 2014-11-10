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
 *      HashTest.hpp
 *
 * $CTPP$
 */
#include <CTPP2HashTable.hpp>

#include <stdio.h>

#ifdef HAVE_SYSEXITS_H
#include <sysexits.h>
#endif

using namespace CTPP;


int main(void)
{
	HashTable oHashTable;

	oHashTable.Put("1", 1, 12345);
	oHashTable.Put("12", 2, 23456);
	oHashTable.Put("123", 3, 34567);
	oHashTable.Put("1234", 4, 45678);
	oHashTable.Put("12345", 5, 56789);


	fprintf(stdout, "1 => %d\n", INT_32(oHashTable.Get("1", 1)));
	fprintf(stdout, "12 => %d\n", INT_32(oHashTable.Get("12", 2)));
	fprintf(stdout, "123 => %d\n", INT_32(oHashTable.Get("123", 3)));
	fprintf(stdout, "1234 => %d\n", INT_32(oHashTable.Get("1234", 4)));
	fprintf(stdout, "12345 => %d\n", INT_32(oHashTable.Get("12345", 5)));


	// make valgrind happy
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.

