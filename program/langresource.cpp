/*
noMeiryoUI (C) 2005,2012-2023 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/

#include "util.h"
#include "langresource.h"

/**
 * リソースの読み込みを開始する。
 *
 * @param file リソースファイル名
 */
void readResourceFile(TCHAR *file)
{
	readResourceItem(file, _T("FONT_FACE"),
		_T("System")
	);
	readResourceItem(file, _T("TITLE"),
		_T("No!! Meiryo UI")
	);
	readResourceItem(file, _T("MENU_FILE"),
		_T("&File")
	);
	readResourceItem(file, _T("MENU_FILE_LOAD"),
		_T("L&oad font settings...")
	);
	readResourceItem(file, _T("MENU_FILE_SAVE"),
		_T("S&ave font settings...")
	);
	readResourceItem(file, _T("MENU_FILE_SET_QUIT"),
		_T("&Set system fonts and quit")
	);
	readResourceItem(file, _T("MENU_FILE_QUIT"),
		_T("E&xit")
	);
	readResourceItem(file, _T("MENU_PRESET"),
		_T("&Preset")
	);
	readResourceItem(file, _T("MENU_PRESET_8"),
		_T("Windows 8/8.1")
	);
	readResourceItem(file, _T("MENU_PRESET_10"),
		_T("Windows 10")
	);
	readResourceItem(file, _T("MENU_TOOLS"),
		_T("&Tools")
	);
	readResourceItem(file, _T("MENU_TOOLS_THREAD"),
		_T("Set system fonts in separated thread")
	);
	readResourceItem(file, _T("MENU_TOOLS_SEVEN"),
		_T("Calculate font size same as Windows 7")
	);
	readResourceItem(file, _T("MENU_HELP"),
		_T("&Help")
	);
	readResourceItem(file, _T("MENU_HELP_HELP"),
		_T("&Help")
	);
	readResourceItem(file, _T("MENU_HELP_ABOUT"),
		_T("&About No!! Meiryo UI")
	);
	readResourceItem(file, _T("DLG_ALL_FONT"),
		_T("All fonts")
	);
	readResourceItem(file, _T("DLG_TITLE_BAR"),
		_T("Title bar")
	);
	readResourceItem(file, _T("DLG_ICON"),
		_T("Icon")
	);
	readResourceItem(file, _T("DLG_PALETTE"),
		_T("Palette title")
	);
	readResourceItem(file, _T("DLG_HINT"),
		_T("Hint")
	);
	readResourceItem(file, _T("DLG_MESSAGE"),
		_T("Message box")
	);
	readResourceItem(file, _T("DLG_MENU"),
		_T("Menu")
	);
	readResourceItem(file, _T("DLG_SELECT"),
		_T("Select...")
	);
	readResourceItem(file, _T("DLG_SET_ALL"),
		_T("Set all")
	);
	readResourceItem(file, _T("DLG_SET"),
		_T("Set")
	);
	readResourceItem(file, _T("DLG_EXIT"),
		_T("Exit")
	);
	readResourceItem(file, _T("DLG_FONT_SEL"),
		_T("Font Selection")
	);
	readResourceItem(file, _T("DLG_FONT_NAME"),
		_T("&Font name")
	);
	readResourceItem(file, _T("DLG_STYLE"),
		_T("St&yle")
	);
	readResourceItem(file, _T("DLG_SIZE"),
		_T("&Size")
	);
	readResourceItem(file, _T("DLG_UNDERLINE"),
		_T("&Under line")
	);
	readResourceItem(file, _T("DLG_STRIKE"),
		_T("S&trikethrough")
	);
	readResourceItem(file, _T("DLG_CHARSET"),
		_T("&Character set")
	);
	readResourceItem(file, _T("DLG_OK"),
		_T("OK")
	);
	readResourceItem(file, _T("DLG_CANCEL"),
		_T("Cancel")
	);

	readResourceItem(file, _T("DLG_CHARSET_ANSI"),
		_T("Ansi")
	);
	readResourceItem(file, _T("DLG_CHARSET_BALTIC"),
		_T("Baltic")
	);
	readResourceItem(file, _T("DLG_CHARSET_BIG5"),
		_T("Traditional Chinese")
	);
	readResourceItem(file, _T("DLG_CHARSET_DEFAULT"),
		_T("Default")
	);
	readResourceItem(file, _T("DLG_CHARSET_EASTEUROPE"),
		_T("East Europe")
	);
	readResourceItem(file, _T("DLG_CHARSET_GB2312"),
		_T("Simplified Chinese")
	);
	readResourceItem(file, _T("DLG_CHARSET_GREEK"),
		_T("Greek")
	);
	readResourceItem(file, _T("DLG_CHARSET_HANGUL"),
		_T("Hangul")
	);
	readResourceItem(file, _T("DLG_CHARSET_MAC"),
		_T("Mac")
	);
	readResourceItem(file, _T("DLG_CHARSET_OEM"),
		_T("OEM")
	);
	readResourceItem(file, _T("DLG_CHARSET_RUSSIAN"),
		_T("Russian")
	);
	readResourceItem(file, _T("DLG_CHARSET_SHIFTJIS"),
		_T("Japanese")
	);
	readResourceItem(file, _T("DLG_CHARSET_SYMBOL"),
		_T("Symbol")
	);
	readResourceItem(file, _T("DLG_CHARSET_TURKISH"),
		_T("Turkish")
	);
	readResourceItem(file, _T("DLG_CHARSET_VIETNAMESE"),
		_T("Vietnamese")
	);
	readResourceItem(file, _T("DLG_CHARSET_JOHAB"),
		_T("Johab Korean")
	);
	readResourceItem(file, _T("DLG_CHARSET_ARABIC"),
		_T("Arabic")
	);
	readResourceItem(file, _T("DLG_CHARSET_HEBREW"),
		_T("Hebrew")
	);
	readResourceItem(file, _T("DLG_CHARSET_THAI"),
		_T("Thai")
	);

	readResourceItem(file, _T("DLG_STYLE_NORMAL"),
		_T("Regular")
	);
	readResourceItem(file, _T("DLG_STYLE_ITALIC"),
		_T("Italic")
	);
	readResourceItem(file, _T("DLG_STYLE_BOLD"),
		_T("Bold")
	);
	readResourceItem(file, _T("DLG_STYLE_BOLD_ITALIC"),
		_T("Bold Italic")
	);

	readResourceItem(file, _T("MSG_SETTING_FILE"),
		_T("Font settings file(*.ini)")
	);
	readResourceItem(file, _T("MSG_ALL_FILE"),
		_T("All file(*.*)")
	);
	readResourceItem(file, _T("MSG_LOAD_FAIL"),
		_T("Font settings file load failed.")
	);
	readResourceItem(file, _T("MSG_SAVE_FAIL"),
		_T("Font settings file save failed.")
	);
	readResourceItem(file, _T("MSG_ERROR"),
		_T("Error")
	);
	readResourceItem(file, _T("MSG_ABOUT"),
		_T("About No!! Meiryo UI")
	);
	readResourceItem(file, _T("MSG_NO_FONT"),
		_T("Please select font.")
	);
	readResourceItem(file, _T("MSG_NO_STYLE"),
		_T("Please select style.")
	);
	readResourceItem(file, _T("MSG_NO_SIZE"),
		_T("Please select size.")
	);
	readResourceItem(file, _T("MSG_NO_CHARSET"),
		_T("Please select charset.")
	);
	readResourceItem(file, _T("MSG_TRANSLATE"),
		_T("Tatsuhiko Shoji(Tatsu)")
	);
	readResourceItem(file, _T("FONT_CHARSET"),
		_T("1")
	);

	// 71
	readResourceItem(file, _T("DLG_APPLY"),
		_T("Apply")
	);
	readResourceItem(file, _T("DLG_ALL_GROUP"),
		_T("All font setting")
	);
	readResourceItem(file, _T("DLG_INDIVIDUAL_GROUP"),
		_T("Individual font setting")
	);

	readResourceItem(file, _T("DLG_STYLE_MEDIUM"),
		_T("Medium")
	);
	readResourceItem(file, _T("DLG_STYLE_SEMI_BOLD"),
		_T("Semibold")
	);
	readResourceItem(file, _T("DLG_STYLE_BOOK"),
		_T("Book")
	);
	readResourceItem(file, _T("DLG_STYLE_OBLIQUE"),
		_T("Oblique")
	);
	readResourceItem(file, _T("DLG_STYLE_LIGHT"),
		_T("Light")
	);
	readResourceItem(file, _T("DLG_STYLE_EXTRA_LIGHT"),
		_T("ExtraLight")
	);
	readResourceItem(file, _T("DLG_STYLE_SEMI_LIGHT"),
		_T("SemiLight")
	);
	readResourceItem(file, _T("DLG_STYLE_SLANTED"),
		_T("Slanted")
	);
	readResourceItem(file, _T("DLG_CHECK_BOLD"),
		_T("&Bold")
	);
	readResourceItem(file, _T("DLG_CHECK_ITALIC"),
		_T("&Italic")
	);
	readResourceItem(file, _T("MSG_WARNING"),
		_T("Warning")
	);
	readResourceItem(file, _T("MSG_WIN11_22H2RESTRICTION"),
		_T("On Windows 11 2022 Update(22H2) due to Windows reason title bar font can't change.")
	);
}

