@ECHO OFF
:Again
SET /P a=�����뿪ʼ�����֣�
SET /P b=��������������֣�
SET sum=0
IF %b% GTR %a% (
    FOR /L %%i IN (%a%,1,%b%) DO (
        SET /A sum=sum+%%i
    )
) ELSE (
    FOR /L %%i IN (%b%,1,%a%) DO (
        SET /A sum=sum+%%i
    )
)
ECHO %sum%
SET /P flag=�Ƿ����(Y/N):
IF "%flag%" == "Y" (goto Again) ELSE EXIT