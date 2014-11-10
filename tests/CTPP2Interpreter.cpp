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
 *      CTPP2Interpreter.cpp
 *
 * $CTPP$
 */

#include <CTPP2Compiler.hpp>
#include <CTPP2JSONParser.hpp>
#include <CTPP2FileOutputCollector.hpp>
#include <CTPP2FileLogger.hpp>
#include <CTPP2FileSourceLoader.hpp>
#include <CTPP2HashTable.hpp>
#include <CTPP2JSONFileParser.hpp>
#include <CTPP2Parser.hpp>
#include <CTPP2ParserException.hpp>
#include <CTPP2StaticData.hpp>
#include <CTPP2StaticText.hpp>
#include <CTPP2SyscallFactory.hpp>
#include <CTPP2VMDebugInfo.hpp>
#include <CTPP2VMDumper.hpp>
#include <CTPP2VM.hpp>
#include <CTPP2VMFileLoader.hpp>
#include <CTPP2VMOpcodeCollector.hpp>
#include <CTPP2VMSTDLib.hpp>
#include <CTPP2VMStackException.hpp>
#include <CTPP2GetText.hpp>


#include <stdio.h>

using namespace CTPP;

int main(int argc, char ** argv)
{
	INT_32 iRetCode = EX_SOFTWARE;

	if (argc != 2 && argc != 3 && argc != 4 && argc != 5)
	{
		fprintf(stdout, "CTPP2 interpreter v" CTPP_VERSION " (" CTPP_IDENT "). Copyright (c) 2004-2011 CTPP Dev. Team.\n\n");
		fprintf(stderr, "usage: %s file.name [data.json] [translation.mo | 0] [limit of steps]\n", argv[0]);
		return EX_USAGE;
	}

	// Output
	FileOutputCollector oOutputCollector(stdout);

	// Initialize Standard CTPP library
	SyscallFactory oSyscallFactory(100);
	// Load standard library
	STDLibInitializer::InitLibrary(oSyscallFactory);

	CTPP2GetText oGetText;
	if (argc >= 4 && strncmp(argv[3], "0", sizeof(argv[3])) != 0) { oGetText.AddTranslation(argv[3], "test", "unknown"); }
	oGetText.InitSTDLibFunction(oSyscallFactory);
	oGetText.SetLanguage(oSyscallFactory, "unknown");
	oGetText.SetDefaultDomain("test");

	UINT_32 iStepsLimit = 10240;
	if(argc == 5) { iStepsLimit = atoi(argv[4]); }
	else
	{
		fprintf(stderr, "WARNING: [limit of steps] not set, use default value of %d\n", iStepsLimit);
	}

	try
	{

		VMOpcodeCollector  oVMOpcodeCollector;
		StaticText         oSyscalls;
		StaticData         oStaticData;
		StaticText         oStaticText;
		HashTable          oHashTable;
		CTPP2Compiler oCompiler(oVMOpcodeCollector, oSyscalls, oStaticData, oStaticText, oHashTable);

		// Load template
		CTPP2FileSourceLoader oSourceLoader;
		oSourceLoader.LoadTemplate(argv[1]);

		// Create template parser
		CTPP2Parser oCTPP2Parser(&oSourceLoader, &oCompiler, argv[1]);

		// Compile template
		oCTPP2Parser.Compile();

		// Get program core
		UINT_32 iCodeSize = 0;
		const VMInstruction * oVMInstruction = oVMOpcodeCollector.GetCode(iCodeSize);
		// Dump program
		VMDumper oDumper(iCodeSize, oVMInstruction, oSyscalls, oStaticData, oStaticText, oHashTable);
		UINT_32 iSize = 0;
		const VMExecutable * aProgramCore = oDumper.GetExecutable(iSize);

		// Get program core
		const VMMemoryCore oVMMemoryCore(aProgramCore);

		CDT oHash(CDT::HASH_VAL);

		// Load JSON data
		if(argc >= 3)
		{
			// Our data
			CTPP2JSONFileParser oJSONFileParser(oHash);
			oJSONFileParser.Parse(argv[2]);
		}
		else
		{
			fprintf(stderr, "WARNING: [data.json] not given\n");
		}

		// Logger
		FileLogger oLogger(stderr);

		// Run program
		VM oVM(&oSyscallFactory, 10240, 10240, iStepsLimit);
		UINT_32 iIP = 0;

		oVM.Init(&oVMMemoryCore, &oOutputCollector, &oLogger);
		oVM.Run(&oVMMemoryCore, &oOutputCollector, iIP, oHash, &oLogger);

		iRetCode = EX_OK;
	}
	catch(CTPPParserSyntaxError       & e) { fprintf(stderr, "ERROR: At line %d, pos. %d: %s\n", e.GetLine(), e.GetLinePos(), e.what()); }
	catch(CTPPParserOperatorsMismatch & e) { fprintf(stderr, "ERROR: At line %d, pos. %d: expected %s, but found </%s>\n", e.GetLine(), e.GetLinePos(), e.Expected(), e.Found()); }

	// CDT
	catch(CDTTypeCastException  & e) { fprintf(stderr, "ERROR: Type Cast %s\n", e.what());                                    }
	catch(CDTAccessException    & e) { fprintf(stderr, "ERROR: Array index out of bounds: %s\n", e.what());                   }

	// Virtual machine
	catch(IllegalOpcode         & e) { fprintf(stderr, "ERROR: Illegal opcode 0x%08X at 0x%08X\n", e.GetOpcode(), e.GetIP()); }
	catch(InvalidSyscall        & e)
	{
		if (e.GetIP() != 0)
		{
			VMDebugInfo oVMDebugInfo(e.GetDebugInfo());
			fprintf(stderr, "ERROR: %s at 0x%08X (Template file \"%s\", Line %d, Pos: %d)\n", e.what(), e.GetIP(), e.GetSourceName(), oVMDebugInfo.GetLine(), oVMDebugInfo.GetLinePos());
		}
		else
		{
			fprintf(stderr, "Unsupported syscall: \"%s\"\n", e.what());
		}
	}
	catch(CodeSegmentOverrun    & e) { fprintf(stderr, "ERROR: %s at 0x%08X\n", e.what(),  e.GetIP());                        }
	catch(StackOverflow         & e) { fprintf(stderr, "ERROR: Stack overflow at 0x%08X\n", e.GetIP());                       }
	catch(StackUnderflow        & e) { fprintf(stderr, "ERROR: Stack underflow at 0x%08X\n", e.GetIP());                      }
	catch(ExecutionLimitReached & e) { fprintf(stderr, "ERROR: Execution limit of %d step(s) reached at 0x%08X\n", iStepsLimit, e.GetIP()); }
	catch(VMException           & e) { fprintf(stderr, "ERROR: VM generic exception: %s at 0x%08X\n", e.what(), e.GetIP()); }

	// CTPP
	catch(CTPPLogicError        & e) { fprintf(stderr, "ERROR: %s\n", e.what());                                              }
	catch(CTPPUnixException     & e) { fprintf(stderr, "ERROR: I/O in %s: %s\n", e.what(), strerror(e.ErrNo()));              }
	catch(CTPPException         & e) { fprintf(stderr, "ERROR: CTPP Generic exception: %s\n", e.what());                      }

	catch(...)                             { fprintf(stderr, "ERROR: Bad thing happened.\n"); }

	// Destroy standard library
	STDLibInitializer::DestroyLibrary(oSyscallFactory);

	// make valgrind happy
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return iRetCode;
}
// End.

