#include "stdafx.h"
#include "BaseForm.h"

/**
 * ウインドウのベースクラス
 */

/**
 * コンストラクタ
 */
BaseForm::BaseForm()
{
	windowClass = _T("");
	title = _T("");
	style = WS_OVERLAPPEDWINDOW;
	left = CW_USEDEFAULT;
	top = CW_USEDEFAULT;
	width = CW_USEDEFAULT;
	height = 0;
}

/**
 * デストラクタ
 */
BaseForm::~BaseForm()
{
}

/**
 * ウインドウを作成します。
 *
 * @param インスタンスハンドル
 * @return ウインドウハンドル
 */
HWND BaseForm::Create(HINSTANCE hInstance)
{
	this->hInstance = hInstance;
	hWnd = CreateWindow(
		windowClass,
		title,
		style,
		left,
		top,
		width,
		height,
		NULL,
		NULL,
		hInstance,
		NULL);

	return hWnd;
}

/**
 * ウインドウプローシジャ
 *
 * @param hDlg ウインドウハンドル
 * @param message メッセージ
 * @param wParam WPARAM
 * @param lParam LPARAM
 * @return 処理結果
 */
LRESULT CALLBACK BaseForm::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	LPNMHDR nmhdr;

	this->hWnd = hWnd;
	this->message = message;
	this->wParam = wParam;
	this->lParam = lParam;

	switch (message)
	{
		case WM_CREATE:
			return OnCreate();
		case WM_COMMAND:
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);

			return OnCommand(wmId, wmEvent);
		case WM_NOTIFY:
			nmhdr = (LPNMHDR)lParam;

			return OnNotify(nmhdr);
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: 描画コードをここに追加してください...
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			return OnDestroy();
		case WM_SYSCOLORCHANGE:	// システム色が変更になった。
			// 非クライアント領域(メニューバーやタイトルバー)も更新するため、これを使う。
			RedrawWindow(hWnd,NULL,NULL,RDW_ERASE | RDW_INVALIDATE | RDW_FRAME);
			return 0;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/**
 * ウインドウスタイルを設定します。
 *
 * @param newStyle ウインドウスタイル
 */
void BaseForm::setStyle(DWORD newStyle)
{
	style = newStyle;
	if (hWnd != NULL) {
		SetWindowLongPtr(hWnd, GWL_STYLE, newStyle);
	}
}

/**
 * ウインドウとウインドウオブジェクトのデータの同期をとります。
 *
 * @param toObj true:ダイアログの内容をオブジェクトに格納する。
 *              false:オブジェクトの内容をダイアログに格納する。
 */
void BaseForm::UpdateData(bool toObj)
{
}

/**
 * WM_COMMANDメッセージが来た時の処理を行います。
 *
 * @param wmId 
 * @param wmEvent
 * @return メッセージ処理結果
 */
LRESULT BaseForm::OnCommand(int wmId, int wmEvent)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/**
 * ウインドウの上に乗るウインドウオブジェクトを追加します。
 *
 * @param ウインドウオブジェクト
 * @return 親になったウインドウのハンドル
 */
HWND BaseForm::add(TwrWnd *obj)
{
	return ::SetParent(obj->getHwnd(), hWnd);
}

/**
 * ウインドウの上に乗るダイアログオブジェクトを追加します。
 *
 * @param ダイアログオブジェクト
 * @return 親になったウインドウのハンドル
 */
HWND BaseForm::add(BaseDialog *obj)
{
	obj->setParent(hWnd);
	return ::SetParent(obj->getHwnd(), hWnd);
}

/**
 * ウインドウが作成されたときに呼び出されます。
 *
 * @return 0
 */
LRESULT BaseForm::OnCreate()
{
	return 0;
}

/**
 * コモンコントロールでイベントが発生したときに呼び出されます。
 *
 * @param nmhdr 通知メッセージ情報
 * @return メッセージ処理結果
 */
LRESULT BaseForm::OnNotify(LPNMHDR nmhdr)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/**
 * ウインドウを閉じる際に呼び出されます。
 *
 * @return 0
 */
LRESULT BaseForm::OnDestroy()
{
	PostQuitMessage(0);

	return 0;
}

