@echo off

if "%1"=="danube" goto CFG_DANUBE
if "%1"=="vinax" goto CFG_VINAX
goto USAGE

:CFG_DANUBE
set OUTFILE=dsl_cpe_cli_danube
set DEVICE_INCLUDE=INCLUDE_DSL_CPE_API_DANUBE
goto START

:CFG_VINAX
set OUTFILE=dsl_cpe_cli_vinax
set DEVICE_INCLUDE=INCLUDE_DSL_CPE_API_VINAX
goto START

:START

if "%SWIGPROC%"=="" set SWIGPROC=C:\tools\swig\SWIG-1.3.24\swig.exe
if "%XSLTPROC%"=="" set XSLTPROC=C:\tools\libxslt-1.1.14.win32\bin\xsltproc.exe

set INPATH=..\..\..\driver\src\include
set OUTPATH=..\src
set OUTFILE_C=%OUTPATH%\%OUTFILE%.c
set OUTFILE_XML=%OUTFILE%.xml

attrib -r .\%OUTFILE_XML%
attrib -r %OUTFILE_C%


echo /* intermediate header for swig */ > tmp.h
echo #define %DEVICE_INCLUDE% >> tmp.h
for %%i in (..\..\..\driver\src\include\drv_dsl_cpe_api*.h) do echo #include "%%i" >> tmp.h
for %%i in (..\..\..\driver\src\include\drv_dsl_cpe_intern.h) do echo #include "%%i" >> tmp.h

echo calling SWIG, generate from interface header the xml file
%SWIGPROC% -o .\%OUTFILE_XML% -ignoremissing -includeall -xml -module dsl_cpe -I%INPATH% tmp.h

rem echo calling xsltproc, transform the xml file into the command line interface file
%XSLTPROC% -o %OUTFILE_C% .\swig2cli.xsl .\%OUTFILE_XML%

rem echo calling indent, bring the c coding into the correct form
indent %OUTFILE_C%
dos2unix %OUTFILE_C%

goto EXIT

:USAGE
echo "usage:"
echo "%0 <device>"
echo ""
echo "<device>: danube, vinax"

:EXIT
