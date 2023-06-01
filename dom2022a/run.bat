@echo off
rem 相対アドレスの場合はWorkを起点とすることに注意
set COMMAND="..\..\x64\Debug\dom2022a.exe"

if not exist "%COMMAND%" (
    echo %COMMAND%: no such file or directory.
    exit 1
)

if "%2" == "" (
    echo usage: %~n0 inputfile outputfile
    exit 1
)

%COMMAND% < %1 > %2