// AppliBase.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "commctrl.h"

#include "TwrCore.h"
#include "WindowAppliBase.h"
#include "ModelessDialog.h"

// グローバル変数:
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名
BaseForm *baseForm;

// アプリケーション固有のグローバル変数
WindowAppliBase *appObj;

extern void initModelessCallback(void);

// このコード モジュールに含まれる関数の宣言を転送します:
int runApplication(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow);
BOOL				InitInstance(HINSTANCE, int);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
bool handleDialogMessage(LPMSG lpMsg);

/**
 * アプリケーションメインルーチン
 *
 * @param hInstance アプリケーションのインスタンスハンドル
 * @param hPrevInstance 未使用
 * @param lpCmdLine コマンドライン
 * @param nCmdShow 表示状態
 * @return プログラム終了コード
 */
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccx.dwICC = ICC_TAB_CLASSES;
	if (!InitCommonControlsEx(&iccx)) {
         return FALSE;
	}

	initModelessDialog();
	initModelessCallback();

	// ここでアプリケーションクラスのインスタンスを設定します。
	appObj = createAppli();
	baseForm = appObj->createBaseForm();

	// ウインドウ表示前のアプリケーションの初期化を行う。
	appObj->OnAppliStart(lpCmdLine);

	int result = runApplication(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	// アプリケーションの後始末を行う。
	appObj->OnAppliEnd();

	delete appObj;
	return result;
}

/**
 * アプリケーション開始
 *
 * @param hInstance アプリケーションのインスタンスハンドル
 * @param hPrevInstance 未使用
 * @param lpCmdLine コマンドライン
 * @param nCmdShow 表示状態
 * @return プログラム終了コード
 */
int runApplication(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDS_APP_CLASS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーションの初期化を実行します:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APP_ACCEL));
	doEvents();

	// ウインドウ表示直後に行う処理を行う。
	appObj->OnWindowShow();

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (handleDialogMessage(&msg)) {
			// メッセージがモードレスダイアログのものだった場合は
			// 何もしない。
			continue;
		}
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   baseForm->setClass(szWindowClass);
   baseForm->setTitle(szTitle);
   hWnd = baseForm->Create(hInstance);
   if (!hWnd)
   {
      return FALSE;
   }

   baseForm->ShowWindow(nCmdShow);
   baseForm->UpdateWindow();

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:  メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = baseForm->WndProc(hWnd, message, wParam, lParam);

	return lResult;
}

/**
 * Windowsのイベントを処理する。
 *
 * @return イベント処理結果
 */
int doEvents()
{
	MSG msg;
    // メッセージの処理
    while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		BOOL bRet = GetMessage(&msg, NULL, 0, 0);
		if (bRet > 0){
			if (handleDialogMessage(&msg)) {
				// メッセージがモードレスダイアログのものだった場合は
				// 何もしない。
				continue;
			} else {
				::TranslateMessage(&msg);
				::DispatchMessage (&msg);
			}
		} else {
			break;
		}
    }
	return 0;
}

/**
 * デストラクタ
 */
WindowAppliBase::~WindowAppliBase()
{
}

/**
 * アプリケーションウインドウクラスのインスタンスを生成します。
 *
 * @return アプリケーションウインドウクラスのインスタンス
 */
BaseForm *WindowAppliBase::createBaseForm()
{
	return new BaseForm();
}

/**
 * アプリケーションの開始処理を実行します。
 *
 * @return 予備
 */
int WindowAppliBase::OnAppliStart(TCHAR *lpCmdLine)
{
	// この関数をオーバーライドしてアプリ固有の初期化を行います。
	return 0;
}

/**
 * ウインドウが表示されたときの処理を実行します。
 *
 * @return 予備
 */
int WindowAppliBase::OnWindowShow()
{
	// この関数をオーバーライドして、初回の表示時の処理を行います。
	return 0;
}

/**
 * アプリケーションの終了処理を実行します。
 *
 * @return 予備
 */
int WindowAppliBase::OnAppliEnd()
{
	// この関数をオーバーライドしてアプリ固有の後処理を行います。
	return 0;
}
