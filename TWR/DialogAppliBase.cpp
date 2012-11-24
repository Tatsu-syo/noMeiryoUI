// AppliBase.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "commctrl.h"

#include "DialogAppliBase.h"

#define MAX_LOADSTRING 100

// グローバル変数:
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

BaseDialog *baseDialog;

// アプリケーション固有のグローバル変数
DialogAppliBase *appObj;

// このコード モジュールに含まれる関数の宣言を転送します:

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
	int result;
	INITCOMMONCONTROLSEX iccx;

	hInst = hInstance;
	iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccx.dwICC = ICC_TAB_CLASSES;
	if (!InitCommonControlsEx(&iccx)) {
         return FALSE;
	}

	// initModelessCallback();

	// TODO: ここにアプリケーションクラスのインスタンスを設定してください。
	appObj = createAppli();
	baseDialog = appObj->createBaseDialog();

	// ウインドウ表示前のアプリケーションの初期化を行う。
	appObj->OnAppliStart(lpCmdLine);

	// ダイアログを表示する。
	result = DialogBox(hInstance,MAKEINTRESOURCE(IDD_MAIN_DIALOG),NULL,(DLGPROC)MainDialogProc);

	// アプリケーションの後始末を行う。
	appObj->OnAppliEnd();

	delete appObj;
	return result;
}

/**
 * デストラクタ
 */
DialogAppliBase::~DialogAppliBase()
{
}

/**
 * アプリケーションウインドウクラスのインスタンスを生成します。
 *
 * @return アプリケーションウインドウクラスのインスタンス
 */
BaseDialog *DialogAppliBase::createBaseDialog()
{
	return new BaseDialog();
}

/**
 * アプリケーションの開始処理を実行します。
 *
 * @return 予備
 */
int DialogAppliBase::OnAppliStart(TCHAR *lpCmdLine)
{
	// この関数をオーバーライドしてアプリ固有の初期化を行います。
	return 0;
}

/**
 * ウインドウが表示されたときの処理を実行します。<br>
 * ウインドウベースアプリとインタフェースを合わせるために用意しています。
 *
 * @return 予備
 */
int DialogAppliBase::OnWindowShow()
{

	// この関数をオーバーライドして、初回の表示時の処理を行います。
	// このタイミングでダイアログが存在するので、ここに処理を入れることで
	// ダイアログがある状態で起動時の初期化処理を行うことができます。

	return 0;
}

/**
 * アプリケーションの終了処理を実行します。
 *
 * @return 予備
 */
int DialogAppliBase::OnAppliEnd()
{
	// この関数をオーバーライドしてアプリ固有の後処理を行います。
	return 0;
}

INT_PTR DialogAppliBase::OnInitDialog()
{
	// 親クラスのダイアログ初期化処理を呼び出す。
	BaseDialog::OnInitDialog();

	// ダイアログ表示時の初期化処理を行う。
	OnWindowShow();

	return (INT_PTR)FALSE;
}
