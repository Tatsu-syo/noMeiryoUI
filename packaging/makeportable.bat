REM noMeiryoUI (C) 2005,2012-2025 Tatsuhiko Shoji
REM The sources for noMeiryoUI are distributed under the MIT open source license
REM Usage:
REM Copy portable version of noMeiryoUI.exe into files directory.
REM run batchfile

nmake
cd portable
7z a -mx9 ..\noMeiryoUI.zip @..\filelist.txt
cd ..
