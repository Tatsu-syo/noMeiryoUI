/*
noMeiryoUI (C) 2005,2012-2016 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#include <string.h>
#include <windows.h>

#include "TwrMenu.h"

/**
 * メニューを扱うクラス
 */

/**
 * コンストラクタ
 *
 * @param オブジェクトに結び付けるウインドウのハンドル
 */
TwrMenu::TwrMenu(HWND wnd)
{
	menuHandle = GetMenu(wnd);
}

/**
 * コンストラクタ
 *
 * @param オブジェクトに結び付けるウインドウのハンドル
 */
TwrMenu::TwrMenu(HMENU menu)
{
	menuHandle = menu;
}

/**
 * メニューのチェック状態を設定する。
 *
 * @param id メニューID
 * @param true:チェックする false:チェックしない
 */
void TwrMenu::CheckMenuItem(int id, bool checked)
{
	 MENUITEMINFO info;

	 memset(&info, 0, sizeof(MENUITEMINFO));
	 info.cbSize = sizeof(MENUITEMINFO);
	 info.fMask = MIIM_STATE;

	 GetMenuItemInfo(menuHandle, id, FALSE, &info);
	 if (checked) {
		 info.fState |= MFS_CHECKED;
	 } else {
		 if (info.fState & MFS_CHECKED) {
			info.fState ^= MFS_CHECKED;
		 }
	 }
	 SetMenuItemInfo(menuHandle, id, FALSE, &info);
}

/**
 * メニューのチェック状態を設定する。
 *
 * @param item メニューID
 * @return true:チェックする false:チェックしない
 */
bool TwrMenu::isChecked(int item)
{
	 MENUITEMINFO info;

	 memset(&info, 0, sizeof(MENUITEMINFO));
	 info.cbSize = sizeof(MENUITEMINFO);
	 info.fMask = MIIM_STATE;

	 GetMenuItemInfo(menuHandle, item, FALSE, &info);
	 if (info.fState & MFS_CHECKED) {
		 return true;
	 } else {
		 return false;
	 }
}

/**
 * メニューの有効状態を設定する。
 *
 * @param id メニューID
 * @param true:有効にする false:有効にしない
 */
void TwrMenu::setEnabled(int id, bool enabled)
{
	MENUITEMINFO info;

	memset(&info, 0, sizeof(MENUITEMINFO));
	info.cbSize = sizeof(MENUITEMINFO);
	info.fMask = MIIM_STATE;

	GetMenuItemInfo(menuHandle, id, FALSE, &info);
	if (enabled) {
		if (info.fState & MFS_ENABLED) {
			info.fState ^= MFS_ENABLED;
		}
	} else {
		info.fState |= MFS_DISABLED;
	}
	SetMenuItemInfo(menuHandle, id, FALSE, &info);
}

/**
 * メニューの文言を設定する。
 *
 * @param position メニューの位置/メニュー項目のID
 * @param message 設定するメッセージ
 * @param byPosition 指定方法(TRUE:メニューの位置 FALSE:メニュー項目のID)
 */
void TwrMenu::setText(int position, const TCHAR *message, BOOL byPosition)
{
	MENUITEMINFO info;

	info.cbSize = sizeof(MENUITEMINFO);
	info.fMask = MIIM_FTYPE | MIIM_STRING;
	info.fType = MFT_STRING;
	info.dwTypeData = (LPTSTR)message;
	SetMenuItemInfo(menuHandle, position, byPosition, &info);
}
