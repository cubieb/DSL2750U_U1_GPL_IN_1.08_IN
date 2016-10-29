@echo off

rem "detect" directory of script:
set dir=%~d0%~p0
echo change to directory %dir%
cd /d %dir%

if "%1"=="" goto do_list
rem prepare single file %1 for check
echo -u %~f1 > %dir%files.lnt
goto doit

:do_list
del %dir%files.lnt
for %%i in (src/*.c) do echo %%~di%%~pisrc\%%~i >> %dir%files.lnt
goto doit

:doit
echo. >> %dir%prj_incl.lnt
lint-nt %2 std.lnt %dir%prj_incl.lnt %dir%files.lnt

echo Lint completed !!!

