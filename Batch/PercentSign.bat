@echo off
if defined str goto next
::set str=
set /p str=����ļ����������ں�س���
call "%~0" %str%
pause
exit

:next
cls
echo ���������ļ�����·��Ϊ��"%~0"
echo �ϵ������ڵ��ļ�����·��Ϊ��"%~1"
goto :eof