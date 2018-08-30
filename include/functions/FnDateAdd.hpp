#ifndef _FN_DATE_ADD_HPP__
#define _FN_DATE_ADD_HPP__ 1
#include "CTPP2SysHeaders.h"

#ifdef ICU_SUPPORT

#include <CDT.hpp>

#include <CTPP2VMSyscall.hpp>
#include <unicode/smpdtfmt.h>

namespace CTPP
{
// Наш обработчик
class FnDateAdd:
  public CTPP::SyscallHandler
{
private:

	SimpleDateFormat *tparser;
	SimpleDateFormat *dparser;

public:
	FnDateAdd();
	~FnDateAdd() throw();

	// Обработчик
	virtual INT_32 Handler(CTPP::CDT     * aArguments,
	                       const UINT_32   iArgNum,
	                       CTPP::CDT     & oCDTRetVal,
	                       CTPP::Logger  & oLogger);

	// Имя функции
	virtual CCHAR_P GetName() const;
};

} // namespace 
#endif // ICU_SUPPORT
#endif // _FN_DATE_ADD_HPP__
// End.

