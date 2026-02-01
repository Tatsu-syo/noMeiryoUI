REM noMeiryoUI (C) 2005,2012-2026 Tatsuhiko Shoji
REM The sources for noMeiryoUI are distributed under the MIT open source license
REM Usage:
REM Copy portable version of noMeiryoUI.exe into files directory.
REM run batchfile

nmake clean
del /Q ..\Output\noMeiryoUI.zip
copy /y ..\program\Release_2012\noMeiryoUI.exe files
nmake all
cd files
7z a -mx9 ..\..\Output\noMeiryoUI.zip @filelist.txt
cd ..
