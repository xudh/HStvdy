@echo off
set "len=for /f "tokens=1-3" %%1 in ("#a#") do setlocal enabledelayedexpansion&(if defined %%2 (set /a z=8180,x=0&(for /l %%a in (1,1,14) do set/a "y=(z-x)/2+x"&(for %%b in (!y!) do if "!%%2:~%%b,1!" equ "" (set/a z=y) else (set/a x=y)))) else (set z=0))&(for %%z in ("!z!") do endlocal&set %%1=%%~z)"
::取字符串长度函数，调用方法：%len:#a#=结果变量名 字符串变量名%
SET /P str=input:
%len:#A#=strlen str%
ECHO;%strlen%
PAUSE


::for /f "skip=1 delims=:" %%i in ('^(echo "%x%"^&echo.^)^|findstr /o ".*"') do set/a l=%%i-5
::echo %l%
::http://www.verybat.org/viewthread.php?tid=19241&extra=page%3D1&frombbs=1
::http://www.bathome.cn/viewthread.php?tid=5925&extra=page%3D1&frombbs=1