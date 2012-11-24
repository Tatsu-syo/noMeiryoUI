#include "stdafx.h"
#include "AppliBase.h"
#include "TwrButton.h"

/**
 * コンストラクタ
 */
TwrButton::TwrButton() : TwrWnd()
{
}

/**
 * ボタンを作成します。
 *
 * @param id メニューID
 */
HWND TwrButton::create(HMENU id)
{
	hWnd = CreateWindow(
		_T("BUTTON"),
		title.c_str(),
		WS_CHILD|WS_VISIBLE|BS_MULTILINE|BS_CENTER|WS_TABSTOP,
		left,	// X
		top,	// Y
		width,	// Width
		height,	// Height
		GetDesktopWindow(),	// Parent
		id,
		hInst,
		NULL);

	return hWnd;
}

/**
 * ボタンを作成します。
 *
 * @param parent 親ウインドウハンドル
 * @param id メニューID
 */
HWND TwrButton::create(HWND parent, HMENU id)
{
	hWnd = CreateWindow(
		_T("BUTTON"),
		title.c_str(),
		WS_CHILD|WS_VISIBLE|BS_MULTILINE|BS_CENTER|WS_TABSTOP,
		left,	// X
		top,	// Y
		width,	// Width
		height,	// Height
		parent,	// Parent
		id,
		hInst,
		NULL);

	return hWnd;
}

