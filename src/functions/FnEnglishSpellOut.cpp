// Подключаем определение нашего класса
#include <FnEnglishSpellOut.hpp>
#include<iostream>

#include "CTPP2Logger.hpp"
namespace CTPP
{

#include "CDT.hpp"
FnEnglishSpellOut::FnEnglishSpellOut() 
{ 
 

}

// Деструктор
FnEnglishSpellOut::~FnEnglishSpellOut() throw() { 
}

void FnEnglishSpellOut::triad(char * buffer, unsigned long x) { 
	static const char* tens[]     = { "", "","twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"};
	static const char* digits[]	  = { "one", "two", "three","four","five","six","seven","eight","nine","ten","eleven","twelve","thirteen","forteen","fifteen","sixteen","seventeen","eighteen","nineteen"};
	int has = 0;
	if(x >999) { // error!
		return;
	}
	if(x >=100) { 
		strcat(buffer, digits[ x / 100 - 1 ]);
		strcat(buffer," hundred");
		x = x % 100;
		has = 1;
	}
	if(x>=20) { 
		if(has) strcat(buffer, " ");
		strcat(buffer, tens[x/10]);
		x = x % 10;
		has = 1;
	}
	if(x > 0) {
		if(has) strcat(buffer, " ");
		strcat(buffer, digits[x-1]);
	} 
}

INT_32 FnEnglishSpellOut::Handler(CTPP::CDT          * aArguments,
                            const UINT_32 iArgNum,
                            CTPP::CDT           & oCDTRetVal,
			    CTPP::Logger& logger)
{
	if (iArgNum != 1) { return -1; }
	char buffer[320];
	*buffer = '\0';
		
	double value = atof(aArguments[0].GetString().c_str()); // toDo: arbitrary length numbers
	static const char* triad_names[4] = { "", "thousand", "million", "billion" };
	static long base[]    = { 1, 1000, 1000000, 1000000000 };
	if(value==0) { 
		strcat(buffer, "ноль ");
		oCDTRetVal = buffer;
		return 0;
	}
	unsigned long accum = 0;
	for(int i = 3; i>=0; i--) { 
		unsigned long x = (unsigned long) ((value - accum) / base[i]) 	;
//		std::cerr << "i="<<i<<" x="<<x<<std::endl;
		if(x) { 
			if(strlen(buffer)>0) strcat(buffer, " ");  
			triad (buffer + strlen(buffer), x); 
		}
		if(x || i==0) { 			
			const char *triad_name = triad_names[i];
			if(strlen(triad_name)>0) strcat(buffer, " ");
			strcat(buffer, triad_name);
		}
		accum += x * base[i];
	}
//	std::cerr <<  "buffer="<< buffer<<std::endl;
	oCDTRetVal = buffer;	

	return 0;
}

// Имя функции
const CCHAR_8* FnEnglishSpellOut::GetName() const { return "english_spellout"; }


} // namespace 

// End
