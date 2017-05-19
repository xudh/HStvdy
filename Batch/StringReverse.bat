@ECHO OFF
set "len=for /f "tokens=1-3" %%1 in ("#a#") do setlocal enabledelayedexpansion&(if defined %%2 (set /a z=8180,x=0&(for /l %%a in (1,1,14) do set/a "y=(z-x)/2+x"&(for %%b in (!y!) do if "!%%2:~%%b,1!" equ "" (set/a z=y) else (set/a x=y)))) else (set z=0))&(for %%z in ("!z!") do endlocal&set %%1=%%~z)"
::取字符串长度函数，调用方法：%len:#a#=结果变量名 字符串变量名%
SETLOCAL ENABLEDELAYEDEXPANSION
SET str=%1
%len:#A#=strlen str%
SET /A fid=%strlen%-1
FOR /L %%i IN (%fid%,-1,0) DO (
    SET cutstr=!str:~%%i,1!
    @<NUL,SET /P=!cutstr!
)
PAUSE