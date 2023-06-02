@echo off

if "%2" == "" (
    echo usage: %~n0 inputfile outputfile
    exit 1
)

set DIRECTORY=%~dp1
call :getproject %DIRECTORY:~0,-1%


rem 相対アドレスの場合はWorkを起点とすることに注意
set COMMAND="..\..\x64\Debug\%PROJECT_NAME%.exe"

if not exist "%COMMAND%" (
    echo %COMMAND%: no such file or directory.
    exit 1
)

%COMMAND% < %1 > %2

exit 0

:getproject
set PROJECT_DIRECTORY=%~dp1
call :getprojectsub %PROJECT_DIRECTORY:~1,-1%
exit /b

:getprojectsub
set PROJECT_NAME=%~n1
exit /b
