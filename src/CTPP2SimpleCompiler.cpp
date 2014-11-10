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
 *      SimpleCompiler.hpp
 *
 * $CTPP$
 */
#include "CTPP2SimpleCompiler.hpp"

#include "CTPP2Compiler.hpp"
#include "CTPP2FileSourceLoader.hpp"
#include "CTPP2Parser.hpp"

#include "CTPP2SyscallFactory.hpp"
#include "CTPP2VM.hpp"
#include "CTPP2VMDumper.hpp"
#include "CTPP2VMLoader.hpp"
#include "CTPP2VMOpcodeCollector.hpp"


namespace CTPP // C++ Template Engine
{

//
// PIMPL
//
struct SimpleCompiler::_SimpleCompiler
{
	// Dumper object
	VMDumper           * vm_dumer;
	// Template core
	UINT_32              vm_executable_size;
	// Template core
	const VMExecutable * vm_executable;
	// Template core
	const VMMemoryCore   vm_core;
//
//		UINT_32 iSize = 0;
//		const VMExecutable * aProgramCore = oDumper.GetExecutable(iSize);
//
//		// Get program core
//		const VMMemoryCore oVMMemoryCore(aProgramCore);

	_SimpleCompiler(VMDumper * pVMDumper);

	~_SimpleCompiler() throw();
};

//
// C-tor
//
SimpleCompiler::_SimpleCompiler::_SimpleCompiler(VMDumper * pVMDumper): vm_dumer(pVMDumper), vm_executable(vm_dumer -> GetExecutable(vm_executable_size)), vm_core(vm_executable)
{
	;;
}

//
// A destructor
//
SimpleCompiler::_SimpleCompiler::~_SimpleCompiler() throw()
{
	delete vm_dumer;
}


//
// Constructor
//
SimpleCompiler::SimpleCompiler(const std::string & sSourceFile)
{
	VMOpcodeCollector  oVMOpcodeCollector;
	StaticText         oSyscalls;
	StaticData         oStaticData;
	StaticText         oStaticText;
	HashTable          oHashTable;
	CTPP2Compiler oCompiler(oVMOpcodeCollector, oSyscalls, oStaticData, oStaticText, oHashTable);

	// Load template
	CTPP2FileSourceLoader oSourceLoader;
	oSourceLoader.LoadTemplate(sSourceFile.c_str());

	// Create template parser
	CTPP2Parser oCTPP2Parser(&oSourceLoader, &oCompiler, sSourceFile);

	// Compile template
	oCTPP2Parser.Compile();

	// Get program core
	UINT_32 iCodeSize = 0;
	const VMInstruction * oVMInstruction = oVMOpcodeCollector.GetCode(iCodeSize);

	pSimpleCompiler = new _SimpleCompiler(new VMDumper(iCodeSize, oVMInstruction, oSyscalls, oStaticData, oStaticText, oHashTable));
}

//
// Save compiled data to file
//
UINT_32 SimpleCompiler::Save(const std::string & sCompiledFile) const
{
	INT_32 iRC = -1;

	// Open file only if compilation is done
	FILE * FW = fopen(sCompiledFile.c_str(), "wb");
	if (FW == NULL) { return iRC; }

	// Write to the disc
	if (fwrite(pSimpleCompiler -> vm_executable, pSimpleCompiler -> vm_executable_size, 1, FW) == 1) { iRC =0 ; }

	// All done
	fclose(FW);

return iRC;
}


//
// Get memory core
//
const VMMemoryCore * SimpleCompiler::GetCore() const
{
	return &(pSimpleCompiler -> vm_core);
}


//
// A destructor
//
SimpleCompiler::~SimpleCompiler() throw()
{
	delete pSimpleCompiler;
}

} // namespace CTPP
// End.
