#include <FnRussianPlural.hpp>
// использууем функцию оттуда

#include "CTPP2Logger.hpp"
#include "FnRussianSpellOut.hpp"
#include <iostream>

namespace CTPP
{

#include "CDT.hpp"
#include<stdio.h>

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
	variants[2] = strdup(aArguments[0].GetString().c_str());
	variants[1] = strdup(aArguments[1].GetString().c_str());
	variants[0] = strdup(aArguments[2].GetString().c_str());

	long n = aArguments[3].GetInt();
	result = CTPP::FnRussianSpellOut::numeral_case(n, variants);
	oCDTRetVal = strdup(result);
	free((void*)variants[0]); 
	free((void*)variants[1]);
	free((void*)variants[2]);
	return 0;
}

// Имя функции
const CCHAR_8* FnRussianPlural::GetName() const { return "russian_plural"; }


} // namespace 



// End
