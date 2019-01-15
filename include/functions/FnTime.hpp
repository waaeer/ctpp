#ifndef _FN_TIME_HPP__
#define _FN_TIME_HPP__ 1
#include "CTPP2SysHeaders.h"

#include <CDT.hpp>
#include <CTPP2VMSyscall.hpp>

namespace CTPP
{
class FnTime:
  public CTPP::SyscallHandler
{

public:
	FnTime() {};
	~FnTime() throw() {};

	// Обработчик
	virtual INT_32 Handler(CTPP::CDT     * aArguments,
	                       const UINT_32   iArgNum,
	                       CTPP::CDT     & oCDTRetVal,
	                       CTPP::Logger  & oLogger);

	// Имя функции
	virtual CCHAR_P GetName() const;
};

} // namespace
#endif // _FN_DATE_ADD_HPP__
// End.

