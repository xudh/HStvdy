@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION
SET tb=ABCDEFGHIJKLMNOPQRSTUVWXYZ
FOR /L %%i IN (-25,1,25) DO (
    SET absi=%%i
    ::求绝对值，把负号换成空白
    SET absi=!absi:-=!
    FOR /L %%j IN (-25,1,25) DO (
        SET absj=%%j
        SET /A "absj=(!absj!*2+1)%%2*!absj!"
        ::求绝对值，模运算再*j，这里不能省略双引号了，会出现括号不匹配问题
        SET max=
        IF !absi! GTR !absj! (SET "max=!absi!") ELSE (SET "max=!absj!")
        CALL SET result=%%tb:~!max!,1%%
        @<NUL,SET /P=!result!
    )
    ECHO.
)
PAUSE
::http://bbs.verybat.org/viewthread.php?tid=19223&extra=page%3D1&frombbs=1