@ECHO OFF
setlocal enabledelayedexpansion ::�����ӳٱ���
SET max=3
SET tb=ABCDEFGHIJKLMNOPQRSTUVWXYZ 
SET result=!tb:~%max%,1!         ::�ӳٱ���Ӧ����!����%,����Ǹ�ֵ���Ǽ��㣬��Ҫ/A
ECHO %result%
PAUSE