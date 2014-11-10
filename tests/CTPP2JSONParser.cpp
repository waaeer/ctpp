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
 *      CTPP2JSONParser.cpp
 *
 * $CTPP$
 */

#include <CTPP2JSONParser.hpp>
#include <CTPP2ParserException.hpp>

#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>

#ifdef HAVE_SYSEXITS_H
#include <sysexits.h>
#endif

using namespace CTPP;

int main(int argc, char ** argv)
{
	fprintf(stdout, "CTPP2 JSON parser v" CTPP_VERSION " (" CTPP_IDENT "). Copyright (c) 2004-2011 CTPP Dev. Team.\n\n");

	if (argc != 2) { fprintf(stderr, "usage: %s file.json\n", argv[0]); return EX_USAGE; }

	// Get file size
	struct stat oStat;
	if (stat(argv[1], &oStat) == -1 || oStat.st_size == 0) { fprintf(stderr, "ERROR: Cannot get size of file `%s`\n", argv[1]); return EX_SOFTWARE; }

	// Load file
	FILE * F = fopen(argv[1], "r");
	if (F == NULL) { fprintf(stderr, "ERROR: Cannot open file `%s` for writing\n", argv[1]); return EX_SOFTWARE; }

	// Allocate memory
	CHAR_8 * szJSONBuffer = (CHAR_8 *)malloc(oStat.st_size + 1);
	// Read from file
	if (fread(szJSONBuffer, oStat.st_size, 1, F) != 1)
	{
		fprintf(stderr, "ERROR: Cannot read from file `%s`\n", argv[1]);
		fclose(F);
		free(szJSONBuffer);
		return EX_SOFTWARE;
	}

	szJSONBuffer[oStat.st_size] = '\0';

	CDT oCDT;
	CTPP2JSONParser oJSONParser(oCDT);

	CCHAR_P szEnd = szJSONBuffer + oStat.st_size;

	try
	{
		oJSONParser.Parse(szJSONBuffer, szEnd);
	}
	catch(CTPPParserSyntaxError & e) { fprintf(stderr, "ERROR: At line %d, char %d: %s\n", e.GetLine(), e.GetLinePos(), e.what()); return EX_SOFTWARE; }

	fprintf(stderr, "%s\n", oCDT.Dump().c_str());

	// All Done
	fclose(F);

	// All done with loading
	free(szJSONBuffer);

	// make valgrind happy
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.

