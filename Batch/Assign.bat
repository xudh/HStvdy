@ECHO OFF
SET max=3
SET tb=ABCDEFGHIJKLMNOPQRSTUVWXYZ 
SET result=%tb:~max,1%                   ::这个是赋值不是计算，不要/A
ECHO %result%
PAUSE