/*
noMeiryoUI (C) 2005,2012,2013 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#include "stdafx.h"
#include "AppliBase.h"
#include "TwrCheckbox.h"

/**
 * コンストラクタ
 */
TwrCheckbox::TwrCheckbox() : TwrWnd()
{
}

/**
 * コンストラクタ
 *
 * @param オブジェクトに結び付けるウインドウのハンドル
 */
TwrCheckbox::TwrCheckbox(HWND newHwnd) : TwrWnd()
{
	hWnd = newHwnd;
}

/**
 * チェックボックスを作成します。
 */
HWND TwrCheckbox::create(void)
{
	hWnd = CreateWindow(
		_T("BUTTON"),
		title.c_str(),
		WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|BS_LEFT|WS_TABSTOP,
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
 * チェックボックスを作成します。
 *
 * @param parent 親ウインドウハンドル
 */
HWND TwrCheckbox::create(HWND parent)
{
	hWnd = CreateWindow(
		_T("BUTTON"),
		title.c_str(),
		WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|BS_LEFT|WS_TABSTOP,
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
 * チェック状態を返します。
 *
 * @return true:チェックされた false:チェックされていない
 */
bool TwrCheckbox::isChecked(void)
{
	LRESULT result = SendMessage(hWnd, BM_GETCHECK, 0, 0);
	if (result == BST_CHECKED) {
		return true;
	} else {
		return false;
	}
}

/**
 * チェック状態を設定します。
 *
 * @param checked true:チェックされた false:チェックされていない
 */
void TwrCheckbox::setChecked(bool checked)
{
	if (checked) {
		SendMessage(hWnd, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
	} else {
		SendMessage(hWnd, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
	}
}
