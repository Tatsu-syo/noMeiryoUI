/*
noMeiryoUI (C) 2005,2012,2013 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#include "stdafx.h"
#include "AppliBase.h"
#include "TwrCombobox.h"

/**
 * コンストラクタ
 */
TwrCombobox::TwrCombobox() : TwrWnd()
{
	style = CBS_DROPDOWNLIST;
}

/**
 * コンストラクタ
 *
 * @param オブジェクトに結び付けるウインドウのハンドル
 */
TwrCombobox::TwrCombobox(HWND newHwnd) : TwrWnd()
{
	hWnd = newHwnd;
}

/**
 * コンボボックスを作成します。
 */
HWND TwrCombobox::create(void)
{
	hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		_T("COMBOBOX"),
		NULL,
		WS_CHILD|WS_VISIBLE|WS_TABSTOP|style,
		left,	// X
		top,	// Y
		width,	// Width
		height,	// Height
		GetDesktopWindow(),	// Parent
		NULL,
		hInst,
		NULL);

	return hWnd;
}

/**
 * コンボボックスを作成します。
 *
 * @param parent 親ウインドウハンドル
 */
HWND TwrCombobox::create(HWND parent)
{
	hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		_T("COMBOBOX"),
		NULL,
		WS_CHILD|WS_VISIBLE|WS_TABSTOP|style,
		left,	// X
		top,	// Y
		width,	// Width
		height,	// Height
		parent,	// Parent
		NULL,
		hInst,
		NULL);

	return hWnd;
}

/**
 * コンボボックスに要素を追加します。
 *
 * @param item 追加する文字列
 */
void TwrCombobox::addItem(TCHAR *item)
{
	SendMessage(hWnd, CB_ADDSTRING, 0, (LPARAM)item);
}

/**
 * 選択された要素の位置を取得します。
 *
 * @return -1:選択されていない -1以外:選択された場所
 */
int TwrCombobox::getSelectedIndex(void)
{
	LRESULT result = SendMessage(hWnd, CB_GETCURSEL, 0, 0);
	if (result == CB_ERR) {
		return -1;
	} else {
		return (int)result;
	}
}

