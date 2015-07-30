#include "CTPP2SysHeaders.h"
#ifdef ICU_SUPPORT
#include <FnICUFormatDate.hpp>
#include <iostream>
#include <regex> 
#include <time.h>

// See http://icu-project.org/apiref/icu4j/com/ibm/icu/text/SimpleDateFormat.html
namespace CTPP
{

FnICUFormatDate::FnICUFormatDate() 
{ 
    UErrorCode err = U_ZERO_ERROR;
//	parser = new SimpleDateFormat(UnicodeString("yyyy-MM-dd' 'hh:mm:ssZZZZZ")	, err);
	parser = new SimpleDateFormat(UnicodeString("yyyy-MM-dd' 'hh:mm:ss")	, err);

}

// Деструктор
FnICUFormatDate::~FnICUFormatDate() throw() { 
	delete parser;
}


// Обработчик
INT_32 FnICUFormatDate::Handler(CTPP::CDT          * aArguments,
                            const UINT_32 iArgNum,
                            CTPP::CDT           & oCDTRetVal,
			    CTPP::Logger& logger)
{
	UDate date;
	// Наша функция принимает один или два агрумента
	if (iArgNum != 2 && iArgNum !=1) { return -1; }
	int iFormat;       // номер аргумента с форматной строкой (в CTPP аргументы считаются сзади)
	if( iArgNum==1) {  // один аргумент - берем текущее время
		iFormat = 0;
		date = 1000* (double)time(NULL);
	} else {           // 2 аргумента
		iFormat = 1;
		if(int v = aArguments[0].GetInt()) {
			date = v * 1000;
		
		} else {
			std::string isodate = aArguments[0].GetString();
// does not work in gc < 4.9 
//			isodate = std::regex_replace (isodate,std::regex("\\.\\d+"),std::string(""));
// so do it manually 
                        size_t dot_pos = isodate.find('.');
                        if( dot_pos != std::string::npos) { //found
                                size_t last_digit;
                                for( last_digit = dot_pos+1; last_digit < isodate.length(); last_digit++) { 
                                        if(!isdigit(isodate[last_digit])) break;
                                }
                                isodate = isodate.substr(0,dot_pos) +  isodate.substr(last_digit);
                        }
//
			ParsePosition pp(0);
			date = parser->parse(UnicodeString(isodate.c_str()), pp);
		}
	}
	UErrorCode err = U_ZERO_ERROR;	
	SimpleDateFormat formatter(UnicodeString(aArguments[iFormat].GetString().c_str())	, err);
	
	UnicodeString myus;
	UnicodeString us = formatter.format (date, myus);
	std::string s;
	us.toUTF8String<std::string> (s) ;
	oCDTRetVal = s.c_str();
	return 0;
}

// Имя функции
const CCHAR_8* FnICUFormatDate::GetName() const { return "icu_format_date"; }


} // namespace 


// End
#endif
