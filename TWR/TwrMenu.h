/*
noMeiryoUI (C) 2005,2012-2016 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#ifndef TWRMENU_H
#define TWRMENU_H

#include <tchar.h>

class TwrMenu {
private:
	HMENU menuHandle;
public:
	TwrMenu(HWND wnd);
	TwrMenu(HMENU menu);

	void CheckMenuItem(int id,bool checked);
	bool isChecked(int item);
	void setEnabled(int id, bool enabled);
	void setText(int id, TCHAR *message, BOOL byPosition = TRUE);

	/**
	 * サブメニューを取得する。
	 *
	 * @param pos メニュー上の位置(0～)
	 * @return サブメニューのオブジェクト
	 */
	TwrMenu *getSubMenu(int pos) {
		HMENU hMenu = GetSubMenu(menuHandle, pos);
		return new TwrMenu(hMenu);
	}
};


#endif
