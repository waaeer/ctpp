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
 *      CTPP2Util.hpp
 *
 * $CTPP$
 */
#include "CTPP2Types.h"

#include "STLString.hpp"

/**
  @file CTPP2Util.hpp
  @brief CTPP2 useful utilities
*/

namespace CTPP // C++ Template Engine
{
// FWD
class CDT;

/**
  @class DumpBuffer CTPP2Util.hpp <CTPP2Util.hpp>
  @brief Buffer for dumping CDTs. Used as an alternative for std::string (as string's append is quite slow)
*/
class CTPP2DECL DumpBuffer {
public:
	/**
	  @var typedef UINT_32 StreamSize
	  @brief internal size_t analog
	*/
	typedef UINT_32 StreamSize;

	/**
	  @brief Constructor
	*/
	DumpBuffer();

	/**
	  @brief Destructor
	*/
	~DumpBuffer();

	/**
	  @brief Appends data to the buffer
	  @param data - ponter to data to append
	  @param n - number of bytes to append
	*/
	void Write(CCHAR_P pData , StreamSize iSize);

	/**
	  @brief Returns pointer to buffer's start
	  @return Pointer to buffer's start
	*/
	CCHAR_P Data() const;

	/**
	  @brief Returns number of bytes available in buffer
	  @return Number of bytes available in buffer
	*/
	StreamSize Size() const;

private:
	/**
	  @brief Reallocates buffer if necessary
	  @param n - new capacity (if it's less then current capacity, method does nothing)
	*/
	void Reserve(StreamSize iSize);

	CHAR_P pBuffer;
	CHAR_P pPos;

	StreamSize iCapacity;
};

/**
  @fn UINT_32 crc32(UCCHAR_P sBuffer, const UINT_32 & iSize)
  @brief Calculate crc32 checksum
  @param sBuffer - buffer with source data to calculate CRC
  @param iSize - buffer size
  @return CRC32 checksum
*/
CTPP2DECL UINT_32 crc32(UCCHAR_P sBuffer, const UINT_32 & iSize);

/**
  @fn UINT_32 Swap32(const UINT_32 & iValue)
  @brief Swap bytes for UINT_32 value
  @param iValue - value to swap
  @return Swapped value
*/
CTPP2DECL UINT_32 Swap32(const UINT_32 & iValue);

/**
  @fn UINT_64 Swap64(const UINT_64 & iValue)
  @brief Swap bytes for UINT_64 value
  @param iValue - value to swap
  @return Swapped value
*/
CTPP2DECL UINT_64 Swap64(const UINT_64 & iValue);

/**
  @fn STLW::string URIEscape(const STLW::string & sData)
  @brief Escape value (URI rules)
  @param sData - value to escape
  @return Escaped value
*/
CTPP2DECL STLW::string URIEscape(const STLW::string  & sData);

/**
  @fn STLW::string URLEscape(const STLW::string & sData)
  @brief Escape value (URL rules)
  @param sData - value to escape
  @return Escaped value
*/
CTPP2DECL STLW::string URLEscape(const STLW::string & sData);

/**
  @fn STLW::string HTMLEscape(const STLW::string & sData)
  @brief Escape value (HTML rules)
  @param sData - value to escape
  @return Escaped value
*/
CTPP2DECL STLW::string HTMLEscape(const STLW::string & sData);

/**
  @fn STLW::string XMLEscape(const STLW::string & sData)
  @brief Escape value (XML rules)
  @param sData - value to escape
  @return Escaped value
*/
CTPP2DECL STLW::string XMLEscape(const STLW::string & sData);

/**
  @fn STLW::string WMLEscape(const STLW::string & sData)
  @brief Escape value (WML rules)
  @param sData - value to escape
  @return Escaped value
*/
CTPP2DECL STLW::string WMLEscape(const STLW::string  & sData);

/**
  @fn void CDT2JSON(const CTPP::CDT & oCDT, STLW::string & sData)
  @brief Dump CDT to JSON
  @param oCDT - input data
  @param sData - output string
*/
CTPP2DECL void CDT2JSON(const CDT & oCDT, STLW::string & sData);

/**
  @fn void DumpCDT2JSON(const CTPP::CDT & oCDT, DumpBuffer & oBuffer)
  @brief Dump CDT to JSON
  @param oCDT - input data
  @param oBuffer - buffer to dump CDT to
  @return reference to oBuffer
*/
CTPP2DECL DumpBuffer & DumpCDT2JSON(const CTPP::CDT & oCDT, DumpBuffer & oBuffer);

/**
  @brief Escape string, if need
  @param sSource - input data
  @param bECMAConventions - use ECMA-262 conventions for escape sequences
  @return Escaped string
*/
CTPP2DECL STLW::string EscapeJSONString(const STLW::string  & sSource,
                                        const bool          & bECMAConventions = true,
                                        const bool          & bHTMLSafe = true);

/**
  @brief Escape and dump string to buffer
  @param oBuffer - buffer to dump string to
  @param sSource - input data
  @param bECMAConventions - use ECMA-262 conventions for escape sequences
  @return reference to oBuffer
*/
CTPP2DECL DumpBuffer & DumpJSONString(DumpBuffer& oBuffer,
                                      const STLW::string & sSource,
                                      const bool & bECMAConventions = true,
                                      const bool & bHTMLSafe = true);

/**
  @fn STLW::string Base64Encode(const STLW::string & sData);
  @brief Encode value in BASE64 encoding
  @param sData - value to encode
  @return Encoded value
*/
CTPP2DECL STLW::string Base64Encode(const STLW::string & sData);

/**
  @fn STLW::string Base64Decode(const STLW::string & sData);
  @brief Decode value from BASE64 encoding
  @param sData - value to decode
  @return Decoded value
*/
CTPP2DECL STLW::string Base64Decode(const STLW::string & sData);

/**
  @fn INT_32 utf_charlen(CCHAR_P szString, CCHAR_P szStringEnd);
  @brief Length of UTF character
  @brief szString - source string
  @brief szStringEnd - pointer to the end of string
  @return >0 - character length,
          -1 - error in multibyte sequence,
          -2 - is not an UTF8 character,
          -3 - unexpected end of string reached
*/
CTPP2DECL INT_32 utf_charlen(CCHAR_P szString, CCHAR_P szStringEnd);

/**
  @fn INT_32 UnicodeToUTF8(UINT_32 iUCS, char * sUTF8)
  @brief Convert Unicode character to UTF8
  @param iUCS - Unicode symbol
  @param sUTF8 - Pointer to buffer, at least 6 octets
  @return UTF8 character length
*/
CTPP2DECL INT_32 UnicodeToUTF8(UINT_32 iUCS, UCHAR_P sUTF8);

} // namespace CTPP
// End.
