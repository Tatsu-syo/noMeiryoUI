/*
noMeiryoUI (C) 2005,2012,2013 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
// noMeiryoUI.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <windows.h>
#include <commdlg.h>
#ifdef DEBUG
#include <vld.h>
#endif
#include "noMeiryoUI.h"
#include "FontSel.h"

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
 * @return TRUE:処理した FALSE:処理しない
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
	// アプリ固有の初期化を行います。
	noMeiryoUI = false;
	verInfo = NULL;

	if (_tcsstr(lpCmdLine, _T("noMeiryoUI")) != NULL) {
		noMeiryoUI = true;
	}

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
	if (verInfo != NULL) {
		delete verInfo;
	}
	return 0;
}

//
// ダイアログベースアプリケーションとしての
// アプリケーション固有処理の実装部分
//

/**
 * ダイアログ初期化処理
 *
 * @return TRUE:フォーカスを設定した FALSE:フォーカスを設定しない
 */
INT_PTR NoMeiryoUI::OnInitDialog()
{
	// 親クラスのダイアログ初期化処理を呼ぶ。
	DialogAppliBase::OnInitDialog();

	// アプリケーションアイコンの設定
	HICON hIcon;

    hIcon = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDC_MYICON), IMAGE_ICON, 16, 16, 0);
    SendMessage(this->hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

	return (INT_PTR)FALSE;
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

	// 
	DWORD dwVersion = GetVersion();
	TCHAR buf[16];

	_stprintf(buf,_T("%d.%d"),
		(DWORD)(LOBYTE(LOWORD(dwVersion))),
		(DWORD)(HIBYTE(LOWORD(dwVersion))) );
	verInfo = GetDlgItem(IDC_STATIC_VERNO);
	verInfo->setText(buf);

	// フォント情報取得用構造体の初期化
	FillMemory(&metrics,sizeof(NONCLIENTMETRICS),0x00);
	FillMemory(&metricsAll,sizeof(NONCLIENTMETRICS),0x00);
	FillMemory(&iconFont,sizeof(LOGFONT),0x00);
	FillMemory(&iconFontAll,sizeof(LOGFONT),0x00);

	//
	// 個別のフォント用の情報取得
	//

	// アイコン以外のフォント情報を取得する。
	metrics.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metrics,
		0);

	// アイコンのフォント情報を取得する。
	SystemParametersInfo(SPI_GETICONTITLELOGFONT,
		sizeof(LOGFONT),
		&iconFont,
		0);

	//
	// すべてのフォント用の情報取得
	//

	// アイコン以外のフォント情報を取得する。
	metricsAll.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metricsAll,
		0);

	// アイコンのフォント情報を取得する。
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

	metricsAll.lfStatusFont = metricsAll.lfMenuFont;
	metricsAll.lfMessageFont = metricsAll.lfMenuFont;
	metricsAll.lfCaptionFont = metricsAll.lfMenuFont;
	metricsAll.lfSmCaptionFont = metricsAll.lfMenuFont;
	iconFontAll = metricsAll.lfMenuFont;

	UpdateData(false);

	return 0;
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

/**
 * 各操作に対する処理の分岐
 *
 * @param wParam WPARAM
 * @return 0:メッセージを処理した。 0以外:メッセージを処理しなかった。
 */
INT_PTR NoMeiryoUI::OnCommand(WPARAM wParam)
{
	INT_PTR result;

	switch (LOWORD(wParam)) {
		case ID_SEL_ALL:
			selectFont(all);
			return (INT_PTR)0;
		case ID_SEL_TITLE:
			selectFont(title);
			return (INT_PTR)0;
		case ID_SEL_ICON:
			selectFont(icon);
			return (INT_PTR)0;
		case ID_SEL_PALETTE:
			selectFont(palette);
			return (INT_PTR)0;
		case ID_SEL_HINT:
			selectFont(hint);
			return (INT_PTR)0;
		case ID_SEL_MESSAGE:
			selectFont(message);
			return (INT_PTR)0;
		case ID_SEL_MENU:
			selectFont(menu);
			return (INT_PTR)0;
		case ID_SET_ALL:
			OnBnClickedAll();
			return (INT_PTR)0;
		case IDOK:
			result = OnBnClickedOk();
			if (!result) {
				return (INT_PTR)0;
			}
			break;
	}
	return BaseDialog::OnCommand(wParam);

}

/**
 * フォントを選択する。
 *
 * @param type 設定するフォントの種類
 */
void NoMeiryoUI::selectFont(enum fontType type)
{

	CHOOSEFONT font;
	INT_PTR result;
	LOGFONT logfont;	// 取得したフォントの情報を入れる構造体

	// 設定したいフォントを選択する。
	FillMemory(&font,sizeof(CHOOSEFONT),0x00);
	FillMemory(&logfont,sizeof(LOGFONT),0x00);
	
	font.lStructSize = sizeof(CHOOSEFONT);
	font.hwndOwner = this->hWnd;
	font.hDC = NULL;
	font.lpLogFont = &logfont;
	font.Flags = CF_SCREENFONTS;
	font.hInstance = hInst;
	font.lpTemplateName = _T("");
	font.lpszStyle = _T("");
	font.nFontType = SCREEN_FONTTYPE;
	font.nSizeMax = 72;
	// font.rgbColors = rgbCurrent;

	try {
		// result = ChooseFont(&font);
		FontSel *selector = new FontSel(this->hWnd, IDD_DIALOG_FONTSEL);
		if (noMeiryoUI) {
			selector->setNoMeiryoUI();
		}

		result = selector->showModal();
		if (result != IDOK){
			delete []selector;
			return;
		}
		logfont = selector->getSelectedFont();
		if (logfont.lfFaceName[0] == _T('\0')) {
			delete []selector;
			return;
		}

		delete []selector;
	} catch (...) {
		MessageBox(this->hWnd,
			_T("フォント選択ダイアログ内で内部エラーが発生しました。"),
			_T("内部エラー"),
			MB_OK | MB_ICONEXCLAMATION);
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

/**
 * OKボタン押下時の動作(選択したフォントを設定する。)
 *
 * @return TRUE:フォントを設定したとき FALSE:フォント設定をキャンセルしたとき
 */
INT_PTR NoMeiryoUI::OnBnClickedOk()
{
	// 誤って縦書き用フォントを指定しないよう問い合わせを行う。
	bool hasVerticalFont = false;
	if (metrics.lfCaptionFont.lfFaceName[0] == _T('@')) {
		hasVerticalFont = true;
	}
	if (metrics.lfSmCaptionFont.lfFaceName[0] == _T('@')) {
		hasVerticalFont = true;
	}
	if (metrics.lfStatusFont.lfFaceName[0] == _T('@')) {
		hasVerticalFont = true;
	}
	if (metrics.lfMessageFont.lfFaceName[0] == _T('@')) {
		hasVerticalFont = true;
	}
	if (metrics.lfMenuFont.lfFaceName[0] == _T('@')) {
		hasVerticalFont = true;	
	}
	if (iconFont.lfFaceName[0] == _T('@')) {
		hasVerticalFont = true;
	}

	if (hasVerticalFont) {
		int answer = MessageBox(hWnd,
			_T("縦書き用フォント(名前が@で始まるフォント)が\n指定されていますがよろしいですか？"),
			_T("確認"),
			MB_ICONQUESTION | MB_YESNO);
		if (answer != IDYES) {
			return (INT_PTR)FALSE;
		}
	}

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
	// のだが、IObit StartMenu 8が起動しているときはSetSysColorsを
	// 呼び出すと応答がなくなるので呼び出しを行わないことにした。
#if 0
	DWORD btnColor;
	btnColor = GetSysColor(COLOR_BTNTEXT);

	INT colorItems[1];
	colorItems[0] = COLOR_BTNTEXT;
	COLORREF colors[1];
	colors[0] = btnColor;
	SetSysColors(1,colorItems,colors);
#endif

	return (INT_PTR)TRUE;
}

/**
 * 一括設定ボタン押下時の動作(すべてのフォントで選択したフォントを設定する。)
 *
 */
void NoMeiryoUI::OnBnClickedAll()
{
	// 誤って縦書き用フォントを指定しないよう問い合わせを行う。
	if (metricsAll.lfMenuFont.lfFaceName[0] == _T('@')) {
		int answer = MessageBox(hWnd,
			_T("縦書き用フォント(名前が@で始まるフォント)が\n指定されていますがよろしいですか？"),
			_T("確認"),
			MB_ICONQUESTION | MB_YESNO);
		if (answer != IDYES) {
			return;
		}
	}

	// アイコン以外のフォント設定
	SystemParametersInfo(SPI_SETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metricsAll,
		SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
		// SPIF_UPDATEINIFILE);

	// アイコンのフォント設定
	SystemParametersInfo(SPI_SETICONTITLELOGFONT,
		sizeof(LOGFONT),
		&iconFontAll,
		SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

	// 色を再設定することで画面をリフレッシュする。
	// のだが、IObit StartMenu 8が起動しているときはSetSysColorsを
	// 呼び出すと応答がなくなるので呼び出しを行わないことにした。
#if 0
	DWORD btnColor;
	btnColor = GetSysColor(COLOR_BTNTEXT);

	INT colorItems[1];
	colorItems[0] = COLOR_BTNTEXT;
	COLORREF colors[1];
	colors[0] = btnColor;
	SetSysColors(1,colorItems,colors);
#endif

	titleFontName = metricsAll.lfCaptionFont.lfFaceName;
	titleFontName = metricsAll.lfCaptionFont.lfFaceName;
	iconFontName = iconFontAll.lfFaceName;
	paletteFontName = metricsAll.lfCaptionFont.lfFaceName;
	hintFontName = metricsAll.lfCaptionFont.lfFaceName;
	messageFontName = metricsAll.lfCaptionFont.lfFaceName;
	// メニューと選択項目
	menuFontName = metricsAll.lfCaptionFont.lfFaceName;

	memcpy(&metrics, &metricsAll,sizeof(NONCLIENTMETRICS));
	memcpy(&iconFont, &iconFontAll,sizeof(LOGFONT));

	UpdateData(false);
}