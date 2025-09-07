REM Usage:
REM Copy portable version of noMeiryoUI.exe into files directory.
REM run batchfile

nmake
cd files
7z a ..\noMeiryoUI.zip @filelist.txt
cd ..
