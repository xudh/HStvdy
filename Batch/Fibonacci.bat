@ECHO OFF
SET /P num=请问你要获得fibonacci数列的前几位: 
IF %num% GTR 0 (
    IF %num% GTR 1 (
        IF %num% GTR 2 (
            SETLOCAL ENABLEDELAYEDEXPANSION
            SET fa=0
            SET fb=1
            <NUL,SET /P=!fa! !fb! 
            FOR /L %%i IN (3,1,%num%) DO (
                SET /A fb=!fa!+!fb!
                SET /A fa=!fb!-!fa!
                <NUL,SET /P=!fb! 
            )
        )ELSE ECHO 0 1
    ) ELSE ECHO 0
)
ECHO.
PAUSE