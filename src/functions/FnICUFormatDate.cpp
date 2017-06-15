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
	tparser = new SimpleDateFormat(UnicodeString("yyyy-MM-dd' 'HH:mm:ss")	, err);
	dparser = new SimpleDateFormat(UnicodeString("yyyy-MM-dd")	, err);
}

// Деструктор
FnICUFormatDate::~FnICUFormatDate() throw() { 
	delete tparser;
	delete dparser;
}


// Обработчик
INT_32 FnICUFormatDate::Handler(CTPP::CDT          * aArguments,
                            const UINT_32 iArgNum,
                            CTPP::CDT           & oCDTRetVal,
			    CTPP::Logger& logger)
{
	UDate date;
	// Наша функция принимает один или два агрумента
	if (iArgNum != 2 && iArgNum !=1 && iArgNum !=4 ) { return -1; }
	int iFormat;       // номер аргумента с форматной строкой (в CTPP аргументы считаются сзади)
	Locale * locale = NULL;
	if( iArgNum==1) {  // один аргумент - берем текущее время
		iFormat = 0;
		date = 1000* (double)time(NULL);
	} else if(iArgNum==2 || iArgNum==4 ) {           // 2 или 4 аргумента
		iFormat  = iArgNum-1;
		int iVal = iArgNum-2;
		if (iArgNum==4) { 
			locale = new Locale(aArguments[1].GetString().c_str(), aArguments[0].GetString().c_str());
		}
		if(int v = aArguments[iVal].GetInt()) {
			date = v * 1000;
		} else {
			std::string isodate = aArguments[iVal].GetString();
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
			date = tparser->parse(UnicodeString(isodate.c_str()), pp);
			if(!date) { 
				pp=0;
				date = dparser->parse(UnicodeString(isodate.c_str()), pp);
			}
		}
	}

	UErrorCode err = U_ZERO_ERROR;	
	if(locale) { 
		SimpleDateFormat formatter(UnicodeString(aArguments[iFormat].GetString().c_str())	, *locale, err);
		UnicodeString myus;
		std::string s;
		formatter.format (date, myus).toUTF8String<std::string> (s) ;
		oCDTRetVal = s.c_str();
		delete locale;
	} else { 
		SimpleDateFormat formatter(UnicodeString(aArguments[iFormat].GetString().c_str())	, err);
		UnicodeString myus;
		std::string s;
		formatter.format (date, myus).toUTF8String<std::string> (s) ;
		oCDTRetVal = s.c_str();
	}
	return 0;
}

// Имя функции
const CCHAR_8* FnICUFormatDate::GetName() const { return "icu_format_date"; }


} // namespace 


// End
#endif
