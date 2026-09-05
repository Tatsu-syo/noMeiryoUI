REM noMeiryoUI (C) 2005,2012-2026 Tatsuhiko Shoji
REM The sources for noMeiryoUI are distributed under the MIT open source license
REM Usage:
REM Copy portable version of noMeiryoUI.exe into files directory.
REM run batchfile

del /S /Q portable
mkdir portable
del /Q .\Output\noMeiryoUI.zip
nmake all
xcopy /Y /S /E files\* portable
REM Please choice virus checker safe execute file.
copy /y ..\program\bin_2012\Release_Portable\noMeiryoUI.exe portable
cd portable
7z a -mx9 ..\Output\noMeiryoUI.zip @filelist.txt
cd ..
