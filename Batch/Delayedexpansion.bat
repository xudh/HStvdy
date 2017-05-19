@ECHO OFF
setlocal enabledelayedexpansion ::开启延迟变量
SET max=3
SET tb=ABCDEFGHIJKLMNOPQRSTUVWXYZ 
SET result=!tb:~%max%,1!         ::延迟变量应该用!代替%,这个是赋值不是计算，不要/A
ECHO %result%
PAUSE