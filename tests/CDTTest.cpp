/*-
 * Copyright (c) 2004 - 2011 CTPP Team
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the CTPP Team nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *      CDTTest.cpp
 *
 * $CTPP$
 */
#include <CDT.hpp>
#include <CDTSortRoutines.hpp>

#include <stdio.h>

#ifdef HAVE_SYSEXITS_H
#include <sysexits.h>
#endif

using namespace CTPP;

int main(void)
{
	CDT oCDT = 10;

	fprintf(stderr, "== INT   ===================================\n");
	fprintf(stderr, "Initial value: %d\n", INT_32(oCDT.GetInt()));
	{
		fprintf(stderr, "Post-increnent\n");
		CDT oTMP = oCDT++;
		fprintf(stderr, "TMP = CDT++: ");
		fprintf(stderr, "%d -> %d\n\n",   INT_32(oTMP.GetInt()), INT_32(oCDT.GetInt()));
	}

	{
		fprintf(stderr, "Pre-increnent\n");
		CDT oTMP = ++oCDT;
		fprintf(stderr, "TMP = ++CDT: ");
		fprintf(stderr, "%d -> %d\n\n",   INT_32(oTMP.GetInt()), INT_32(oCDT.GetInt()));
	}

	{
		fprintf(stderr, "Post-decrement\n");
		CDT oTMP = oCDT--;
		fprintf(stderr, "TMP = CDT--: ");
		fprintf(stderr, "%d -> %d\n\n",   INT_32(oTMP.GetInt()), INT_32(oCDT.GetInt()));
	}

	{
		fprintf(stderr, "Pre-decrement\n");
		CDT oTMP = --oCDT;
		fprintf(stderr, "TMP = --CDT: ");
		fprintf(stderr, "%d -> %d\n\n",   INT_32(oTMP.GetInt()), INT_32(oCDT.GetInt()));
	}

	fprintf(stderr, "operator+=(INT_32)\n");
	oCDT += 10;
	fprintf(stderr, "CDT += 10 -> %d\n\n",   INT_32(oCDT.GetInt()));

	fprintf(stderr, "operator-=(INT_32)\n");
	oCDT -= 10;
	fprintf(stderr, "CDT -= 10 -> %d\n\n",   INT_32(oCDT.GetInt()));

	fprintf(stderr, "operator*=(INT_32)\n");
	oCDT *= 10;
	fprintf(stderr, "CDT *= 10  -> %d\n\n",   INT_32(oCDT.GetInt()));

	fprintf(stderr, "operator/=(INT_32)\n");
	oCDT /= 10;
	fprintf(stderr, "CDT /= 10  -> %d\n\n", INT_32(oCDT.GetInt()));

	fprintf(stderr, "operator+(INT_32)\n");
	oCDT = oCDT + 10;
	fprintf(stderr, "CDT = CDT + 10  -> %d\n\n",   INT_32(oCDT.GetInt()));

	fprintf(stderr, "standalone operator+(int, CDT)\n");
	oCDT = 10 + oCDT;
	fprintf(stderr, "CDT = 10 + CDT  -> %d\n\n",   INT_32(oCDT.GetInt()));

	fprintf(stderr, "operator-(INT_32)\n");
	oCDT = oCDT - 10;
	fprintf(stderr, "CDT = CDT - 10  -> %d\n\n",   INT_32(oCDT.GetInt()));

	fprintf(stderr, "standalone operator-(int, CDT)\n");
	oCDT = 10 - oCDT;
	fprintf(stderr, "CDT = 10 - CDT  -> %d\n\n",   INT_32(oCDT.GetInt()));

	fprintf(stderr, "operator*(INT_32)\n");
	oCDT = oCDT * 10;
	fprintf(stderr, "CDT = CDT * 10  -> %d\n\n",   INT_32(oCDT.GetInt()));

	fprintf(stderr, "standalone operator*(int, CDT)\n");
	oCDT = 10 * oCDT;
	fprintf(stderr, "CDT = 10 * CDT  -> %d\n\n",   INT_32(oCDT.GetInt()));

	fprintf(stderr, "operator/(INT_32)\n");
	oCDT = oCDT / 10;
	fprintf(stderr, "CDT = CDT * 10  -> %d\n\n",   INT_32(oCDT.GetInt()));

	fprintf(stderr, "standalone operator*(int, CDT)\n");
	oCDT = 10 / oCDT;
	fprintf(stderr, "CDT = 10 * CDT  -> %d\n\n",   INT_32(oCDT.GetInt()));

	fprintf(stderr, "Auto-cast to float\n");
	oCDT = -1.0 * oCDT;
	fprintf(stderr, "CDT -> %f: %s\n\n", W_FLOAT(oCDT.GetFloat()), oCDT.PrintableType());


	fprintf(stderr, "== REAL  ===================================\n");
	fprintf(stderr, "Initial value: %f\n", W_FLOAT(oCDT.GetFloat()));
	{
		fprintf(stderr, "Post-increnent\n");
		CDT oTMP = oCDT++;
		fprintf(stderr, "TMP = CDT++: ");
		fprintf(stderr, "%f -> %f\n\n",   W_FLOAT(oTMP.GetFloat()), W_FLOAT(oCDT.GetFloat()));
	}

	{
		fprintf(stderr, "Pre-increnent\n");
		CDT oTMP = ++oCDT;
		fprintf(stderr, "TMP = ++CDT: ");
		fprintf(stderr, "%f -> %f\n\n",   W_FLOAT(oTMP.GetFloat()), W_FLOAT(oCDT.GetFloat()));
	}

	{
		fprintf(stderr, "Post-decrement\n");
		CDT oTMP = oCDT--;
		fprintf(stderr, "TMP = CDT--: ");
		fprintf(stderr, "%f -> %f\n\n",   W_FLOAT(oTMP.GetFloat()), W_FLOAT(oCDT.GetFloat()));
	}

	{
		fprintf(stderr, "Pre-decrement\n");
		CDT oTMP = --oCDT;
		fprintf(stderr, "TMP = --CDT: ");
		fprintf(stderr, "%f -> %f\n\n",   W_FLOAT(oTMP.GetFloat()), W_FLOAT(oCDT.GetFloat()));
	}

	fprintf(stderr, "operator+=(INT_32)\n");
	oCDT += 10;
	fprintf(stderr, "CDT += 10 -> %f\n\n",   W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "operator-=(INT_32)\n");
	oCDT -= 10;
	fprintf(stderr, "CDT -= 10 -> %f\n\n",   W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "operator*=(INT_32)\n");
	oCDT *= 10;
	fprintf(stderr, "CDT *= 10  -> %f\n\n",  W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "operator/=(INT_32)\n");
	oCDT /= 10;
	fprintf(stderr, "CDT /= 10  -> %f\n\n",  W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "operator+(INT_32)\n");
	oCDT = oCDT + 10;
	fprintf(stderr, "CDT = CDT + 10  -> %f\n\n", W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "standalone operator+(int, CDT)\n");
	oCDT = 10 + oCDT;
	fprintf(stderr, "CDT = 10 + CDT  -> %f\n\n", W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "operator-(INT_32)\n");
	oCDT = oCDT - 10;
	fprintf(stderr, "CDT = CDT - 10  -> %f\n\n", W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "standalone operator-(int, CDT)\n");
	oCDT = 10 - oCDT;
	fprintf(stderr, "CDT = 10 - CDT  -> %f\n\n", W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "operator*(INT_32)\n");
	oCDT = oCDT * 10;
	fprintf(stderr, "CDT = CDT * 10  -> %f\n\n", W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "standalone operator*(int, CDT)\n");
	oCDT = 10 * oCDT;
	fprintf(stderr, "CDT = 10 * CDT  -> %f\n\n", W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "operator/(INT_32)\n");
	oCDT = oCDT / 10;
	fprintf(stderr, "CDT = CDT * 10  -> %f\n\n", W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "standalone operator*(int, CDT)\n");
	oCDT = 10 / oCDT;
	fprintf(stderr, "CDT = 10 * CDT  -> %f\n\n", W_FLOAT(oCDT.GetFloat()));

// //
	fprintf(stderr, "operator+=(W_FLOAT)\n");
	oCDT += 10.10;
	fprintf(stderr, "CDT += 10.10 -> %f\n\n",   W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "operator-=(W_FLOAT)\n");
	oCDT -= 10.10;
	fprintf(stderr, "CDT -= 10.10 -> %f\n\n",   W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "operator*=(W_FLOAT)\n");
	oCDT *= 10.10;
	fprintf(stderr, "CDT *= 10.10  -> %f\n\n",  W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "operator/=(W_FLOAT)\n");
	oCDT /= 10.10;
	fprintf(stderr, "CDT /= 10.10  -> %f\n\n",  W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "operator+(W_FLOAT)\n");
	oCDT = oCDT + 10.10;
	fprintf(stderr, "CDT = CDT + 10.10  -> %f\n\n", W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "standalone operator+(W_FLOAT, CDT)\n");
	oCDT = 10.10 + oCDT;
	fprintf(stderr, "CDT = 10.10 + CDT  -> %f\n\n", W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "operator-(INT_32)\n");
	oCDT = oCDT - 10.10;
	fprintf(stderr, "CDT = CDT - 10.10  -> %f\n\n", W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "standalone operator-(W_FLOAT, CDT)\n");
	oCDT = 10.10 - oCDT;
	fprintf(stderr, "CDT = 10.10 - CDT  -> %f\n\n", W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "operator*(W_FLOAT)\n");
	oCDT = oCDT * 10.10;
	fprintf(stderr, "CDT = CDT * 10.10  -> %f\n\n", W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "standalone operator*(W_FLOAT, CDT)\n");
	oCDT = 10.10 * oCDT;
	fprintf(stderr, "CDT = 10.10 * CDT  -> %f\n\n", W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "operator/(W_FLOAT)\n");
	oCDT = oCDT / 10.10;
	fprintf(stderr, "CDT = CDT * 10.10  -> %f\n\n", W_FLOAT(oCDT.GetFloat()));

	fprintf(stderr, "standalone operator*(W_FLOAT, CDT)\n");
	oCDT = 10.10 / oCDT;
	fprintf(stderr, "CDT = 10.10 * CDT  -> %f\n\n", W_FLOAT(oCDT.GetFloat()));


	fprintf(stderr, "Auto-cast to complex type\n");
	oCDT = "2";
	fprintf(stderr, "CDT -> %d: %s\n", INT_32(oCDT.GetInt()), oCDT.PrintableType());
	fprintf(stderr, "CDT -> %s: %s\n\n", oCDT.GetString().c_str(), oCDT.PrintableType());

	oCDT *= 3.141;
	fprintf(stderr, "CDT -> %f: %s\n", W_FLOAT(oCDT.GetFloat()), oCDT.PrintableType());
	fprintf(stderr, "CDT -> %s: %s\n\n", oCDT.GetString().c_str(), oCDT.PrintableType());

	oCDT = "3.141";
	fprintf(stderr, "CDT -> %d: %s\n", INT_32(oCDT.GetInt()), oCDT.PrintableType());
	fprintf(stderr, "CDT -> %s: %s\n\n", oCDT.GetString().c_str(), oCDT.PrintableType());

	oCDT *= 2;
	fprintf(stderr, "CDT -> %f: %s\n", W_FLOAT(oCDT.GetFloat()), oCDT.PrintableType());
	fprintf(stderr, "CDT -> %s: %s\n\n", oCDT.GetString().c_str(), oCDT.PrintableType());

	// Shared date types
	fprintf(stderr, "Shared data types\n");
	{
		oCDT = "3.141";
		fprintf(stderr, "CDT -> %s: %s\n\n", oCDT.GetString().c_str(), oCDT.PrintableType());

		CDT oCDT1 = oCDT;
		fprintf(stderr, "CDT1 -> %d: %s\n", INT_32(oCDT1.GetInt()), oCDT1.PrintableType());
		fprintf(stderr, "CDT1 -> %f: %s\n", W_FLOAT(oCDT1.GetFloat()), oCDT1.PrintableType());
		fprintf(stderr, "CDT1 -> %s: %s\n\n", oCDT1.GetString().c_str(), oCDT1.PrintableType());

		fprintf(stderr, "CDT -> %d: %s\n", INT_32(oCDT.GetInt()), oCDT.PrintableType());
		fprintf(stderr, "CDT -> %f: %s\n", W_FLOAT(oCDT.GetFloat()), oCDT.PrintableType());
		fprintf(stderr, "CDT -> %s: %s\n\n", oCDT.GetString().c_str(), oCDT.PrintableType());
	}

	fprintf(stderr, "Comparisons/NUMERIC/equal\n");
	{
		CDT oCDT1   = 10;
		CDT oCDT2   = 10.0;

		fprintf(stderr, "%f EQ %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.Equal(oCDT2)) ? 't':'f');
		fprintf(stderr, "%f GE %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.GreaterOrEqual(oCDT2)) ? 't':'f');
		fprintf(stderr, "%f LE %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.LessOrEqual(oCDT2)) ? 't':'f');

		fprintf(stderr, "%f NE %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.NotEqual(oCDT2)) ? 't':'f');
		fprintf(stderr, "%f GT %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.Greater(oCDT2)) ? 't':'f');
		fprintf(stderr, "%f LT %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.Less(oCDT2)) ? 't':'f');

		fprintf(stderr, "%f == %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1 == oCDT2) ? 't':'f');
		fprintf(stderr, "%f >= %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1 >= oCDT2) ? 't':'f');
		fprintf(stderr, "%f <= %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1 <= oCDT2) ? 't':'f');

		fprintf(stderr, "%f != %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1 != oCDT2) ? 't':'f');
		fprintf(stderr, "%f >  %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1 >  oCDT2) ? 't':'f');
		fprintf(stderr, "%f <  %f === '%c'\n\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1 <  oCDT2) ? 't':'f');
	}

	fprintf(stderr, "Comparisons/NUMERIC/not equal\n");
	{
		CDT oCDT1   = 10;
		CDT oCDT2   = 10.1;

		fprintf(stderr, "%f EQ %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.Equal(oCDT2)) ? 't':'f');
		fprintf(stderr, "%f GE %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.GreaterOrEqual(oCDT2)) ? 't':'f');
		fprintf(stderr, "%f LE %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.LessOrEqual(oCDT2)) ? 't':'f');

		fprintf(stderr, "%f NE %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.NotEqual(oCDT2)) ? 't':'f');
		fprintf(stderr, "%f GT %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.Greater(oCDT2)) ? 't':'f');
		fprintf(stderr, "%f LT %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.Less(oCDT2)) ? 't':'f');

		fprintf(stderr, "%f == %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1 == oCDT2) ? 't':'f');
		fprintf(stderr, "%f >= %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1 >= oCDT2) ? 't':'f');
		fprintf(stderr, "%f <= %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1 <= oCDT2) ? 't':'f');

		fprintf(stderr, "%f != %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1 != oCDT2) ? 't':'f');
		fprintf(stderr, "%f >  %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1 >  oCDT2) ? 't':'f');
		fprintf(stderr, "%f <  %f === '%c'\n\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1 <  oCDT2) ? 't':'f');
	}

	fprintf(stderr, "Comparisons/STRING/equal\n");
	{
		CDT oCDT1 = "abac";
		CDT oCDT2 = "abac";

		fprintf(stderr, "%s EQ %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1.Equal(oCDT2)) ? 't':'f');
		fprintf(stderr, "%s GE %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1.GreaterOrEqual(oCDT2)) ? 't':'f');
		fprintf(stderr, "%s LE %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1.LessOrEqual(oCDT2)) ? 't':'f');

		fprintf(stderr, "%s NE %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1.NotEqual(oCDT2)) ? 't':'f');
		fprintf(stderr, "%s GT %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1.Greater(oCDT2)) ? 't':'f');
		fprintf(stderr, "%s LT %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1.Less(oCDT2)) ? 't':'f');

		fprintf(stderr, "%s == %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 == oCDT2) ? 't':'f');
		fprintf(stderr, "%s >= %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 >= oCDT2) ? 't':'f');
		fprintf(stderr, "%s <= %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 <= oCDT2) ? 't':'f');

		fprintf(stderr, "%s != %s === '%c'\n",  oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 != oCDT2) ? 't':'f');
		fprintf(stderr, "%s >  %s === '%c'\n",   oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 > oCDT2) ? 't':'f');
		fprintf(stderr, "%s <  %s === '%c'\n\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 < oCDT2) ? 't':'f');

	}

	fprintf(stderr, "Comparisons/STRING/not equal\n");
	{
		CDT oCDT1 = "abac";
		CDT oCDT2 = "abaca";

		fprintf(stderr, "%s EQ %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1.Equal(oCDT2)) ? 't':'f');
		fprintf(stderr, "%s GE %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1.GreaterOrEqual(oCDT2)) ? 't':'f');
		fprintf(stderr, "%s LE %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1.LessOrEqual(oCDT2)) ? 't':'f');

		fprintf(stderr, "%s NE %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1.NotEqual(oCDT2)) ? 't':'f');
		fprintf(stderr, "%s GT %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1.Greater(oCDT2)) ? 't':'f');
		fprintf(stderr, "%s LT %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1.Less(oCDT2)) ? 't':'f');

		fprintf(stderr, "%s == %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 == oCDT2) ? 't':'f');
		fprintf(stderr, "%s >= %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 >= oCDT2) ? 't':'f');
		fprintf(stderr, "%s <= %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 <= oCDT2) ? 't':'f');

		fprintf(stderr, "%s != %s === '%c'\n",  oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 != oCDT2) ? 't':'f');
		fprintf(stderr, "%s >  %s === '%c'\n",   oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 > oCDT2) ? 't':'f');
		fprintf(stderr, "%s <  %s === '%c'\n\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 < oCDT2) ? 't':'f');
	}

	fprintf(stderr, "Comparisons/INT-STRING/equal\n");
	{
		CDT oCDT1 = 100;
		CDT oCDT2 = "100";

		fprintf(stderr, "%f EQ %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.Equal(oCDT2)) ? 't':'f');
		fprintf(stderr, "%f GE %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.GreaterOrEqual(oCDT2)) ? 't':'f');
		fprintf(stderr, "%f LE %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.LessOrEqual(oCDT2)) ? 't':'f');

		fprintf(stderr, "%f NE %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.NotEqual(oCDT2)) ? 't':'f');
		fprintf(stderr, "%f GT %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.Greater(oCDT2)) ? 't':'f');
		fprintf(stderr, "%f LT %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.Less(oCDT2)) ? 't':'f');

		fprintf(stderr, "%s == %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 == oCDT2) ? 't':'f');
		fprintf(stderr, "%s >= %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 >= oCDT2) ? 't':'f');
		fprintf(stderr, "%s <= %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 <= oCDT2) ? 't':'f');

		fprintf(stderr, "%s != %s === '%c'\n",  oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 != oCDT2) ? 't':'f');
		fprintf(stderr, "%s >  %s === '%c'\n",   oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 > oCDT2) ? 't':'f');
		fprintf(stderr, "%s <  %s === '%c'\n\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 < oCDT2) ? 't':'f');
	}

	fprintf(stderr, "Comparisons/INT-STRING/not equal\n");
	{
		CDT oCDT1 = 100.1;
		CDT oCDT2 = "100";

		fprintf(stderr, "%f EQ %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.Equal(oCDT2)) ? 't':'f');
		fprintf(stderr, "%f GE %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.GreaterOrEqual(oCDT2)) ? 't':'f');
		fprintf(stderr, "%f LE %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.LessOrEqual(oCDT2)) ? 't':'f');

		fprintf(stderr, "%f NE %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.NotEqual(oCDT2)) ? 't':'f');
		fprintf(stderr, "%f GT %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.Greater(oCDT2)) ? 't':'f');
		fprintf(stderr, "%f LT %f === '%c'\n", oCDT1.GetFloat(), oCDT2.GetFloat(), (oCDT1.Less(oCDT2)) ? 't':'f');

		fprintf(stderr, "%s == %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 == oCDT2) ? 't':'f');
		fprintf(stderr, "%s >= %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 >= oCDT2) ? 't':'f');
		fprintf(stderr, "%s <= %s === '%c'\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 <= oCDT2) ? 't':'f');

		fprintf(stderr, "%s != %s === '%c'\n",   oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 != oCDT2) ? 't':'f');
		fprintf(stderr, "%s >  %s === '%c'\n",   oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 > oCDT2) ? 't':'f');
		fprintf(stderr, "%s <  %s === '%c'\n\n", oCDT1.GetString().c_str(), oCDT2.GetString().c_str(), (oCDT1 < oCDT2) ? 't':'f');
	}

	oCDT = 0xdeadbeef;

	fprintf(stderr, "As String 1: `%s` %s\n",   oCDT.GetString().c_str(),  oCDT.PrintableType());
	fprintf(stderr, "As String 2: `%s` %s\n\n", oCDT.GetString("0x%08X").c_str(),  oCDT.PrintableType());

	fprintf(stderr, "== POINTER ==================================\n");
	void * vTMP = (void *)0xDeadBeef;
	oCDT = vTMP;
	fprintf(stderr, "As String 1: `%s`\n",   oCDT.GetString("").c_str());
	fprintf(stderr, "As String 2: `%s`\n\n", oCDT.GetString("Pointer: %p").c_str());

	fprintf(stderr, "As Pointer 1: %p\n",   oCDT.GetPointer());
	fprintf(stderr, "As Pointer 2: %p\n\n", (void *)(oCDT.GetObject<CHAR_P>()));

	fprintf(stderr, "== STRING ===================================\n");
	oCDT = ", ";
	oCDT.Prepend("Hello");
	oCDT.Append("World!");

	fprintf(stderr, "CDT: `%s` %s\n", oCDT.GetString("").c_str(), oCDT.PrintableType());

	oCDT.Prepend(111);
	oCDT.Append(222);
	fprintf(stderr, "CDT: `%s` %s\n\n", oCDT.GetString("").c_str(), oCDT.PrintableType());


	fprintf(stderr, "== Comparators ==============================\n");
	{
		W_FLOAT dValue = 123.456;
		oCDT = 123.456;
		fprintf(stderr, "%f EQ %f === '%c'\n", oCDT.GetFloat(), dValue, (oCDT.Equal(dValue)) ? 't':'f');
		fprintf(stderr, "%f GE %f === '%c'\n", oCDT.GetFloat(), dValue, (oCDT.GreaterOrEqual(dValue)) ? 't':'f');
		fprintf(stderr, "%f LE %f === '%c'\n", oCDT.GetFloat(), dValue, (oCDT.LessOrEqual(dValue)) ? 't':'f');

		fprintf(stderr, "%f NE %f === '%c'\n", oCDT.GetFloat(), dValue, (oCDT.NotEqual(dValue)) ? 't':'f');
		fprintf(stderr, "%f GT %f === '%c'\n", oCDT.GetFloat(), dValue, (oCDT.Greater(dValue)) ? 't':'f');
		fprintf(stderr, "%f LT %f === '%c'\n", oCDT.GetFloat(), dValue, (oCDT.Less(dValue)) ? 't':'f');

		fprintf(stderr, "%f == %f === '%c'\n", oCDT.GetFloat(), dValue, (oCDT == dValue) ? 't':'f');
		fprintf(stderr, "%f >= %f === '%c'\n", oCDT.GetFloat(), dValue, (oCDT >= dValue) ? 't':'f');
		fprintf(stderr, "%f <= %f === '%c'\n", oCDT.GetFloat(), dValue, (oCDT <= dValue) ? 't':'f');

		fprintf(stderr, "%f != %f === '%c'\n",   oCDT.GetFloat(), dValue, (oCDT != dValue) ? 't':'f');
		fprintf(stderr, "%f >  %f === '%c'\n",   oCDT.GetFloat(), dValue, (oCDT >  dValue) ? 't':'f');
		fprintf(stderr, "%f <  %f === '%c'\n\n", oCDT.GetFloat(), dValue, (oCDT <  dValue) ? 't':'f');
	}

	{
		W_FLOAT dValue = 456.321;
		oCDT = 123.456;
		fprintf(stderr, "%f EQ %f === '%c'\n", oCDT.GetFloat(), dValue, (oCDT.Equal(dValue)) ? 't':'f');
		fprintf(stderr, "%f GE %f === '%c'\n", oCDT.GetFloat(), dValue, (oCDT.GreaterOrEqual(dValue)) ? 't':'f');
		fprintf(stderr, "%f LE %f === '%c'\n", oCDT.GetFloat(), dValue, (oCDT.LessOrEqual(dValue)) ? 't':'f');

		fprintf(stderr, "%f NE %f === '%c'\n", oCDT.GetFloat(), dValue, (oCDT.NotEqual(dValue)) ? 't':'f');
		fprintf(stderr, "%f GT %f === '%c'\n", oCDT.GetFloat(), dValue, (oCDT.Greater(dValue)) ? 't':'f');
		fprintf(stderr, "%f LT %f === '%c'\n", oCDT.GetFloat(), dValue, (oCDT.Less(dValue)) ? 't':'f');

		fprintf(stderr, "%f == %f === '%c'\n", oCDT.GetFloat(), dValue, (oCDT == dValue) ? 't':'f');
		fprintf(stderr, "%f >= %f === '%c'\n", oCDT.GetFloat(), dValue, (oCDT >= dValue) ? 't':'f');
		fprintf(stderr, "%f <= %f === '%c'\n", oCDT.GetFloat(), dValue, (oCDT <= dValue) ? 't':'f');

		fprintf(stderr, "%f != %f === '%c'\n",   oCDT.GetFloat(), dValue, (oCDT != dValue) ? 't':'f');
		fprintf(stderr, "%f >  %f === '%c'\n",   oCDT.GetFloat(), dValue, (oCDT >  dValue) ? 't':'f');
		fprintf(stderr, "%f <  %f === '%c'\n\n", oCDT.GetFloat(), dValue, (oCDT <  dValue) ? 't':'f');
	}

	{
		STLW::string sValue = "Foobar";
		oCDT = "Foobar";

		fprintf(stderr, "%s EQ %s === '%c'\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT.Equal(sValue)) ? 't':'f');
		fprintf(stderr, "%s GE %s === '%c'\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT.GreaterOrEqual(sValue)) ? 't':'f');
		fprintf(stderr, "%s LE %s === '%c'\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT.LessOrEqual(sValue)) ? 't':'f');

		fprintf(stderr, "%s NE %s === '%c'\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT.NotEqual(sValue)) ? 't':'f');
		fprintf(stderr, "%s GT %s === '%c'\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT.Greater(sValue)) ? 't':'f');
		fprintf(stderr, "%s LT %s === '%c'\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT.Less(sValue)) ? 't':'f');

		fprintf(stderr, "%s == %s === '%c'\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT == sValue) ? 't':'f');
		fprintf(stderr, "%s >= %s === '%c'\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT >= sValue) ? 't':'f');
		fprintf(stderr, "%s <= %s === '%c'\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT <= sValue) ? 't':'f');

		fprintf(stderr, "%s != %s === '%c'\n",   oCDT.GetString().c_str(), sValue.c_str(), (oCDT != sValue) ? 't':'f');
		fprintf(stderr, "%s >  %s === '%c'\n",   oCDT.GetString().c_str(), sValue.c_str(), (oCDT >  sValue) ? 't':'f');
		fprintf(stderr, "%s <  %s === '%c'\n\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT <  sValue) ? 't':'f');
	}

	{
		STLW::string sValue = "Flurbamatic";
		oCDT = "Foobar";

		fprintf(stderr, "%s EQ %s === '%c'\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT.Equal(sValue)) ? 't':'f');
		fprintf(stderr, "%s GE %s === '%c'\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT.GreaterOrEqual(sValue)) ? 't':'f');
		fprintf(stderr, "%s LE %s === '%c'\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT.LessOrEqual(sValue)) ? 't':'f');

		fprintf(stderr, "%s NE %s === '%c'\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT.NotEqual(sValue)) ? 't':'f');
		fprintf(stderr, "%s GT %s === '%c'\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT.Greater(sValue)) ? 't':'f');
		fprintf(stderr, "%s LT %s === '%c'\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT.Less(sValue)) ? 't':'f');

		fprintf(stderr, "%s == %s === '%c'\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT == sValue) ? 't':'f');
		fprintf(stderr, "%s >= %s === '%c'\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT >= sValue) ? 't':'f');
		fprintf(stderr, "%s <= %s === '%c'\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT <= sValue) ? 't':'f');

		fprintf(stderr, "%s != %s === '%c'\n",   oCDT.GetString().c_str(), sValue.c_str(), (oCDT != sValue) ? 't':'f');
		fprintf(stderr, "%s >  %s === '%c'\n",   oCDT.GetString().c_str(), sValue.c_str(), (oCDT >  sValue) ? 't':'f');
		fprintf(stderr, "%s <  %s === '%c'\n\n", oCDT.GetString().c_str(), sValue.c_str(), (oCDT <  sValue) ? 't':'f');
	}
/*

	try
	{
		CDT oCDT_array(CDT::ARRAY_VAL);
		oCDT_array[0]  = 10;
		oCDT_array[1]  = 10.5;
		oCDT_array[2]  = "test";
		oCDT_array[3]  = "passed";
		oCDT_array[10] = 11;
		oCDT_array[11][3] = 5;

		fprintf(stderr, "ARRAY size: %d\n", oCDT_array.Size());
		for (UINT_32 iI = 0; iI < oCDT_array.Size(); ++iI)
		{
			fprintf(stderr, "oCDT_array[%d] (%s) = `%s`\n", iI, oCDT_array[iI].PrintableType(), oCDT_array[iI].GetString().c_str());
		}
		fprintf(stderr, "ARRAY size: %d\n\n", oCDT_array.Size());
	}
	catch(CDTTypeCastException &e)
	{
		fprintf(stderr, "ERROR: Type cast %s\n", e.what());
	}
	catch(std::exception &e)
	{
		fprintf(stderr, "ERROR: std %s\n", e.what());
	}
	catch(...)
	{
		fprintf(stderr, "Ouch!\n");
	}
/ *
my %H;
$H{'q'} -> [1] -> {"test"} = "passed";
* /
*/
	fprintf(stderr, "== ARRAYS & HASHES ==========================\n");
	CDT oCDT_array;
	oCDT_array["abc"]       = 10;
	oCDT_array["xyz"][0]    = 2;
	oCDT_array["1"]["2"][3] = 4;

	fprintf(stderr, "oCDT_array[\"abc\"]         (%s) = `%s`\n",   oCDT_array["abc"].PrintableType(),       oCDT_array["abc"].GetString().c_str());
	fprintf(stderr, "oCDT_array[\"1\"]           (%s) = `%s`\n",   oCDT_array["1"].PrintableType(),         oCDT_array["1"].GetString().c_str());
	fprintf(stderr, "oCDT_array[\"1\"][\"2\"]      (%s) = `%s`\n", oCDT_array["1"]["2"].PrintableType(),    oCDT_array["1"]["2"].GetString().c_str());
	fprintf(stderr, "oCDT_array[\"abc\"][\"2\"][3] (%s) = `%s`\n", oCDT_array["1"]["2"][3].PrintableType(), oCDT_array["1"]["2"][3].GetString().c_str());

	CDT oCDT_empty;

	fprintf(stderr, "oCDT_empty = `%s`\n\n", oCDT_empty["test"].GetString().c_str());

	fprintf(stderr, "DUMP: %s\n", oCDT_array.RecursiveDump().c_str());

	fprintf(stderr, "Join HASH keys: %s\n", oCDT_array.JoinHashKeys(":").c_str());

	fprintf(stderr, "Join HASH values: %s\n", oCDT_array.JoinHashValues(":").c_str());

	fprintf(stderr, "Get HASH keys: %s\n", oCDT_array.GetHashKeys().Dump().c_str());

	fprintf(stderr, "Get HASH values: %s\n", oCDT_array.GetHashValues().Dump().c_str());

	CDT::Iterator itCDTArray = oCDT_array.Begin();
	while (itCDTArray != oCDT_array.End())
	{
		fprintf(stderr, "oCDT_array[\"%s\"] => %s\n", itCDTArray -> first.c_str(), itCDTArray -> second.GetString().c_str());
		itCDTArray -> second = "Empty";
		++itCDTArray;
	}

	CDT::ConstIterator itCDTCArray = oCDT_array.Begin();
	while (itCDTCArray != oCDT_array.End())
	{
		fprintf(stderr, "oCDT_array[\"%s\"] => %s\n", itCDTCArray -> first.c_str(), itCDTCArray -> second.GetString().c_str());

		++itCDTCArray;
	}
	fprintf(stderr, "DUMP: %s", oCDT_array.RecursiveDump().c_str());

	CDT oCDT_array1(CDT::ARRAY_VAL);
	oCDT_array1.PushBack(1);
	oCDT_array1.PushBack(1.23);
	oCDT_array1.PushBack("some string");
	oCDT_array1.PushBack(oCDT_array);

	fprintf(stderr, "DUMP: %s", oCDT_array1.RecursiveDump().c_str());

	fprintf(stderr, "DUMP: %s\n", oCDT_array1.RecursiveDump().c_str());

	fprintf(stderr, "Join ARRAY: %s\n", oCDT_array1.JoinArrayElements(":").c_str());

	// Swap two values
	{
		oCDT         = "Foo";
		CDT oCDTSwap = "Bar";

		fprintf(stderr, "Swap:   `%s` to `%s`\n", oCDT.GetString().c_str(), oCDTSwap.GetString().c_str());
		oCDT.Swap(oCDTSwap);
		fprintf(stderr, "Result: `%s` <> `%s`\n\n", oCDT.GetString().c_str(), oCDTSwap.GetString().c_str());
	}

	// Sort array
	{
		CDT oCDT1;
		oCDT1[0] = "12";
		oCDT1[1] = "34";
		oCDT1[2] = "56";
		oCDT1[3] = 78;
		oCDT1[4] = 910;
		fprintf(stderr, "Source: `%s`\n", oCDT1.RecursiveDump().c_str());

		oCDT1.SortArray(SortCompareNum());
		fprintf(stderr, "SortCompareNum: `%s`\n", oCDT1.RecursiveDump().c_str());

		oCDT1.SortArray(SortCompareStr());
		fprintf(stderr, "SortCompareStr: `%s`\n", oCDT1.RecursiveDump().c_str());

		oCDT1 = CDT();
		oCDT1[0][1] = "apple";
		oCDT1[1][1] = "pera";
		oCDT1[2][1] = "orange";
		oCDT1[3][1] = "raspberry";
		oCDT1[4][1] = "banana";

		oCDT1.SortArray(SortCompareNumArrayElement(1));
		fprintf(stderr, "SortCompareNumArryElement(ASC): `%s`\n", oCDT1.RecursiveDump().c_str());

		oCDT1.SortArray(SortCompareStrArrayElement(1));
		fprintf(stderr, "SortCompareStrArryElement(ASC): `%s`\n", oCDT1.RecursiveDump().c_str());

		oCDT1.SortArray(SortCompareNumArrayElement(1, CDT::SortingComparator::DESC));
		fprintf(stderr, "SortCompareNumArryElement(DESC): `%s`\n", oCDT1.RecursiveDump().c_str());

		oCDT1.SortArray(SortCompareStrArrayElement(1, CDT::SortingComparator::DESC));
		fprintf(stderr, "SortCompareStrArryElement(DESC): `%s`\n", oCDT1.RecursiveDump().c_str());

		oCDT1 = CDT();
		oCDT1[0]["foo"] = "apple";
		oCDT1[1]["foo"] = "pera";
		oCDT1[2]["foo"] = "orange";
		oCDT1[3]["foo"] = "raspberry";
		oCDT1[4]["foo"] = "banana";

		oCDT1.SortArray(SortCompareNumHashElement("foo"));
		fprintf(stderr, "SortCompareNumHashElement(ASC): `%s`\n", oCDT1.RecursiveDump().c_str());

		oCDT1.SortArray(SortCompareStrHashElement("foo"));
		fprintf(stderr, "SortCompareNumHashElement(ASC): `%s`\n", oCDT1.RecursiveDump().c_str());

		oCDT1.SortArray(SortCompareNumHashElement("foo", CDT::SortingComparator::DESC));
		fprintf(stderr, "SortCompareNumHashElement(DESC): `%s`\n", oCDT1.RecursiveDump().c_str());

		oCDT1.SortArray(SortCompareStrHashElement("foo", CDT::SortingComparator::DESC));
		fprintf(stderr, "SortCompareNumHashElement(DESC): `%s`\n", oCDT1.RecursiveDump().c_str());

	}

	// Merge hashes, arrays, etc
	fprintf(stderr, "== Merge ARRAYS & HASHES ====================\n");
	{
		CDT oSource;
		CDT oDestination;
		oDestination.MergeCDT(oSource);

		// Array-to-array
		oDestination[0] = "one";
		oDestination[1] = "two";
		oDestination[2] = "three";
		oDestination.MergeCDT(oSource);

		oSource[0] = "four";
		oSource[1] = "five";
		oSource[2] = "six";

		oDestination.MergeCDT(oSource);
		fprintf(stderr, "Merge: `%s`\n", oDestination.RecursiveDump().c_str());

		// Array-to-hash
		oDestination = CDT(CDT::HASH_VAL);
		oDestination["one"]   = 1;
		oDestination["two"]   = 2;
		oDestination["three"] = 3;
		oDestination.MergeCDT(oSource);
		fprintf(stderr, "Merge: `%s`\n", oDestination.RecursiveDump().c_str());

		// Hash-to-array
		oDestination = CDT(CDT::ARRAY_VAL);
		oDestination[0] = "one";
		oDestination[1] = "two";
		oDestination[2] = "three";

		oSource = CDT(CDT::HASH_VAL);
		oSource["four"] = 4;
		oSource["five"] = 5;
		oSource["six"]  = 6;
		oDestination.MergeCDT(oSource);
		fprintf(stderr, "Merge: `%s`\n", oDestination.RecursiveDump().c_str());

		// Hash-to-hash
		oDestination = CDT(CDT::HASH_VAL);
		oDestination["one"]   = 1;
		oDestination["two"]   = 2;
		oDestination["three"] = 3;
		oDestination.MergeCDT(oSource);
		fprintf(stderr, "Merge: `%s`\n", oDestination.RecursiveDump().c_str());
	}

	fprintf(stderr, "== Merge DEEP ARRAYS & HASHES ===============\n");
	{
		CDT oSource;
		CDT oDestination;

		oSource["foo"][0] = "apple";
		oSource["bar"][0] = "cat";
		oSource["foo"][1] = "pera";
		oSource["bar"][1] = "dog";
		oSource["foo"][2] = "orange";
		oSource["bar"][2] = "fox";
		oSource["foo"][3] = "raspberry";
		oSource["bar"][3] = "bear";
		oSource["foo"][4] = "banana";
		oSource["bar"][4] = "rat";

		oDestination["foo"][0] = "melon";
		oDestination["baz"][0] = "whale";
		oDestination["baz"][1] = "dolphin";
		oDestination["boo"][1] = "raven";
		CDT oDestination1 = oDestination;

		oDestination.MergeCDT(oSource);
		fprintf(stderr, "Merge: `%s`\n", oDestination.RecursiveDump().c_str());

		oDestination1.MergeCDT(oSource, CDT::DEEP_MERGE);
		fprintf(stderr, "Merge: `%s`\n", oDestination1.RecursiveDump().c_str());
	}
	fprintf(stderr, "== END ======================================\n");

	// make valgrind happy
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.

