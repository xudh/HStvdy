@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION
SET tb=ABCDEFGHIJKLMNOPQRSTUVWXYZ
FOR /L %%i IN (-25,1,25) DO (
    SET absi=%%i
    ::�����ֵ���Ѹ��Ż��ɿհ�
    SET absi=!absi:-=!
    FOR /L %%j IN (-25,1,25) DO (
        SET absj=%%j
        SET /A "absj=(!absj!*2+1)%%2*!absj!"
        ::�����ֵ��ģ������*j�����ﲻ��ʡ��˫�����ˣ���������Ų�ƥ������
        SET max=
        IF !absi! GTR !absj! (SET "max=!absi!") ELSE (SET "max=!absj!")
        CALL SET result=%%tb:~!max!,1%%
        @<NUL,SET /P=!result!
    )
    ECHO.
)
PAUSE
::http://bbs.verybat.org/viewthread.php?tid=19223&extra=page%3D1&frombbs=1