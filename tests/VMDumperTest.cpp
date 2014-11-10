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
 *      VMDumperTest.cpp
 *
 * $CTPP$
 */

#include <CTPP2Exception.hpp>
#include <CTPP2VMFileLoader.hpp>
#include <CTPP2VMDumper.hpp>

#include <sys/stat.h>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include <stdio.h>
#include <string.h>

using namespace CTPP;

int main(int argc, char ** argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "usage: %s file_in.ct2 filename_out.ct2\n", argv[0]);
		return EX_USAGE;
	}

	try
	{
		// Load program from file
		VMFileLoader oLoader(argv[1]);

		// Get program core
		const VMMemoryCore * pVMMemoryCore = oLoader.GetCore();

		// Dump program
		VMDumper oDumper(*pVMMemoryCore);
		UINT_32 iSize = 0;
		const VMExecutable * aProgramCore = oDumper.GetExecutable(iSize);

		// Open file only if compilation is done
		FILE * FW = fopen(argv[2], "wb");
		if (FW == NULL) { fprintf(stderr, "ERROR: Cannot open destination file `%s` for writing\n", argv[2]); return EX_SOFTWARE; }

		// Write to the disc
		fwrite(aProgramCore, iSize, 1, FW);
		// All done
		fclose(FW);

	}
	catch(CTPPLogicError        & e) { fprintf(stderr, "ERROR: %s\n", e.what());                                              }
	catch(CTPPUnixException     & e) { fprintf(stderr, "ERROR: I/O in %s: %s\n", e.what(), strerror(e.ErrNo()));              }
	catch(std::exception        & e) { fprintf(stderr, "ERROR: %s\n", e.what());                                              }

	// make valgrind happy
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.

