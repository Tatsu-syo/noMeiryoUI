# No!! MeiryoUI (Japanese name Meiryo UI mo daikkirai!!)

For early adapters who runs Windows 11 22H2, check discussions.

## About No!! MeiryoUI

This program sets the system font setting on Windows 8.1/10/11.  
On Windows 8.1/10/11, Microsoft removes the way to change system font settings.  
Microsoft's system font setting choice is very ugly, especially in Japanese. This is why, I created this tool.  

## What No!! Meiryo UI can

This program sets the system font setting on Windows 8.1/10/11.  
In other words, this program can change fonts in Win32/WinForms program's title bar, menu, message box, icon name (also Explorer's file and folder name).  

## What No!! Meiryo UI can't

This program sets the system font settings.  
No!! Meiryo UI doesn't have effect on programs that ignore the system font settings.  

## System requirement

Windows 8/8.1/10/11  
To check result of this software, No!! Meiryo UI can also run on Windows Vista and 7.

## Download

[https://github.com/Tatsu-syo/noMeiryoUI/releases](https://github.com/Tatsu-syo/noMeiryoUI/releases)

## Wiki

Detailed information is witten in the wiki.

[https://github.com/Tatsu-syo/noMeiryoUI/wiki](https://github.com/Tatsu-syo/noMeiryoUI/wiki)

## License

Source files are distributed under the MIT open source license.  
[http://opensource.org/licenses/mit-license.php](http://opensource.org/licenses/mit-license.php)

## Recommended action before use this program

If you change system font by this software you should take backup current registry by this command line.

```
REG SAVE "HKCU\Control Panel" %LOCALAPPDATA%\noMeiryoUI_HKCU.reg /y
```

Regestry backup file is created on LOCALAPPDATA folder.

If some trouble occured after system font change, you restore registry by this command line. And sign off or reboot.

```
REG RESTORE "HKCU\Control Panel" %LOCALAPPDATA%\noMeiryoUI_HKCU.reg
```
