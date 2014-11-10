/*-
 * Copyright (c) 2004 - 2010 CTPP Team
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
 *      CTPP2SysHeaders.h
 *
 * $CTPP$
 */
#ifndef _CTPP2_SYS_HEADERS_H__
#define _CTPP2_SYS_HEADERS_H__ 1

#cmakedefine HAVE_SYS_TYPES_H     1

#cmakedefine HAVE_SYS_TIME_H      1

#cmakedefine HAVE_SYS_UIO_H       1

#cmakedefine HAVE_FCNTL_H         1

#cmakedefine HAVE_MATH_H          1

#cmakedefine HAVE_STDIO_H         1

#cmakedefine HAVE_STDLIB_H        1

#cmakedefine HAVE_STRING_H        1

#cmakedefine HAVE_STRINGS_H       1

#cmakedefine HAVE_TIME_H          1

#cmakedefine HAVE_UNISTD_H        1

#cmakedefine HAVE_SYSEXITS_H      1

#cmakedefine DEBUG_MODE           1

#cmakedefine NO_STL_STD_NS_PREFIX 1

#cmakedefine GETTEXT_SUPPORT      1

#cmakedefine MD5_SUPPORT          1

#cmakedefine MD5_WITHOUT_OPENSSL  1

#cmakedefine CTPP_FLOAT_PRECISION   ${CTPP_FLOAT_PRECISION}

#cmakedefine CTPP_ESCAPE_BUFFER_LEN ${CTPP_ESCAPE_BUFFER_LEN}

#cmakedefine CTPP_MAX_TEMPLATE_RECURSION_DEPTH ${CTPP_MAX_TEMPLATE_RECURSION_DEPTH}

#cmakedefine ICONV_SUPPORT       1

#cmakedefine ICONV_DISCARD_ILSEQ 1

#cmakedefine ICONV_TRANSLITERATE 1

#cmakedefine CTPP_VERSION         "${CTPP_VERSION}"
#cmakedefine CTPP_IDENT           "${CTPP_IDENT}"
#cmakedefine CTPP_MASTER_SITE_URL "${CTPP_MASTER_SITE_URL}"

#cmakedefine THROW_EXCEPTION_IN_COMPARATORS 1

#endif /* _CTPP2_SYS_HEADERS_H__ */
/* End. */
