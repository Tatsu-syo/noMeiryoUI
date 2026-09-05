REM noMeiryoUI (C) 2005,2012-2026 Tatsuhiko Shoji
REM The sources for noMeiryoUI are distributed under the MIT open source license
REM Usage:
REM Copy installable version of noMeiryoUI.exe into files directory.
REM run batchfile

del /S /Q installable
mkdir installable
del /Q .\Output\noMeiryoUIsetup.exe
nmake all
xcopy /Y /S /E files\* installable
REM Please choice virus checker safe execute file.
copy /Y ..\program\bin_2012\Release\noMeiryoUI.exe installable
