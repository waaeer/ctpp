@echo off

if exist Makefile nmake clean



for /F  %%a in ('dir /AD /B *.dir') do if exist %%a rmdir /S /Q %%a
for %%a in (CMakeFiles Testing doc\html) do if exist %%a rmdir /S /Q %%a


for %%a in (CMakeCache.txt DartTestfile.txt Makefile cmake_install.cmake progress.make *.ftss data\*.ftss include\CTPP2SysHeaders.h *.ct2 ctpp2-config.cmd CTestTestfile.cmake) do (
	if exist %%a del /F /Q %%a
)

for %%a in (*.cmake) do if not "%%a" == "CTPP2SysHeaders.h.cmake" del %%a

for %%a in (*.rule *.dsp *.dsw *.vcproj *.sln *.vcproj.*.user *.ncb *.ilk *.exp *.suo *.manifest *.manifest.res *.resource.txt) do (
	if exist %%a del /F /Q %%a
)

del /F /Q /AH *.suo 2>nul
