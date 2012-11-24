#include "stdafx.h"
#include <string.h>
#include "CommCtrl.h"

#include "AppliBase.h"
#include "TwrTab.h"

/**
 * コンストラクタ
 */
TwrTab::TwrTab() : TwrWnd()
{
}

/**
 * タブコントロールを作成します。
 *
 * @return 作成したタブコントロールのウインドウハンドル
 */
HWND TwrTab::create()
{
	hWnd = CreateWindowEx(
		WS_EX_LEFT,
		WC_TABCONTROL,
		title.c_str(),
		WS_CHILD|WS_VISIBLE,
		left,	// X
		top,	// Y
		width,	// Width
		height,	// Height
		GetDesktopWindow(),	// Parent
		(HMENU)IDC_STATIC,
		hInst,
		NULL);

	return hWnd;
}

/**
 * タブコントロールを作成します。
 *
 * @param parent 親ウインドウハンドル
 * @return 作成したタブコントロールのウインドウハンドル
 */
HWND TwrTab::create(HWND parent)
{
	hWnd = CreateWindowEx(
		WS_EX_LEFT,
		WC_TABCONTROL,
		title.c_str(),
		WS_CHILD|WS_VISIBLE,
		left,	// X
		top,	// Y
		width,	// Width
		height,	// Height
		parent,	// Parent
		(HMENU)IDC_STATIC,
		hInst,
		NULL);

	return hWnd;
}

/**
 * タブコントロールを追加します。
 *
 * @param title タブのタイトル
 */
void TwrTab::add(TCHAR *title)
{
	TCITEM item;

	memset(&item, 0, sizeof(TCITEM));
	item.mask = TCIF_TEXT;
	item.pszText = title;
	item.cchTextMax = (int)((_tcslen(title) + 1) * sizeof(TCHAR));

	INT_PTR count = SendMessage(hWnd, TCM_GETITEMCOUNT, 0 ,0);
	::SendMessage(hWnd, TCM_INSERTITEM, (WPARAM)count, (LPARAM)&item);
}

/**
 * 選択されたタブの位置を返します。
 *
 * @return > -1:選択されたタブの位置 -1:選択されていない
 */
int TwrTab::getSelectedIndex(void)
{
	int count = (int)SendMessage(hWnd, TCM_GETCURSEL, 0, 0);

	return count;
}

/**
 * タブを選択します。
 *
 * @param index 選択するタブの位置
 */
void TwrTab::select(int index)
{
	SendMessage(hWnd, TCM_SETCURSEL, (WPARAM)index, 0);

}

