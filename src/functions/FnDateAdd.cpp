#include "CTPP2SysHeaders.h"
#ifdef ICU_SUPPORT
#include <FnDateAdd.hpp>
#include <iostream>
#include <regex> 
#include <time.h>
#include <unicode/gregocal.h>

// See http://icu-project.org/apiref/icu4j/com/ibm/icu/text/SimpleDateFormat.html
namespace CTPP
{

FnDateAdd::FnDateAdd() 
{ 
    UErrorCode err = U_ZERO_ERROR;
//	parser = new SimpleDateFormat(UnicodeString("yyyy-MM-dd' 'hh:mm:ssZZZZZ")	, err);
	tparser = new icu::SimpleDateFormat(icu::UnicodeString("yyyy-MM-dd' 'HH:mm:ss")	, err);
	dparser = new icu::SimpleDateFormat(icu::UnicodeString("yyyy-MM-dd")	, err);
}

// Деструктор
FnDateAdd::~FnDateAdd() throw() { 
	delete tparser;
	delete dparser;
}


// Обработчик
INT_32 FnDateAdd::Handler(CTPP::CDT          * aArguments,
                            const UINT_32 iArgNum,
                            CTPP::CDT           & oCDTRetVal,
			    CTPP::Logger& logger)
{
	UDate date;
	// Наша функция принимает два агрумента
	if (iArgNum != 2  ) { return -1; }

	std::string isodate = aArguments[1].GetString();
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
	icu::ParsePosition pp(0);
	date = tparser->parse(icu::UnicodeString(isodate.c_str()), pp);
	if(!date) { 
		pp=0;
		date = dparser->parse(icu::UnicodeString(isodate.c_str()), pp);
	}
	
// parse the delta
	const char* delta = aArguments[0].GetString().c_str();
	int n;
	char *next;
	UCalendarDateFields field = UCAL_YEAR;
	n = strtod(delta,&next);
	if(next == delta) {
		n = 0;
	} else { 
		while(*next==' ') next++;
		if(*next) { 
			if     (!strncmp(next,"month",5)) field = UCAL_MONTH;
			else if(!strncmp(next,"week" ,4)) field = UCAL_WEEK_OF_YEAR;
			else if(!strncmp(next,"day"  ,3)) field = UCAL_DATE;
			else if(!strncmp(next,"year" ,4)) field = UCAL_YEAR;
			else n = 0;
		} else { 
			n = 0;
		}
	}


// now perform the arithmetics

	UErrorCode err = U_ZERO_ERROR;	
	icu::GregorianCalendar gc(err);
	gc.setTime(date, err);
	gc.add(field, n, err);
    
	icu::UnicodeString ret;
	std::string s;
	icu::FieldPosition fpos;
	tparser->format(gc,ret, fpos).toUTF8String<std::string> (s) ;
	oCDTRetVal = s.c_str();
	return 0;
}

// Имя функции
const CCHAR_8* FnDateAdd::GetName() const { return "date_add"; }


} // namespace 


// End
#endif
