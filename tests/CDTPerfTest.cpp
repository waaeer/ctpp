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
 *      CDTPerfTest.cpp
 *
 * $CTPP$
 */
#include <CDT.hpp>

#include <stdio.h>

#ifdef HAVE_SYSEXITS_H
    #include <sysexits.h>
#endif

#include <time.h>

#ifdef HAVE_SYS_TIME_H
    #include <sys/time.h>
#endif

#ifdef WIN32
    #include <CTPP2Time.h>
#endif

using namespace CTPP;

int main(void)
{
	try
	{
		struct timeval sStartTime;
		struct timeval sEndTime;

		gettimeofday(&sStartTime, NULL);
		CHAR_8 szBuf[1024 + 1];
		CDT oCDT_array(CDT::ARRAY_VAL);
		for (UINT_32 iI = 0; iI < 10000; ++iI)
		{
			if (iI % 2 == 0)  { oCDT_array[iI] = iI; }
			else
			{
				for (UINT_32 iJ = 0; iJ < 100; ++iJ)
				{
					snprintf(szBuf, 1024, "_%d_%d_", iI, iJ);

					oCDT_array[iI][szBuf][10] = iI;
					// PERL analog:
					// $AArray[$II] -> {$SKey} -> [10] = $II;

					// PHP analog:
					// $AArray[$II][$SKey][10] = $II;
				}
			}
		}

		gettimeofday(&sEndTime, NULL);

		fprintf(stderr, "Time: %f\n", ((sEndTime.tv_sec - sStartTime.tv_sec) + 1.0 * (sEndTime.tv_usec - sStartTime.tv_usec) / 1000000));

		fprintf(stderr, "ARRAY size: %d\n", oCDT_array.Size());
	}
	catch(CDTTypeCastException &e)
	{
		fprintf(stderr, "ERROR: Type cast %s\n", e.what());
	}
	catch(std::exception &e)
	{
		fprintf(stderr, "ERROR: std %s\n", e.what());
	}
	catch(...)
	{
		fprintf(stderr, "Ouch!\n");
	}

	// make valgrind happy
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.

