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
 *      VMEmulator.cpp
 *
 * $CTPP$
 */

#include <CTPP2JSONParser.hpp>
#include <CTPP2FileOutputCollector.hpp>
#include <CTPP2FileLogger.hpp>
#include <CTPP2SyscallFactory.hpp>
#include <CTPP2VMDebugInfo.hpp>
#include <CTPP2VM.hpp>
#include <CTPP2VMFileLoader.hpp>
#include <CTPP2VMSTDLib.hpp>
#include <CTPP2VMStackException.hpp>

#include <sys/stat.h>

#ifdef HAVE_SYS_TIME_H
    #include <sys/time.h>
#endif

#ifdef WIN32
    #include <CTPP2Time.h>
#endif

#include <stdio.h>
#include <string.h>

using namespace CTPP;

int main(int argc, char ** argv)
{
	INT_32 iRetCode = EX_SOFTWARE;

	if (argc != 2 && argc != 3 && argc != 4)
	{
		fprintf(stdout, "CTPP2 virtual machine v" CTPP_VERSION " (" CTPP_IDENT "). Copyright (c) 2004-2011 CTPP Dev. Team.\n\n");
		fprintf(stderr, "usage: %s file.name [data.json] [limit of steps]\n", argv[0]);
		return EX_USAGE;
	}

	// Output
	FileOutputCollector oOutputCollector(stdout);

	// Initialize Standard CTPP library
	SyscallFactory oSyscallFactory(100);
	// Load standard library
	STDLibInitializer::InitLibrary(oSyscallFactory);

	UINT_32 iStepsLimit = 10240;
	if(argc == 4) { iStepsLimit = atoi(argv[3]); }
	else
	{
		fprintf(stderr, "WARNING: [limit of steps] not set, use default value of %d\n", iStepsLimit);
	}

	try
	{
		// Load program from file
		VMFileLoader oLoader(argv[1]);

		// Get program core
		const VMMemoryCore * pVMMemoryCore = oLoader.GetCore();

		CDT oHash(CDT::HASH_VAL);

		// Load JSON data
		if(argc >= 3)
		{
			struct stat oStat;
			// Get file size
			if (stat(argv[2], &oStat) == -1 || oStat.st_size == 0) { fprintf(stderr, "ERROR: Cannot get size of file `%s`\n", argv[2]); return EX_SOFTWARE; }

			// Allocate memory
			CHAR_8 * szJSONBuffer = (CHAR_8 *)malloc(oStat.st_size + 1);
			// Read from file
			FILE * F = fopen(argv[2], "rb");
			if (F == NULL) { fprintf(stderr, "ERROR: Cannot open file `%s` for reading\n", argv[2]); return EX_SOFTWARE; }

			fread(szJSONBuffer, oStat.st_size, 1, F);
			szJSONBuffer[oStat.st_size] = '\0';

			CTPP2JSONParser oJSONParser(oHash);
			CCHAR_P szEnd = szJSONBuffer + oStat.st_size;
			oJSONParser.Parse(szJSONBuffer, szEnd);

			// All Done
			fclose(F);
			// All done with loading
			free(szJSONBuffer);
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

		struct timeval sTimeValLocBegin;
		gettimeofday(&sTimeValLocBegin, NULL);

		oVM.Init(pVMMemoryCore, &oOutputCollector, &oLogger);
		oVM.Run(pVMMemoryCore, &oOutputCollector, iIP, oHash, &oLogger);

		struct timeval sTimeValLocEnd;
		gettimeofday(&sTimeValLocEnd, NULL);

		fprintf(stderr, "Completed in %f seconds.\n", (1.0 * (sTimeValLocEnd.tv_sec - sTimeValLocBegin.tv_sec) + 1.0 * (sTimeValLocEnd.tv_usec - sTimeValLocBegin.tv_usec) / 1000000));

		iRetCode = EX_OK;
	}
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
			fprintf(stderr, "ERROR: %s at 0x%08X (Template file \"%s\", Line %d, Pos %d)\n", e.what(), e.GetIP(), e.GetSourceName(), oVMDebugInfo.GetLine(), oVMDebugInfo.GetLinePos());
		}
		else
		{
			fprintf(stderr, "Unsupported syscall: \"%s\"\n", e.what());
		}
	}
	catch(InvalidCall           & e)
	{
		VMDebugInfo oVMDebugInfo(e.GetDebugInfo());
		fprintf(stderr, "ERROR: at 0x%08X: Invalid block name \"%s\" in file \"%s\", Line %d, Pos %d\n",  e.GetIP(), e.what(), e.GetSourceName(), oVMDebugInfo.GetLine(), oVMDebugInfo.GetLinePos());
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

	// Destroy standard library
	STDLibInitializer::DestroyLibrary(oSyscallFactory);

	// make valgrind happy
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return iRetCode;
}
// End.

