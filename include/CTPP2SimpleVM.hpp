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
 *      SimpleVM.hpp
 *
 * $CTPP$
 */
#ifndef _SIMPLE_VM_HPP__
#define _SIMPLE_VM_HPP__ 1

/**
  @file SimpleVM.hpp
  @brief Virtual Machine, simple use case
*/

#include "CTPP2Types.h"
#include <string>
#include <stdio.h>

namespace CTPP // C++ Template Engine
{

// FWD
class CDT;
class OutputCollector;
class Logger;
class VMLoader;
struct VMMemoryCore;

/**
  @class SimpleVM SimpleVM.hpp <SimpleVM.hpp>
  @brief Simple CTPP2 Virtual machine
*/
class CTPP2DECL SimpleVM
{
public:
	/**
	  @brief Constructor
	  @param iIMaxFunctions - max. number of functions in syscall factory
	  @param iIMaxArgStackSize - max. size of arguments stack
	  @param iIMaxCodeStackSize - max. size of code stack
	  @param iIMaxSteps - max. number of executed steps
	  @param iIDebugLevel - debugging level
	*/
	SimpleVM(const UINT_32  & iIMaxFunctions     = 1024,
	         const UINT_32  & iIMaxArgStackSize  = 4096,
	         const UINT_32  & iIMaxCodeStackSize = 4096,
	         const UINT_32  & iIMaxSteps         = 10240,
	         const UINT_32  & iIDebugLevel       = 0);

	/**
	  @brief Run program
	  @param oData - initial data
	  @param oLoader - template loader
	  @param sResult - result collector, STL string
	  @param oLogger - logger object
	  @return instruction pointer
	*/
	UINT_32 Run(CDT & oData, const VMLoader & oLoader, std::string & sResult, Logger & oLogger);

	/**
	  @brief Run program
	  @param oData - initial data
	  @param oLoader - template loader
	  @param F - result collector, file handle
	  @param oLogger - logger object
	  @return instruction pointer
	*/
	UINT_32 Run(CDT & oData, const VMLoader & oLoader, FILE * F, Logger & oLogger);

	/**
	  @brief Run program
	  @param oData - initial data
	  @param oLoader - template loader
	  @param oCollector - result collector, CTPP::OutputCollector
	  @param oLogger - logger object
	  @return instruction pointer
	*/
	UINT_32 Run(CDT & oData, const VMLoader & oLoader, OutputCollector & oCollector, Logger & oLogger);

	/**
	  @brief Run program
	  @param oData - initial data
	  @param pCore - memory core
	  @param sResult - result collector, STL string
	  @param oLogger - logger object
	  @return instruction pointer
	*/
	UINT_32 Run(CDT & oData, const VMMemoryCore * pCore, std::string & sResult, Logger & oLogger);

	/**
	  @brief Run program
	  @param oData - initial data
	  @param pCore - memory core
	  @param F - result collector, file handle
	  @param oLogger - logger object
	  @return instruction pointer
	*/
	UINT_32 Run(CDT & oData, const VMMemoryCore * pCore, FILE * F, Logger & oLogger);

	/**
	  @brief Run program
	  @param oData - initial data
	  @param pCore - memory core
	  @param oCollector - result collector, CTPP::OutputCollector
	  @param oLogger - logger object
	  @return instruction pointer
	*/
	UINT_32 Run(CDT & oData, const VMMemoryCore * pCore, OutputCollector & oCollector, Logger & oLogger);

	/**
	  @brief A destructor
	*/
	~SimpleVM() throw();

private:
	// FWD
	struct _SimpleVM;

	/** Pompl for Simple VM internal data */
	_SimpleVM    * pSimpleVM;

	// Prevent object copying
	SimpleVM(const SimpleVM & oRhs);
	SimpleVM & operator=(const SimpleVM & oRhs);
};

} // namespace CTPP
#endif // _SIMPLE_VM_HPP__
// End.
