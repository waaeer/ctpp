Summary: 	CTPP2 template engine.
Name: 		ctpp2
Version: 	2.8.3
Release: 	0%{?dist}
License: 	BSD
Source: 	ctpp2-%{version}.tar.gz
Group:		System Environment/Libraries
BuildRoot: 	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildRequires:	openssl-devel cmake gcc-c++

%package devel
Summary:	Header files and development documentation for %{name}
Group: 		Development/Libraries
Requires:	%{name} = %{version}-%{release}

%description
CTPP2 library.
%description devel
CTPP2 header files.

This package contains the header files, static libraries and development
documentation for %{name}. If you like to develop programs using %{name},
you will need to install %{name}-devel.

%prep
%setup -q -n ctpp2-%{version}

%build
cmake -D SKIP_RELINK_RPATH=ON . -DCMAKE_INSTALL_PREFIX=/usr -DCFLAGS="${CFLAGS}" -DCXXFLAGS="${CXXFLAGS}"
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
mkdir %{buildroot}
make DESTDIR=%{buildroot} install
mkdir $RPM_BUILD_ROOT/usr/share
mv $RPM_BUILD_ROOT/usr/man $RPM_BUILD_ROOT/usr/share/man
%if %_lib == "lib64"
mkdir %{buildroot}/usr/lib64
mv %{buildroot}/usr/lib/* %{buildroot}/usr/lib64
%endif

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%{_bindir}/ctpp2-config
%{_bindir}/ctpp2c
%{_bindir}/ctpp2i
%{_bindir}/ctpp2json
%{_bindir}/ctpp2vm
%{_libdir}/libctpp2.so*
#%{_datadir}/locale/ru_RU.CP1251/LC_MESSAGES/ctpp2.mo
#%{_datadir}/locale/ru_RU.CP866/LC_MESSAGES/ctpp2.mo
#%{_datadir}/locale/ru_RU.KOI8-R/LC_MESSAGES/ctpp2.mo
#%{_datadir}/locale/ru_RU.UTF-8/LC_MESSAGES/ctpp2.mo
%{_mandir}/*/*

%files devel
%defattr(-,root,root,-)
%{_includedir}/ctpp2
%{_libdir}/libctpp2-st.a

%changelog
* Sat Nov 10 2012 Alexander Pankov <pianist@usrsrc.ru> - 2.8.3-0
+ Returned curly brakets as an alias for square
+ Fixed error reporting

* Fri Jul 13 2012 Andrei V. Shetuhin <reki@reki.ru> - 2.8.2-0
- Removed TMPL_loop. Added TMPL_foreach's attributes for iterator.
- TMPL_foreach works with HASH.
- Fixed TMPL_call. It works with variables.
- Removed the attribute 'map' of TMPL_include.
- Added CONTEXT(), LIST().
- Removed NUMERAL(), ARRAY_ELEMENT(), HASH_ELEMENT().
- HASH_ELEMENT().
- Renamed JSESCAPE() as JSONESCAPE(). Refactored the code.
- Added new documentation of the CT++ template language.
- Added the operator <TMPL_break>.
- Small fixes.
 
* Wed Dec 28 2011 Andrei V. Shetuhin <reki@reki.ru> - 2.8.0-0
- Improved access to arrays and hashes

* Wed Dec 28 2011 Andrei V. Shetuhin <reki@reki.ru> - 2.7.6-0
- Bug fixes

* Fri Dec 23 2011 Andrei V. Shetuhin <reki@reki.ru> - 2.7.5-0
- Bug fixes

* Mon Dec 19 2011 Andrei V. Shetuhin <reki@reki.ru> - 2.7.4-0
- Bug fixes

* Mon Nov 14 2011 Andrei V. Shetuhin <reki@reki.ru> - 2.7.3-0
- Bug fixes

* Wed Oct  5 2011 Andrei V. Shetuhin <reki@reki.ru> - 2.7.2-0
- Bug fixes

* Fri Jul  8 2011 Andrei V. Shetuhin <reki@reki.ru> - 2.7.1-0
- Production-ready version of CTPP2 2.7.X

* Thu Jun 16 2011 Andrei V. Shetuhin <reki@reki.ru> - 2.7.0-0
- New version of CTPP: 2.7.0

* Mon May 16 2011 Andrei V. Shetuhin <reki@reki.ru> - 2.6.15-0
- Bug fixes

* Thu Apr 28 2011 Andrei V. Shetuhin <reki@reki.ru> - 2.6.14-0
- Bug fixes

* Fri Apr  1 2011 Andrei V. Shetuhin <reki@reki.ru> - 2.6.13-0
- Bug fixes

* Tue Mar 11 2011 Andrei V. Shetuhin <reki@reki.ru> - 2.6.12-0
- Bug fixes

* Mon Feb 28 2011 Andrei V. Shetuhin <reki@reki.ru> - 2.6.11-0
- Bug fixes

* Fri Feb 11 2011 Andrei V. Shetuhin <reki@reki.ru> - 2.6.10-0
- Bug fixes, new functions: GET_TYPE and HOSTNAME

* Mon Nov  1 2010 Andrei V. Shetuhin <reki@reki.ru> - 2.6.7-0
- Bug fixes

* Tue Oct  5 2010 Andrei V. Shetuhin <reki@reki.ru> - 2.6.6-0
- Bug fixes

* Wed Sep 22 2010 Andrei V. Shetuhin <reki@reki.ru> - 2.6.5-0
- Fixes in CTPP2 dialect compiler

* Mon Sep 20 2010 Andrei V. Shetuhin <reki@reki.ru> - 2.6.4-0
- Improvements in SortArray method

* Tue Jul 27 2010 Andrei V. Shetuhin <reki@reki.ru> - 2.6.3-0
- Fixes in TMPL_foreach iterators (thanks to Kirichenko Sergey <kirichenko@post.km.ru>)

* Wed Jul 20 2010 Andrei V. Shetuhin <reki@reki.ru> - 2.6.2-0
- New methods for CDT class: Swap & SortArray.

* Wed Jun 16 2010 Andrei V. Shetuhin <reki@reki.ru> - 2.6.1-0
- New minor version. See CHANGES.

* Mon May 24 2010 Andrei V. Shetuhin <reki@reki.ru> - 2.5.19-0
- Bug fixes

* Fri Apr  2 2010 Andrei V. Shetuhin <reki@reki.ru> - 2.5.17-0
- New function SPRINTF, bug fixes

* Wed Dec 30 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.5.15-0
- Fixes in function NUM_FORMAT

* Fri Dec  4 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.5.14-0
- New method for class CDT: Erase. Erase element from hash by specified key name

* Tue Nov 24 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.5.13-0
- Fixes in Base64Encode function

* Sun Oct 18 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.5.12-0
- New output data collector, port to CygWin

* Thu Aug 27 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.5.11-0
- Fixes in virtual machine: CMP (STACK/STACK)

* Tue Aug 18 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.5.10-0
- New functions: LIST_ELEMENT & ARRAY_ELEMENT. Fixes in JSON parser.

* Thu Aug 13 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.5.9-0
- Fixes in <TMPL_foreach .. > <TMPL_include "file"> </TMPL_foreach>

* Wed Aug  5 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.5.8-0
- Completely rewrite code of JSON parser

* Wed Jun 24 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.5.7-0
- New function: NUMERAL, fixes in TRUNCATE & MB_TRUNCATE functions

* Tue Jun  9 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.5.6-0
- Fixes in JSESCAPE function

* Tue Jun  2 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.5.5-0
- Fixes in math. expressions inside functions: <TMPL_var FOO(bar + baz)>

* Tue May 20 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.5.4-0
- Avoid a lot of warnings on gcc 4.3+, #include <...> changed to #include "..." for all files of project

* Tue May 12 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.5.3-0
- Removed inline methods of exception classes, fixed install-no-mkdir-buildroot error in .spec file.

* Mon Apr 20 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.5.2-0
- Bug fixes in branches inside foreach operator

* Fri Apr 17 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.5.1-0
- Bug fixes, documentation

* Thu Apr 16 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.5.0-0
- New operator: <TMPL_foreach; Gettext function improvements

* Wed Apr  8 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.4.10-0
- New function: WMLESCAPE

* Tue Apr  2 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.4.9-0
- New classes: SimpleVM, SimpleCompiler

* Tue Mar 18 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.4.8-0
- New functions: CONCAT, SUBSTR, TRUNCATE, MB_SIZE, MB_TRUNCATE, MB_SUBSTR

* Mon Mar 16 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.4.7-0
- New flags for iconv converter

* Sat Mar  7 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.4.6-0
- New output data collector with charset recoding, new functions

* Wed Mar  4 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.4.5-0
- Port to MacOS, new functions: RANDOM and LOG

* Sat Feb 15 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.4.4-0
- New functions: HMAC_MD5, URIESCAPE

* Tue Feb 10 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.4.3-0
- Port to Win32 platform

* Sun Jan 25 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.4.2-0
- Bug fixes

* Sun Jan 18 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.4.1-0
- New functions: JSON, JSESCAPE; Documentation for new features

* Wed Jan 14 2009 Andrei V. Shetuhin <reki@reki.ru> - 2.4.0-0
- <TMPL_call & <TMPL_block operators

* Fri Nov  7 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.3.11-0
- JSON parser improvements

* Tue Sep 16 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.3.10-0
- Make Valgrind happy & port to ALT Linux

* Fri Sep 12 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.3.9-0
- Improvements in parsing Non-HTML templates

* Fri Sep  5 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.3.8-0
- Port on SunOs 5.10 i386 and amd64

* Tue Sep  2 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.3.7-0
- Nested loops from various data sources, bug fixes

* Mon Aug 11 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.3.6-0
- <TMPL_loop accept now contextual variable __CONTENT__ as argument

* Thu Aug  7 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.3.5-0
- New input data formats of CAST and DATE_FORMAT functions

* Fri Jul 11 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.3.4-0
- Debian Lenny bug fixes

* Wed Jun 24 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.3.3-0
- Debug subsystem improvements

* Wed Jun 18 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.3.2-0
- New methods of CDT class

* Tue Jun  3 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.3.1-0
- More information about errors in compiler

* Wed May 21 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.3.0-0
- Bug fixes; new CTPP2 debug subsystem

* Wed May 14 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.2.3-0
- Bug fixes; new function: NUM_FORMAT

* Mon Apr 28 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.2.2-0
- Bug fixes only

* Wed Apr 23 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.2.1-0
- New function: ARRAY_SIZE

* Fri Apr 18 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.2.0-0
- Bug fixes, support of crossplatform bytecode file

* Fri Mar 29 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.1.2-0
- Inverse translation map in <TMPL_include ...

* Wed Mar 19 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.1.1-0
- Translation map in <TMPL_include ...

* Fri Mar 14 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.1.0-0
- Math. operations, speed improvements, bug fixes

* Mon Mar  3 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.0.5-0
- Namespaces in variable names, bug fixes

* Mon Feb 18 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.0.4-0
- New contextual variable: __COUNTER__, small bug fixes

* Fri Feb 15 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.0.3-0
- New function: iconv

* Fri Feb  8 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.0.2-0
- Bug fixes, new functions

* Mon Feb  4 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.0.1-0
- Bug fixes

* Fri Jan 26 2008 Andrei V. Shetuhin <reki@reki.ru> - 2.0.0-0
- First version 2.X release.
