#ifndef _FN_RUS_PLURAL_HPP__
#define _FN_RUS_PLURAL_HPP__ 1

#include <CDT.hpp>

#include <CTPP2VMSyscall.hpp>


namespace CTPP
{

class FnRussianPlural:
  public CTPP::SyscallHandler
{

public:
	FnRussianPlural();
	~FnRussianPlural() throw();

	virtual INT_32 Handler(CTPP::CDT            * aArguments,
	                       const UINT_32   iArgNum,
	                       CTPP::CDT            & oCDTRetVal,
	                       CTPP::Logger         & oLogger);


	virtual CCHAR_P GetName() const;
};

} // namespace 
#endif // _FN_RUS_PLURAL_HPP__
// End.

