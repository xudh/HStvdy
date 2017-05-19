@echo off
if defined str goto next
::set str=
set /p str=请把文件拉到本窗口后回车：
call "%~0" %str%
pause
exit

:next
cls
echo 本批处理文件完整路径为："%~0"
echo 拖到本窗口的文件完整路径为："%~1"
goto :eof