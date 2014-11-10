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
 *      CTPP2MT.cpp
 *
 * $CTPP$
 */
#include <pthread.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "CTPP2FileLogger.hpp"

#include "CTPP2StringOutputCollector.hpp"
#include "CTPP2SyscallFactory.hpp"

#include "CTPP2VMFileLoader.hpp"
#include "CTPP2VM.hpp"
#include "CTPP2VMSTDLib.hpp"

using namespace CTPP;

#define MAX_ITERATIONS 10000

#define MAX_THREADS 100

// Thread context
struct ThreadContext
{
	// Output mutex
	pthread_mutex_t     output_mutex;
	// Template access mutex
	pthread_mutex_t     template_mutex;
	// Max handlers
	INT_32              max_handlers;
	// Set of templates
	STLW::map<STLW::string, VMFileLoader *>    templates;
};

// Thread function
static void * ThreadFunction(void * pContext)
{
	ThreadContext * pThreadContext = (ThreadContext *)pContext;

	pthread_mutex_lock(&(pThreadContext -> output_mutex));
	fprintf(stderr, "Initilalizing...\n");
	pthread_mutex_unlock(&(pThreadContext -> output_mutex));

	// Create per-thread VM instance

	// Syscall factory
	SyscallFactory * pSyscallFactory = new SyscallFactory(pThreadContext -> max_handlers);
	// Init standard library
	STDLibInitializer::InitLibrary(*pSyscallFactory);
	// Virtual machine
	VM * pVM = new VM(pSyscallFactory);
	// Okay, all done with thread-specific

	// Fill data
	CDT oData;
	oData["hello"] = "Hello, World!";

	pthread_mutex_lock(&(pThreadContext -> output_mutex));
	fprintf(stderr, "Okay, ready to work\n");
	pthread_mutex_unlock(&(pThreadContext -> output_mutex));

	FileLogger oLogger(stderr);

	// Perform some work
	const VMMemoryCore * pVMMemoryCore = NULL;
	for (UINT_32 iCount = 0; iCount < MAX_ITERATIONS; ++iCount)
	{
		STLW::string sResult;
		StringOutputCollector  oDataCollector(sResult);

		// Get template, thread-safe
		pthread_mutex_lock(&(pThreadContext -> template_mutex));
		STLW::map<STLW::string, VMFileLoader *>::iterator itLoader = pThreadContext -> templates.find("hello.ct2");
		if (itLoader == pThreadContext -> templates.end())
		{
			continue;
		}

		pVMMemoryCore = itLoader -> second -> GetCore();
		pthread_mutex_unlock(&(pThreadContext -> template_mutex));

		// Run VM
		pVM -> Init(pVMMemoryCore, &oDataCollector, &oLogger);
		UINT_32 iIP = 0;
		pVM -> Run(pVMMemoryCore, &oDataCollector, iIP, oData, &oLogger);

		// All done, print results
		pthread_mutex_lock(&(pThreadContext -> output_mutex));
		fwrite(sResult.c_str(), sResult.size(), 1, stdout);
		pthread_mutex_unlock(&(pThreadContext -> output_mutex));
	}

	delete pVM;
	delete pSyscallFactory;

return NULL;
}

int main(int argc, char ** argv)
{
	ThreadContext oContext;

	// Load file
	oContext.templates["hello.ct2"] = new VMFileLoader("hello.ct2");
	oContext.max_handlers = 1024;

	// Init mutexes
	pthread_mutex_init(&oContext.template_mutex, NULL);
	pthread_mutex_init(&oContext.output_mutex, NULL);

	pthread_attr_t        oAttrs;
	pthread_attr_init(&oAttrs);
	pthread_attr_setdetachstate(&oAttrs, PTHREAD_CREATE_JOINABLE);

	pthread_t aThreads[MAX_THREADS];

	// Create set of threads
	printf("Creating %d threads\n", MAX_THREADS);
	INT_32 iPos;
	for (iPos = 0; iPos < MAX_THREADS; ++iPos)
	{
		INT_32 iRC = pthread_create(&aThreads[iPos], &oAttrs, ThreadFunction, &oContext);
		if (iRC != 0)
		{
			char szErrorBuf[1024 + 1];
			strerror_r(iRC, szErrorBuf, 1024);

			fprintf(stderr, "FATAL: %s", szErrorBuf);
			return EX_SOFTWARE;
		}
	}

	// Wait for results
	printf("Wait for results\n");
	for (iPos = 0; iPos < MAX_THREADS; ++iPos)
	{
		INT_32 iRC = pthread_join(aThreads[iPos], NULL);
		if (iRC != 0)
		{
			char szErrorBuf[1024 + 1];
			strerror_r(iRC, szErrorBuf, 1024);

			fprintf(stderr, "FATAL: %s", szErrorBuf);
			return EX_SOFTWARE;
		}
	}

	fprintf(stderr, "Cleanup and exit\n");
	pthread_attr_destroy(&oAttrs);
	pthread_mutex_destroy(&oContext.template_mutex);
	pthread_mutex_destroy(&oContext.output_mutex);
}
// End.
