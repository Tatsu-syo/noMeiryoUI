English | [简体中文](README_CN.md) | [繁體中文](README_TW.md) | [日本語](README_JA.md)

---
# No!! MeiryoUI

No!! MeiryoUI ("Meiryo UI mo daikkirai!!" in Japanese) brings back the capability to customize the user interface font on Windows, a feature that was removed since Windows 8.1.

No!! MeiryoUI has some limitations on Windows 11 22H2 and later.

If you have some ideas check [discussions](https://github.com/Tatsu-syo/noMeiryoUI/discussions) for support.

## Requirements

- Windows 8 or newer.
- While No!! MeiryoUI should work on Windows Vista, it is not actively tested for compatibility.

## Installation

> [!IMPORTANT]
> No!! MeiryoUI makes changes to registry. It is recommended to backup your registry before applying any changes.
>
> ```
> reg save "HKCU\Control Panel" .\reset_font.reg /y
> ```
>
> To restore it later, run the following command with administrator privilege.
>
> ```
> reg restore "HKCU\Control Panel" .\reset_font.reg
> ```
>
> The archive also contains `utils\SAVEreg.bat` and `utils\RESTOREreg.bat` which do the same.

- Download zip archive from [GitHub Releases](https://github.com/Tatsu-syo/noMeiryoUI/releases/latest).
- Extract the archive and run "noMeiryoUI.exe".

## Features

- Change the font of each user interface element separately, or all at once:
  title bar, icon, palette title, hint (status bar), message box and menu.
- Preset menu to reset fonts to the defaults of Windows 8/8.1, 10 and 11.
- Save / load font settings to / from a file.
- Interface and help are available in English, Japanese, Korean, Brazilian Portuguese, Simplified Chinese and Traditional Chinese.

## Known Limitations

- No!! MeiryoUI does not work with:
  - Windows taskbar.
  - Preinstalled UWP Apps (e.g. Settings, Paint, etc.).
  - Applications that ignore the system font settings.

- No!! MeiryoUI cannot modify the titlebar font on Windows 11.
- No!! MeiryoUI might be detected as Trojan/virus software by some antivirus software because No!! MeiryoUI changes system font settings.

## Documentations

You can find detailed documentation on the [wiki](https://github.com/Tatsu-syo/noMeiryoUI/wiki).
The help file (`.chm`) is also included in the release archive; its sources are in the [help](help) folder.

## Building

The source code is in the [program](program) folder.

1. Install Visual Studio with the C++ desktop development workload.
2. Open the solution file for your Visual Studio version.

   | Solution | Visual Studio |
   | --- | --- |
   | `program\noMeiryoUI.sln` | 2022 or later (v143) |
   | `program\noMeiryoUI_2012.sln` | 2012 (v110) |
   | `program\noMeiryoUI_2008.sln` | 2008 |

3. Build the Release configuration. The executable is generated in `program\bin\Release` (2022 or later) or `program\bin_2012\Release` (2012).

Release packages (portable zip, installer and language packs) are made with the scripts in the [packaging](packaging/readme.md) folder.
The portable and installer versions use the Visual Studio 2012 build, because Microsoft Defender treats executables built by later versions as a virus.

## Translations

Translations of the interface (`lang`) and the help (`help`) are welcome.
See [CONTRIBUTING.md](CONTRIBUTING.md) for how to make and submit them.

## License

Licensed under [the MIT License](https://github.com/Tatsu-syo/noMeiryoUI/blob/master/LICENSE).
