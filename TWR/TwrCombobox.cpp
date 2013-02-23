/*
This file is part of noMeiryoUI
Copyright (C) 2005,2012,2013 Tatsuhiko Shoji

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
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
void TwrCombobox::addItem(const TCHAR *item)
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

/**
 * コンボボックスをクリアします。
 */
void TwrCombobox::clear(void)
{
	SendMessage(hWnd, CB_RESETCONTENT, 0, 0);
}

/**
 * 指定した位置の要素を選択します。
 *
 * @param index 選択する位置
 */
void TwrCombobox::setSelectedIndex(int index)
{
	SendMessage(hWnd, CB_SETCURSEL, index, 0);
}

/**
 * 選択した位置の文字列を返します。
 *
 * @return 選択した位置の文字列(選択されてない場合は空)
 */
tstring TwrCombobox::getSelectedText(void)
{
	TCHAR *buf;
	tstring ret = _T("");
	LRESULT len;

	int item = getSelectedIndex();
	if (item < 0) {
		return ret;
	}

	len = SendMessage(hWnd, CB_GETLBTEXTLEN, item, 0);
	try {
		buf = new TCHAR[len + 1];
		SendMessage(hWnd, CB_GETLBTEXT, item, (LPARAM)buf);

		ret = buf;
		delete []buf;
	} catch (...) {
	}

	return ret;	
}

