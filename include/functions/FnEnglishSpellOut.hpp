#ifndef _FN_ENGLISH_SPELLOUT_HPP__
#define _FN_ENGLISH_SPELLOUT_HPP__ 1

#include <CDT.hpp>

#include <CTPP2VMSyscall.hpp>


namespace CTPP
{

class FnNumPlural;
// Наш обработчик
class FnEnglishSpellOut:
  public CTPP::SyscallHandler
{

private:
	static void triad(char * buffer, unsigned long x);

public:
	FnEnglishSpellOut();
	~FnEnglishSpellOut() throw();

	// Обработчик
	virtual INT_32 Handler(CTPP::CDT            * aArguments,
	                       const UINT_32   iArgNum,
	                       CTPP::CDT            & oCDTRetVal,
	                       CTPP::Logger         & oLogger);

	// Имя функции
	virtual CCHAR_P GetName() const;
};

} // namespace 
#endif // _FN_ENGLISH_SPELLOUT_HPP__
// End.

