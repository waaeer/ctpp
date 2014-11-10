/*-
 * Copyright (c) 2004 - 2010 CTPP Team
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
 *      CTPP2SimpleVM.hpp
 *
 * $CTPP$
 */
#include "CTPP2SimpleVM.hpp"

#include "CTPP2FileOutputCollector.hpp"
#include "CTPP2Logger.hpp"
#include "CTPP2StringOutputCollector.hpp"
#include "CTPP2VMSTDLib.hpp"
#include "CTPP2SyscallFactory.hpp"
#include "CTPP2VM.hpp"
#include "CTPP2VMLoader.hpp"

namespace CTPP // C++ Template Engine
{

//
// PIMPL
//
struct SimpleVM::_SimpleVM
{
	// Syscall factory
	SyscallFactory   syscall_factory;
	// Virtual machine
	VM               vm;

	_SimpleVM(const UINT_32  & iIMaxFunctions,
	          const UINT_32  & iIMaxArgStackSize,
	          const UINT_32  & iIMaxCodeStackSize,
	          const UINT_32  & iIMaxSteps,
	          const UINT_32  & iIDebugLevel);

	~_SimpleVM() throw();
};

//
// C-tor
//
SimpleVM::_SimpleVM::_SimpleVM(const UINT_32  & iIMaxFunctions,
                               const UINT_32  & iIMaxArgStackSize,
                               const UINT_32  & iIMaxCodeStackSize,
                               const UINT_32  & iIMaxSteps,
                               const UINT_32  & iIDebugLevel): syscall_factory(iIMaxFunctions),
                                                               vm(&syscall_factory, iIMaxArgStackSize, iIMaxCodeStackSize, iIMaxSteps, iIDebugLevel)
{
	STDLibInitializer::InitLibrary(syscall_factory);
}

//
// A destructor
//
SimpleVM::_SimpleVM::~_SimpleVM() throw()
{
	STDLibInitializer::DestroyLibrary(syscall_factory);
}

//
// C-tor
//
SimpleVM::SimpleVM(const UINT_32  & iIMaxFunctions,
                   const UINT_32  & iIMaxArgStackSize,
                   const UINT_32  & iIMaxCodeStackSize,
                   const UINT_32  & iIMaxSteps,
                   const UINT_32  & iIDebugLevel): pSimpleVM(NULL)
{
	pSimpleVM = new _SimpleVM(iIMaxFunctions, iIMaxArgStackSize, iIMaxCodeStackSize, iIMaxSteps, iIDebugLevel);
}

//
// Write Output to STL string
//
UINT_32 SimpleVM::Run(CDT & oData, const VMLoader & oLoader, std::string & sResult, Logger & oLogger)
{
	StringOutputCollector oOutputCollector(sResult);

return Run(oData, oLoader, oOutputCollector, oLogger);
}

//
// Write Output to file
//
UINT_32 SimpleVM::Run(CDT & oData, const VMLoader & oLoader, FILE * F, Logger & oLogger)
{
	FileOutputCollector oOutputCollector(F);

return Run(oData, oLoader, oOutputCollector, oLogger);
}

//
// Output to specified data collector
//
UINT_32 SimpleVM::Run(CDT & oData, const VMLoader & oLoader, OutputCollector & oCollector, Logger & oLogger)
{
	const VMMemoryCore * pCore = oLoader.GetCore();

return Run(oData, pCore, oCollector, oLogger);
}

//
// Write Output to string
//
UINT_32 SimpleVM::Run(CDT & oData, const VMMemoryCore * pCore, std::string & sResult, Logger & oLogger)
{
	StringOutputCollector oOutputCollector(sResult);

return Run(oData, pCore, oOutputCollector, oLogger);
}

//
// Write Output to file
//
UINT_32 SimpleVM::Run(CDT & oData, const VMMemoryCore * pCore, FILE * F, Logger & oLogger)
{
	FileOutputCollector oOutputCollector(F);

return Run(oData, pCore, oOutputCollector, oLogger);
}

//
// Output to specified data collector
//
UINT_32 SimpleVM::Run(CDT & oData, const VMMemoryCore * pCore, OutputCollector & oCollector, Logger & oLogger)
{
	UINT_32 iIP = 0;

	pSimpleVM -> vm.Init(pCore, &oCollector, &oLogger);
	pSimpleVM -> vm.Run(pCore, &oCollector, iIP, oData, &oLogger);

return iIP;
}

//
// A destructor
//
SimpleVM::~SimpleVM() throw()
{
	delete pSimpleVM;
}

} // namespace CTPP
// End.
