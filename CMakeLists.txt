#
# CTPP - C++ Template Engine
#
# Cmake configuration file
#
PROJECT(CTPP)

SET(CTPP_VERSION_MAJOR 2)
SET(CTPP_VERSION_MINOR 8)
SET(CTPP_VERSION_PATCH 3)

SET(CTPP_VERSION         "${CTPP_VERSION_MAJOR}.${CTPP_VERSION_MINOR}.${CTPP_VERSION_PATCH}")
SET(CTPP_IDENT           "Dzoraget")
SET(CTPP_MASTER_SITE_URL "http://ctpp.havoc.ru/")

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)

OPTION(DEBUG_MODE          "Enable debug output [default: OFF]"                 OFF)
OPTION(ENABLE_OPTIMIZATION "Enable optimization [default: ON]"                  ON)
OPTION(SKIP_RELINK_RPATH   "Skip relinking and full RPATH for the install tree" OFF)

OPTION(ICONV_DISCARD_ILSEQ "Discard illegal sequence and continue (iconv) [default: ON]"    ON)
OPTION(ICONV_TRANSLITERATE "Enable transliteration in the conversion (iconv) [default: ON]" ON)

# Build optimized code for following CPU (default i386)
#SET(CPU_TUNE               "i686")

# Floating point precision for formatting output variables
SET(CTPP_FLOAT_PRECISION   12)

# Escape and tempoprary buffer size
SET(CTPP_ESCAPE_BUFFER_LEN 1024)

# Max. template recursion depth
SET(CTPP_MAX_TEMPLATE_RECURSION_DEPTH 1024)

# ################################################################################
# Standard library
#
OPTION(MD5_SUPPORT         "Enable md5 support [default: ON]"                   ON)
OPTION(ICONV_SUPPORT       "Enable iconv support [default: ON]"                 ON)

# ################################################################################
# Check Includes
#
INCLUDE(CheckIncludeFiles)
INCLUDE(CheckCXXSourceCompiles)
INCLUDE(CheckFunctionExists)
INCLUDE(CheckSymbolExists)
INCLUDE(CheckLibraryExists)

CHECK_LIBRARY_EXISTS(m round    "" HAVE_ROUND_FN)
CHECK_LIBRARY_EXISTS(m roundl   "" HAVE_ROUNDL_FN)

CHECK_INCLUDE_FILES(sys/types.h HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILES(sys/time.h  HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILES(sys/uio.h   HAVE_SYS_UIO_H)

CHECK_INCLUDE_FILES(fcntl.h     HAVE_FCNTL_H)
CHECK_INCLUDE_FILES(math.h      HAVE_MATH_H)
CHECK_INCLUDE_FILES(stdio.h     HAVE_STDIO_H)
CHECK_INCLUDE_FILES(stdlib.h    HAVE_STDLIB_H)
CHECK_INCLUDE_FILES(string.h    HAVE_STRING_H)
CHECK_INCLUDE_FILES(strings.h   HAVE_STRINGS_H)
CHECK_INCLUDE_FILES(time.h      HAVE_TIME_H)
CHECK_INCLUDE_FILES(unistd.h    HAVE_UNISTD_H)
CHECK_INCLUDE_FILES(sysexits.h  HAVE_SYSEXITS_H)

CHECK_CXX_SOURCE_COMPILES("#include <map>
                           namespace std { }
                           using namespace std;
                           int main(void)
                           {
                               map<int, int> m;
                               m.insert(pair<int, int>(1, 2));
                               m[2] = 3;
                               return 0;
                           }
                          " STL_MAP_CHECK)
IF(NOT STL_MAP_CHECK)
    MESSAGE(ERROR "!!!! STL map does not work!")
ENDIF(NOT STL_MAP_CHECK)

CHECK_CXX_SOURCE_COMPILES("#include <vector>
                           namespace std { }
                           using namespace std;
                           int main(void)
                           {
                               vector<int> v;
                               v.push_back(1);
                               return 0;
                           }
                          " STL_VECTOR_CHECK)
IF(NOT STL_VECTOR_CHECK)
    MESSAGE(ERROR "!!!! STL vector does not work!")
ENDIF(NOT STL_VECTOR_CHECK)

CHECK_CXX_SOURCE_COMPILES("#include <string>
                           using namespace std;
                           int main(void)
                           {
                               string s;
                               s.assign(\"Hello, World!\");
                               return 0;
                           }
                          " STL_STRING_CHECK)

IF(NOT STL_STRING_CHECK)
    MESSAGE(ERROR "!!!! STL string does not work!")
ENDIF(NOT STL_STRING_CHECK)

CHECK_CXX_SOURCE_COMPILES("#include <stdexcept>
                           namespace std { }
                           using namespace std;
                           int main(void)
                           {
                               try
                               {
                                   throw logic_error(\"Ouch!\");
                               }
                               catch(exception &e)
                               {
                                   return 0;
                               }
                               return 0;
                           }
                          " STL_STDEXCEPT_CHECK)

IF(NOT STL_STDEXCEPT_CHECK)
    MESSAGE(ERROR "!!!! STL logic_error/exception does not work!")
ENDIF(NOT STL_STDEXCEPT_CHECK)

CHECK_CXX_SOURCE_COMPILES("#include <vector>
                           int main(void)
                           {
                               vector<int> v;
                               v.push_back(1);
                               return 0;
                           }
                          " STL_VECTOR_NO_STD_CHECK)

IF (STL_VECTOR_NO_STD_CHECK)
    MESSAGE(STATUS "#### INFO: Using STL classes without STD namespace")
    SET(NO_STL_STD_PREFIX "ON")
ELSE (STL_VECTOR_NO_STD_CHECK)
    MESSAGE(STATUS "#### INFO: Using STL classes with STD namespace")
ENDIF (STL_VECTOR_NO_STD_CHECK)

MESSAGE(STATUS "#### System name is: ${CMAKE_SYSTEM_NAME}")

IF("${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD")
    MESSAGE(STATUS "#### Building for FreeBSD...")
ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD")

IF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")

    SET(SKIP_ICONV_LIRARY   "ON")

    MESSAGE(STATUS "#### Building for Linux...")

    IF("${SKIP_RELINK_RPATH}" MATCHES "ON")
        SET(CMAKE_SKIP_RPATH  TRUE)

        MESSAGE(STATUS "#### Building for Linux...")
        # skip the full RPATH for the build tree
        SET(CMAKE_SKIP_BUILD_RPATH  TRUE)

        # when building, use the install RPATH already
        # (so it doesn't need to relink when installing)
        SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

        # the RPATH to be used when installing
        SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

        # add the automatically determined parts of the RPATH
        # which point to directories outside the build tree to the install RPATH
        SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    ENDIF("${SKIP_RELINK_RPATH}" MATCHES "ON")

ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")

IF("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
    SET(CMAKE_SKIP_RPATH FALSE)
    SET(CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
    SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,-headerpad_max_install_names")
ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")

IF("${CMAKE_SYSTEM_NAME}" MATCHES "SunOS")
    SET(SKIP_ICONV_LIRARY   "ON")
    MESSAGE(STATUS "#### Building for SunOS...")
ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "SunOS")

IF("${CMAKE_SYSTEM_NAME}" MATCHES "HP-UX")
    MESSAGE(STATUS "#### Building for HP-UX...")
ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "HP-UX")

SET(MD5_WITHOUT_OPENSSL "")
#
# MD5 library support
#
IF (${MD5_SUPPORT} MATCHES "ON")
    IF (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        MESSAGE(STATUS "#### INFO: MD5 (Win32 CryptoAPI) hash function support enabled")
    ELSE (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

        FIND_PATH(MD5_INCLUDE openssl/md5.h PATHS /opt/include
                                                  /usr/include
                                                  /usr/local/include
                  DOC "Path where the openssl header files can be found")

        IF (MD5_INCLUDE)
            INCLUDE_DIRECTORIES(${MD5_INCLUDE})

            FIND_LIBRARY(MD5_LIBRARY NAMES crypto PATHS /lib
                                                        /opt/lib
                                                        /usr/lib
                                                        /usr/local/lib
                         DOC "Path where the openssl libraries can be found")
            IF (MD5_LIBRARY)
                SET(PROJECT_LIBS "${PROJECT_LIBS} crypto")
            ELSE (MD5_LIBRARY)
                MESSAGE(STATUS "#### ERROR: Cannot find libcrypto.so, aborting")
            ENDIF (MD5_LIBRARY)

            SET(MD5_SUPPORT "ON")

            MESSAGE(STATUS "#### INFO: MD5 (openssl) hash function support enabled")

        ELSE (MD5_INCLUDE)
            # reset FIND_PATH
            SET(MD5_INCLUDE)
            FIND_PATH(MD5_INCLUDE md5.h PATHS /opt/include
                                              /usr/include
                                              /usr/local/include
                  DOC "Path where the openssl header files can be found")
            IF (MD5_INCLUDE)
                INCLUDE_DIRECTORIES(${MD5_INCLUDE})

                FIND_LIBRARY(MD5_LIBRARY NAMES md5 PATHS /lib
                                                         /opt/lib
                                                         /usr/lib
                                                         /usr/local/lib
                             DOC "Path where the md5 libraries can be found")
                IF (MD5_LIBRARY)
                    SET(PROJECT_LIBS "${PROJECT_LIBS} md5")
                    SET(MD5_SUPPORT         "ON")
                    SET(MD5_WITHOUT_OPENSSL "ON")
                    MESSAGE(STATUS "#### INFO: MD5 (libmd5) hash function support enabled")
                ELSE (MD5_LIBRARY)
                    MESSAGE(STATUS "#### ERROR: Cannot find md5.so, aborting")
                ENDIF (MD5_LIBRARY)
            ELSE (MD5_INCLUDE)
                SET(MD5_SUPPORT "")
                MESSAGE(STATUS "#### WARNING: MD5 hash function support disabled: md5.h not found")
            ENDIF (MD5_INCLUDE)

        ENDIF (MD5_INCLUDE)

    ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
ENDIF (${MD5_SUPPORT} MATCHES "ON")

#
# Iconv converter support
#
IF (${ICONV_SUPPORT} MATCHES "ON")
    FIND_PATH(ICONV_INCLUDE iconv.h PATHS /opt/include
                                          /usr/include
                                          /usr/local/include
              DOC "Path where the iconv header files can be found")

    IF (ICONV_INCLUDE)
        INCLUDE_DIRECTORIES(${ICONV_INCLUDE})

        IF (NOT "${SKIP_ICONV_LIRARY}" MATCHES "ON")
            FIND_LIBRARY(ICONV_LIBRARY NAMES iconv PATHS /lib
                                                         /opt/lib
                                                         /usr/lib
                                                         /usr/local/lib
                         DOC "Path where the openssl libraries can be found")
            IF (ICONV_LIBRARY)
                SET(PROJECT_LIBS "${PROJECT_LIBS} iconv")
            ELSE (ICONV_LIBRARY)
                MESSAGE(STATUS "#### ERROR: Cannot find libiconv.so, aborting")
            ENDIF (ICONV_LIBRARY)
        ENDIF (NOT "${SKIP_ICONV_LIRARY}" MATCHES "ON")

        SET(ICONV_SUPPORT "ON")
        MESSAGE(STATUS "#### INFO: ICONV converter support enabled")

    ELSE (ICONV_INCLUDE)

        SET(ICONV_SUPPORT "")
        MESSAGE(STATUS "#### WARNING: ICONV converter support disabled: iconv.h not found")

    ENDIF (ICONV_INCLUDE)
ENDIF (${ICONV_SUPPORT} MATCHES "ON")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    FIND_LIBRARY(WS2_32_LIBRARY NAMES ws2_32)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(include/functions)

IF("${CMAKE_COMPILER_IS_GNUCXX}" MATCHES "1")

    SET(CMAKE_CXX_PARAMS      " --param large-function-growth=5000 --param inline-unit-growth=600 -finline-limit=2000 ")

    SET(CMAKE_CXX_WARN_FLAGS  " -Wall -pedantic -Wno-long-long -Wno-inline -finline-functions ")

    IF(ENABLE_OPTIMIZATION MATCHES "ON")
        SET(CMAKE_CXX_FLAGS "${CXX_FLAGS} ${CMAKE_CXX_WARN_FLAGS} ${CMAKE_CXX_PARAMS} -O3 ")

        IF (NOT "${CPU_TUNE}" STREQUAL "")
            SET(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -mtune=${CPU_TUNE} ")
        ENDIF (NOT "${CPU_TUNE}" STREQUAL "")

    ELSE(ENABLE_OPTIMIZATION MATCHES "OFF")
        SET(CMAKE_CXX_FLAGS "${CXX_FLAGS} ${CMAKE_CXX_WARN_FLAGS} ${CMAKE_CXX_PARAMS} ")
    ENDIF(ENABLE_OPTIMIZATION MATCHES "ON")

    IF(DEBUG_MODE MATCHES "ON")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
        SET(CMAKE_CXX_WARN_FLAGS  "${CMAKE_CXX_WARN_FLAGS} -fprofile-arcs -ftest-coverage ")
    ENDIF(DEBUG_MODE MATCHES "ON")

ENDIF("${CMAKE_COMPILER_IS_GNUCXX}" MATCHES "1")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

IF(ENABLE_OPTIMIZATION MATCHES "ON")
    MESSAGE(STATUS "#### INFO: Optimized build mode enabled")
ELSE(ENABLE_OPTIMIZATION MATCHES "OFF")
    MESSAGE(STATUS "#### INFO: Optimized build mode disabled")
ENDIF(ENABLE_OPTIMIZATION MATCHES "ON")

IF(DEBUG_MODE MATCHES "ON")
    MESSAGE(STATUS "#### ATTENTION: Debug mode enabled. Use this option only for testing!")
ENDIF(DEBUG_MODE MATCHES "ON")

# ################################################################################
# Sources
#
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/CTPP2SysHeaders.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/include/CTPP2SysHeaders.h)
#
SET(LIBSRCS
            src/CDT.cpp
            src/CDTSortRoutines.cpp

            src/CTPP2BitIndex.cpp
            src/CTPP2Compiler.cpp
            src/CTPP2DTOA.cpp
            src/CTPP2Exception.cpp
            src/CTPP2Error.cpp
            src/CTPP2FileOutputCollector.cpp
            src/CTPP2FileSourceLoader.cpp
            src/CTPP2FileLogger.cpp
            src/CTPP2HashTable.cpp
            src/CTPP2JSONParser.cpp
            src/CTPP2JSONFileParser.cpp
            src/CTPP2Logger.cpp
            src/CTPP2Parser.cpp
            src/CTPP2ParserException.cpp
            src/CTPP2SimpleCompiler.cpp
            src/CTPP2SimpleVM.cpp
            src/CTPP2Sprintf.cpp
            src/CTPP2StringBuffer.cpp
            src/CTPP2StaticData.cpp
            src/CTPP2StaticText.cpp
            src/CTPP2StreamOutputCollector.cpp
            src/CTPP2StringOutputCollector.cpp
            src/CTPP2StringIconvOutputCollector.cpp
            src/CTPP2SyscallFactory.cpp
            src/CTPP2Util.cpp
            src/CTPP2VM.cpp
            src/CTPP2VMArgStack.cpp
            src/CTPP2VMCodeStack.cpp
            src/CTPP2VMDebugInfo.cpp
            src/CTPP2VMDumper.cpp
            src/CTPP2VMException.cpp
            src/CTPP2VMExecutable.cpp
            src/CTPP2VMFileLoader.cpp
            src/CTPP2VMMemoryCore.cpp
            src/CTPP2VMOpcodeCollector.cpp
            src/CTPP2VMSTDLib.cpp
            src/CTPP2VMSyscall.cpp
            src/CTPP2GetText.cpp

            src/functions/FnAvg.cpp
            src/functions/FnBase64Decode.cpp
            src/functions/FnBase64Encode.cpp
            src/functions/FnCast.cpp
            src/functions/FnConcat.cpp
            src/functions/FnContext.cpp
            src/functions/FnDateFormat.cpp
            src/functions/FnDefault.cpp
            src/functions/FnDefined.cpp
            src/functions/FnEmitter.cpp
            src/functions/FnError.cpp
            src/functions/FnFormParam.cpp
            src/functions/FnGetText.cpp
            src/functions/FnGetType.cpp
            src/functions/FnHashKeys.cpp
            src/functions/FnHMACMD5.cpp
            src/functions/FnHTMLEscape.cpp
            src/functions/FnHrefParam.cpp
            src/functions/FnHostname.cpp
            src/functions/FnIconv.cpp
            src/functions/FnInSet.cpp
            src/functions/FnInArray.cpp
            src/functions/FnJSONEscape.cpp
            src/functions/FnJSON.cpp
            src/functions/FnList.cpp
            src/functions/FnListElement.cpp
            src/functions/FnLog.cpp
            src/functions/FnMBSize.cpp
            src/functions/FnMBSubstring.cpp
            src/functions/FnMBTruncate.cpp
            src/functions/FnMD5.cpp
            src/functions/FnMax.cpp
            src/functions/FnMin.cpp
            src/functions/FnNumFormat.cpp
            src/functions/FnObjDump.cpp
            src/functions/FnRandom.cpp
            src/functions/FnSize.cpp
            src/functions/FnSprintf.cpp
            src/functions/FnSubstring.cpp
            src/functions/FnTruncate.cpp
            src/functions/FnURIEscape.cpp
            src/functions/FnURLEscape.cpp
            src/functions/FnVersion.cpp
            src/functions/FnWMLEscape.cpp
            src/functions/FnXMLEscape.cpp
           )

# ################################################################################
# Lib binary
#
ADD_LIBRARY(ctpp2-static STATIC ${LIBSRCS})
SET_TARGET_PROPERTIES(ctpp2-static PROPERTIES OUTPUT_NAME ctpp2-st)
SET_TARGET_PROPERTIES(ctpp2-static PROPERTIES LINKER_LANGUAGE CXX)

ADD_LIBRARY(ctpp2 SHARED ${LIBSRCS})
SET_TARGET_PROPERTIES(ctpp2 PROPERTIES OUTPUT_NAME ctpp2)
SET_TARGET_PROPERTIES(ctpp2 PROPERTIES LINKER_LANGUAGE CXX)
SET_TARGET_PROPERTIES(ctpp2 PROPERTIES VERSION ${CTPP_VERSION} SOVERSION ${CTPP_VERSION_MAJOR})
IF(DEBUG_MODE MATCHES "ON")
    SET_TARGET_PROPERTIES(ctpp2 PROPERTIES LINK_FLAGS -Wl,-lgcov)
ENDIF(DEBUG_MODE MATCHES "ON")

TARGET_LINK_LIBRARIES(ctpp2 ${MD5_LIBRARY} ${ICONV_LIBRARY} ${WS2_32_LIBRARY})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    SET_TARGET_PROPERTIES(ctpp2 PROPERTIES COMPILE_DEFINITIONS CTPP2_DLL)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")


# ################################################################################
# Exec binary
#

# CTPP Compiler
ADD_EXECUTABLE(ctpp2c                       tests/CTPP2Compiler.cpp)
TARGET_LINK_LIBRARIES(ctpp2c                ctpp2)

# CTPP2 Interpreter
ADD_EXECUTABLE(ctpp2i                       tests/CTPP2Interpreter.cpp)
TARGET_LINK_LIBRARIES(ctpp2i                ctpp2)

# CTPP2 virtual machine
ADD_EXECUTABLE(ctpp2vm                      tests/CTPP2Emulator.cpp)
TARGET_LINK_LIBRARIES(ctpp2vm               ctpp2)

# CTPP2 JSON parser
ADD_EXECUTABLE(ctpp2json                    tests/CTPP2JSONParser.cpp)
TARGET_LINK_LIBRARIES(ctpp2json             ctpp2)

# ################################################################################
# Test suite
#
ENABLE_TESTING()

ADD_EXECUTABLE(CDTTest                      tests/CDTTest.cpp)
TARGET_LINK_LIBRARIES(CDTTest               ctpp2)

ADD_EXECUTABLE(CDTPerfTest                  tests/CDTPerfTest.cpp)
TARGET_LINK_LIBRARIES(CDTPerfTest           ctpp2)

ADD_EXECUTABLE(BitIndexTest                 tests/BitIndexText.cpp)
TARGET_LINK_LIBRARIES(BitIndexTest          ctpp2)

ADD_EXECUTABLE(HashTest                     tests/HashTest.cpp)
TARGET_LINK_LIBRARIES(HashTest              ctpp2)

ADD_EXECUTABLE(StaticTextTest               tests/StaticTextTest.cpp)
TARGET_LINK_LIBRARIES(StaticTextTest        ctpp2)

#ADD_EXECUTABLE(StaticDataTest               tests/StaticDataTest.cpp)
#TARGET_LINK_LIBRARIES(StaticDataTest        ctpp2)

ADD_EXECUTABLE(VMArgStackTest               tests/VMArgStackTest.cpp)
TARGET_LINK_LIBRARIES(VMArgStackTest        ctpp2)

ADD_EXECUTABLE(VMCodeStackTest              tests/VMCodeStackTest.cpp)
TARGET_LINK_LIBRARIES(VMCodeStackTest       ctpp2)

ADD_EXECUTABLE(CTPP2VMTest                  tests/CTPP2VMTest.cpp)
TARGET_LINK_LIBRARIES(CTPP2VMTest           ctpp2)

ADD_EXECUTABLE(CTPP2DTOATest                tests/CTPP2DTOA.cpp)
TARGET_LINK_LIBRARIES(CTPP2DTOATest         ctpp2)

ADD_EXECUTABLE(simple-compiler              tests/SimpleCompilerTest.cpp)
TARGET_LINK_LIBRARIES(simple-compiler       ctpp2)

ADD_EXECUTABLE(simple-vm                    tests/SimpleVMTest.cpp)
TARGET_LINK_LIBRARIES(simple-vm             ctpp2)

ADD_TEST(CDT_performance_test               CDTPerfTest)
ADD_TEST(CDT_ops_test                       CDTTest)
ADD_TEST(Bit_index_test                     BitIndexTest)
ADD_TEST(Hash_test                          HashTest)
ADD_TEST(Static_text_test                   StaticTextTest)
#ADD_TEST(Static_data_test                   StaticDataTest)
ADD_TEST(Argument_stack_test                VMArgStackTest)
ADD_TEST(Code_stack_test                    VMCodeStackTest)
ADD_TEST(Create_executable_file             CTPP2VMTest selftest.ct2)
ADD_TEST(VM_self_test                       ctpp2vm selftest.ct2)
SET_TESTS_PROPERTIES(VM_self_test PROPERTIES DEPENDS Create_executable_file)

ADD_EXECUTABLE(CDT2JSONBenchmark            benchmarks/CDT2JSON.cpp)
TARGET_LINK_LIBRARIES(CDT2JSONBenchmark     ctpp2)

ADD_TEST(StreamLikeJsonDump_1                 CDT2JSONBenchmark -t ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/data/test.json)
ADD_TEST(StreamLikeJsonDump_2                 CDT2JSONBenchmark -t ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/data/lebowski-bench.json)

FIND_PROGRAM(DIFF_EXECUTABLE "diff" /usr/local/bin /usr/bin)

ADD_TEST(Output_variables_C                 ctpp2c ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/output_variables.tmpl Output_variables.ct2)
ADD_TEST(Output_variables_R                 ctpp2vm Output_variables.ct2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/testdata.json Output_variables.out)
SET_TESTS_PROPERTIES(Output_variables_R PROPERTIES DEPENDS Output_variables_C)
IF (DIFF_EXECUTABLE)
    ADD_TEST(Output_variables_D             ${DIFF_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/output_variables.out Output_variables.out)
    SET_TESTS_PROPERTIES(Output_variables_D PROPERTIES DEPENDS Output_variables_R)
ENDIF (DIFF_EXECUTABLE)

ADD_TEST(Comparisons_C                      ctpp2c ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/comparisons.tmpl Comparisons.ct2)
ADD_TEST(Comparisons_R                      ctpp2vm Comparisons.ct2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/testdata.json Comparisons.out)
SET_TESTS_PROPERTIES(Comparisons_R PROPERTIES DEPENDS Comparisons_C)
IF (DIFF_EXECUTABLE)
    ADD_TEST(Comparisons_D                  ${DIFF_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/comparisons.out Comparisons.out)
    SET_TESTS_PROPERTIES(Comparisons_D      PROPERTIES DEPENDS Comparisons_R)
ENDIF (DIFF_EXECUTABLE)

ADD_TEST(Arith_ops_C                        ctpp2c ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/arith_ops.tmpl Arith_ops.ct2)
ADD_TEST(Arith_ops_R                        ctpp2vm Arith_ops.ct2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/testdata.json Arith_ops.out)
SET_TESTS_PROPERTIES(Arith_ops_R PROPERTIES DEPENDS Arith_ops_C)
IF (DIFF_EXECUTABLE)
    ADD_TEST(Arith_ops_D                    ${DIFF_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/arith_ops.out Arith_ops.out)
    SET_TESTS_PROPERTIES(Arith_ops_D        PROPERTIES DEPENDS Arith_ops_R)
ENDIF (DIFF_EXECUTABLE)

ADD_TEST(Formulas_C                         ctpp2c ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/formulas.tmpl Formulas.ct2)
ADD_TEST(Formulas_R                         ctpp2vm Formulas.ct2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/testdata.json Formulas.out)
SET_TESTS_PROPERTIES(Formulas_R PROPERTIES DEPENDS Formulas_C)
IF (DIFF_EXECUTABLE)
    ADD_TEST(Formulas_D                     ${DIFF_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/formulas.out Formulas.out)
    SET_TESTS_PROPERTIES(Formulas_D         PROPERTIES DEPENDS Formulas_R)
ENDIF (DIFF_EXECUTABLE)

ADD_TEST(Functions_C                        ctpp2c ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/functions.tmpl Functions.ct2)
ADD_TEST(Functions_R                        ctpp2vm Functions.ct2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/testdata.json Functions.out)
SET_TESTS_PROPERTIES(Functions_R PROPERTIES DEPENDS Functions_C)
IF (DIFF_EXECUTABLE)
    ADD_TEST(Functions_D                    ${DIFF_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/functions.out Functions.out)
    SET_TESTS_PROPERTIES(Functions_D        PROPERTIES DEPENDS Functions_R)
ENDIF (DIFF_EXECUTABLE)

ADD_TEST(Function_random_C                  ctpp2c ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/function_random.tmpl Function_random.ct2)
ADD_TEST(Function_random_R                  ctpp2vm Function_random.ct2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/testdata.json)
SET_TESTS_PROPERTIES(Function_random_R PROPERTIES DEPENDS Function_random_C)

ADD_TEST(Function_obj_dump_C                ctpp2c ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/function_obj_dump.tmpl Function_obj_dump.ct2)
ADD_TEST(Function_obj_dump_R                ctpp2vm Function_obj_dump.ct2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/testdata.json)
SET_TESTS_PROPERTIES(Function_random_R PROPERTIES DEPENDS Function_random_C)

ADD_TEST(Function_hostname_C                  ctpp2c ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/function_hostname.tmpl Function_hostname.ct2)
ADD_TEST(Function_hostname_R                  ctpp2vm Function_hostname.ct2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/testdata.json)
SET_TESTS_PROPERTIES(Function_hostname_R PROPERTIES DEPENDS Function_hostname_C)

IF (${MD5_SUPPORT} MATCHES "ON")
    ADD_TEST(Function_md5_C                 ctpp2c ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/function_md5.tmpl Function_md5.ct2)
    ADD_TEST(Function_md5_R                 ctpp2vm Function_md5.ct2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/testdata.json Function_md5.out)
    SET_TESTS_PROPERTIES(Function_md5_R PROPERTIES DEPENDS Function_md5_C)
    IF (DIFF_EXECUTABLE)
        ADD_TEST(Function_md5_D             ${DIFF_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/function_md5.out Function_md5.out)
        SET_TESTS_PROPERTIES(Function_md5_D PROPERTIES DEPENDS Function_md5_R)
    ENDIF (DIFF_EXECUTABLE)
ENDIF (${MD5_SUPPORT} MATCHES "ON")

IF (${ICONV_SUPPORT} MATCHES "ON")
    ADD_TEST(Function_iconv_C               ctpp2c ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/function_iconv.tmpl Function_iconv.ct2)
    ADD_TEST(Function_iconv_R               ctpp2vm Function_iconv.ct2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/testdata.json Function_iconv.out)
    SET_TESTS_PROPERTIES(Function_iconv_R PROPERTIES DEPENDS Function_iconv_C)
    IF (DIFF_EXECUTABLE)
        ADD_TEST(Function_iconv_D             ${DIFF_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/function_iconv.out Function_iconv.out)
        SET_TESTS_PROPERTIES(Function_iconv_D PROPERTIES DEPENDS Function_iconv_R)
    ENDIF (DIFF_EXECUTABLE)
ENDIF (${ICONV_SUPPORT} MATCHES "ON")

ADD_TEST(Function_gettext_C             ctpp2c ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/function_gettext.tmpl Function_gettext.ct2)
ADD_TEST(Function_gettext_R             ctpp2vm Function_gettext.ct2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/testdata.json Function_gettext.out ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/test.mo)
SET_TESTS_PROPERTIES(Function_gettext_R PROPERTIES DEPENDS Function_gettext_C)
IF (DIFF_EXECUTABLE)
    ADD_TEST(Function_gettext_D             ${DIFF_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/function_gettext.out Function_gettext.out)
    SET_TESTS_PROPERTIES(Function_gettext_D PROPERTIES DEPENDS Function_gettext_R)
ENDIF (DIFF_EXECUTABLE)

ADD_TEST(Loops_C                            ctpp2c ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/loops.tmpl Loops.ct2)
ADD_TEST(Loops_R                            ctpp2vm Loops.ct2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/testdata.json Loops.out)
SET_TESTS_PROPERTIES(Loops_R PROPERTIES DEPENDS Loops_C)
IF (DIFF_EXECUTABLE)
    ADD_TEST(Loops_D             ${DIFF_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/loops.out Loops.out)
    SET_TESTS_PROPERTIES(Loops_D PROPERTIES DEPENDS Loops_R)
ENDIF (DIFF_EXECUTABLE)

ADD_TEST(Lebowski_bench_foreach_C           ctpp2c ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/lebowski-bench-foreach.tmpl lebowski-bench-foreach.ct2)
ADD_TEST(Lebowski_bench_foreach_R           ctpp2vm lebowski-bench-foreach.ct2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/lebowski-bench.json lebowski-bench-foreach.out)
SET_TESTS_PROPERTIES(Lebowski_bench_foreach_R PROPERTIES DEPENDS Lebowski_bench_foreach_C)
IF (DIFF_EXECUTABLE)
    ADD_TEST(Lebowski_bench_foreach_D             ${DIFF_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/lebowski-bench-foreach.out lebowski-bench-foreach.out)
    SET_TESTS_PROPERTIES(Lebowski_bench_foreach_D PROPERTIES DEPENDS Lebowski_bench_foreach_R)
ENDIF (DIFF_EXECUTABLE)

ADD_TEST(ArrayAndHashAccess_C               ctpp2c ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/array_and_hash_access.tmpl array_and_hash_access.ct2)
ADD_TEST(ArrayAndHashAccess_R               ctpp2vm array_and_hash_access.ct2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/array_and_hash_access.json array_and_hash_access.out)
SET_TESTS_PROPERTIES(ArrayAndHashAccess_R PROPERTIES DEPENDS ArrayAndHashAccess_C)
IF (DIFF_EXECUTABLE)
    ADD_TEST(ArrayAndHashAccess_D             ${DIFF_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/array_and_hash_access.out array_and_hash_access.out)
    SET_TESTS_PROPERTIES(ArrayAndHashAccess_D PROPERTIES DEPENDS ArrayAndHashAccess_R)
ENDIF (DIFF_EXECUTABLE)

ADD_TEST(DTOA                               CTPP2DTOATest)

ADD_TEST(Verbose_mode_C                 ctpp2c ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/verbose_mode.tmpl Verbose_mode.ct2)
ADD_TEST(Verbose_mode_R                 ctpp2vm Verbose_mode.ct2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/testdata.json Verbose_mode.out)
SET_TESTS_PROPERTIES(Verbose_mode_R PROPERTIES DEPENDS Verbose_mode_C)
IF (DIFF_EXECUTABLE)
    ADD_TEST(Verbose_mode_D             ${DIFF_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/verbose_mode.out Verbose_mode.out)
    SET_TESTS_PROPERTIES(Verbose_mode_D PROPERTIES DEPENDS Verbose_mode_R)
ENDIF (DIFF_EXECUTABLE)

ADD_TEST(Calls_C                 ctpp2c ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/calls.tmpl Calls.ct2)
ADD_TEST(Calls_R                 ctpp2vm Calls.ct2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/testdata.json Calls.out)
SET_TESTS_PROPERTIES(Calls_R PROPERTIES DEPENDS Calls_C)
IF (DIFF_EXECUTABLE)
    ADD_TEST(Calls_D             ${DIFF_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata/calls.out Calls.out)
    SET_TESTS_PROPERTIES(Calls_D PROPERTIES DEPENDS Calls_R)
ENDIF (DIFF_EXECUTABLE)

FIND_PROGRAM(RST2HTML_EXECUTABLE "rst2html" /usr/local/bin /usr/bin)
IF (RST2HTML_EXECUTABLE)
    ADD_CUSTOM_COMMAND(
        OUTPUT template_language.html
        COMMAND ${RST2HTML_EXECUTABLE}  --stylesheet-path=${CMAKE_CURRENT_SOURCE_DIR}/doc/bootstrap.css,${CMAKE_CURRENT_SOURCE_DIR}/doc/colors.css -l en ${CMAKE_CURRENT_SOURCE_DIR}/doc/template_language.rst ${CMAKE_CURRENT_BINARY_DIR}/template_language.html
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/doc/template_language.rst
        )
    ADD_CUSTOM_TARGET(doc ALL DEPENDS template_language.html)
ENDIF (RST2HTML_EXECUTABLE)

# Install Headers
INSTALL(FILES include/CDT.hpp
              include/CDTSortRoutines.hpp
              include/CTPP2BitIndex.hpp
              include/CTPP2CharIterator.hpp
              include/CTPP2Compiler.hpp
              include/CTPP2DTOA.hpp
              include/CTPP2Exception.hpp
              include/CTPP2Error.hpp
              include/CTPP2ErrorCodes.h
              include/CTPP2FileLogger.hpp
              include/CTPP2FileOutputCollector.hpp
              include/CTPP2FileSourceLoader.hpp
              include/CTPP2GetText.hpp
              include/CTPP2GlobalDefines.h
              include/CTPP2HashTable.hpp
              include/CTPP2JSONFileParser.hpp
              include/CTPP2JSONParser.hpp
              include/CTPP2Logger.hpp
              include/CTPP2OutputCollector.hpp
              include/CTPP2Parser.hpp
              include/CTPP2ParserException.hpp
              include/CTPP2SimpleCompiler.hpp
              include/CTPP2SimpleVM.hpp
              include/CTPP2SourceLoader.hpp
              include/CTPP2Sprintf.hpp
              include/CTPP2StaticData.hpp
              include/CTPP2StaticText.hpp
              include/CTPP2StreamOutputCollector.hpp
              include/CTPP2StringBuffer.hpp
              include/CTPP2StringIconvOutputCollector.hpp
              include/CTPP2StringOutputCollector.hpp
              include/CTPP2SymbolTable.hpp
              include/CTPP2Syntax.h
              include/CTPP2SysHeaders.h
              include/CTPP2SysTypes.h
              include/CTPP2SyscallFactory.hpp
              include/CTPP2Types.h
              include/CTPP2Util.hpp
              include/CTPP2VM.hpp
              include/CTPP2VMArgStack.hpp
              include/CTPP2VMCodeStack.hpp
              include/CTPP2VMDebugInfo.hpp
              include/CTPP2VMDumper.hpp
              include/CTPP2VMException.hpp
              include/CTPP2VMExecutable.hpp
              include/CTPP2VMFileLoader.hpp
              include/CTPP2VMInstruction.hpp
              include/CTPP2VMLoader.hpp
              include/CTPP2VMMemoryCore.hpp
              include/CTPP2VMOpcodeCollector.hpp
              include/CTPP2VMOpcodes.h
              include/CTPP2VMSTDLib.hpp
              include/CTPP2VMStackException.hpp
              include/CTPP2VMSyscall.hpp
              include/STLException.hpp
              include/STLFunctional.hpp
              include/STLIosfwd.hpp
              include/STLMap.hpp
              include/STLOstream.hpp
              include/STLPair.hpp
              include/STLString.hpp
              include/STLVector.hpp
        DESTINATION include/ctpp2
        PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ)

IF("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")

SET(CTPP_COMPILER_HELPER "@echo off
rem
rem CTPP2 Configurator for ${CMAKE_SYSTEM_NAME}
rem

set PREFIX=\"${CMAKE_INSTALL_PREFIX}\"
set CXXFLAGS=\"${CMAKE_CXX_FLAGS}\"
set MAKE=\"${CMAKE_MAKE_PROGRAM}\"
set CC=\"${CMAKE_C_COMPILER}\"
set CXX=\"${CMAKE_CXX_COMPILER}\"
set INCLUDE=\"${ICONV_INCLUDE} ${MD5_INCLUDE} ${CMAKE_INSTALL_PREFIX}/include/ctpp2\"
set INCLUDE2=\"-I${ICONV_INCLUDE} -I${MD5_INCLUDE} -I${CMAKE_INSTALL_PREFIX}/include/ctpp2\"
set LIBS=\"${CMAKE_INSTALL_PREFIX}/lib\"
set VERSION=\"${CTPP_VERSION}\"

if \"%1\" == \"--version\"   echo %VERSION% & goto end
if \"%1\" == \"--flags\"     echo %FLAGS%   & goto end
if \"%1\" == \"--cc\"        echo %CC%      & goto end
if \"%1\" == \"--cxx\"       echo %CXX%     & goto end
if \"%1\" == \"--make\"      echo %MAKE%    & goto end
if \"%1\" == \"--libs\"      echo %LIBS%    & goto end
if \"%1\" == \"--includes2\" echo %INCLUDE2% & goto end
if \"%1\" == \"--includes\"  echo %INCLUDE% & goto end


echo Usage:
echo   ctpp2-config [--flags] [--cc] [--cxx] [--make] [--libs] [--includes] [--version]
echo                   ... [see below for complete flag list]
echo.
echo     --version         displays the ctpp2 version number
echo     --flags           displays C++ compiler flags
echo     --cc              displays executable name of C compiler
echo     --cxx             displays executable name of C++ compiler
echo     --make            displays executable name of make
echo     --libs            displays list of libraries
echo     --includes        displays list of include dirs
echo     --includes2       displays list of include dirs with '-I' prefixes
echo.

:end
")

    INSTALL(TARGETS ctpp2-static DESTINATION lib${LIB_SUFFIX})

    INSTALL(TARGETS ctpp2 DESTINATION .)

    FILE(WRITE ctpp2-config.cmd ${CTPP_COMPILER_HELPER})

    # Install binaries
    INSTALL(FILES ctpp2-config.cmd
            DESTINATION .)

    INSTALL(TARGETS ctpp2vm
                    ctpp2c
                    ctpp2i
                    ctpp2json
            DESTINATION .)

ELSE ("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
# Install library
INSTALL(TARGETS ctpp2 ctpp2-static DESTINATION lib${LIB_SUFFIX})

# Install Manpages
INSTALL(FILES
              man/ctpp2-config.1
              man/ctpp2c.1
              man/ctpp2i.1
              man/ctpp2json.1
              man/ctpp2vm.1
        DESTINATION man/man1
        PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ)

IF(RST2HTML_EXECUTABLE)
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/template_language.html
            DESTINATION share/doc
            PERMISSIONS OWNER_READ OWNER_WRITE
                        GROUP_READ
                        WORLD_READ)
ENDIF(RST2HTML_EXECUTABLE)

ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")


IF("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")


ELSE("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
SET(CTPP_COMPILER_HELPER "#!/bin/sh
#
# CTPP2 Configurator
#
PREFIX=${CMAKE_INSTALL_PREFIX}
CXXFLAGS=\"${CMAKE_CXX_FLAGS}\"
MAKE=${CMAKE_MAKE_PROGRAM}
CC=${CMAKE_C_COMPILER}
CXX=${CMAKE_CXX_COMPILER}
INCLUDE=\"${ICONV_INCLUDE} ${MD5_INCLUDE} ${CMAKE_INSTALL_PREFIX}/include/ctpp2\"
INCLUDE2=\"-I${ICONV_INCLUDE} -I${MD5_INCLUDE} -I${CMAKE_INSTALL_PREFIX}/include/ctpp2\"
LIBS=\"${CMAKE_INSTALL_PREFIX}/lib\"
VERSION=\"${CTPP_VERSION}\"

if test \"x$1\" = \"x\"\; then
  echo \"Usage:\"
  echo \"  ctpp2-config [--flags] [--cc] [--cxx] [--make] [--libs] [--includes] [--version]\"
  echo \"                  ... [see below for complete flag list]\"
  echo \"\"
  echo \"    --version         displays the ctpp2 version number\"
  echo \"    --flags           displays C++ compiler flags\"
  echo \"    --cc              displays executable name of C compiler\"
  echo \"    --cxx             displays executable name of C++ compiler\"
  echo \"    --make            displays executable name of make\"
  echo \"    --libs            displays list of libraries\"
  echo \"    --includes        displays list of include dirs\"
  echo \"    --includes2       displays list of include dirs with '-I' prefixes\"
  echo \"\"
else
   while test \"x$done\" = \"x\" -a \"x$1\" != \"x\"\; do
       case $1 in
           --version*)
           echo \${VERSION}
           \;\;

           --flags*)
           echo \${CXXFLAGS}
           \;\;

           --cc*)
           echo \${CC}
           \;\;

           --cxx*)
           echo \${CXX}
           \;\;

           --make*)
           echo \${MAKE}
           \;\;

           --libs*)
           echo \${LIBS}
           \;\;

           --includes2*)
           echo \${INCLUDE2}
           \;\;

           --includes*)
           echo \${INCLUDE}
           \;\;

       esac
       shift
   done
fi
")

    # Write file
    FILE(WRITE ctpp2-config ${CTPP_COMPILER_HELPER})

    # Install binaries
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/ctpp2vm
                  ${CMAKE_CURRENT_BINARY_DIR}/ctpp2c
                  ${CMAKE_CURRENT_BINARY_DIR}/ctpp2i
                  ${CMAKE_CURRENT_BINARY_DIR}/ctpp2json
                  ctpp2-config
            DESTINATION bin
            PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE
                        GROUP_EXECUTE GROUP_READ
                        WORLD_EXECUTE WORLD_READ)

ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")

# End.
