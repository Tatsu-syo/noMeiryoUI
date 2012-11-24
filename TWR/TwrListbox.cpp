#include "stdafx.h"
#include "AppliBase.h"
#include "TwrListbox.h"

/**
 * コンストラクタ
 */
TwrListbox::TwrListbox() : TwrWnd()
{
}

/**
 * コンストラクタ
 *
 * @param オブジェクトに結び付けるウインドウのハンドル
 */
TwrListbox::TwrListbox(HWND newHwnd) : TwrWnd()
{
	hWnd = newHwnd;
}


/**
 * リストボックスを作成します。
 */
HWND TwrListbox::create(void)
{
	hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		_T("LISTBOX"),
		NULL,
		WS_CHILD|WS_VISIBLE|LBS_NOINTEGRALHEIGHT|WS_TABSTOP,
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
 * リストボックスに要素を追加します。
 *
 * @param item 追加する文字列
 */
void TwrListbox::addItem(TCHAR *item)
{
	SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)item);
}

/**
 * 選択された要素の位置を取得します。
 *
 * @return -1:選択されていない -1以外:選択された場所
 */
int TwrListbox::getSelectedIndex(void)
{
	LRESULT result = SendMessage(hWnd, LB_GETCURSEL, 0, 0);
	if (result == LB_ERR) {
		return -1;
	} else {
		return (int)result;
	}
}

/**
 * リストボックスに要素を挿入します。
 *
 * @param index 挿入する位置
 * @param item 追加する文字列
 */
void TwrListbox::insertItem(int index, TCHAR *item)
{
	SendMessage(hWnd, LB_INSERTSTRING , (WPARAM)index, (LPARAM)item);
}

/**
 * リストボックスの要素を削除します。
 *
 * @param index 削除する要素の位置
 */
void TwrListbox::removeItem(int index)
{
	SendMessage(hWnd, LB_DELETESTRING , (WPARAM)index, (LPARAM)0);
}

/**
 * リストボックスの要素を選択します。
 *
 * @param index 選択する要素の位置
 */
void TwrListbox::selectItem(int index)
{
	SendMessage(hWnd, LB_SETCURSEL , (WPARAM)index, (LPARAM)0);
}

