// AppliBase.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "commctrl.h"

#include "TwrCore.h"
#include "AppliBase.h"
#include "ModelessDialog.h"
#include "BaseForm.h"

// アプリケーション固有のヘッダファイル
#include "../MyAppli.h"

#define MAX_LOADSTRING 100

// グローバル変数:
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名
BaseForm *baseForm;

// アプリケーション固有のグローバル変数
MyAppli *appObj;

extern void initModelessCallback(void);

// このコード モジュールに含まれる関数の宣言を転送します:
int runApplication(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow);
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
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

	// TODO: ここにアプリケーションクラスのインスタンスを設定してください。
	appObj = new MyAppli();
	baseForm = appObj->createBaseWindow();

	// ウインドウ表示前のアプリケーションの初期化を行う。
	appObj->OnAppliStart(lpCmdLine);

	int result = runApplication(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	// アプリケーションの後始末を行う。
	appObj->OnAppliEnd();

	delete baseForm;
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
	LoadString(hInstance, IDC_BOOKSYNC, szWindowClass, MAX_LOADSTRING);
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
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
//  コメント:
//
//    この関数および使い方は、'RegisterClassEx' 関数が追加された
//    Windows 95 より前の Win32 システムと互換させる場合にのみ必要です。
//    アプリケーションが、関連付けられた
//    正しい形式の小さいアイコンを取得できるようにするには、
//    この関数を呼び出してください。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.lpszClassName	= szWindowClass;

	// アプリケーション固有の情報
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	// wcex.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_BOOKSYNC);
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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

