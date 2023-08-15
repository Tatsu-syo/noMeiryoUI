# No!! MeiryoUI

No!! MeiryoUI ("Meiryo UI mo daikkirai!!" in Japanese) brings back the capability to customize the user interface font on Windows, a feature that was removed since Windows 8.1.

No!! MeiryoUI is currently in alpha stage on Windows 11 22H2, check [discussions](https://github.com/Tatsu-syo/noMeiryoUI/discussions) for support.

## Requirements

- Windows 8 or newer.
- While No!! MeiryoUI might work on Windows Vista, it is not actively tested for compatibility.

## Installation

> [!IMPORTANT]  
> No!! MeiryoUI makes changes to registry. It is recommanded to backup your registry before applying any changes.
> 
> ```
> reg save "HKCU\Control Panel" .\reset_font.reg /y
> ```

- Download zip archive from [GitHub Releases](https://github.com/Tatsu-syo/noMeiryoUI/releases/latest).
- Unzip the archive and run "noMeiryoUI.exe".

## Known Limitations

- No!! MeiryoUI doesn't work on:
  - Windows taskbar.
  - Preinstalled UWP Apps (e.g. Settings, Paint, etc.).
  - Applications that ignore the system font settings.

- No!! MeiryoUI cannot modify the titlebar font on Windows 11.

## Documentations

You can find the documentation on the [wiki](https://github.com/Tatsu-syo/noMeiryoUI/wiki).

## License

Licensed under [the MIT License](https://github.com/Tatsu-syo/noMeiryoUI/blob/master/LICENSE).
