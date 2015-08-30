#ifndef _FN_RUSSIAN_SPELLOUT_HPP__
#define _FN_RUSSIAN_SPELLOUT_HPP__ 1

#include <CDT.hpp>

#include <CTPP2VMSyscall.hpp>


namespace CTPP
{

class FnNumPlural;
// Наш обработчик
class FnRussianSpellOut:
  public CTPP::SyscallHandler
{
friend class FnRussianPlural; //uses numeral_case()
private:
	static void triad(char * buffer, unsigned long x, int gender);
	static const char * numeral_case(long x, const char** variants);

public:
	FnRussianSpellOut();
	~FnRussianSpellOut() throw();

	// Обработчик
	virtual INT_32 Handler(CTPP::CDT            * aArguments,
	                       const UINT_32   iArgNum,
	                       CTPP::CDT            & oCDTRetVal,
	                       CTPP::Logger         & oLogger);

	// Имя функции
	virtual CCHAR_P GetName() const;
};

} // namespace 
#endif // _FN_RUSSIAN_SPELLOUT_HPP__
// End.

