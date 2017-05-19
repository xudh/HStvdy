@ECHO OFF
for /l %%a in (1,1,6) do @<nul,set /p=%%a
for /l %%a in (1,1,6) do @<nul set /p=%%a
for /L %%a in (1 1 6) do @set /p=%%a 0<nul
for /L %%a in (1 1 6) do @set /p=%%a <NUL
ECHO.
ECHO.
::<NUL的话可以删除掉空行而且避免输入
::set /p=%%a是个特殊的语法。直接现实变量数值,相对于ECHO XXX的好处是没有换行。
pause
