@echo off

echo Are you running this batch file having Administrator privilege?
echo If you don't have Administrator privilege, press CTRL+C to quit.
pause

echo. 
echo Your local application data folder: %LOCALAPPDATA%
echo Restore registry related to No!! MeiryoUI from your home folder backup file and reboot OK?
pause

REM REG RESTORE HKLM\SOFTWARE %LOCALAPPDATA%\noMeiryoUI_HKLM.reg
if ERRORLEVEL 1 exit
REG RESTORE "HKCU\Control Panel" %LOCALAPPDATA%\noMeiryoUI_HKCU.reg
if ERRORLEVEL 1 exit

shutdown /r /t 0 /c "Restore registry." /d 4:1
