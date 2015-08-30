#include <FnRussianPlural.hpp>
// использууем функцию оттуда

#include "CTPP2Logger.hpp"
#include "FnRussianSpellOut.hpp"
#include <iostream>

namespace CTPP
{

#include "CDT.hpp"


FnRussianPlural::FnRussianPlural() 
{ 
 

}

// Деструктор
FnRussianPlural::~FnRussianPlural() throw() { 
}


// Обработчик
INT_32 FnRussianPlural::Handler(CTPP::CDT          * aArguments,
                const UINT_32 iArgNum,
                CTPP::CDT           & oCDTRetVal,
			    CTPP::Logger& logger)
{

	const char *variants[3];
	const char * result;
	variants[0] = aArguments[2].GetString().c_str();
	variants[1] = aArguments[1].GetString().c_str();
	variants[2] = aArguments[0].GetString().c_str();
	result = CTPP::FnRussianSpellOut::numeral_case(aArguments[3].GetInt(), variants);
	oCDTRetVal = result;
	return 0;
}

// Имя функции
const CCHAR_8* FnRussianPlural::GetName() const { return "russian_plural"; }


} // namespace 



// End
