@ECHO OFF
SET /P a=请输入开始的数字：
SET /P b=请输入结束的数字：
::SET sum=0
IF %b% GTR %a% (
    ECHO %b%
) ELSE (
ECHO %a%
)
PAUSE