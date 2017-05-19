@ECHO OFF
ECHO 冒泡排序：
SETLOCAL ENABLEDELAYEDEXPANSION
FOR /L %%i IN (0,1,9) DO SET /P a[%%i]=请输入第%%i个数据: 
FOR /L %%i IN (0,1,8) DO (    
    SET /A inj=8-%%i
    FOR /L %%j IN (0,1,!inj!) DO (
        SET /A ink=%%j+1
        ::为了绕过!a[!ink!]!多用一个FOR语句
        FOR %%k IN (!ink!) DO (
            IF !a[%%j]! GTR !a[%%k]! (
                SET tmp=!a[%%j]!
                SET a[%%j]=!a[%%k]!
                SET a[%%k]=!tmp!
            )
        )
    )
)
ECHO 排序结果如下：
FOR /L %%i IN (0,1,9) DO <NUL,SET /P=!a[%%i]! 
PAUSE