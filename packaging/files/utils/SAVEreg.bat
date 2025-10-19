@echo off

echo Are you running this batch file having Administrator privilege?
echo If you don't have Administrator privilege, press CTRL+C to quit.
pause

echo.
echo Your local application data folder: %LOCALAPPDATA%
echo Save registry related to No!! MeiryoUI on your home folder OK?
pause

REG SAVE HKLM\SOFTWARE %LOCALAPPDATA%\noMeiryoUI_HKLM.reg /y 
REG SAVE "HKCU\Control Panel" %LOCALAPPDATA%\noMeiryoUI_HKCU.reg /y

echo.
echo backup file1: %LOCALAPPDATA%\noMeiryoUI_HKLM.reg
echo backup file2: %LOCALAPPDATA%\noMeiryoUI_HKCU.reg

echo.
echo Save registry related to No!! MeiryoUI complete.
