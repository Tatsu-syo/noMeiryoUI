/*
noMeiryoUI (C) 2005,2012,2013 Tatsuhiko Shoji
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