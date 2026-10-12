# How to contribute

## Release process

To avoid contamination and gather translations 

1. I or people who want to contribute open issue about new version.
2. On any trigger for release such as new translations, new code release process starts.
3. Review process write in below starts.
4. On all review process ends and software runs well I release it.
5. If no translation come, I release software without translation.

## How to make translations

Language files and help files are named after the Windows UI language.
For example, a Traditional Chinese (Taiwan) environment loads `zh-TW.lng` and `zh-TW.chm` placed next to `noMeiryoUI.exe`.
If there is no file for the language and region, the file for the language only (e.g. `en.lng`) is used,
and if there is none either, `default.lng` and `default.chm` are used.
To see the locale name of the current Windows, run `util\localetest.exe` from a command prompt.

### Language file

1. Copy a file in the [lang](lang) folder (e.g. `en.lng`) and rename it to the locale name, such as `fr-FR.lng`.
2. In the `[RESOURCE]` section, translate the value of each `KEY=value` line.
   - `FONT_FACE` is the font name used to display the dialogs.
   - `FONT_CHARSET` is the character set number of that font.

     | Character set | Number |
     | --- | --- |
     | ANSI (usually this) | 0 |
     | Default | 1 |
     | Symbol | 2 |
     | Shift-JIS | 128 |
     | Hangul | 129 |
     | Johab | 130 |
     | GB2312 (Simplified Chinese) | 134 |
     | Big5 (Traditional Chinese) | 136 |
     | Greek | 161 |
     | Turkish | 162 |
     | Vietnamese | 163 |
     | Hebrew | 177 |
     | Arabic | 178 |
     | Baltic | 186 |
     | Russian | 204 |
     | Thai | 222 |
     | East Europe | 238 |
     | OEM | 255 |

3. In the `[PRESET]` section, write the default font face, size and charset of each Windows version
   for the preset menu. Keys end with `_8` (Windows 8/8.1), `_10` (Windows 10) or `_11` (Windows 11), and the prefix means:

   | Prefix | Element |
   | --- | --- |
   | `CAPTION` | Title bar |
   | `ICON` | Icon |
   | `SMALLCAPTION` | Palette title |
   | `STATUS` | Status bar / hint |
   | `MESSAGE` | Message box |
   | `MENU` | Menu |

   `FACE` is the font name, `SIZE` is the font size and `CHARSET` is the character set.
   Example: title bar of Windows 8.1 is Segoe UI, 11 point, default charset.

   ```
   CAPTION_FACE_8=Segoe UI
   CAPTION_SIZE_8=11
   CAPTION_CHARSET_8=1
   ```

4. Put the file next to `noMeiryoUI.exe` on a Windows with that UI language and check the dialogs.

### Help file

1. Copy a folder in the [help](help) folder (e.g. `en-us`) and translate the HTML files in its `contents` folder.
2. Open the `.hhp` file with HTML Help Workshop (`hhc`) and compile it to a `.chm` file.
3. Rename the `.chm` file to the same name as the language file (e.g. `fr-FR.chm`).

## How to include translations

If you modify translations, you must follow this process.

### By GitHub

1. Create pull request.
2. Write message about pull request on issue about next version.
3. Review on pull request.
4. When translations come to arrive an agreement I merge in master.

### By e-mail

1. Send e-mail for me.
2. I exchange GitHub user and author.
3. Process goes on GitHub.

## How to merge code

If you modify program, you must follow this process.

1. Create pull request.
2. Write message about pull request on issue about next version.
3. Review on pull request.
4. When code come to  to arrive an agreement I merge in master.
