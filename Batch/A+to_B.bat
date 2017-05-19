@ECHO OFF
:Again
SET /P a=请输入开始的数字：
SET /P b=请输入结束的数字：
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
SET /P flag=是否继续(Y/N):
IF "%flag%" == "Y" (goto Again) ELSE EXIT