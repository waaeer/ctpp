
#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>

#ifdef HAVE_SYSEXITS_H
#include <sysexits.h>
#endif

#ifdef _MSC_VER
#include <Windows.h>
#else
#include <sys/time.h>
#endif

#include <iostream>
#include <sstream>


#include <CTPP2JSONParser.hpp>
#include <CTPP2ParserException.hpp>
#include <CDT.hpp>
#include <CTPP2Util.hpp>

using namespace CTPP;
using namespace std;

#ifdef _MSC_VER
unsigned long getMSTime() {
	SYSTEMTIME st;
	GetSystemTime(&st);
	return st.wSecond * 1000000 + st.wMilliseconds * 1000;
}
#else
unsigned long getMSTime() {
	struct timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}
#endif

void usage(const char * name) {
	cout << "Usage:\n\t" << name << " -[t|b] filename \n\t\t -t - test stream and non-stream CDT to JSON conversions\n\t\t -b - run stream vs non-stream CDT to JSON conversions benchmark\n\t filename - name of file with JSON object which will be used in test/benchmark" << endl;
}

int main(int argc, char ** argv) {
	CDT cdt(CDT::HASH_VAL);

	if (argc < 3 || argv[1][0] != '-' || (argv[1][1] != 't' && argv[1][1] != 'b')) {
		usage(argv[0]);
		return 3;
	}

	struct stat oStat;
	const char * filename = argv[2];
	if (stat(filename, &oStat) == -1 || oStat.st_size == 0) {
		cerr << "ERROR: Cannot get size of file " << filename << endl;
		return 2;
	}

	FILE * F = fopen(filename, "r");
	if (F == NULL) {
		cerr << "ERROR: Cannot open file " << filename << " for reading" << endl;
		return 1;
	}

	// Allocate memory
	CHAR_8 * szJSONBuffer = (CHAR_8 *)malloc(oStat.st_size + 1);
	// Read from file
	if (fread(szJSONBuffer, oStat.st_size, 1, F) != 1)
	{
		fprintf(stderr, "ERROR: Cannot read from file `%s`\n", argv[1]);
		fclose(F);
		free(szJSONBuffer);
		return EX_SOFTWARE;
	}

	szJSONBuffer[oStat.st_size] = '\0';
	CCHAR_P szEnd = szJSONBuffer + oStat.st_size;
	CTPP2JSONParser oJSONParser(cdt);
	oJSONParser.Parse(szJSONBuffer, szEnd);
	free(szJSONBuffer);

	unsigned long total = 1000;
	unsigned int tmp = 0;
	STLW::string s1;
	STLW::string s2;
	CDT2JSON(cdt, s1);
	DumpBuffer ss;
	DumpCDT2JSON(cdt, ss);
	s2.assign(ss.Data(), ss.Data() + ss.Size());
	if(s1 != s2) {
		cout << "S1 = " << s1 << endl << endl;
		cout << "S2 = " << s2 << endl << endl;
		cerr << "Results doens't match\n";
		return 3;
	}
	// -t mode
	if(argv[1][1] == 't')
		return 0;

	unsigned long stringTime = 1;
	unsigned long streamTime = 1;
	if(1) {
		unsigned long t0 = getMSTime();
		STLW::string s;
		for(unsigned long i = 0; i < total; ++i) {
			s.clear();
			CDT2JSON(cdt, s);
			tmp += s.size();
		}
		unsigned long t1 = getMSTime();

		cout << "Stats: " << total << " in " << (t1 - t0) << " microsecs, avg = " << 1.* (t1 - t0) / total << " microsec/conv, "
			<< 1000000. *  total / (t1 - t0) << endl;
		stringTime = t1 - t0;
	}
	{
		unsigned long t0 = getMSTime();
		for(unsigned long i = 0; i < total; ++i) {
			DumpBuffer ss;
			DumpCDT2JSON(cdt, ss);
		}
		unsigned long t1 = getMSTime();

		cout << "Stats: " << total << " in " << (t1 - t0) << " microsecs, avg = " << 1.* (t1 - t0) / total << " microsec/conv, "
			<< 1000000. *  total / (t1 - t0) << endl;
		streamTime = t1 - t0;
	}
	cout << "Diff = " << (stringTime - streamTime) << " ms = " << (100. * (stringTime - streamTime) / stringTime) << "%\n";


	return 0;
}
