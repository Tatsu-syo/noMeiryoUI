#include "stdafx.h"
#include "TwrWnd.h"

/**
 * コンストラクタ
 */
TwrWnd::TwrWnd()
{
	hWnd = NULL;
	title = _T("");
	left = 0;
	top = 0;
	width = 0;
	height = 0;
}

/**
 * コンストラクタ
 *
 * @param newHwnd オブジェクトに結び付けるウインドウハンドル
 */
TwrWnd::TwrWnd(HWND newWnd)
{
	hWnd = newWnd;
	title = _T("");
	left = 0;
	top = 0;
	width = 0;
	height = 0;
}

/**
 * デストラクタ<br>
 * <br>
 * Windowsのウインドウも破棄します。
 */
TwrWnd::~TwrWnd()
{
	if (hWnd != NULL) {
		::DestroyWindow(hWnd);
	}
}

/**
 * ウインドウを更新します。
 *
 * @return 更新結果
 */
BOOL TwrWnd::UpdateWindow()
{
	return ::UpdateWindow(hWnd);
}

/**
 * ウインドウの表示状態を変更します。
 *
 * @return 変更結果
 */
BOOL TwrWnd::ShowWindow(int nCmdShow)
{
	return ::ShowWindow(hWnd, nCmdShow);
}

/**
 * ウインドウの利用可能状態を変更します。
 *
 * @param enabled true:使用可能 false:使用不可能
 */
void TwrWnd::EnableWindow(BOOL enabled)
{
	::EnableWindow(hWnd, enabled);
}

/**
 * ウインドウのタイトル文字列を変更します。
 *
 * @param text 設定する文字列
 */
void TwrWnd::setText(const TCHAR *text)
{
	title = text;
	if (hWnd != NULL) {
		SetWindowText(hWnd, text);
	}
}

/**
 * ウインドウのタイトル文字列を変更します。
 *
 * @param x x座標
 * @param y y座標
 */
void TwrWnd::setPoint(int x, int y)
{
	left = x;
	top = y;

	if (hWnd != NULL) {
		MoveWindow(hWnd,left,top,width,height,true);
	}
}

/**
 * ウインドウのサイズを変更します。
 *
 * @param w 幅
 * @param h 高さ
 */
void TwrWnd::setSize(int w, int h)
{
	width = w;
	height = h;

	if (hWnd != NULL) {
		MoveWindow(hWnd,left,top,width,height,true);
	}
}

/**
 * ウインドウのフォントを変更します。
 *
 * @param font フォントのハンドル
 */
void TwrWnd::setFont(HFONT font)
{
	SendMessage(hWnd, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
}