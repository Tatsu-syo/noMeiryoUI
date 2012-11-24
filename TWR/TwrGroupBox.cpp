#include "stdafx.h"
#include "AppliBase.h"
#include "TwrGroupBox.h"

/**
 * コンストラクタ
 */
TwrGroupBox::TwrGroupBox() : TwrWnd()
{
}

/**
 * グループボックスを作成します。
 */
HWND TwrGroupBox::create()
{
	hWnd = CreateWindow(
		_T("BUTTON"),
		title.c_str(),
		WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
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
 * グループボックスを作成します。
 *
 * @param parent 親ウインドウハンドル
 */
HWND TwrGroupBox::create(HWND parent)
{
	hWnd = CreateWindow(
		_T("BUTTON"),
		title.c_str(),
		WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
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
