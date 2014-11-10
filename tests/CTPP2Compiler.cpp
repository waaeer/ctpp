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
 *      CTPP2Compiler.cpp
 *
 * $CTPP$
 */
#include <CTPP2Parser.hpp>
#include <CTPP2FileSourceLoader.hpp>
#include <CTPP2ParserException.hpp>
#include <CTPP2HashTable.hpp>
#include <CTPP2VMDumper.hpp>
#include <CTPP2VMOpcodes.h>

#include <sys/stat.h>

#include <stdio.h>
#include <string.h>

using namespace CTPP;

int main(int argc, char ** argv)
{
	if (argc != 3)
	{
		fprintf(stdout, "CTPP2 template compiler v" CTPP_VERSION " (" CTPP_IDENT "). Copyright (c) 2004-2011 CTPP Dev. Team.\n\n");
		fprintf(stderr, "usage: %s source.ctpp2 destination.ct2\n", argv[0]);
		return EX_USAGE;
	}

	VMOpcodeCollector  oVMOpcodeCollector;
	StaticText         oSyscalls;
	StaticData         oStaticData;
	StaticText         oStaticText;
	HashTable          oHashTable;
	CTPP2Compiler oCompiler(oVMOpcodeCollector, oSyscalls, oStaticData, oStaticText, oHashTable);

	try
	{
		// Load template
		CTPP2FileSourceLoader oSourceLoader;
		oSourceLoader.LoadTemplate(argv[1]);

		// Create template parser
		CTPP2Parser oCTPP2Parser(&oSourceLoader, &oCompiler, argv[1]);

		// Compile template
		oCTPP2Parser.Compile();
	}
	catch(CTPPLogicError        & e)
	{
		fprintf(stderr, "ERROR: %s\n", e.what());
		return EX_SOFTWARE;
	}
	catch(CTPPUnixException     & e)
	{
		fprintf(stderr, "ERROR: I/O in %s: %s\n", e.what(), strerror(e.ErrNo()));
		return EX_SOFTWARE;
	}
	catch(CTPPParserSyntaxError & e)
	{
		fprintf(stderr, "ERROR: At line %d, pos. %d: %s\n", e.GetLine(), e.GetLinePos(), e.what());
		return EX_SOFTWARE;
	}
	catch (CTPPParserOperatorsMismatch &e)
	{
		fprintf(stderr, "ERROR: At line %d, pos. %d: expected %s, but found </%s>\n", e.GetLine(), e.GetLinePos(), e.Expected(), e.Found());
		return EX_SOFTWARE;
	}
	catch(...)
	{
		fprintf(stderr, "ERROR: Bad thing happened.\n");
		return EX_SOFTWARE;
	}

	// Get program core
	UINT_32 iCodeSize = 0;
	const VMInstruction * oVMInstruction = oVMOpcodeCollector.GetCode(iCodeSize);
	// Dump program
	VMDumper oDumper(iCodeSize, oVMInstruction, oSyscalls, oStaticData, oStaticText, oHashTable);
	UINT_32 iSize = 0;
	const VMExecutable * aProgramCore = oDumper.GetExecutable(iSize);

	// Open file only if compilation is done
	FILE * FW = fopen(argv[2], "wb");
	if (FW == NULL) { fprintf(stderr, "ERROR: Cannot open destination file `%s` for writing\n", argv[2]); return EX_SOFTWARE; }

	// Write to the disc
	fwrite(aProgramCore, iSize, 1, FW);
	// All done
	fclose(FW);

	// Make valgrind happy
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
