#include "CTPP2SysHeaders.h"
#include "CTPP2Logger.hpp"
#include <FnTime.hpp>
#include <time.h>

namespace CTPP
{

INT_32 FnTime::Handler(
	CDT          * aArguments,
	const UINT_32 iArgNum,
	CDT         & oCDTRetVal,
	Logger      & logger)
{
	if (iArgNum != 0  ) { 
		logger.Emerg("Usage: TIME()");
		return -1; 
	}

	oCDTRetVal = time(NULL);
	return 0;
}

// Имя функции
const CCHAR_8* FnTime::GetName() const { return "time"; }


} // namespace 


// End

