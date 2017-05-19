@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION
SET /P num=请输入你要求的阶层: 
IF %num% GTR 1000 (ECHO 太大了拒绝计算) ELSE (
    SET s[0]=1
    SET wei=1
    FOR /L %%i IN (1,1,%num%) DO (
        SET jin=0
   	SET /A wei_1=!wei!-1
        FOR /L %%j IN (0,1,!wei_1!) DO (
 	    SET /A tmp=!jin!+!s[%%j]!*%%i
            SET /A "s[%%j]=!tmp!%%10000"
            SET /A jin=!tmp!/10000
	)
    	IF !jin! GTR 0 (
 	    SET /A wei=!wei!+1
            SET /A wei_1=!wei!-1
            FOR %%k IN (!wei_1!) DO SET /A s[%%k]=!jin!
        )
    )
    SET /A wei_1=!wei!-1
    FOR %%i IN (!wei_1!) DO @<NUL,SET /P=!s[%%i]!
    SET /A wei_2=!wei!-2    
    FOR /L %%i IN (!wei_2!,-1,0) DO (
        FOR %%j IN (!s[%%i]!) DO (
	    SET len4=0000%%j
            SET len4=!len4:~-4,4!
            ::保证输出的是四位数
            <NUL SET /P=!len4!
        )
    )
)
ECHO.
PAUSE