@ECHO OFF
SET /A a=45
SET b=70
SET /A sum=a+b 
::有/A的是计算用，没/A的是赋值用，但是计算的有时可以覆盖赋值用的
@ECHO %a%+%b%=%sum%
PAUSE