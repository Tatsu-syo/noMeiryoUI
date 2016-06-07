/*
noMeiryoUI (C) 2005,2012-2016 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
// noMeiryoUI.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <process.h>
#include <Objbase.h>
#include <shellapi.h>
#include "noMeiryoUI.h"
#include "FontSel.h"
#include "NCFileDialog.h"
#include "util.h"

#define MAX_LOADSTRING 100

//
// ダイアログベースアプリケーションフレームワークと
// ユーザー実装のブリッジルーチン
//

// アプリケーションオブジェクト
NoMeiryoUI *appObj;
static bool use7Compat = true;

/**
 * アプリケーションオブジェクトを作成します。
 */
DialogAppliBase *createAppli()
{
	CoInitialize(NULL);

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

	allFont = NULL;
	titleFont = NULL;
	iconFontHandle = NULL;
	paletteFont = NULL;
	hintFont = NULL;
	messageFont = NULL;
	menuFont = NULL;

	allFontTextBox = NULL;
	titleFontTextBox = NULL;
	iconFontTextBox = NULL;
	paletteFontTextBox = NULL;
	hintFontTextBox = NULL;
	messageFontTextBox = NULL;
	menuFontTextBox = NULL;

	if (_tcsstr(lpCmdLine, _T("noMeiryoUI")) != NULL) {
		noMeiryoUI = true;
	}

	DWORD dwVersion = GetVersion();

	DWORD major = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	DWORD minor = (DWORD)(HIBYTE(LOWORD(dwVersion)));
	if (major < 6) {
		// Windows XP or earlyer
		WIN8_SIZE = false;
		use7Compat = false;
	} else if (major == 6) {
		if (minor < 2) {
			// Windows Vista/7
			WIN8_SIZE = false;
			use7Compat = false;
		} else {
			// Windows 8/8.1
			WIN8_SIZE = true;
			use7Compat = true;
		}
	} else {
		// Windows 10 or later
		WIN8_SIZE = false;
		use7Compat = false;
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

	if (allFont != NULL) {
		DeleteObject(allFont);
	}
	if (titleFont != NULL) {
		DeleteObject(titleFont);
	}
	if (iconFontHandle != NULL) {
		DeleteObject(iconFontHandle);
	}
	if (paletteFont != NULL) {
		DeleteObject(paletteFont);
	}
	if (hintFont != NULL) {
		DeleteObject(hintFont);
	}
	if (messageFont != NULL) {
		DeleteObject(messageFont);
	}
	if (menuFont != NULL) {
		DeleteObject(menuFont);
	}

	if (allFontTextBox != NULL) {
		delete allFontTextBox;
	}
	if (titleFontTextBox != NULL) {
		delete titleFontTextBox;
	}
	if (iconFontTextBox != NULL) {
		delete iconFontTextBox;
	}
	if (paletteFontTextBox != NULL) {
		delete paletteFontTextBox;
	}
	if (hintFontTextBox != NULL) {
		delete hintFontTextBox;
	}
	if (messageFontTextBox != NULL) {
		delete messageFontTextBox;
	}
	if (menuFontTextBox != NULL) {
		delete menuFontTextBox;
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

	appMenu = new TwrMenu(this->hWnd);

	if (!use7Compat) {
		// Windows 7以前の場合はフォントサイズの取り扱いモードを変更できなくする。
		appMenu->setEnabled(IDM_COMPAT7, false);
	}
	appMenu->CheckMenuItem(IDM_ANOTHER, true);

	// Windows 8.1以前ではWindows 10用のプリセットを使用不可とする。
	DWORD dwVersion = GetVersion();

	DWORD major = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	DWORD minor = (DWORD)(HIBYTE(LOWORD(dwVersion)));
	if (major < 10) {
		appMenu->setEnabled(IDM_SET_10, false);
	}

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

	SetWinVer();

	// フォント情報取得用構造体の初期化
	FillMemory(&metrics,sizeof(NONCLIENTMETRICS),0x00);
	FillMemory(&metricsAll,sizeof(NONCLIENTMETRICS),0x00);
	FillMemory(&iconFont,sizeof(LOGFONT),0x00);
	FillMemory(&iconFontAll,sizeof(LOGFONT),0x00);

	// 現在のフォントを取得する。
	getActualFont();

	// テキストボックス制御用にダイアログの各テキストボックスを取得する。
	allFontTextBox = GetDlgItem(IDC_EDIT_ALL);
	titleFontTextBox = GetDlgItem(IDC_EDIT_TITLE);
	iconFontTextBox = GetDlgItem(IDC_EDIT_ICON);
	paletteFontTextBox = GetDlgItem(IDC_EDIT_PALETTE);
	hintFontTextBox = GetDlgItem(IDC_EDIT_HINT);
	messageFontTextBox = GetDlgItem(IDC_EDIT_MESSAGE);
	menuFontTextBox = GetDlgItem(IDC_EDIT_MENU);

	// 表示を更新する。
	updateDisplay();

	return 0;
}

/**
 * 現在設定されているフォントを取得する。
 */
void NoMeiryoUI::getActualFont(void)
{
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

	// メニューフォントの情報を他のフォントの情報にあてる。
	metricsAll.lfStatusFont = metricsAll.lfMenuFont;
	metricsAll.lfMessageFont = metricsAll.lfMenuFont;
	metricsAll.lfCaptionFont = metricsAll.lfMenuFont;
	metricsAll.lfSmCaptionFont = metricsAll.lfMenuFont;
	iconFontAll = metricsAll.lfMenuFont;
}

/**
 * フォント情報からフォントのハンドルを作成する。
 *
 * @param font フォントの情報
 * @return フォントハンドル
 */
HFONT NoMeiryoUI::createFont(LOGFONT *font)
{
	HDC hDC = GetDC(this->hWnd);

	HFONT newFont = CreateFont(
		-MulDiv(10, GetDeviceCaps(hDC, LOGPIXELSY), 72),
		0,
		0,
		0,
		font->lfWeight,
		font->lfItalic,
		font->lfUnderline,
		font->lfStrikeOut,
		font->lfCharSet,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, // CLEARTYPE_QUALITY,
		FIXED_PITCH | FF_MODERN,
		font->lfFaceName);

	ReleaseDC(this->hWnd, hDC);

	return newFont;
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
 * フォント表示を更新する。
 */
void NoMeiryoUI::updateDisplay(void)
{
	// フォント名、ポイント数表示文字列を作成する。
	int point;
	TCHAR buf[16];

	allFontName = metricsAll.lfMenuFont.lfFaceName;
	point = getFontPointInt(&(metricsAll.lfMenuFont), this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	allFontName = allFontName + buf;

	titleFontName = metrics.lfCaptionFont.lfFaceName;
	point = getFontPointInt(&(metrics.lfCaptionFont), this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	titleFontName = titleFontName + buf;

	iconFontName = iconFont.lfFaceName;
	point = getFontPointInt(&iconFont, this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	iconFontName = iconFontName + buf;

	paletteFontName = metrics.lfSmCaptionFont.lfFaceName;
	point = getFontPointInt(&metrics.lfSmCaptionFont, this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	paletteFontName = paletteFontName + buf;

	hintFontName = metrics.lfStatusFont.lfFaceName;
	point = getFontPointInt(&metrics.lfStatusFont, this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	hintFontName = hintFontName + buf;

	messageFontName = metrics.lfMessageFont.lfFaceName;
	point = getFontPointInt(&metrics.lfMessageFont, this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	messageFontName = messageFontName + buf;

	// メニューと選択項目
	menuFontName = metrics.lfMenuFont.lfFaceName;
	point = getFontPointInt(&metrics.lfMenuFont, this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	menuFontName = menuFontName + buf;

	UpdateData(false);


	// 選択したフォントをテキストボックスに設定する。
	if (allFont != NULL) {
		DeleteObject(allFont);
	}
	allFont = createFont(&metricsAll.lfMenuFont);
	allFontTextBox->setFont(allFont);

	if (titleFont != NULL) {
		DeleteObject(titleFont);
	}
	titleFont = createFont(&metrics.lfCaptionFont);
	titleFontTextBox->setFont(titleFont);

	if (iconFontHandle != NULL) {
		DeleteObject(iconFontHandle);
	}
	iconFontHandle = createFont(&iconFont);
	iconFontTextBox->setFont(iconFontHandle);

	if (paletteFont != NULL) {
		DeleteObject(paletteFont);
	}
	paletteFont = createFont(&metrics.lfSmCaptionFont);
	paletteFontTextBox->setFont(paletteFont);

	if (hintFont != NULL) {
		DeleteObject(hintFont);
	}
	hintFont = createFont(&metrics.lfStatusFont);
	hintFontTextBox->setFont(hintFont);

	if (messageFont != NULL) {
		DeleteObject(messageFont);
	}
	messageFont = createFont(&metrics.lfMessageFont);
	messageFontTextBox->setFont(messageFont);

	if (menuFont != NULL) {
		DeleteObject(menuFont);
	}
	menuFont = createFont(&metrics.lfMenuFont);
	menuFontTextBox->setFont(menuFont);

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
		case IDM_OPEN:
			OnLoad();
			return (INT_PTR)0;
		case IDM_SAVE:
			OnSave();
			return (INT_PTR)0;
		case IDOK:
			result = OnBnClickedOk();
			if (!result) {
				return (INT_PTR)0;
			}
			break;
		case IDM_EXIT:
			EndDialog(hWnd, LOWORD(wParam));
			break;
		case IDM_SET_8:
			OnSet8();
			return (INT_PTR)0;
		case IDM_SET_10:
			OnSet10();
			return (INT_PTR)0;
		case IDM_ANOTHER:
			if (appMenu->isChecked(IDM_ANOTHER)) {
				appMenu->CheckMenuItem(IDM_ANOTHER, false);
			} else {
				appMenu->CheckMenuItem(IDM_ANOTHER, true);
			}
			return (INT_PTR)0;
		case IDM_COMPAT7:
			if (appMenu->isChecked(IDM_COMPAT7)) {
				appMenu->CheckMenuItem(IDM_COMPAT7, false);
				WIN8_SIZE = true;
			} else {
				appMenu->CheckMenuItem(IDM_COMPAT7, true);
				WIN8_SIZE = false;
			}
			updateDisplay();

			return (INT_PTR)0;
		case IDM_HELPTOPIC:
			showHelp();
			return (INT_PTR)0;
		case IDM_ABOUT:
			MessageBox(hWnd, 
				_T("Meiryo UIも大っきらい!! Version 2.17\n\nBy Tatsuhiko Syoji(Tatsu) 2005,2012-2016"),
				_T("Meiryo UIも大っきらい!!について"),
				MB_OK | MB_ICONINFORMATION);

			return (INT_PTR)0;
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
		LOGFONT *target;
		switch (type) {
			case all:
				target = &metricsAll.lfMenuFont;
				break;

			case title:
				target = &metrics.lfCaptionFont;
				break;

			case icon:
				target = &iconFont;
				break;

			case palette:
				target = &metrics.lfSmCaptionFont;
				break;

			case hint:
				target = &metrics.lfStatusFont;
				break;

			case message:
				target = &metrics.lfMessageFont;
				break;

			case menu:
				// メニューと選択項目
				target = &metrics.lfMenuFont;
				break;
		}

		// result = ChooseFont(&font);
		FontSel *selector = new FontSel(this->hWnd, IDD_DIALOG_FONTSEL);
		if (noMeiryoUI) {
			selector->setNoMeiryoUI();
		}
		// 選択していたフォントをフォント選択ダイアログに設定する。
		selector->setPreviousFont(target);

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

			DeleteObject(allFont);
			allFont = createFont(&metricsAll.lfMenuFont);
			allFontTextBox->setFont(allFont);

			break;

		case title:
			metrics.lfCaptionFont = logfont;
			titleFontName = logfont.lfFaceName;

			DeleteObject(titleFont);
			titleFont = createFont(&metrics.lfCaptionFont);
			titleFontTextBox->setFont(titleFont);
			break;

		case icon:
			iconFont = logfont;
			iconFontName = logfont.lfFaceName;

			DeleteObject(iconFontHandle);
			iconFontHandle = createFont(&iconFont);
			iconFontTextBox->setFont(iconFontHandle);
			break;

		case palette:
			metrics.lfSmCaptionFont = logfont;
			paletteFontName = logfont.lfFaceName;

			DeleteObject(paletteFont);
			paletteFont = createFont(&metrics.lfSmCaptionFont);
			paletteFontTextBox->setFont(paletteFont);
			break;

		case hint:
			metrics.lfStatusFont = logfont;
			hintFontName = logfont.lfFaceName;

			DeleteObject(hintFont);
			hintFont = createFont(&metrics.lfStatusFont);
			hintFontTextBox->setFont(hintFont);
			break;

		case message:
			metrics.lfMessageFont = logfont;
			messageFontName = logfont.lfFaceName;

			DeleteObject(messageFont);
			messageFont = createFont(&metrics.lfMessageFont);
			messageFontTextBox->setFont(messageFont);
			break;

		case menu:
			// メニューと選択項目
			metrics.lfMenuFont = logfont;
			menuFontName = logfont.lfFaceName;

			DeleteObject(menuFont);
			menuFont = createFont(&metrics.lfMenuFont);
			menuFontTextBox->setFont(menuFont);
			break;
	}

	// 表示を更新する。
	updateDisplay();
}

/**
 * フォント設定を保存するを選択した時の動作
 */
void NoMeiryoUI::OnLoad()
{
	NCFileDialog *dlg = new NCFileDialog(
		TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("設定ファイル(*.ini)\0*.ini\0すべてのファイル(*.*)\0*.*\0\0"),
		this->getHwnd(),
		0);

	int result = dlg->DoModal();
	if (result != IDOK) {
		delete dlg;
		return;
	}

	BOOL loadResult;
	loadResult = loadFontInfo(dlg->GetPathName());
	if (!loadResult) {
		MessageBox(
			this->getHwnd(),
			_T("フォント設定の読み込みに失敗しました。"),
			_T("エラー"),
			MB_OK | MB_ICONEXCLAMATION);
	} else {
		// フォント設定の読み込みに成功したらテキストボックスに設定する。
		updateDisplay();
	}


	delete dlg;
}

/**
 * フォント情報を読み込む。
 *
 * @param filename iniファイル名
 * @result TRUE:保存成功 FALSE:保存失敗
 */
BOOL NoMeiryoUI::loadFontInfo(TCHAR *filename)
{
	BOOL loadResult;
	LOGFONT captionFont;
	LOGFONT newIconFont;
	LOGFONT smCaptionFont;
	LOGFONT statusFont;
	LOGFONT messageFont;
	LOGFONT menuFont;

	loadResult = loadFont(filename, _T("TitleFont"), &captionFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, _T("IconFont"), &newIconFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, _T("PaletteFont"), &smCaptionFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, _T("HintFont"), &statusFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, _T("MessageFont"), &messageFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, _T("MenuFont"), &menuFont);
	if (!loadResult) {
		return FALSE;
	}

	metrics.lfCaptionFont = captionFont;
	iconFont = newIconFont;
	metrics.lfSmCaptionFont = smCaptionFont;
	metrics.lfStatusFont = statusFont;
	metrics.lfMessageFont = messageFont;
	metrics.lfMenuFont = menuFont;

	return TRUE;
}

/**
 * フォント情報を読み込む。
 *
 * @param filename iniファイル名
 * @param category 読み込み対象フォントのiniファイルセクション名
 * @param font 読み込み対象フォントのLOGFONT構造体
 * @result TRUE:読み込み成功 FALSE:読み込み失敗
 */
BOOL NoMeiryoUI::loadFont(TCHAR *filename, TCHAR *section, LOGFONT *font)
{
	TCHAR buf[32];
	DWORD result;

	result = GetPrivateProfileString(section,
		_T("FaceName"),
		_T(""),
		font->lfFaceName,
		32,
		filename);
	if (!result) {
		return FALSE;
	}

	result = GetPrivateProfileString(section,
		_T("Height"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfHeight = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("Width"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfWidth = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("Escapement"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfEscapement = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("Orientation"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfOrientation = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("Weight"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfWeight = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("Italic"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfItalic = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("Underline"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfUnderline = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("StrikeOut"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfStrikeOut = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("CharSet"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfCharSet = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("OutPrecision"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfOutPrecision = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("ClipPrecision"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfClipPrecision = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("Quality"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfQuality = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("PitchAndFamily"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfPitchAndFamily = _ttoi(buf);

	return TRUE;
}


/**
 * フォント設定を保存するを選択した時の動作
 */
void NoMeiryoUI::OnSave()
{
	NCFileDialog *dlg = new NCFileDialog(
		FALSE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("設定ファイル(*.ini)\0*.ini\0すべてのファイル(*.*)\0*.*\0\0"),
		this->getHwnd(),
		0);

	int result = dlg->DoModal();
	if (result != IDOK) {
		delete dlg;
		return;
	}

	BOOL saveResult;
	saveResult = startSaveFont(dlg->GetPathName());
	if (!saveResult) {
		MessageBox(
			this->getHwnd(),
			_T("フォント設定の保存に失敗しました。"),
			_T("エラー"),
			MB_OK | MB_ICONEXCLAMATION);
	}


	delete dlg;
}

/**
 * フォント情報保存を開始する。
 *
 * @param filename iniファイル名
 * @result TRUE:保存成功 FALSE:保存失敗
 */
BOOL NoMeiryoUI::startSaveFont(TCHAR *filename)
{
	BOOL saveResult;

	saveResult = saveFont(filename, _T("TitleFont"), &metrics.lfCaptionFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(filename, _T("IconFont"), &iconFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(filename, _T("PaletteFont"), &metrics.lfSmCaptionFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(filename, _T("HintFont"), &metrics.lfStatusFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(filename, _T("MessageFont"), &metrics.lfMessageFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(filename, _T("MenuFont"), &metrics.lfMenuFont);
	if (!saveResult) {
		return FALSE;
	}

	return TRUE;
}

/**
 * フォント情報を保存する。
 *
 * @param filename iniファイル名
 * @param category 保存対象フォントのiniファイルセクション名
 * @param font 保存対象フォントのLOGFONT構造体
 * @result TRUE:保存成功 FALSE:保存失敗
 */
BOOL NoMeiryoUI::saveFont(TCHAR *filename, TCHAR *section, LOGFONT *font)
{
	TCHAR buf[32];
	BOOL result;

	result = WritePrivateProfileString(section,
		_T("FaceName"),
		font->lfFaceName,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfHeight);
	result = WritePrivateProfileString(section,
		_T("Height"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfWidth);
	result = WritePrivateProfileString(section,
		_T("Width"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfEscapement);
	result = WritePrivateProfileString(section,
		_T("Escapement"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfOrientation);
	result = WritePrivateProfileString(section,
		_T("Orientation"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfWeight);
	result = WritePrivateProfileString(section,
		_T("Weight"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfItalic);
	result = WritePrivateProfileString(section,
		_T("Italic"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfUnderline);
	result = WritePrivateProfileString(section,
		_T("Underline"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfStrikeOut);
	result = WritePrivateProfileString(section,
		_T("StrikeOut"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfCharSet);
	result = WritePrivateProfileString(section,
		_T("CharSet"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfOutPrecision);
	result = WritePrivateProfileString(section,
		_T("OutPrecision"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfClipPrecision);
	result = WritePrivateProfileString(section,
		_T("ClipPrecision"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfQuality);
	result = WritePrivateProfileString(section,
		_T("Quality"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfPitchAndFamily);
	result = WritePrivateProfileString(section,
		_T("PitchAndFamily"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	return TRUE;
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

	// フォント変更を実施する。
	setFont(&metrics, &iconFont);

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

	// フォント変更を実施する。
	setFont(&metricsAll, &iconFontAll);

	memcpy(&metrics, &metricsAll,sizeof(NONCLIENTMETRICS));
	memcpy(&iconFont, &iconFontAll,sizeof(LOGFONT));

	// 表示を更新する。
	updateDisplay();
}

/**
 * システムのDPIを取得する。
 *
 * @return 取得したDPI
 */
int NoMeiryoUI::getDPI(void)
{
	// DPIを取得する。
	int dpiY = 96;
	HDC hdc = GetDC(NULL);

	if (hdc) {
		dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
		ReleaseDC(NULL, hdc);
	}

	return dpiY;
}

/**
 * Windows 8/8.1の場合のプリセット値を設定する。
 */
void NoMeiryoUI::OnSet8(void)
{
	// DPIを取得する。
	int dpiY = getDPI();

	// フォント以外のNONCLIENTMETRICSの現在値を保持するため、
	// NONCLIENTMETRICSの内容を取得しなおす。
	FillMemory(&metrics,sizeof(NONCLIENTMETRICS),0x00);
	metrics.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metrics,
		0);

	memset(&metrics.lfCaptionFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfCaptionFont.lfFaceName, _T("Meiryo UI"));
	metrics.lfCaptionFont.lfHeight = MulDiv(-15,dpiY,96);
	metrics.lfCaptionFont.lfWeight = 400;
	metrics.lfCaptionFont.lfCharSet = 1;
	metrics.lfCaptionFont.lfQuality = 5;

	memset(&iconFont, 0, sizeof(LOGFONTW));
	_tcscpy(iconFont.lfFaceName, _T("Meiryo UI"));
	iconFont.lfHeight = MulDiv(-12, dpiY, 96);
	iconFont.lfWeight = 400;
	iconFont.lfCharSet = 1;
	iconFont.lfQuality = 5;

	memset(&metrics.lfSmCaptionFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfSmCaptionFont.lfFaceName, _T("Meiryo UI"));
	metrics.lfSmCaptionFont.lfHeight = MulDiv(-15, dpiY, 96);
	metrics.lfSmCaptionFont.lfWeight = 400;
	metrics.lfSmCaptionFont.lfCharSet = 1;
	metrics.lfSmCaptionFont.lfQuality = 5;

	memset(&metrics.lfStatusFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfStatusFont.lfFaceName, _T("Meiryo UI"));
	metrics.lfStatusFont.lfHeight = MulDiv(-12, dpiY, 96);
	metrics.lfStatusFont.lfWeight = 400;
	metrics.lfStatusFont.lfCharSet = 1;
	metrics.lfStatusFont.lfQuality = 5;

	memset(&metrics.lfMessageFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfMessageFont.lfFaceName, _T("Meiryo UI"));
	metrics.lfMessageFont.lfHeight = MulDiv(-12, dpiY, 96);
	metrics.lfMessageFont.lfWeight = 400;
	metrics.lfMessageFont.lfCharSet = 1;
	metrics.lfMessageFont.lfQuality = 5;

	memset(&metrics.lfMenuFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfMenuFont.lfFaceName, _T("Meiryo UI"));
	metrics.lfMenuFont.lfHeight = MulDiv(-12, dpiY, 96);
	metrics.lfMenuFont.lfWeight = 400;
	metrics.lfMenuFont.lfCharSet = 1;
	metrics.lfMenuFont.lfQuality = 5;

	// 表示を更新する。
	updateDisplay();

}

/**
 * Windows 10の場合のプリセット値を設定する。
 */
void NoMeiryoUI::OnSet10(void)
{
	// DPIを取得する。
	int dpiY = getDPI();

	// フォント以外のNONCLIENTMETRICSの現在値を保持するため、
	// NONCLIENTMETRICSの内容を取得しなおす。
	FillMemory(&metrics, sizeof(NONCLIENTMETRICS), 0x00);
	metrics.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metrics,
		0);

	memset(&metrics.lfCaptionFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfCaptionFont.lfFaceName, _T("Yu Gothic UI"));
	metrics.lfCaptionFont.lfHeight = MulDiv(-12, dpiY, 96);
	metrics.lfCaptionFont.lfWeight = 400;
	metrics.lfCaptionFont.lfCharSet = 1;
	metrics.lfCaptionFont.lfQuality = 5;

	memset(&iconFont, 0, sizeof(LOGFONTW));
	_tcscpy(iconFont.lfFaceName, _T("Yu Gothic UI"));
	iconFont.lfHeight = MulDiv(-12, dpiY, 96);
	iconFont.lfWeight = 400;
	iconFont.lfCharSet = 1;
	iconFont.lfQuality = 5;

	memset(&metrics.lfSmCaptionFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfSmCaptionFont.lfFaceName, _T("Yu Gothic UI"));
	metrics.lfSmCaptionFont.lfHeight = MulDiv(-12, dpiY, 96);
	metrics.lfSmCaptionFont.lfWeight = 400;
	metrics.lfSmCaptionFont.lfCharSet = 1;
	metrics.lfSmCaptionFont.lfQuality = 5;

	memset(&metrics.lfStatusFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfStatusFont.lfFaceName, _T("Yu Gothic UI"));
	metrics.lfStatusFont.lfHeight = MulDiv(-12, dpiY, 96);
	metrics.lfStatusFont.lfWeight = 400;
	metrics.lfStatusFont.lfCharSet = 1;
	metrics.lfStatusFont.lfQuality = 5;

	memset(&metrics.lfMessageFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfMessageFont.lfFaceName, _T("Yu Gothic UI"));
	metrics.lfMessageFont.lfHeight = MulDiv(-12, dpiY, 96);
	metrics.lfMessageFont.lfWeight = 400;
	metrics.lfMessageFont.lfCharSet = 1;
	metrics.lfMessageFont.lfQuality = 5;

	memset(&metrics.lfMenuFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfMenuFont.lfFaceName, _T("Yu Gothic UI"));
	metrics.lfMenuFont.lfHeight = MulDiv(-12, dpiY, 96);
	metrics.lfMenuFont.lfWeight = 400;
	metrics.lfMenuFont.lfCharSet = 1;
	metrics.lfMenuFont.lfQuality = 5;

	// 表示を更新する。
	updateDisplay();

}

NONCLIENTMETRICS *s_fontMetrics;

/**
 * スレッドでアイコン以外のフォントを設定する。
 *
 * @param p スレッドに渡すパラメータ(未使用)
 * @return 0
 */
unsigned _stdcall setOnThread(void *p)
{
	DWORD_PTR ptr;
	LRESULT messageResult;

	SystemParametersInfo(SPI_SETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		s_fontMetrics,
		SPIF_UPDATEINIFILE); // | SPIF_SENDCHANGE);

	return 0;
}


/**
 * 画面各部のフォントを設定する。
 *
 * @param fontMetrics アイコン以外のフォント指定用NONCLIENTMETRICS
 * @param iconLogFont アイコンのフォント
 */
void NoMeiryoUI::setFont(
	NONCLIENTMETRICS *fontMetrics,
	LOGFONT *iconLogFont
) {

	DWORD_PTR ptr;
	LRESULT messageResult;

	// アイコンのフォント設定
	SystemParametersInfo(SPI_SETICONTITLELOGFONT,
		sizeof(LOGFONT),
		iconLogFont,
		SPIF_UPDATEINIFILE); // | SPIF_SENDCHANGE);

	messageResult = SendMessageTimeout(
		HWND_BROADCAST,
		WM_SETTINGCHANGE,
		SPI_SETICONTITLELOGFONT,
		0, // (LPARAM)_T("Environment"),
		SMTO_ABORTIFHUNG,
		5000,
		&ptr);
	if (messageResult == 0) {
		if (GetLastError() == ERROR_TIMEOUT) {
			MessageBox(this->getHwnd(), 
				_T("SendMessage timeout."),
				_T("Information"),
				MB_OK | MB_ICONEXCLAMATION);
		}
	}

	// アイコン以外のフォント設定
	if (appMenu->isChecked(IDM_ANOTHER)) {
		// UIと別スレッドでSystemParametersInfo(SPI_SETNONCLIENTMETRICSを
		// 実行する。
		s_fontMetrics = fontMetrics;

		// フォント設定を実行するスレッドを開始する。
		uintptr_t startResult = _beginthreadex(NULL, 0, setOnThread, NULL, 0, NULL);
		if (startResult != 0) {
			// 正常にスレッドを開始したらスレッド終了を待機する。
			HANDLE handle;
			handle = (HANDLE)startResult;

			// 一応5秒ほど待つ
			DWORD waitResult = WaitForSingleObject(handle, 5000);
			if (waitResult == WAIT_TIMEOUT) {
				// スレッドが終了しない場合はどうしようもないのでスレッドを終了する。
				TerminateThread(handle, 0);
			}
			CloseHandle(handle);
		}
	} else {
		// UIと同じスレッドでSystemParametersInfo(SPI_SETNONCLIENTMETRICSを
		// 実行する。
		SystemParametersInfo(SPI_SETNONCLIENTMETRICS,
			sizeof(NONCLIENTMETRICS),
			fontMetrics,
			SPIF_UPDATEINIFILE); // | SPIF_SENDCHANGE);
	}

	messageResult = SendMessageTimeout(
		HWND_BROADCAST,
		WM_SETTINGCHANGE,
		SPI_SETNONCLIENTMETRICS,
		(LPARAM)_T("WindowMetrics"),
		SMTO_ABORTIFHUNG,
		5000,
		&ptr);
	if (messageResult == 0) {
		if (GetLastError() == ERROR_TIMEOUT) {
			MessageBox(this->getHwnd(), 
				_T("SendMessage timeout."),
				_T("Information"),
				MB_OK | MB_ICONEXCLAMATION);
		}
	}

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

}

/**
 * Windowsのバージョンを取得して、画面に設定する。
 */
void NoMeiryoUI::SetWinVer()
{
	// Windowsの内部バージョンを調べる。
	DWORD dwVersion = GetVersion();
	TCHAR buf[128];

	DWORD major = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	DWORD minor = (DWORD)(HIBYTE(LOWORD(dwVersion)));

	// サーバーかどうかの追加情報を取得するため
	// GetVersionExをOSVERSIONINFOEXを渡して呼び出す。
	// Windows 98/Me,NT4以前は考慮しないので呼び分けはなし。
	OSVERSIONINFOEX infoEx;
	memset(&infoEx, 0, sizeof(OSVERSIONINFOEX));
	infoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&infoEx);

	switch(major) {
		case 5:
			switch (minor) {
				case 0:
					_stprintf(buf,
						_T("Windows 2000 (%d.%d)"),
						major,minor);
					break;
				case 1:
					_stprintf(buf,
						_T("Windows XP (%d.%d)"),
						major,minor);
					break;
				case 2:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("Windows XP 64bit (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("Windows Server 2003 (%d.%d)"),
							major,minor);
					}
					break;
			}
			break;
		case 6:
			switch (minor) {
				case 0:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("Windows Vista (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("Windows Server 2008 (%d.%d)"),
							major,minor);
					}
					break;
				case 1:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("Windows 7 (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("Windows Server 2008 R2 (%d.%d)"),
							major,minor);
					}
					break;
				case 2:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("Windows 8 (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("Windows Server 2012 (%d.%d)"),
							major,minor);
					}
					break;
				case 3:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("Windows 8.1 (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("Windows Server 2012 R2 (%d.%d)"),
							major,minor);
					}
					break;
				default:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("Future Windows Client (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("Future Windows Server (%d.%d)"),
							major,minor);
					}
					break;
			}
			break;
		case 10:
			if (infoEx.wProductType == VER_NT_WORKSTATION) {
				_stprintf(buf,
					_T("Windows 10 (%d.%d)"),
					major,minor);
			} else {
				_stprintf(buf,
					_T("Windows Server 2016 (%d.%d)"),
					major,minor);
			}
			break;
		default:
			if (infoEx.wProductType == VER_NT_WORKSTATION) {
				_stprintf(buf,
					_T("Future Windows Client (%d.%d)"),
					major,minor);
			} else {
				_stprintf(buf,
					_T("Future Windows Server (%d.%d)"),
					major,minor);
			}
			break;
	}

	verInfo = GetDlgItem(IDC_STATIC_VERNO);
	verInfo->setText(buf);
}

/**
 * ドキュメントファイルを表示する。
 *
 */
void NoMeiryoUI::showHelp(void)
{
	// 実行ファイルの情報を得るためのバッファ群
	TCHAR path[_MAX_PATH+1],drive[_MAX_DRIVE+1],dir[_MAX_DIR+1],helpFile[_MAX_PATH+1];

	// 実行ファイルのあるところのBShelp.htmlのパス名を生成する。
	::GetModuleFileName(NULL,path,_MAX_PATH);
	::_tsplitpath(path,drive,dir,NULL,NULL);
	::_stprintf(helpFile,_T("%s%s%s"),drive,dir,_T("noMeiryoUI.html"));
	
	// 関連付けられたアプリでドキュメントファイルを表示する。
	ShellExecute(hWnd,_T("open"),helpFile,NULL,NULL,SW_SHOW);
}

