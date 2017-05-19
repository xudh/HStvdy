@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION
FOR /L %%i IN (1,1,7) DO (
    SET /A a=%%i-4
    SET a=!a:-=!
    FOR /L %%j IN (1,1,!a!) DO <NUL,SET /P= 
    ::注意上行最后是空格
    SET /A b=7-2*!a!
    FOR /L %%j IN (1,1,!b!) DO <NUL,SET /P=*
    ECHO.
)
PAUSE