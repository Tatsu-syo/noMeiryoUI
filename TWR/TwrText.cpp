#include "stdafx.h"
#include "AppliBase.h"
#include "TwrText.h"

/**
 * コンストラクタ
 */
TwrText::TwrText() : TwrWnd()
{
}

/**
 * スタティックテキストを作成します。
 *
 * @return 作成したスタティックテキストのウインドウハンドル
 */
HWND TwrText::create()
{
	hWnd = CreateWindow(
		_T("STATIC"),
		title.c_str(),
		WS_CHILD | WS_VISIBLE,
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
 * スタティックテキストを作成します。
 *
 * @param parent 親ウインドウのハンドル
 * @return 作成したスタティックテキストのウインドウハンドル
 */
HWND TwrText::create(HWND parent)
{
	hWnd = CreateWindow(
		_T("STATIC"),
		title.c_str(),
		WS_CHILD | WS_VISIBLE,
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
