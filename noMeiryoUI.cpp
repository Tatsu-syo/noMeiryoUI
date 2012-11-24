// noMeiryoUI.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <windows.h>
#include <commdlg.h>
#include "noMeiryoUI.h"

#define MAX_LOADSTRING 100

//
// ダイアログベースアプリケーションフレームワークと
// ユーザー実装のブリッジルーチン
//

// アプリケーションオブジェクト
NoMeiryoUI *appObj;

/**
 * アプリケーションオブジェクトを作成します。
 */
DialogAppliBase *createAppli()
{
	// ここでユーザーのアプリケーションオブジェクトを作成します。
	appObj = new NoMeiryoUI();
	return appObj;
}

/**
 * ダイアログプローシジャ
 *
 */
INT_PTR CALLBACK MainDialogProc(
  HWND hwndDlg,  // ダイアログボックスのハンドル
  UINT uMsg,     // メッセージ
  WPARAM wParam, // 最初のメッセージパラメータ
  LPARAM lParam  // 2 番目のメッセージパラメータ
  )
{
	// ユーザーアプリケーションのダイアログプローシジャに丸投げします。
	return appObj->dialogProc(hwndDlg, uMsg,wParam,lParam);
}

//
// ダイアログベースアプリケーションフレームワークの実装部
//

/**
 * アプリケーションウインドウクラスのインスタンスを生成します。
 *
 * @return アプリケーションウインドウクラスのインスタンス
 */
BaseDialog *NoMeiryoUI::createBaseDialog()
{
	return appObj;
}

/**
 * アプリケーションの開始処理を実行します。
 *
 * @param lpCmdLine コマンドライン
 * @return 予備
 */
int NoMeiryoUI::OnAppliStart(TCHAR *lpCmdLine)
{
	// この関数をオーバーライドしてアプリ固有の初期化を行います。
	// 本アプリケーションでは特に処理なし。
	return 0;
}

/**
 * ウインドウが表示されたときの処理を実行します。<br>
 * ウインドウベースアプリとインタフェースを合わせるために用意しています。
 *
 * @return 予備
 */
int NoMeiryoUI::OnWindowShow()
{

	// この関数をオーバーライドして、初回の表示時の処理を行います。
	// このタイミングでダイアログが存在するので、ここに処理を入れることで
	// ダイアログがある状態で起動時の初期化処理を行うことができます。

	FillMemory(&metrics,sizeof(NONCLIENTMETRICS),0x00);
	FillMemory(&metricsAll,sizeof(NONCLIENTMETRICS),0x00);

	// アイコン以外のフォント情報を取得する。
	metrics.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metrics,
		0);

	SystemParametersInfo(SPI_GETICONTITLELOGFONT,
		sizeof(LOGFONT),
		&iconFont,
		0);

	metricsAll.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metricsAll,
		0);

	SystemParametersInfo(SPI_GETICONTITLELOGFONT,
		sizeof(LOGFONT),
		&iconFontAll,
		0);

	allFontName = metricsAll.lfMenuFont.lfFaceName;
	titleFontName = metrics.lfCaptionFont.lfFaceName;
	iconFontName = iconFont.lfFaceName;
	paletteFontName = metrics.lfSmCaptionFont.lfFaceName;
	hintFontName = metrics.lfStatusFont.lfFaceName;
	messageFontName = metrics.lfMessageFont.lfFaceName;
	// メニューと選択項目
	menuFontName = metrics.lfMenuFont.lfFaceName;

	UpdateData(false);

	return 0;
}

/**
 * アプリケーションの終了処理を実行します。
 *
 * @return 予備
 */
int NoMeiryoUI::OnAppliEnd()
{
	// この関数をオーバーライドしてアプリ固有の後処理を行います。
	// 本アプリケーションでは特に処理なし。
	return 0;
}

//
// ダイアログベースアプリケーションとしての
// アプリケーション固有処理の実装部分
//

/**
 * ダイアログ初期化処理
 *
 * @return TRUE: FALSE:フォーカスを設定した
 */
INT_PTR NoMeiryoUI::OnInitDialog()
{
	// 親クラスのダイアログ初期化処理を呼ぶ。
	DialogAppliBase::OnInitDialog();

	HICON hIcon;

    hIcon = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDC_MYICON), IMAGE_ICON, 16, 16, 0);
    SendMessage(this->hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);


	UpdateData(false);

	return (INT_PTR)FALSE;
}

/**
 * ダイアログコントロールとオブジェクトの内容の同期を取る。
 *
 * @param toObj true:コントロール→オブジェクト false:オブジェクト→コントロール
 */
void NoMeiryoUI::UpdateData(bool toObj)
{
	// ここにダイアログのコントロールと同期を取るメンバ変数を記述します。
	DDX_Text(toObj,IDC_EDIT_ALL, allFontName);
	DDX_Text(toObj,IDC_EDIT_TITLE, titleFontName);
	DDX_Text(toObj,IDC_EDIT_ICON, iconFontName);
	DDX_Text(toObj,IDC_EDIT_PALETTE, paletteFontName);
	DDX_Text(toObj,IDC_EDIT_HINT, hintFontName);
	DDX_Text(toObj,IDC_EDIT_MESSAGE, messageFontName);
	DDX_Text(toObj,IDC_EDIT_MENU, menuFontName);
}

INT_PTR NoMeiryoUI::OnCommand(WPARAM wParam)
{
	switch (LOWORD(wParam)) {
		case ID_SEL_ALL:
			selectFont(all);
			return (INT_PTR)TRUE;
		case ID_SEL_TITLE:
			selectFont(title);
			return (INT_PTR)TRUE;
		case ID_SEL_ICON:
			selectFont(icon);
			return (INT_PTR)TRUE;
		case ID_SEL_PALETTE:
			selectFont(palette);
			return (INT_PTR)TRUE;
		case ID_SEL_HINT:
			selectFont(hint);
			return (INT_PTR)TRUE;
		case ID_SEL_MESSAGE:
			selectFont(message);
			return (INT_PTR)TRUE;
		case ID_SEL_MENU:
			selectFont(menu);
			return (INT_PTR)TRUE;
		case ID_SET_ALL:
			OnBnClickedAll();
			return (INT_PTR)TRUE;
		case IDOK:
			OnBnClickedOk();
			break;
	}
	return BaseDialog::OnCommand(wParam);

}

void NoMeiryoUI::selectFont(enum fontType type)
{
	CHOOSEFONT font;
	int result;
	LOGFONT logfont;	// 取得したフォントの情報を入れる構造体

	// 設定したいフォントを選択する。
	FillMemory(&font,sizeof(CHOOSEFONT),0x00);
	
	font.lStructSize = sizeof(CHOOSEFONT);
	font.hwndOwner = NULL;
	font.lpLogFont = &logfont;
	font.Flags = CF_SCREENFONTS;
	result = ChooseFont(&font);

	if (!result){
		return;
	}
	if (logfont.lfFaceName[0] == _T('\0')) {
		return;
	}
	switch (type) {
		case all:
			metricsAll.lfMenuFont = logfont;
			metricsAll.lfStatusFont = logfont;
			metricsAll.lfMessageFont = logfont;
			metricsAll.lfCaptionFont = logfont;
			metricsAll.lfSmCaptionFont = logfont;
			iconFontAll = logfont;

			allFontName = logfont.lfFaceName;
			break;

		case title:
			metrics.lfCaptionFont = logfont;
			titleFontName = logfont.lfFaceName;
			break;

		case icon:
			iconFont = logfont;
			iconFontName = logfont.lfFaceName;
			break;

		case palette:
			metrics.lfSmCaptionFont = logfont;
			paletteFontName = logfont.lfFaceName;
			break;

		case hint:
			metrics.lfStatusFont = logfont;
			hintFontName = logfont.lfFaceName;
			break;

		case message:
			metrics.lfMessageFont = logfont;
			messageFontName = logfont.lfFaceName;
			break;

		case menu:
			// メニューと選択項目
			metrics.lfMenuFont = logfont;
			menuFontName = logfont.lfFaceName;
			break;
	}
	UpdateData(false);
}

void NoMeiryoUI::OnBnClickedOk()
{
	// アイコン以外のフォント設定
	SystemParametersInfo(SPI_SETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metrics,
		SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

	// アイコンのフォント設定
	SystemParametersInfo(SPI_SETICONTITLELOGFONT,
		sizeof(LOGFONT),
		&iconFont,
		SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

	// 色を再設定することで画面をリフレッシュする。
	DWORD btnColor;
	btnColor = GetSysColor(COLOR_BTNTEXT);

	INT colorItems[1];
	colorItems[0] = COLOR_BTNTEXT;
	COLORREF colors[1];
	colors[0] = btnColor;
	SetSysColors(1,colorItems,colors);
}

void NoMeiryoUI::OnBnClickedAll()
{

	// アイコン以外のフォント設定
	SystemParametersInfo(SPI_SETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metricsAll,
		SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

	// アイコンのフォント設定
	SystemParametersInfo(SPI_SETICONTITLELOGFONT,
		sizeof(LOGFONT),
		&iconFontAll,
		SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

	// 色を再設定することで画面をリフレッシュする。
	DWORD btnColor;
	btnColor = GetSysColor(COLOR_BTNTEXT);

	INT colorItems[1];
	colorItems[0] = COLOR_BTNTEXT;
	COLORREF colors[1];
	colors[0] = btnColor;
	SetSysColors(1,colorItems,colors);

	titleFontName = metricsAll.lfCaptionFont.lfFaceName;
	titleFontName = metricsAll.lfCaptionFont.lfFaceName;
	iconFontName = iconFontAll.lfFaceName;
	paletteFontName = metricsAll.lfCaptionFont.lfFaceName;
	hintFontName = metricsAll.lfCaptionFont.lfFaceName;
	messageFontName = metricsAll.lfCaptionFont.lfFaceName;
	// メニューと選択項目
	menuFontName = metricsAll.lfCaptionFont.lfFaceName;

	memcpy(&metrics, &metricsAll,sizeof(NONCLIENTMETRICS));

	UpdateData(false);
}