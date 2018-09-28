// Подключаем определение нашего класса
#include <FnRussianSpellOut.hpp>
#include<iostream>

#include "CTPP2Logger.hpp"
namespace CTPP
{

#include "CDT.hpp"
FnRussianSpellOut::FnRussianSpellOut() 
{ 
 

}

// Деструктор
FnRussianSpellOut::~FnRussianSpellOut() throw() { 
}

void FnRussianSpellOut::triad(char * buffer, unsigned long x, int gender) { 
	static const char* hundreds[] = { "", "сто","двести","триста","четыреста","пятьсот","шестьсот","семьсот","восемьсот","девятьсот"};
	static const char* tens[]     = { "", "","двадцать","тридцать","сорок","пятьдесят","шестьдесят","семьдесят","восемьдесят","девяносто"};
	static const char* ones[2][2] = {{ "один","одна"},{ "два","две"}};
	static const char* digits[]	  = { "три","четыре","пять","шесть","семь","восемь","девять","десять","одиннадцать","двенадцать","тринадцать","четырнадцать","пятнадцать","шестнадцать","семнадцать","восемнадцать","девятнадцать"};
	int has = 0;
	if(x >999) { // error!
		return;
	}
	if(x >=100) { 
		strcat(buffer, hundreds[ x / 100 ]);
		x = x % 100;
		has = 1;
	}
	if(x>=20) { 
		if(has) strcat(buffer, " ");
		strcat(buffer, tens[x/10]);
		x = x % 10;
		has = 1;
	}
	if(x > 2) {
		if(has) strcat(buffer, " ");
		strcat(buffer, digits[x-3]);
	} else if (x > 0) { 
		if(has) strcat(buffer, " ");
		strcat(buffer, ones[x-1][gender-1]);
	}
}

const char * FnRussianSpellOut::numeral_case(long x, const char** variants) { 
	x = x % 100;
	if ( x >=11 && x < 20) return variants[2];
	x = x % 10;
	return variants[ 
		  x == 1          ? 0
		: x >= 2 && x <=4 ? 1
		:                   2
	];
}

// Обработчик
INT_32 FnRussianSpellOut::Handler(CTPP::CDT          * aArguments,
                            const UINT_32 iArgNum,
                            CTPP::CDT           & oCDTRetVal,
			    CTPP::Logger& logger)
{
	if (iArgNum != 1) { return -1; }
	char buffer[320];
	*buffer = '\0';
		
	double value = atof(aArguments[0].GetString().c_str()); // toDo: arbitrary length numbers
	static const char* triad_names[4][3] = {
		{ "", "", ""},
		{ "тысяча","тысячи", "тысяч"},
		{ "миллион", "миллиона", "миллионов"},
		{ "миллиард", "миллиарда", "миллиардов"},
	};
	static long base[]    = { 1, 1000, 1000000, 1000000000 };
	static int  genders[] = { 1, 2, 1, 1 };
	if(value==0) { 
		strcat(buffer, "ноль ");
		strcat(buffer, triad_names[0][2]);
		oCDTRetVal = buffer;
		return 0;
	}
	unsigned long accum = 0;
	for(int i = 3; i>=0; i--) { 
		unsigned long x = (unsigned long) ((value - accum) / base[i]) 	;
//		std::cerr << "i="<<i<<" x="<<x<<std::endl;
		if(x) { 
			triad (buffer + strlen(buffer), x, genders[i]); 
		}
		if(x || i==0) { 			
			const char *triad_name = numeral_case(x, triad_names[i]);
			if(strlen(triad_name)>0) strcat(buffer, " ");
			strcat(buffer, triad_name);
			if(i>0) strcat(buffer, " "); 
		}
		accum += x * base[i];
	}
//	std::cerr <<  "buffer="<< buffer<<std::endl;
	oCDTRetVal = buffer;	

	return 0;
}

// Имя функции
const CCHAR_8* FnRussianSpellOut::GetName() const { return "russian_spellout"; }


} // namespace 

// End
