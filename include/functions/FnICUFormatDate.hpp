#ifndef _FN_ICU_DATE_FORMAT_HPP__
#define _FN_ICU_DATE_FORMAT_HPP__ 1
#include "CTPP2SysHeaders.h"

#ifdef ICU_SUPPORT

#include <CDT.hpp>

#include <CTPP2VMSyscall.hpp>
#include <unicode/smpdtfmt.h>

namespace CTPP
{
// Наш обработчик
class FnICUFormatDate:
  public CTPP::SyscallHandler
{
private:

	icu::SimpleDateFormat *tparser;
	icu::SimpleDateFormat *dparser;

public:
	FnICUFormatDate();
	~FnICUFormatDate() throw();

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
#endif // _FN_ICU_DATE_FORMAT_HPP__
// End.

