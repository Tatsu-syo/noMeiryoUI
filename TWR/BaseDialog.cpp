#include "stdafx.h"

#include "TwrCore.h"
#include "BaseDialog.h"
#include "ModelessDialog.h"

BaseDialog *BaseDialog::modalCallback;
static BaseDialog *modelessCallback[10];
static int modelessCallbacks = 0;

/**
 * モードレスダイアログのコールバックを初期化する。
 *
 */
void initModelessCallback(void)
{
	for (int i = 0; i < 10; i++) {
		modelessCallback[i] = NULL;
	}
}

/**
 * コンストラクタ
 */
BaseDialog::BaseDialog() : TwrWnd()
{
	parentWindow = NULL;
	// dialogResource = resource;
	isModeless = false;
}


/**
 * コンストラクタ
 *
 * @param parent 親ウインドウハンドル
 * @param resource ダイアログリソース番号
 */
BaseDialog::BaseDialog(HWND parent, int resource) : TwrWnd()
{
	parentWindow = parent;
	dialogResource = resource;
	isModeless = false;
}

/**
 * デストラクタ
 */
BaseDialog::~BaseDialog()
{
	if (isModeless) {
		delModelessDialog(hWnd);

		for (int i = 0; i < 10; i++) {
			if (modelessCallback[i] == this) {
				modelessCallback[i] = NULL;
				modelessCallbacks--;
				break;
			}
		}
		if (hWnd != NULL) {
			if (IsWindow(hWnd)) {
				DestroyWindow(hWnd);
			}
		}
	}
}

/**
 * モーダルダイアログを表示する。
 *
 * @return ダイアログの戻り値
 */
INT_PTR BaseDialog::showModal()
{
	modalCallback = this;
	return ::DialogBox(
		hInst,
		MAKEINTRESOURCE(dialogResource),
		parentWindow,
		&BaseDialog::modalDialogProc);
}

/**
 * モードレスダイアログを表示する。
 *
 * @return ダイアログの戻り値
 */
HWND BaseDialog::showModeless()
{
	isModeless = true;

	hWnd = CreateDialog(
		hInst,
		MAKEINTRESOURCE(dialogResource),
		parentWindow,
		&BaseDialog::modelessDialogProc);

	if (hWnd != NULL) {
		bool found = false;
		int empty = -1;
		for (int i = 0; i < 10; i++) {
			if (modelessCallback[i] == this) {
				found = true;
				break;
			}
			if ((modelessCallback[i] == NULL) && (empty == -1)){
				empty = i;
			}
		}
		if (!found) {
			if (empty > -1) {
				modelessCallback[empty] = this;
				modelessCallbacks++;
			}
		}

		addModelessDialog(hWnd);
	}
	this->OnInitDialog();

	return hWnd;
}

/**
 * モーダルダイアログのダイアログプローシジャ
 *
 * @param hDlg ダイアログのウインドウハンドル
 * @param message メッセージ
 * @param wParam WPARAM
 * @param lParam LPARAM
 * @return 処理結果
 */
INT_PTR CALLBACK BaseDialog::modalDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	return modalCallback->dialogProc(hDlg, message, wParam, lParam);
}

/**
 * モードレスダイアログのダイアログプローシジャ
 *
 * @param hDlg ダイアログのウインドウハンドル
 * @param message メッセージ
 * @param wParam WPARAM
 * @param lParam LPARAM
 * @return 処理結果
 */
INT_PTR CALLBACK BaseDialog::modelessDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	for (int i = 0;i < 10; i++) {
		if (modelessCallback[i] == NULL) {
			continue;
		}
		if (hDlg == modelessCallback[i]->getHwnd()) {
			return modelessCallback[i]->dialogProc(hDlg, message, wParam, lParam);
		}
	}
	return 0;
}

/**
 * ダイアログのダイアログプローシジャ
 *
 * @param hDlg ダイアログのウインドウハンドル
 * @param message メッセージ
 * @param wParam WPARAM
 * @param lParam LPARAM
 * @return 処理結果 TRUE:メッセージを処理した。 FALSE:メッセージを処理しない
 */
INT_PTR CALLBACK BaseDialog::dialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	INT_PTR result;

	hWnd = hDlg;
	switch (message)
	{
		case WM_INITDIALOG:
            result = OnInitDialog();
			return result;

		case WM_COMMAND:
			result = OnCommand(wParam);
			if (result == 0) {
				return (INT_PTR)TRUE;
			} else {
				return (INT_PTR)FALSE;
			}
	}

	return (INT_PTR)FALSE;
}

/**
 * ダイアログとダイアログオブジェクトのデータの同期をとります。
 *
 * @param toObj true:ダイアログの内容をオブジェクトに格納する。
 *              false:オブジェクトの内容をダイアログに格納する。
 */
void BaseDialog::UpdateData(bool toObj)
{
}


/**
 * ダイアログの初期化時に呼び出されます。
 *
 * @return 処理結果
 */
INT_PTR BaseDialog::OnInitDialog()
{
	if (title.length() > 0) {
		::SetWindowText(hWnd, title.c_str());
	}
	return (INT_PTR)FALSE;
}


/**
 * ダイアログ操作が行われた時に呼び出されます。
 *
 * @return 処理結果 0:処理を行った 非0:処理を行わない
 */
INT_PTR BaseDialog::OnCommand(WPARAM wParam)
{
	if (!isModeless) {
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)0;
		}
	}
	return (INT_PTR)TRUE;
}

/**
 * ダイアログリソース上のウインドウを取得します。
 *
 * @param item リソースID
 * @return ダイアログ上のオブジェクト
 */
TwrWnd *BaseDialog::GetDlgItem(int item)
{
	HWND itemHwnd = ::GetDlgItem(hWnd, item);
	if (itemHwnd == NULL) {
		return NULL;
	}

	TwrWnd *newWnd;
	try {
		newWnd = new TwrWnd(itemHwnd);
		if (newWnd == NULL) {
			return NULL;
		}
		
	}catch(...) {
		return NULL;
	}
	return newWnd;
}

/**
 * ダイアログリソース上のウインドウハンドルを取得します。
 *
 * @param リソースID
 * @return ダイアログ上のウインドウのハンドル
 */
HWND BaseDialog::GetDlgItemRaw(int item)
{
	return ::GetDlgItem(hWnd, item);
}

