@ECHO OFF
for /l %%a in (1,1,6) do @<nul,set /p=%%a
for /l %%a in (1,1,6) do @<nul set /p=%%a
for /L %%a in (1 1 6) do @set /p=%%a 0<nul
for /L %%a in (1 1 6) do @set /p=%%a <NUL
ECHO.
ECHO.
::<NUL�Ļ�����ɾ�������ж��ұ�������
::set /p=%%a�Ǹ�������﷨��ֱ����ʵ������ֵ,�����ECHO XXX�ĺô���û�л��С�
pause
