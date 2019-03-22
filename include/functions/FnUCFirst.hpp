#ifndef _FN_UCFIRST_HPP__
#define _FN_UCFIRST_HPP__ 1
#include "CTPP2SysHeaders.h"

#include <CDT.hpp>

#include <CTPP2VMSyscall.hpp>
#include <unicode/smpdtfmt.h>

namespace CTPP
{
// Наш обработчик
class FnUCFirst:
  public CTPP::SyscallHandler
{
private:


public:
	FnUCFirst();
	~FnUCFirst() throw();

	// Обработчик
	virtual INT_32 Handler(CTPP::CDT     * aArguments,
	                       const UINT_32   iArgNum,
	                       CTPP::CDT     & oCDTRetVal,
	                       CTPP::Logger  & oLogger);

	// Имя функции
	virtual CCHAR_P GetName() const;
};

} // namespace 
#endif // _FN_UCFIRST_HPP__
// End.

