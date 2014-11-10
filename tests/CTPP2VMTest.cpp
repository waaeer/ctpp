/*-
 * Copyright (c) 2006, 2007 CTPP Team
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
 *      CTPP2VMTest.cpp
 *
 * $CTPP$
 */
#include <CTPP2HashTable.hpp>
#include <CTPP2VMDumper.hpp>
#include <CTPP2VMExecutable.hpp>
#include <CTPP2VMOpcodes.h>
#include <CTPP2VMInstruction.hpp>

#include <stdio.h>

#ifdef HAVE_SYSEXITS_H
#include <sysexits.h>
#endif

using namespace CTPP;

int main(int argc, char ** argv)
{
	if (argc != 2) { fprintf(stderr, "usage: %s file.name\n", argv[0]); return EX_USAGE; }

	// 1. Create static text segment
	StaticText oStaticText;

	StaticText oSyscalls;

	HashTable oHashTable;

	// Text Data
	//                     1234567890123456789
	const UINT_32 iS1 = oStaticText.StoreData("CTPP2 Hello, World\n",    19); // returns '0'
	//                     1234567890123
	const UINT_32 iS2 = oStaticText.StoreData("Concatenation",           13); // returns '1'
	//                     1
	const UINT_32 iS3 = oStaticText.StoreData(" ",                       1);  // returns '2'
	//                     1234
	const UINT_32 iS4 = oStaticText.StoreData("test.\n",                 6);  // returns '3'
	//                     1234
	const UINT_32 iOK  = oStaticText.StoreData("OK\n",                   2);  // returns '4'
	const UINT_32 iNOK = oStaticText.StoreData("NOK\n",                  3);  // returns '5'

	// Numberic Data
	StaticData oStaticData;
	const UINT_32 iN0    = oStaticData.StoreInt(0);
	const UINT_32 iN1977 = oStaticData.StoreInt(1977);
	const UINT_32 iN10   = oStaticData.StoreInt(10);
	const UINT_32 iN8    = oStaticData.StoreInt(8);


	//const UINT_32 iN1    = oStaticData.StoreInt(1);
	const UINT_32 iN2    = oStaticData.StoreInt(2);
	const UINT_32 iN3    = oStaticData.StoreInt(3);
	//const UINT_32 iN4    = oStaticData.StoreInt(4);

	const UINT_32 iF1234 = oStaticData.StoreFloat(12.34);

	//const UINT_32 iSVar = oStaticText.StoreData("var", 3);
	const UINT_32 iSDic = oStaticText.StoreData("dic", 3);
	const UINT_32 iSArr = oStaticText.StoreData("arr", 3);
	const UINT_32 iSVal = oStaticText.StoreData("val", 3);

	UINT_32 iSyscallEmitterNum = oSyscalls.StoreData("__ctpp2_emitter", 15);
	//UINT_32 iSyscallObjDumpNum = oSyscalls.StoreData("OBJ_DUMP", 8);

	VMInstruction aCode[] =
	{
		{NOP              ,     0, 0},                      // 0
		{PUSH | ARG_SRC_AR,     0, 0},                      // 1
		{PUSH | ARG_SRC_BR,     0, 0},                      // 2
		{PUSH | ARG_SRC_CR,     0, 0},                      // 3
		{PUSH | ARG_SRC_DR,     0, 0},                      // 4
		{PUSH | ARG_SRC_ER,     0, 0},                      // 5
		{PUSH | ARG_SRC_FR,     0, 0},                      // 6
		{PUSH | ARG_SRC_GR,     0, 0},                      // 7
		{PUSH | ARG_SRC_HR,     0, 0},                      // 8

		{PUSH13           ,     0, 0},                      // 9
		{PUSH47           ,     0, 0},                      // 10
		{PUSHA            ,     0, 0},                      // 11

		{POPA             ,     0, 0},                      // 12
		{POP47            ,     0, 0},                      // 13
		{POP13            ,     0, 0},                      // 14

		{POP | ARG_SRC_HR ,     0, 0},                      // 15
		{POP | ARG_SRC_GR ,     0, 0},                      // 16
		{POP | ARG_SRC_FR ,     0, 0},                      // 17
		{POP | ARG_SRC_ER ,     0, 0},                      // 18
		{POP | ARG_SRC_DR ,     0, 0},                      // 19
		{POP | ARG_SRC_CR ,     0, 0},                      // 20
		{POP | ARG_SRC_BR ,     0, 0},                      // 21
		{POP | ARG_SRC_AR ,     0, 0},                      // 22

		{PUSH | ARG_SRC_INT,   iN1977, 0},                  // 23
		{PUSH | ARG_SRC_INT,   iN1977, 0},                  // 24
		{PUSH | ARG_SRC_INT,   iN1977, 0},                  // 25
		{PUSH | ARG_SRC_FLOAT, iF1234, 0},                  // 26
		{PUSH | ARG_SRC_INT,   iN8, 0},                     // 27
		{PUSH | ARG_SRC_INT,   iN3, 0},                     // 28
		{PUSH | ARG_SRC_INT,   iN2, 0},                     // 29

		{ADD  | ARG_SRC_STACK | ARG_DST_STACK,     0, 0 },  // 30   3 + 2        -> 5
		{SUB  | ARG_SRC_STACK | ARG_DST_STACK,     0, 0 },  // 31   8 - 5        -> 3
		{MUL  | ARG_SRC_STACK | ARG_DST_STACK,     0, 0 },  // 32   3 * 12.34    -> 37.02
		{DIV  | ARG_SRC_STACK | ARG_DST_STACK,     0, 0 },  // 33   1977 / 37.02 -> 53.4035656401945
		{IDIV | ARG_SRC_STACK | ARG_DST_STACK,     0, 0 },  // 34   1977 div 53  -> 37
		{MOD  | ARG_SRC_STACK | ARG_DST_STACK,     0, 0 },  // 35   1977 mod 37  -> 16

		{INC  | ARG_SRC_STACK,     0, 0 },                  // 35 // ++ 16
		{DEC  | ARG_SRC_STACK,     0, 0 },                  // 36 // -- 16

		{PUSH | ARG_SRC_STR,   iS2, 0},                     // 38
		{PUSH | ARG_SRC_STR,   iS3, 0},                     // 39
		{PUSH | ARG_SRC_STR,   iS4, 0},                     // 40

		{CONCAT | ARG_SRC_STACK | ARG_DST_STACK,     0, 0 },// 41
		{CONCAT | ARG_SRC_STACK | ARG_DST_STACK,     0, 0 },// 42
		{CONCAT | ARG_SRC_STACK | ARG_DST_STACK,     0, 0 },// 43

		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 44


		{MOV  | ARG_SRC_INT |ARG_DST_FR,           iN8, 0}, // 45
		{MOV  | ARG_SRC_INT |ARG_DST_ER,           iN0, 0}, // 46

		{PUSH | ARG_SRC_STR,   iS1, 0},                     // 47
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 48
		{LOOP  | ARG_SRC_FR | ARG_DST_ER, 47, 0},           // 49

		{PUSH | ARG_SRC_INT,   iN8,  0},                    // 50
		{PUSH | ARG_SRC_INT,   iN10, 0},                    // 51
		{CMP  | ARG_SRC_STACK | ARG_DST_STACK, 0, 0},       // 52

		{JE,                    57, 0},                     // 53
		{PUSH | ARG_SRC_STR,  iNOK, 0},                     // 54
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 55
		{JMP,                   59, 0},                     // 56
		{PUSH | ARG_SRC_STR,   iOK, 0},                     // 57
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 58

		{JN,                    63, 0},                     // 59
		{PUSH | ARG_SRC_STR,  iNOK, 0},                     // 60
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 61
		{JMP,                   65, 0},                     // 62
		{PUSH | ARG_SRC_STR,   iOK, 0},                     // 63
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 64

		{JL,                    69, 0},                     // 65
		{PUSH | ARG_SRC_STR,  iNOK, 0},                     // 66
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 67
		{JMP,                   71, 0},                     // 68
		{PUSH | ARG_SRC_STR,   iOK, 0},                     // 69
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 70

		{JG,                    75, 0},                     // 71
		{PUSH | ARG_SRC_STR,  iNOK, 0},                     // 72
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 73
		{JMP,                   77, 0},                     // 74
		{PUSH | ARG_SRC_STR,   iOK, 0},                     // 75
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 76

		{JGE,                   81, 0},                     // 77
		{PUSH | ARG_SRC_STR,  iNOK, 0},                     // 78
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 79
		{JMP,                   83, 0},                     // 80
		{PUSH | ARG_SRC_STR,   iOK, 0},                     // 81
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 82

		{JLE,                   87, 0},                     // 83
		{PUSH | ARG_SRC_STR,  iNOK, 0},                     // 84
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 85
		{JMP,                   89, 0},                     // 86
		{PUSH | ARG_SRC_STR,   iOK, 0},                     // 87
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 88

		{RJE,                    4, 0},                     // 89
		{PUSH | ARG_SRC_STR,  iNOK, 0},                     // 90
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 91
		{RJMP,                   3, 0},                     // 92
		{PUSH | ARG_SRC_STR,   iOK, 0},                     // 93
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 94

		{RJN,                    4, 0},                     // 95
		{PUSH | ARG_SRC_STR,  iNOK, 0},                     // 96
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 97
		{RJMP,                   3, 0},                     // 98
		{PUSH | ARG_SRC_STR,   iOK, 0},                     // 99
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 100

		{RJL,                    4, 0},                     // 101
		{PUSH | ARG_SRC_STR,  iNOK, 0},                     // 102
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 103
		{JMP,                    3, 0},                     // 104
		{PUSH | ARG_SRC_STR,   iOK, 0},                     // 105
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 106

		{RJG,                    4, 0},                     // 106
		{PUSH | ARG_SRC_STR,  iNOK, 0},                     // 107
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 108
		{RJMP,                   3, 0},                     // 109
		{PUSH | ARG_SRC_STR,   iOK, 0},                     // 110
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 111

		{RJGE,                   4, 0},                     // 112
		{PUSH | ARG_SRC_STR,  iNOK, 0},                     // 113
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 114
		{RJMP,                   3, 0},                     // 115
		{PUSH | ARG_SRC_STR,   iOK, 0},                     // 116
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 117

		{RJLE,                   4, 0},                     // 118
		{PUSH | ARG_SRC_STR,  iNOK, 0},                     // 119
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 120
		{RJMP,                   3, 0},                     // 121
		{PUSH | ARG_SRC_STR,   iOK, 0},                     // 122
		{OUTPUT | ARG_SRC_STACK, 0, 0},                     // 123

		// // { 'dic': { 'arr': [ 0, 1, 2, 3], 'val' : 'ind' }, 'ind' : 2 }
		// dic{'arr'}[dic{val}] === ( ( HR{'dic'} ){'arr'} ) [ (HR{'dic'}){ (HR{'dic'} ) {'val'}
		{SYSCALL, SYSCALL_PARAMS(iSyscallEmitterNum, 0), 0},   // 124
		{MOV | ARG_SRC_STACK | ARG_DST_HR, 0, 0},              // 125
		{POP | ARG_SRC_AR, 0, 0},                              // 126

		{PUSH| ARG_SRC_AR, 0, 0},                              // 127

		// IsVar
		// ( HR{'dic'} ){'arr'}
		{PUSH| ARG_SRC_HR, 0, 0},                              // 128
		{REPLACE | ARG_SRC_IND_STR | ARG_DST_STACK, iSDic, 0}, // 129
		{REPLACE | ARG_SRC_IND_STR | ARG_DST_STACK, iSArr, 0}, // 130

		// IsVar
		// ( HR{'dic'} ){'val'}
		{PUSH| ARG_SRC_HR, 0, 0},                              // 131
		{REPLACE | ARG_SRC_IND_STR | ARG_DST_STACK, iSDic, 0}, // 132
		{REPLACE | ARG_SRC_IND_STR | ARG_DST_STACK, iSVal, 0}, // 133

		// {}
		{POP | ARG_SRC_AR, 0, 0},                              // 134
		// IsVar
		{PUSH| ARG_SRC_HR, 0, 0},                              // 135
		{REPLSTR | ARG_SRC_AR, 0, 0},                          // 136

		// []
		{POP | ARG_SRC_AR, 0, 0},                              // 137
		{REPLINT | ARG_SRC_AR, 0, 0},                          // 138

//		{SYSCALL, SYSCALL_PARAMS(iSyscallObjDumpNum, 0), 0},   // 139
		{OUTPUT | ARG_SRC_STACK, 0, 0},                        // 140


		{HLT               ,    0,  0}                         // 141
	};

	const UINT_32 iCodeSize = sizeof(aCode) / sizeof(VMInstruction);

	fprintf(stderr, "Code size: %d\n", iCodeSize);

	FILE * F = fopen(argv[1], "wb");
	if (F == NULL) { fprintf(stderr, "ERROR: Cannot open file `%s` for writing\n", argv[1]); return EX_SOFTWARE; }

	VMDumper oDumper(iCodeSize, aCode, oSyscalls, oStaticData, oStaticText, oHashTable);

	UINT_32 iSize = 0;
	const VMExecutable * aProgramCore = oDumper.GetExecutable(iSize);

	fwrite(aProgramCore, iSize, 1, F);

	fclose(F);

	// make valgrind happy
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.

