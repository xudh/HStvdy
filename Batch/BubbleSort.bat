@ECHO OFF
ECHO ð������
SETLOCAL ENABLEDELAYEDEXPANSION
FOR /L %%i IN (0,1,9) DO SET /P a[%%i]=�������%%i������: 
FOR /L %%i IN (0,1,8) DO (    
    SET /A inj=8-%%i
    FOR /L %%j IN (0,1,!inj!) DO (
        SET /A ink=%%j+1
        ::Ϊ���ƹ�!a[!ink!]!����һ��FOR���
        FOR %%k IN (!ink!) DO (
            IF !a[%%j]! GTR !a[%%k]! (
                SET tmp=!a[%%j]!
                SET a[%%j]=!a[%%k]!
                SET a[%%k]=!tmp!
            )
        )
    )
)
ECHO ���������£�
FOR /L %%i IN (0,1,9) DO <NUL,SET /P=!a[%%i]! 
PAUSE