#include "stdafx.h"

#include "commctrl.h"
#include "AppliBase.h"
#include "TwrListView.h"

/**
 * コンストラクタ
 */
TwrListView::TwrListView() : TwrWnd()
{
}

/**
 * コンストラクタ
 *
 * @param オブジェクトに結び付けるウインドウのハンドル
 */
TwrListView::TwrListView(HWND newHwnd) : TwrWnd()
{
	hWnd = newHwnd;
}


/**
 * リストビューを作成します。
 *
 * @return 作成したリストビューのウインドウハンドル
 */
HWND TwrListView::create(void)
{
	hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		_T("LISTVIEW"),
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
 * リストビューに要素を挿入します。
 *
 * @param index 挿入する位置
 * @param item 追加する文字列
 */
void TwrListView::insertItem(int index, TCHAR *item)
{
	LVITEM itemInfo;
	
	memset(&itemInfo, 0x00, sizeof(LVITEM));
	itemInfo.mask = LVIF_TEXT;
	itemInfo.iItem = index;
	itemInfo.pszText = item;
	itemInfo.cchTextMax = _tcslen(item) + 1;

	SendMessage(hWnd, LVM_INSERTITEM , (WPARAM)0, (LPARAM)&itemInfo);
}

/**
 * リストビューのスタイルを設定します。
 *
 * @param newStyle 新しいスタイル
 * @return 設定結果
 */
unsigned int TwrListView::SetExtendedStyle(unsigned int newStyle)
{
	return SendMessage(hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, (WPARAM)0, (LPARAM)newStyle);
}

/**
 * リストビューに列を挿入します。
 *
 * @param no 列番号
 * @param title 列のタイトル
 * @param nFormat フォーマット
 * @param nWidth 列幅
 * @param nSubItem 列のサブアイテムのインデックス
 * @return 挿入結果
 */
int TwrListView::insertColumn(
	int no,
	TCHAR *title, 
	int nFormat,
	int nWidth,
	int nSubItem 
)
{
	LVCOLUMN columnInfo;
	
	columnInfo.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	columnInfo.pszText = title;
	columnInfo.fmt = nFormat;
	columnInfo.cx = nWidth;
	if (nSubItem > -1) {
		columnInfo.iSubItem = nSubItem;
	} else {
		columnInfo.iSubItem = 0;
	}
	
	return SendMessage(hWnd, LVM_INSERTCOLUMN, (WPARAM)no, (LPARAM)(&columnInfo));
}

/**
 * リストビューのチェック状態を変更します。
 *
 * @param index チェック状態を変更する行
 * @param checked true:チェックされている false:チェックされていない
 */
void TwrListView::checkItem(int index, bool checked)
{
	ListView_SetCheckState(hWnd, index, (checked ? TRUE : FALSE));
}

/**
 * リストビューの行を選択します。
 *
 * @param index 選択する行
 */
void TwrListView::selectItem(int index)
{
	LVITEM itemInfo;
	
	itemInfo.mask = LVIF_STATE;
	itemInfo.iItem = index;
	itemInfo.state = LVIS_SELECTED ;
	itemInfo.stateMask = LVIS_SELECTED ;

	SendMessage(hWnd, LVM_SETITEM, (WPARAM)0, (LPARAM)(&itemInfo));
}

/**
 * リストビューの行のチェック状態を取得します。
 *
 * @param index チェック状態を取得する行
 * @return true:チェックされている false:チェックされていない
 */
bool TwrListView::isChecked(int index)
{
	BOOL state;
	
	state = ListView_GetCheckState(hWnd, index);

	return (state ? true : false);
}

