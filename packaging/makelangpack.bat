REM noMeiryoUI (C) 2005,2012-2026 Tatsuhiko Shoji
REM The sources for noMeiryoUI are distributed under the MIT open source license
REM Usage:
REM Copy installable version of noMeiryoUI.exe into files directory.
REM run batchfile

del /S /Q langpack
mkdir langpack
nmake all

cd files

7z a -mx9 ..\langpack\default.zip default.chm default.lng
7z a -mx9 ..\langpack\en.zip en.chm en.lng
7z a -mx9 ..\langpack\ja-JP.zip ja-JP.chm ja-JP.lng
7z a -mx9 ..\langpack\ko-KR.zip ko-KR.chm ko-KR.lng
7z a -mx9 ..\langpack\pt-BR.zip pt-BR.chm pt-BR.lng
7z a -mx9 ..\langpack\zh-CN.zip zh-CN.chm zh-CN.lng
7z a -mx9 ..\langpack\zh-TR.zip zh-TW.chm zh-TW.lng

cd ..
