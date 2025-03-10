/*
noMeiryoUI (C) 2005,2012-2025 Tatsuhiko Shoji
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
#include <locale.h>
#include <mbctype.h>
#include <uxtheme.h>
#include "noMeiryoUI.h"
#include "FontSel.h"
#include "NCFileDialog.h"
#include "util.h"
#include "langresource.h"

#include "country\default.h"
#include "country\japan.h"
#include "country\korea.h"

//
// ダイアログベースアプリケーションフレームワークと
// ユーザー実装のブリッジルーチン
//

// アプリケーションオブジェクト
NoMeiryoUI *appObj;
static bool use7Compat = true;
bool has8Preset = true;
bool has10Preset = true;
bool has11Preset = true;
// 複数起動を許すか
bool multiRun = true;
TCHAR helpFileName[MAX_PATH];
RECT myMonitorLect;
bool firstMonitor = false;
DWORD helpPid;
bool helpMoved = false;
bool usePreset = false;
DWORD majorVersion;
DWORD minorVersion;

/**
 * アプリケーションオブジェクトを作成します。
 */
DialogAppliBase *createAppli()
{
	CoInitialize(NULL);

	initializeLocale();

	// ここでユーザーのアプリケーションオブジェクトを作成します。
	appObj = new NoMeiryoUI();

	return appObj;
}

/**
 * 各国語の判定と各国語に合わせた初期化を行います。
 */
void initializeLocale(void)
{
	TCHAR iniPath[MAX_PATH];
	TCHAR *p;
	TCHAR langFileName[MAX_PATH];

	::GetModuleFileName(NULL, iniPath, _MAX_PATH);
	p = _tcsrchr(iniPath, '\\');
	if (p != NULL) {
		*(p + 1) = '\0';
	}

	// ロケールの初期化
	char *localeName = setlocale(LC_ALL, "");

	setResourceFileName(langFileName, helpFileName, localeName, iniPath);
	// Language support routine ends here.

	readResourceFile(langFileName);

	int readResult = readFontResource8(langFileName);
	if (!readResult) {
		has8Preset = false;
	}

	readResult = readFontResource10(langFileName);
	if (!readResult) {
		has10Preset = false;
	}

	readResult = readFontResource11(langFileName);
	if (!readResult) {
		has11Preset = false;
	}

}

/**
 * リソースファイル名を設定する
 * 
 * @param langFileName 言語ファイル名
 * @param helpFileName ヘルプファイル名
 * @param localeName ロケール名
 * @param iniPath iniファイルのパス
 */
void setResourceFileName(TCHAR * langFileName, TCHAR * helpFileName, char*systemlocaleName, TCHAR *iniPath)
{
	TCHAR findPath[MAX_PATH];
	TCHAR langWork[LOCALE_NAME_MAX_LENGTH];

	if (systemlocaleName == NULL) {
		_tcscpy(langFileName, iniPath);
		_tcscat(langFileName, _T("default.lng"));

		_tcscpy(helpFileName, iniPath);
		_tcscpy(helpFileName, _T("default.chm"));

		return;
	}

	wchar_t localeName[LOCALE_NAME_MAX_LENGTH];

	LANGID langId = GetUserDefaultUILanguage();
	LCIDToLocaleName(langId, localeName, LOCALE_NAME_MAX_LENGTH, 0);
	wcscpy(langWork, localeName);


	wchar_t *codePageDelim = wcschr(localeName, '.');
	if (codePageDelim != NULL) {
		_setmbcp(_wtoi(codePageDelim + 1));
		codePage = _wtoi(codePageDelim + 1);
	}
	else {
		_setmbcp(_MB_CP_LOCALE);
	}

	// Language detection
	setLocationInternalCode(langWork);

	_tcscpy(findPath, iniPath);
	TCHAR *p = _tcsrchr(langWork, _T('.'));
	if (p != NULL) {
		*p = _T('\0');
	}

	_tcscat(findPath, langWork);
	_tcscat(findPath, _T(".lng"));
	WIN32_FIND_DATA fileInfo;

	HANDLE found = FindFirstFile(findPath, &fileInfo);
	if (found != INVALID_HANDLE_VALUE) {
		// 言語_地域形式のファイルがある場合
		_tcscpy(langFileName, findPath);

		_tcscpy(helpFileName, langWork);
		_tcscat(helpFileName, _T(".chm"));
	}
	else {
		_tcscpy(findPath, iniPath);
		p = _tcsrchr(langWork, _T('-'));
		if (p != NULL) {
			*p = _T('\0');
		}
		_tcscat(findPath, langWork);
		_tcscat(findPath, _T(".lng"));
		found = FindFirstFile(findPath, &fileInfo);
		if (found != INVALID_HANDLE_VALUE) {
			// 言語のファイルがある場合
			_tcscpy(langFileName, findPath);

			_tcscpy(helpFileName, langWork);
			_tcscat(helpFileName, _T(".chm"));
		}
		else {
			// 言語ファイルが存在しない場合
			_tcscpy(langFileName, iniPath);
			_tcscat(langFileName, _T("default.lng"));

			_tcscpy(helpFileName, _T("default.chm"));
		}
	}
}

/**
 * @brief Set internal lanuguage enumeration
 * @param langWork Locale name
*/
void setLocationInternalCode(TCHAR  langWork[85])
{
	if (wcsstr(langWork, L"ja-JP") != NULL) {
		runningCountry = Japan;
		return;
	}
	if (wcsstr(langWork, L"ko-KR") != NULL) {
		runningCountry = Korea;
		return;
	}
	runningCountry = NoCountry;
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
	firstShow = true;

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
	noTahoma = false;
	setOnStart = false;
	_tcscpy(settingFile, _T(""));
	verInfo = NULL;

	displayFont = NULL;
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

	usePreset = false;
	autosetDelay = 0;
	forceTitleFontSet = false;

	loadConfig();
	handleMultipleRun();

	// メジャーバージョンを取得する
	DWORD dwVersion = GetVersionForApp(majorVersion, minorVersion, buildNumber);
	// If Windows 11 Build 22621 Switch onにしようと思ったけど
	// 21H2のあとのInsiderから起きてたので21H2のあとは全部同じ扱い
	if (majorVersion == 11 && buildNumber > 22000) {
		compatLevel = 1;
	} else {
		compatLevel = 0;
	}

	if (majorVersion < 6) {
		// Windows XP or earlyer
		WIN8_SIZE = false;
		use7Compat = false;
	} else if (majorVersion == 6) {
		if (minorVersion < 2) {
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

	// オプションを取得する。
	getOption();

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
	if (titleFontButton != NULL) {
		delete titleFontButton;
	}

	if (displayFont != NULL) {
		DeleteObject(displayFont);
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

	// フォント情報取得用構造体の初期化
	FillMemory(&metrics, sizeof(NONCLIENTMETRICS), 0x00);
	FillMemory(&metricsAll, sizeof(NONCLIENTMETRICS), 0x00);
	FillMemory(&iconFont, sizeof(LOGFONT), 0x00);
	FillMemory(&iconFontAll, sizeof(LOGFONT), 0x00);

	// 現在のフォントと付随する画面各部の幅等の情報を取得しておく。
	getActualFont();

	// フォントを読み込む場合の処理
	if (settingFile[0] != _T('\0')) {

		BOOL loadResult = loadFontInfo(settingFile);
		if (loadResult) {
			// メニューフォントの情報をすべてのフォントの各フォントの情報にあてる。
			metricsAll.lfMenuFont = metrics.lfMenuFont;
			metricsAll.lfStatusFont = metricsAll.lfMenuFont;
			metricsAll.lfMessageFont = metricsAll.lfMenuFont;
			metricsAll.lfCaptionFont = metricsAll.lfMenuFont;
			metricsAll.lfSmCaptionFont = metricsAll.lfMenuFont;
			iconFontAll = metricsAll.lfMenuFont;
		} else {
			// 読み込み失敗時は念のため再度現在のフォント等の情報を
			// 取得しておく。
			getActualFont();
		}
	}
	return (INT_PTR)FALSE;
}

/**
 * ウインドウ生成完了時の処理
 *
 * @param wParam WPARAM
 * @param lParam lParam
 * @return 0:処理した 非0:処理しない
 */
INT_PTR NoMeiryoUI::OnWindowShown(WPARAM wParam, LPARAM lParam)
{
	DialogAppliBase::OnWindowShown(wParam, lParam);

	if (firstShow) {

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

		// 海外版は初期設定のフォントが異なるのでプリセットメニュー情報が
		// ある場合のみプリセットを有効にする。
		appMenu->setEnabled(IDM_SET_8, has8Preset);
		appMenu->setEnabled(IDM_SET_10, has10Preset);
		appMenu->setEnabled(IDM_SET_11, has11Preset);

		// 複数起動設定
		if (!multiRun) {
			appMenu->CheckMenuItem(IDM_NO_MULTI_RUN, true);
		}

		// 先発のOSではフォントがない場合があるので
		// 後発OS用のプリセットを使用不可とする。
		if (majorVersion < 10) {
			appMenu->setEnabled(IDM_SET_10, false);
		}
		if (majorVersion < 11) {
			appMenu->setEnabled(IDM_SET_11, false);
		}

		// UI文字列をリソースに合わせて変更する。
		applyResource();

		if (setOnStart) {

			if (autosetDelay > 0) {
				Sleep(autosetDelay * 1000);
			}

			// -setオプションが指定された場合はフォントを設定してダイアログを閉じる。
			if (compatLevel > 0) {
				// Windows 11 22H2以降の場合、タイトルを元の物に変更する
				set11TitlePreset(&metrics);
			}

			// フォント変更を実施する。
			setFont(&metrics, &iconFont, true);

			ExitProcess(0);

			return (INT_PTR)TRUE;
		}

		// メインダイアログのバージョン表記設定
		TCHAR buf[64];
		TCHAR verString[32];
		const TCHAR *appName;
		LoadString(hInst, IDS_VERSION, verString, 32);
		appName = langResource[1].c_str();
		_stprintf(buf, verString, appName);
		setChildText(IDC_STATIC_APP_TITLE, buf);

		// フォント名表示を更新する。
		updateDisplay();

		EnumDisplayMonitors(NULL, NULL, MonitorNearMouseCallback, 0);

		adjustCenter(myMonitorLect, HWND_TOP, this->hWnd);

		adjustWindowSize();

		firstShow = false;
	}

	return (INT_PTR)0;
}

/**
 * ダイアログが表示された後に行う処理
 *
 * @param wParam WPARAM
 * @param lParam lParam
 * @return 0:処理した 非0:処理しない
 */
INT_PTR NoMeiryoUI::OnWindowCreated(WPARAM wParam, LPARAM lParam)
{
	DialogAppliBase::OnWindowCreated(wParam, lParam);

	if (compatLevel > 0) {
		titleFontButton->EnableWindow(FALSE);
		appMenu->setEnabled(IDM_FORCE_TITLE_SET, true);

		// ワーニングメッセージ in Win11 22H2
		MessageBox(this->getHwnd(), langResource[MSG_WIN11_22H2RESTRICTION].c_str(),
			langResource[MSG_WARNING].c_str(), MB_OK | MB_ICONWARNING);
		//MessageBox(this->getHwnd(), _T("Windows 11のバカヤロー"),
		//	_T("何じゃぁこりゃぁ"), MB_OK | MB_ICONWARNING);
	} else {
		appMenu->setEnabled(IDM_FORCE_TITLE_SET, false);
	}

	return (INT_PTR)0;
}


/**
 * カーソルのいるモニターを判定するためのEnumDisplayMonitorsのコールバック
 *
 * @param hMonitor モニターのハンドル
 * @param hdcMonitor モニターのディスプレイコンテキスト
 * @param lprcMonitor モニターの座標情報
 * @param dwData EnumDisplayMonitors
 * @return TRUE:列挙を続ける FALSE:列挙をやめ、モニターの座標情報を確定させる
 */
BOOL CALLBACK MonitorNearMouseCallback(
	HMONITOR hMonitor,
	HDC hdcMonitor,
	LPRECT lprcMonitor,
	LPARAM dwData
)
{
	if (!firstMonitor) {
		// ディスプレイの情報が何もない状態は避ける。
		myMonitorLect = *lprcMonitor;
		firstMonitor = true;
	}

	CURSORINFO cursofInfo;
	cursofInfo.cbSize = sizeof(CURSORINFO);
	BOOL result = GetCursorInfo(&cursofInfo);
	if (result == 0) {
		// カーソルの情報を利用できないときはプライマリモニタを
		// カーソルのいるモニタ扱いとする。
		if ((myMonitorLect.left == 0) && (myMonitorLect.top == 0)) {
			myMonitorLect = *lprcMonitor;
			return FALSE;
		} else {
			return TRUE;
		}
	}

	// カーソルのいるモニタかどうか判定する。
	int x = cursofInfo.ptScreenPos.x;
	int y = cursofInfo.ptScreenPos.y;
	if ((x >= lprcMonitor->left) && (x <= lprcMonitor->right)) {
		if ((y >= lprcMonitor->top) && (y <= lprcMonitor->bottom)) {
			myMonitorLect = *lprcMonitor;

			return FALSE;
		}
	}
	return TRUE;
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

	// テキストボックス制御用にダイアログの各テキストボックスを取得する。
	allFontTextBox = GetDlgItem(IDC_EDIT_ALL);
	titleFontTextBox = GetDlgItem(IDC_EDIT_TITLE);
	iconFontTextBox = GetDlgItem(IDC_EDIT_ICON);
	paletteFontTextBox = GetDlgItem(IDC_EDIT_PALETTE);
	hintFontTextBox = GetDlgItem(IDC_EDIT_HINT);
	messageFontTextBox = GetDlgItem(IDC_EDIT_MESSAGE);
	menuFontTextBox = GetDlgItem(IDC_EDIT_MENU);
	titleFontButton = GetDlgItem(ID_SEL_TITLE);

	return 0;
}

/**
 * @brief ウインドウサイズを調整する
 */
void NoMeiryoUI::adjustWindowSize(void)
{
	RECT r;

	GetClientRect(getHwnd(), &r);

	NONCLIENTMETRICS nowMetrics;

	nowMetrics.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&nowMetrics,
		0);

	HDC dc = GetDC(getHwnd());

	int logPixelY = GetDeviceCaps(dc, LOGPIXELSY);
	double scale = (double)logPixelY / 96;

	int width;
	int height;

	width = 740 * scale + nowMetrics.iBorderWidth * 2;
	height = 410 * scale +
		nowMetrics.iCaptionHeight +
		nowMetrics.iMenuHeight +
		nowMetrics.iBorderWidth * 2;

	RECT nowRect;
	GetWindowRect(getHwnd(), &nowRect);

	RECT newRect;
	SetWindowPos(
		getHwnd(),
		HWND_TOP,
		nowRect.left,
		nowRect.top,
		width,
		height,
		SWP_SHOWWINDOW);
}

/**
 * コマンドラインオプションを取得する。
 */
void NoMeiryoUI::getOption()
{

	std::vector<tstring *> *argv = extractArguments();

	parseOption(argv);

	deleteArguments(argv);
}

/**
 * オプションの位置に応じた解析を行う。
 *
 * @param param パラメータ
 * @param argCount オプションの個数
 */
void NoMeiryoUI::parseOption(std::vector<tstring *> *param)
{
	if (param->size() < 2) {
		return;
	}

	if (_tcscmp(_T("--"), (*param)[1]->c_str())) {
		if ((_tcschr((*param)[1]->c_str(), _T('\\')) == NULL) && (_tcschr((*param)[1]->c_str(), _T('/')) == NULL)) {
			GetCurrentDirectory(MAX_PATH, settingFile);
			_tcscat(settingFile, _T("\\"));
		}
		else {
			settingFile[0] = _T('\0');
		}
		_tcscat(settingFile, (*param)[1]->c_str());
	}

	if (param->size() < 3) {
		return;
	}

	for (int i = 2; i < param->size(); i++) {
		if (!_tcscmp((*param)[i]->c_str(), _T("-set"))) {
			setOnStart = true;
		} else if (!_tcscmp((*param)[i]->c_str(), _T("noMeiryoUI"))) {
			noMeiryoUI = true;
		} else if (!_tcscmp((*param)[i]->c_str(), _T("noTahoma"))) {
			noTahoma = true;
		} else if ((*param)[i]->compare(_T("delay"))) {
			if ((i + 1) < param->size()) {
				autosetDelay = _ttoi((*param)[i + 1]->c_str());
				i++;
			}
		}

	}
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

	if (metrics.lfCaptionFont.lfHeight > 0) {
		metrics.lfCaptionFont.lfHeight = 0 - metrics.lfCaptionFont.lfHeight;
	}
	if (metrics.lfSmCaptionFont.lfHeight > 0) {
		metrics.lfSmCaptionFont.lfHeight = 0 - metrics.lfSmCaptionFont.lfHeight;
	}
	if (metrics.lfStatusFont.lfHeight > 0) {
		metrics.lfStatusFont.lfHeight = 0 - metrics.lfStatusFont.lfHeight;
	}
	if (metrics.lfMessageFont.lfHeight > 0) {
		metrics.lfMessageFont.lfHeight = 0 - metrics.lfMessageFont.lfHeight;
	}
	if (metrics.lfMenuFont.lfHeight > 0) {
		metrics.lfMenuFont.lfHeight = 0 - metrics.lfMenuFont.lfHeight;
	}
	if (iconFont.lfHeight > 0) {
		iconFont.lfHeight = 0 - iconFont.lfHeight;
	}

	fontPoints.title = getFontPointInt(&(metrics.lfCaptionFont), this->getHwnd());
	fontPoints.palette = getFontPointInt(&(metrics.lfSmCaptionFont), this->getHwnd());
	fontPoints.hint = getFontPointInt(&(metrics.lfStatusFont), this->getHwnd());
	fontPoints.message = getFontPointInt(&(metrics.lfMessageFont), this->getHwnd());
	fontPoints.menu = getFontPointInt(&(metrics.lfMenuFont), this->getHwnd());
	fontPoints.icon = getFontPointInt(&iconFont, this->getHwnd());

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
		-MulDiv(APP_FONTSIZE, GetDeviceCaps(hDC, LOGPIXELSY), 72),
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
 * リソースを各項目に設定する。
 */
void NoMeiryoUI::applyResource()
{
	tstring font = langResource[0];

	// Get fallback font
	font = getLanguageFallbackForCountry(langResource[0]);

	applyDisplayText();

	applyDisplayFont();
}


/**
 * リソースを各項目に設定する(文字列)。
 */
void NoMeiryoUI::applyDisplayText()
{
	// アプリタイトル
	setText(langResource[1].c_str());

	// メニュー文字列変更
	appMenu->setText(0, langResource[2].c_str(), TRUE);
	appMenu->setText(IDM_OPEN, langResource[3].c_str(), FALSE);
	appMenu->setText(IDM_SAVE, langResource[4].c_str(), FALSE);
	appMenu->setText(IDOK, langResource[5].c_str(), FALSE);
	appMenu->setText(IDM_EXIT, langResource[6].c_str(), FALSE);
	appMenu->setText(1, langResource[7].c_str(), TRUE);
	appMenu->setText(IDM_SET_8, langResource[8].c_str(), FALSE);
	appMenu->setText(IDM_SET_10, langResource[9].c_str(), FALSE);
	appMenu->setText(2, langResource[10].c_str(), TRUE);
	appMenu->setText(IDM_CHOICE_APP_FONT, langResource[MENU_CHOICE_APP_FONT].c_str(), FALSE);
	appMenu->setText(IDM_NO_MULTI_RUN, langResource[MENU_DONT_RUN_MULTIPLY].c_str(), FALSE);
	appMenu->setText(IDM_FORCE_TITLE_SET, langResource[MENU_FORCE_TITLE_SET].c_str(), FALSE);
	appMenu->setText(IDM_ANOTHER, langResource[MENU_TOOLS_THREAD].c_str(), FALSE);
	appMenu->setText(IDM_COMPAT7, langResource[MENU_TOOLS_SEVEN].c_str(), FALSE);
	appMenu->setText(3, langResource[13].c_str(), TRUE);
	appMenu->setText(IDM_HELPTOPIC, langResource[14].c_str(), FALSE);
	appMenu->setText(IDM_ABOUT, langResource[15].c_str(), FALSE);

	setChildText(IDC_STATIC_ALL_FONT, langResource[16].c_str());
	setChildText(IDC_STATIC_TITLE_BAR, langResource[17].c_str());
	setChildText(IDC_STATIC_ICON, langResource[18].c_str());
	setChildText(IDC_STATIC_PALETTE_TITLE, langResource[19].c_str());
	setChildText(IDC_STATIC_HINT, langResource[20].c_str());
	setChildText(IDC_STATIC_MESSAGE, langResource[21].c_str());
	setChildText(IDC_STATIC_MENU, langResource[22].c_str());
	setChildText(ID_SEL_ALL, langResource[23].c_str());
	setChildText(ID_SEL_TITLE, langResource[23].c_str());
	setChildText(ID_SEL_ICON, langResource[23].c_str());
	setChildText(ID_SEL_PALETTE, langResource[23].c_str());
	setChildText(ID_SEL_HINT, langResource[23].c_str());
	setChildText(ID_SEL_MESSAGE, langResource[23].c_str());
	setChildText(ID_SEL_MENU, langResource[23].c_str());
	setChildText(ID_SET_ALL, langResource[24].c_str());
	setChildText(IDOK, langResource[25].c_str());
	setChildText(IDCANCEL, langResource[26].c_str());
	setChildText(ID_APPLY, langResource[71].c_str());
	setChildText(ID_APPLY_ALL, langResource[71].c_str());
	setChildText(IDC_GROUP_ALL, langResource[72].c_str());
	setChildText(IDC_GROUP_INDIVIDUAL, langResource[73].c_str());
}

/**
 * リソースを各項目に設定する(フォント)。
 */
void NoMeiryoUI::applyDisplayFont()
{
	HDC hDC = GetDC(this->hWnd);

	tstring font = langResource[0];

	HFONT displayFont = CreateFont(
		-MulDiv(APP_FONTSIZE, GetDeviceCaps(hDC, LOGPIXELSY), 72),
		0,
		0,
		0,
		FW_NORMAL,
		FALSE,
		FALSE,
		FALSE,
		_tstoi(langResource[70].c_str()),
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, // CLEARTYPE_QUALITY,
		FIXED_PITCH | FF_MODERN,
		font.c_str());

	ReleaseDC(this->hWnd, hDC);

	setChildFont(IDC_STATIC_ALL_FONT, displayFont);
	setChildFont(IDC_STATIC_TITLE_BAR, displayFont);
	setChildFont(IDC_STATIC_ICON, displayFont);
	setChildFont(IDC_STATIC_PALETTE_TITLE, displayFont);
	setChildFont(IDC_STATIC_HINT, displayFont);
	setChildFont(IDC_STATIC_MESSAGE, displayFont);
	setChildFont(IDC_STATIC_MENU, displayFont);

	setChildFont(ID_SEL_ALL, displayFont);
	setChildFont(ID_SEL_TITLE, displayFont);
	setChildFont(ID_SEL_ICON, displayFont);
	setChildFont(ID_SEL_PALETTE, displayFont);
	setChildFont(ID_SEL_HINT, displayFont);
	setChildFont(ID_SEL_MESSAGE, displayFont);
	setChildFont(ID_SEL_MENU, displayFont);
	setChildFont(ID_SET_ALL, displayFont);
	setChildFont(IDOK, displayFont);
	setChildFont(IDCANCEL, displayFont);

	setChildFont(ID_APPLY, displayFont);
	setChildFont(ID_APPLY_ALL, displayFont);

	setChildFont(IDC_GROUP_ALL, displayFont);

	setChildFont(IDC_GROUP_INDIVIDUAL, displayFont);

	setChildFont(IDC_STATIC_APP_TITLE, displayFont);

	setChildFont(IDC_STATIC_VERNO, displayFont);
	setChildFont(IDC_STATIC_AUTHOR, displayFont);

}

/**
 * @brief Get fallback font when font settings in language file didn't exist in Windows(For example older Windows)
 * @param settingString Application display font name in language file.
 * @return Font name adjusted by font installed in Windows.
*/
tstring NoMeiryoUI::getLanguageFallbackForCountry(tstring &settingString)
{
	tstring font;

	switch (runningCountry) {
		case Japan:
			font = japan::getJapaneseFontFallback(langResource[0]);
			break;
		default:
			font = default::getDefaultFontFallback(langResource[0]);
			break;
	}

	return font;
}

/**
 * フォント表示を更新する。
 */
void NoMeiryoUI::updateDisplay(void)
{
	// フォント名、ポイント数表示文字列を作成する。
	int point;
	TCHAR buf[16];
	TCHAR dispName[32];

	allFontName = metricsAll.lfMenuFont.lfFaceName;
	if (runningCountry == Korea) {
		_tcscpy(dispName, allFontName.c_str());
		korea::getKoreanFontName(dispName);
		allFontName = dispName;
	}
	point = getFontPointInt(&(metricsAll.lfMenuFont), this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	allFontName = allFontName + buf;

	titleFontName = metrics.lfCaptionFont.lfFaceName;
	if (runningCountry == Korea) {
		_tcscpy(dispName, titleFontName.c_str());
		korea::getKoreanFontName(dispName);
		titleFontName = dispName;
	}
	point = getFontPointInt(&(metrics.lfCaptionFont), this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	titleFontName = titleFontName + buf;

	iconFontName = iconFont.lfFaceName;
	if (runningCountry == Korea) {
		_tcscpy(dispName, iconFontName.c_str());
		korea::getKoreanFontName(dispName);
		iconFontName = dispName;
	}
	point = getFontPointInt(&iconFont, this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	iconFontName = iconFontName + buf;

	paletteFontName = metrics.lfSmCaptionFont.lfFaceName;
	if (runningCountry == Korea) {
		_tcscpy(dispName, paletteFontName.c_str());
		korea::getKoreanFontName(dispName);
		paletteFontName = dispName;
	}
	point = getFontPointInt(&metrics.lfSmCaptionFont, this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	paletteFontName = paletteFontName + buf;

	hintFontName = metrics.lfStatusFont.lfFaceName;
	if (runningCountry == Korea) {
		_tcscpy(dispName, hintFontName.c_str());
		korea::getKoreanFontName(dispName);
		hintFontName = dispName;
	}
	point = getFontPointInt(&metrics.lfStatusFont, this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	hintFontName = hintFontName + buf;

	messageFontName = metrics.lfMessageFont.lfFaceName;
	if (runningCountry == Korea) {
		_tcscpy(dispName, messageFontName.c_str());
		korea::getKoreanFontName(dispName);
		messageFontName = dispName;
	}
	point = getFontPointInt(&metrics.lfMessageFont, this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	messageFontName = messageFontName + buf;

	// メニューと選択項目
	menuFontName = metrics.lfMenuFont.lfFaceName;
	if (runningCountry == Korea) {
		_tcscpy(dispName, menuFontName.c_str());
		korea::getKoreanFontName(dispName);
		menuFontName = dispName;
	}
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
			EndDialog(hWnd, LOWORD(wParam));
			return BaseDialog::OnCommand(wParam);
		case ID_APPLY_ALL:
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
		case ID_APPLY:
			result = OnBnClickedOk();
			if (!result) {
				return (INT_PTR)0;
			}
			break;
		case IDM_SET_8:
			OnSet8();
			return (INT_PTR)0;
		case IDM_SET_10:
			OnSet10();
			return (INT_PTR)0;
		case IDM_SET_11:
			OnSet11();
			return (INT_PTR)0;
		case IDM_CHOICE_APP_FONT:
			OnChoiceAppFont();
			return (INT_PTR)0;
		case IDM_ANOTHER:
			if (appMenu->isChecked(IDM_ANOTHER)) {
				appMenu->CheckMenuItem(IDM_ANOTHER, false);
			} else {
				appMenu->CheckMenuItem(IDM_ANOTHER, true);
			}
			return (INT_PTR)0;
		case IDM_COMPAT7:
			toggleWin7aliculate();

			return (INT_PTR)0;
		case IDM_NO_MULTI_RUN:
			toggleMultiRun();

			return (INT_PTR)0;
		case IDM_FORCE_TITLE_SET:
			toggleForceTitleSet();
			return (INT_PTR)0;
		case IDM_HELPTOPIC:
			showHelp();
			return (INT_PTR)0;
		case IDM_ABOUT:
			showVersion();
			return (INT_PTR)0;
	}
	return BaseDialog::OnCommand(wParam);

}

/**
 * @brief Windows 7式計算のトグル処理
 */
void NoMeiryoUI::toggleWin7aliculate()
{
	if (appMenu->isChecked(IDM_COMPAT7)) {
		appMenu->CheckMenuItem(IDM_COMPAT7, false);
		WIN8_SIZE = true;
	}
	else {
		appMenu->CheckMenuItem(IDM_COMPAT7, true);
		WIN8_SIZE = false;
	}
	updateDisplay();
}

/**
 * @brief 複数起動フラグ切り替え
 */
void NoMeiryoUI::toggleMultiRun()
{
	if (appMenu->isChecked(IDM_NO_MULTI_RUN)) {
		appMenu->CheckMenuItem(IDM_NO_MULTI_RUN, false);
		multiRun = true;
	}
	else {
		appMenu->CheckMenuItem(IDM_NO_MULTI_RUN, true);
		multiRun = false;
		saveConfig();
	}
	saveConfig();
}

/**
 *  @brief Windows 11 22H2以降での強制フォント設定切り替え
 */
void NoMeiryoUI::toggleForceTitleSet()
{
	if (appMenu->isChecked(IDM_FORCE_TITLE_SET)) {
		appMenu->CheckMenuItem(IDM_FORCE_TITLE_SET, false);
		forceTitleFontSet = false;
		if (compatLevel > 0) {
			titleFontButton->EnableWindow(FALSE);
		}
	} else {
		appMenu->CheckMenuItem(IDM_FORCE_TITLE_SET, true);
		forceTitleFontSet = true;
		if (compatLevel > 0) {
			titleFontButton->EnableWindow(TRUE);
			MessageBox(this->getHwnd(), langResource[MSG_FORCE_TITLE_SET].c_str(),
				langResource[MSG_WARNING].c_str(), MB_OK | MB_ICONWARNING);
		}
	}
}



/**
 * フォントを選択する。
 *
 * @param type 設定するフォントの種類
 */
void NoMeiryoUI::selectFont(enum fontType type)
{

	INT_PTR result;
	LOGFONT logfont;	// 取得したフォントの情報を入れる構造体

	FillMemory(&logfont,sizeof(LOGFONT),0x00);
	double points;

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

		FontSel *selector = new FontSel(this->hWnd, IDD_DIALOG_FONTSEL);
		if (noMeiryoUI) {
			selector->setNoMeiryoUI();
		}
		if (noTahoma) {
			selector->setNoTahoma();
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
		points = selector->getPoint();

		delete []selector;
	} catch (...) {
		MessageBox(this->hWnd,
			_T("Internal error in font selection dialog."),
			_T("Error"),
			MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	usePreset = false;

	switch (type) {
		case all:
			metricsAll.lfMenuFont = logfont;
			metricsAll.lfStatusFont = logfont;
			metricsAll.lfMessageFont = logfont;
			// Silently ignore on Win11 22H2
			if ((compatLevel < 1) || (forceTitleFontSet)) {
				metricsAll.lfCaptionFont = logfont;
			} else {
				set11TitlePreset(&metricsAll);
			}

			metricsAll.lfSmCaptionFont = logfont;
			iconFontAll = logfont;

			allFontName = logfont.lfFaceName;

			DeleteObject(allFont);
			allFont = createFont(&metricsAll.lfMenuFont);
			allFontTextBox->setFont(allFont);

			if (compatLevel < 1) {
				fontPoints.title = points;
			}
			fontPoints.palette = points;
			fontPoints.icon = points;
			fontPoints.hint = points;
			fontPoints.menu = points;
			fontPoints.message = points;

			break;

		case title:
			// Silently ignore on Win11 22H2
			if ((compatLevel < 1) || (forceTitleFontSet)) {
				metrics.lfCaptionFont = logfont;
				titleFontName = logfont.lfFaceName;

				DeleteObject(titleFont);
				titleFont = createFont(&metrics.lfCaptionFont);
				titleFontTextBox->setFont(titleFont);
				fontPoints.title = points;
			}
			break;

		case icon:
			iconFont = logfont;
			iconFontName = logfont.lfFaceName;

			DeleteObject(iconFontHandle);
			iconFontHandle = createFont(&iconFont);
			iconFontTextBox->setFont(iconFontHandle);
			fontPoints.icon = points;
			break;

		case palette:
			metrics.lfSmCaptionFont = logfont;
			paletteFontName = logfont.lfFaceName;

			DeleteObject(paletteFont);
			paletteFont = createFont(&metrics.lfSmCaptionFont);
			paletteFontTextBox->setFont(paletteFont);
			fontPoints.palette = points;
			break;

		case hint:
			metrics.lfStatusFont = logfont;
			hintFontName = logfont.lfFaceName;

			DeleteObject(hintFont);
			hintFont = createFont(&metrics.lfStatusFont);
			hintFontTextBox->setFont(hintFont);
			fontPoints.hint = points;
			break;

		case message:
			metrics.lfMessageFont = logfont;
			messageFontName = logfont.lfFaceName;

			DeleteObject(messageFont);
			messageFont = createFont(&metrics.lfMessageFont);
			messageFontTextBox->setFont(messageFont);
			fontPoints.message = points;
			break;

		case menu:
			// メニューと選択項目
			metrics.lfMenuFont = logfont;
			menuFontName = logfont.lfFaceName;

			DeleteObject(menuFont);
			menuFont = createFont(&metrics.lfMenuFont);
			menuFontTextBox->setFont(menuFont);
			fontPoints.menu = points;
			break;
	}

	// 表示を更新する。
	updateDisplay();
}

/**
 * ファイルダイアログ用のファイルマスク文字列作成
 *
 * @param ファイルマスク文字列バッファ
 * @param ファイルの種類名
 * @param ファイルのマスク
 * @param すべてのファイルの種類名
 * @param すべてのファイルのマスク
 */
void setFileMask(
	TCHAR *buf,
	const TCHAR *fileMsg,
	const TCHAR *fileExt,
	const TCHAR *allMsg,
	const TCHAR *allExt)
{
	TCHAR *pDst = buf;
	int len;

	len = _tcslen(fileMsg);
	for (int i = 0; i < len; i++) {
		*pDst = fileMsg[i];
		pDst++;
	}
	*pDst = _T('\0');
	pDst++;

	len = _tcslen(fileExt);
	for (int i = 0; i < len; i++) {
		*pDst = fileExt[i];
		pDst++;
	}
	*pDst = _T('\0');
	pDst++;

	len = _tcslen(allMsg);
	for (int i = 0; i < len; i++) {
		*pDst = allMsg[i];
		pDst++;
	}
	*pDst = _T('\0');
	pDst++;

	len = _tcslen(allExt);
	for (int i = 0; i < len; i++) {
		*pDst = allExt[i];
		pDst++;
	}
	*pDst = _T('\0');
	pDst++;

	*pDst = _T('\0');
	pDst++;
}


/**
 * フォント設定を保存するを選択した時の動作
 */
void NoMeiryoUI::OnLoad()
{
	TCHAR buf[128];

	setFileMask(
		buf,
		langResource[59].c_str(),
		_T("*.ini"),
		langResource[60].c_str(),
		_T("*.*")
	);

	NCFileDialog *dlg = new NCFileDialog(
		TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		buf,
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
		const TCHAR *message;
		const TCHAR *title;

		message = langResource[61].c_str();
		title = langResource[63].c_str();
		MessageBox(
			this->getHwnd(),
			message,
			title,
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

	if (compatLevel < 1) {
		loadResult = loadFont(filename, _T("TitleFont"), &captionFont);
		if (!loadResult) {
			return FALSE;
		}
		metrics.lfCaptionFont = captionFont;
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

	iconFont = newIconFont;
	metrics.lfSmCaptionFont = smCaptionFont;
	metrics.lfStatusFont = statusFont;
	metrics.lfMessageFont = messageFont;
	metrics.lfMenuFont = menuFont;

	int dpi = getSystemDPI();

	fontPoints.title = getFontPointDouble(&(metrics.lfCaptionFont), this->getHwnd());
	fontPoints.palette = getFontPointDouble(&(metrics.lfSmCaptionFont), this->getHwnd());
	fontPoints.hint = getFontPointDouble(&(metrics.lfStatusFont), this->getHwnd());
	fontPoints.message = getFontPointDouble(&(metrics.lfMessageFont), this->getHwnd());
	fontPoints.menu = getFontPointDouble(&(metrics.lfMenuFont), this->getHwnd());
	fontPoints.icon = getFontPointDouble(&iconFont, this->getHwnd());

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

	int dpi = getSystemDPI();

	result = GetPrivateProfileString(section,
		_T("Height"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	double height = _wtof(buf);
	if (height < 0) {
		// Ver.2
		font->lfHeight = height;
	} else {
		// Ver.3
		font->lfHeight = -MulDiv(height, dpi, 72);
	}

	result = GetPrivateProfileString(section,
		_T("Width"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	double width = _wtof(buf);
	if (height < 0) {
		// Ver.2
		font->lfWidth = width;
	} else {
		// Ver.3
		font->lfWidth = -MulDiv(width, dpi, 72);
	}

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
	TCHAR buf[128];

	setFileMask(
		buf,
		langResource[59].c_str(),
		_T("*.ini"),
		langResource[60].c_str(),
		_T("*.*")
	);

	NCFileDialog *dlg = new NCFileDialog(
		FALSE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		buf,
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
		const TCHAR *message;
		const TCHAR *title;

		message = langResource[62].c_str();
		title = langResource[63].c_str();

		MessageBox(
			this->getHwnd(),
			message,
			title,
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

	saveResult = saveFont(
		filename, _T("TitleFont"), &metrics.lfCaptionFont, fontPoints.title);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(
		filename, _T("IconFont"), &iconFont, fontPoints.icon);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(
		filename, _T("PaletteFont"), &metrics.lfSmCaptionFont, fontPoints.palette);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(
		filename, _T("HintFont"), &metrics.lfStatusFont, fontPoints.hint);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(
		filename, _T("MessageFont"), &metrics.lfMessageFont, fontPoints.message);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(
		filename, _T("MenuFont"), &metrics.lfMenuFont, fontPoints.menu);
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
 * @param point フォントサイズ(ポイント)
 * @result TRUE:保存成功 FALSE:保存失敗
 */
BOOL NoMeiryoUI::saveFont(TCHAR *filename, TCHAR *section, LOGFONT *font, double point)
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

	_stprintf(buf, _T("%f"), point);
	result = WritePrivateProfileString(section,
		_T("Height"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%f"), 0);
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
#if 0
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
#endif
	if ((compatLevel > 0) && (forceTitleFontSet == false)){
		// Windows 11 22H2以降の場合、タイトルを元の物に変更する
		set11TitlePreset(&metrics);
	}

	// フォント変更を実施する。
	setFont(&metrics, &iconFont, appMenu->isChecked(IDM_ANOTHER));

	adjustWindowSize();
	// COLORREF ref = GetThemeSysColor(NULL, COLOR_ACTIVECAPTION);

	return (INT_PTR)TRUE;
}

/**
 * 一括設定ボタン押下時の動作(すべてのフォントで選択したフォントを設定する。)
 *
 */
void NoMeiryoUI::OnBnClickedAll()
{
#if 0
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
#endif
	if ((compatLevel > 0) && (forceTitleFontSet == false)) {
		set11TitlePreset(&metricsAll);
	}

	// フォント変更を実施する。
	setFont(&metricsAll, &iconFontAll, appMenu->isChecked(IDM_ANOTHER));

	adjustWindowSize();

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

	setPreset(
		&metrics,
		&iconFont,
		fontFaces8,
		fontSizes8,
		fontCharset8,
		Win8PresetWindowsMetric,
		dpiY);

	setPreset(
		&metricsAll,
		&iconFontAll,
		fontFaces8,
		fontSizes8,
		fontCharset8,
		Win8PresetWindowsMetric,
		dpiY);


	// 表示を更新する。
	updateDisplay();

	usePreset = true;

}

/**
 * Windows 10の場合のプリセット値を設定する。
 */
void NoMeiryoUI::OnSet10(void)
{
	// DPIを取得する。
	int dpiY = getDPI();

	setPreset(
		&metrics,
		&iconFont,
		fontFaces10,
		fontSizes10,
		fontCharset10,
		Win10PresetWindowsMetric,
		dpiY);

	setPreset(
		&metricsAll,
		&iconFontAll,
		fontFaces10,
		fontSizes10,
		fontCharset10,
		Win10PresetWindowsMetric,
		dpiY);

	// 表示を更新する。
	updateDisplay();

	usePreset = true;

}

/**
 * Windows 11のタイトルのプリセットだけを設定する。<br>
 * Windows 11 22H2用
 * 
 * @param metrics 設定するNONCLIENTMETRICS
 */
void NoMeiryoUI::set11TitlePreset(NONCLIENTMETRICS *metrics)
{
	// DPIを取得する。
	int dpiY = getDPI();

	memset(&((*metrics).lfCaptionFont), 0, sizeof(LOGFONTW));
	_tcscpy((*metrics).lfCaptionFont.lfFaceName, fontFaces11[0].c_str());
	(*metrics).lfCaptionFont.lfHeight = -MulDiv(fontSizes11[0], dpiY, 72);
	(*metrics).lfCaptionFont.lfWeight = 400;
	(*metrics).lfCaptionFont.lfCharSet = fontCharset11[0];
	(*metrics).lfCaptionFont.lfQuality = 5;
}

/**
 * Windows 11の場合のプリセット値を設定する。
 */
void NoMeiryoUI::OnSet11(void)
{
	// DPIを取得する。
	int dpiY = getDPI();

	setPreset(
		&metrics,
		&iconFont,
		fontFaces11,
		fontSizes11,
		fontCharset11,
		Win11PresetWindowsMetric,
		dpiY);

	setPreset(
		&metricsAll,
		&iconFontAll,
		fontFaces11,
		fontSizes11,
		fontCharset11,
		Win11PresetWindowsMetric,
		dpiY);


	// 表示を更新する。
	updateDisplay();

	usePreset = true;

}

/**
 * アプリケーションフォントを選択する。
 */
void NoMeiryoUI::OnChoiceAppFont()
{

	INT_PTR result;
	LOGFONT logfont;	// 取得したフォントの情報を入れる構造体

	FillMemory(&logfont, sizeof(LOGFONT), 0x00);

	try {

		FontSel *selector = new FontSel(this->hWnd, IDD_DIALOG_FONTSEL, TRUE);
		if (noMeiryoUI) {
			selector->setNoMeiryoUI();
		}
		if (noTahoma) {
			selector->setNoTahoma();
		}
		// 選択していたフォントをフォント選択ダイアログに設定する。
		selector->setPreviousFont(&logfont);

		result = selector->showModal();
		if (result != IDOK) {
			delete[]selector;
			return;
		}
		logfont = selector->getSelectedFont();
		if (logfont.lfFaceName[0] == _T('\0')) {
			delete[]selector;
			return;
		}

		delete[]selector;
	}
	catch (...) {
		MessageBox(this->hWnd,
			_T("Internal error in font selection dialog."),
			_T("Error"),
			MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	langResource[0] = logfont.lfFaceName;
	applyDisplayFont();
	adjustWindowSize();

	// 表示を更新する。
	updateDisplay();

	// 設定を保存する。
	saveConfig();
}

// 設定するシステムフォントの情報格納用構造体
// システムフォント設定スレッドで使用する。
NONCLIENTMETRICS *s_fontMetrics;

/**
 * フォントの設定を行う。
 * 
 * @param fontMetrics 設定するフォント設定
 */
void setFontAdjusted(NONCLIENTMETRICS* fontMetrics)
{
	NONCLIENTMETRICS realMetrics;

	memcpy(&realMetrics, fontMetrics, fontMetrics->cbSize);

	if (!usePreset) {
		// Adjust caption Height
		// 高くしすぎないための配慮
		int captionHeight =
			0 - realMetrics.lfCaptionFont.lfHeight + (10 * round(getSystemDPI() / 96));
		realMetrics.iCaptionHeight = captionHeight;
	}

	// Windows 11の枠線用調整
	if (majorVersion > 10) {
		if (realMetrics.iPaddedBorderWidth == 0) {
			realMetrics.iPaddedBorderWidth = 1 + round((double)getSystemDPI() / 96);
		}
	}

	SystemParametersInfo(SPI_SETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&realMetrics,
		SPIF_UPDATEINIFILE); // | SPIF_SENDCHANGE);
}

/**
 * スレッドでアイコン以外のフォントを設定する。
 *
 * @param p スレッドに渡すパラメータ(未使用)
 * @return 0
 */
unsigned _stdcall setOnThread(void *p)
{
	setFontAdjusted(s_fontMetrics);

	return 0;
}

/**
 * 画面各部のフォントを設定する。
 *
 * @param fontMetrics アイコン以外のフォント指定用NONCLIENTMETRICS
 * @param iconLogFont アイコンのフォント
 * @param anotherThread 別スレッドで実行するか
 */
void NoMeiryoUI::setFont(
	NONCLIENTMETRICS *fontMetrics,
	LOGFONT *iconLogFont,
	bool anotherThread
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
		0,
		(LPARAM)_T("Environment"),
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
	if (anotherThread) {
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
		setFontAdjusted(fontMetrics);
	}

	messageResult = SendMessageTimeout(
		HWND_BROADCAST,
		WM_SETTINGCHANGE,
		0,
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
void NoMeiryoUI::SetWinVer(void)
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
						_T("OS:Windows 2000 (%d.%d)"),
						major,minor);
					break;
				case 1:
					_stprintf(buf,
						_T("OS:Windows XP (%d.%d)"),
						major,minor);
					break;
				case 2:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("OS:Windows XP 64bit (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("OS:Windows Server 2003 (%d.%d)"),
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
							_T("OS:Windows Vista (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("OS:Windows Server 2008 (%d.%d)"),
							major,minor);
					}
					break;
				case 1:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("OS:Windows 7 (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("OS:Windows Server 2008 R2 (%d.%d)"),
							major,minor);
					}
					break;
				case 2:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("OS:Windows 8 (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("OS:Windows Server 2012 (%d.%d)"),
							major,minor);
					}
					break;
				case 3:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("OS:Windows 8.1 (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("OS:Windows Server 2012 R2 (%d.%d)"),
							major,minor);
					}
					break;
				default:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("OS:Future Windows Client (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("OS:Future Windows Server (%d.%d)"),
							major,minor);
					}
					break;
			}
			break;
		case 10:
			if (infoEx.wProductType == VER_NT_WORKSTATION) {
				getWin10Ver(buf, major, minor);
			} else {
				_stprintf(buf,
					_T("OS:Windows Server 2016/2019/2022 (%d.%d)"),
					major,minor);
			}
			break;
		default:
			if (infoEx.wProductType == VER_NT_WORKSTATION) {
				_stprintf(buf,
					_T("OS:Future Windows Client (%d.%d)"),
					major,minor);
			} else {
				_stprintf(buf,
					_T("OS:Future Windows Server (%d.%d)"),
					major,minor);
			}
			break;
	}

	verInfo = GetDlgItem(IDC_STATIC_VERNO);
	verInfo->setText(buf);
}

/**
 * Windows 10のバージョンを取得する。
 *
 * @param buf バージョン番号格納先
 * @param major メジャーバージョン
 * @param minor マイナーバージョン
 */
void NoMeiryoUI::getWin10Ver(TCHAR *buf, DWORD major, DWORD minor)
{
	TCHAR release[8];
	TCHAR releaseName[8];
	TCHAR build[8];
	DWORD ubr = 0;

	HKEY key;
	LONG result;
	DWORD size;

	_tcscpy(release, _T("????"));
	_tcscpy(build, _T("?"));

	int buildNumber = 0;

	result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"),
		0, KEY_READ, &key);
	if (result == ERROR_SUCCESS) {
		size = sizeof(TCHAR) * 8;
		RegQueryValueEx(key, _T("ReleaseId"), NULL, NULL, (LPBYTE)release, (LPDWORD)&size);

		// Display Version for Windows 10 20H2 and later
		size = sizeof(TCHAR) * 8;
		LSTATUS found = RegQueryValueEx(
			key, _T("DisplayVersion"), NULL, NULL, (LPBYTE)releaseName, (LPDWORD)&size);
		if (found == ERROR_SUCCESS) {
			_tcscpy(release, releaseName);
		}

		size = sizeof(TCHAR) * 8;
		RegQueryValueEx(key, _T("CurrentBuild"), NULL, NULL, (LPBYTE)build, (LPDWORD)&size);
		size = sizeof(DWORD);
		RegQueryValueEx(key, _T("UBR"), NULL, NULL, (LPBYTE)&ubr, (LPDWORD)&size);
		RegCloseKey(key);

		buildNumber = _tstoi(build);
	}

	TCHAR calledVer[32];
	switch (majorVersion) {
		case 5:
			if (minorVersion == 0) {
				_tcscpy_s(calledVer, _T("2000"));
			}
			else {
				_tcscpy_s(calledVer, _T("XP"));
			}
			break;
		case 6:
			switch (majorVersion) {
				case 0:
					_tcscpy_s(calledVer, _T("Vista"));
					break;
				case 1:
					_tcscpy_s(calledVer, _T("7"));
					break;
				case 2:
					_tcscpy_s(calledVer, _T("8"));
					break;
				case 3:
					_tcscpy_s(calledVer, _T("8.1"));
					break;
			}
			break;
		case 10:
			_tcscpy_s(calledVer, _T("10"));
			break;
		case 11:
			_tcscpy_s(calledVer, _T("11"));
			if (buildNumber > 27999) {
				_tcscat_s(calledVer, _T(" Insider"));
			} else if (buildNumber > 27000) {
				_tcscat_s(calledVer, _T(" 2025 Update"));
			} else if (buildNumber > 26000) {
				_tcscat_s(calledVer, _T(" 2024 Update"));
			} else if (buildNumber == 22631) {
				_tcscat_s(calledVer, _T(" 2023 Update"));
			} else if (buildNumber >= 22621) {
				_tcscat_s(calledVer, _T(" 2022 Update"));
			}
			break;
	}

	_stprintf(buf,
		_T("OS:Windows %s (%d.%d) Version %s Build %s.%d"),
		calledVer, major, minor, release, build, ubr);

}

/**
 * ドキュメントファイルを表示する。
 *
 */
void NoMeiryoUI::showHelp(void)
{
	// 実行ファイルの情報を得るためのバッファ群
	TCHAR path[_MAX_PATH+1],drive[_MAX_DRIVE+1],dir[_MAX_DIR+1],commandLine[_MAX_PATH+32];

	// 実行ファイルのあるところのBShelp.htmlのパス名を生成する。
	::GetModuleFileName(NULL,path,_MAX_PATH);
	::_tsplitpath(path,drive,dir,NULL,NULL);
	::_stprintf(commandLine, _T("hh.exe \"%s%s%s\""), drive, dir, helpFileName);
	
	// 関連付けられたアプリでドキュメントファイルを表示する。
	// ShellExecute(hWnd,_T("open"),helpFile,NULL,NULL,SW_SHOW);

	// ウインドウ位置を制御するためCreateProcessでヘルプファイルを表示する
	STARTUPINFO info;
	PROCESS_INFORMATION procInfo;
	BOOL startResult;

	memset(&info, 0, sizeof(STARTUPINFO));
	memset(&procInfo, 0, sizeof(PROCESS_INFORMATION));

	startResult = CreateProcess(
		NULL,
		commandLine,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&info,
		&procInfo
	);
	if (startResult == FALSE) {
		return;
	}
	helpPid = procInfo.dwProcessId;
	helpMoved = false;

	// メインウインドウのあるディスプレイの座標系を取得する。
	EnumDisplayMonitors(NULL, NULL, MonitorNearWindowCallback, (LPARAM)this->hWnd);

	for (int i = 0; helpMoved == false && i < 25; i++) {
		// 起動したヘルプのウインドウを検索し、メインウインドウのあるディスプレイの
		// 中央にもっていく。
		EnumWindows(setWindowSize, 0);
		Sleep(200);
	}

}

/**
 * 各ウインドウに対するチェック処理を行うコールバック
 *
 * @param hWnd 見つかったウインドウのハンドル
 * @param lparam EnumWindowsのLPARAM(未使用)
 * @return TRUE:列挙を続ける FALSE:列挙を終える。
 */
BOOL CALLBACK setWindowSize(HWND hWnd, LPARAM lparam)
{
	DWORD wndPid;
	WINDOWPLACEMENT place;
	place.length = sizeof(WINDOWPLACEMENT);

	// 起動したプログラムのウインドウを探す
	GetWindowThreadProcessId(hWnd, &wndPid);
	if (wndPid == helpPid) {
		if (IsWindowVisible(hWnd)) {
			if (IsWindow(hWnd)) {
				// メインウインドウのあるディスプレイの中央に
				// ヘルプのウインドウ位置を設定する。
				adjustCenter(myMonitorLect, HWND_TOP, hWnd);
				helpMoved = true;

				return FALSE;
			}
		}
	}
	return TRUE;
}

/**
 * ウインドウのいるモニターを判定するためのEnumDisplayMonitorsのコールバック
 *
 * @param hMonitor モニターのハンドル
 * @param hdcMonitor モニターのディスプレイコンテキスト
 * @param lprcMonitor モニターの座標情報
 * @param dwData EnumDisplayMonitorsのパラメータ(メインウインドウのウインドウハンドル)
 * @return TRUE:列挙を続ける FALSE:列挙をやめ、モニターの座標情報を確定させる
 */
BOOL CALLBACK MonitorNearWindowCallback(
	HMONITOR hMonitor,
	HDC hdcMonitor,
	LPRECT lprcMonitor,
	LPARAM dwData
)
{
	HWND myHwnd = (HWND)dwData;
	if (!firstMonitor) {
		// ディスプレイの情報が何もない状態は避ける。
		myMonitorLect = *lprcMonitor;
		firstMonitor = true;
	}

	RECT rect;
	GetWindowRect(myHwnd, &rect);

	// カーソルのいるモニタかどうか判定する。
	int x = rect.left;
	int y = rect.top;
	if ((x >= lprcMonitor->left) && (x <= lprcMonitor->right)) {
		if ((y >= lprcMonitor->top) && (y <= lprcMonitor->bottom)) {
			myMonitorLect = *lprcMonitor;

			return FALSE;
		}
	}
	return TRUE;
}



/**
 * バージョン番号を表示する。
 *
 */
void NoMeiryoUI::showVersion(void)
{
	TCHAR aboutContent[256];
	TCHAR version[64];
	TCHAR transAuthor[64];
	TCHAR title[64];
	const TCHAR *appName;
	TCHAR verString[32];

	appName = langResource[1].c_str();
	_stprintf(title, _T("%s"),
		langResource[64].c_str());
	_tcscpy(transAuthor, langResource[69].c_str());

	LoadString(hInst, IDS_VERSION, verString, 32);

	_stprintf(version, verString, appName);
	_stprintf(aboutContent,
		_T("%s\n\nProgrammed By Tatsuhiko Syoji(Tatsu) 2005,2012-2025\nTranslated by %s"),
		version, transAuthor);

	MessageBox(hWnd,
		aboutContent,
		title,
		MB_OK | MB_ICONINFORMATION);
}

/**
 * @brief 設定ファイルを保存する
 */
void NoMeiryoUI::saveConfig(void)
{
	DWORD result;
	TCHAR pathname[MAX_PATH];
	TCHAR iniFile[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE + 1];
	TCHAR cDir[_MAX_DIR + 1];
	HMODULE hModule;
	int multiRunValue = 1;
	TCHAR multiRunString[8];

	// 実行ファイルのディレクトリを得る。
	hModule = GetModuleHandle(EXE_NAME);
	if (hModule == NULL) {
		return;
	}

	result = GetModuleFileName(hModule, pathname, MAX_PATH);

	_tsplitpath(pathname, drive, cDir, NULL, NULL);
	_stprintf(iniFile, _T("%s%s%s"), drive, cDir, INI_FILE);

	WritePrivateProfileString(CONFIG_SECTION, UIFONT_KEY, (LPCTSTR)(langResource[0].c_str()), iniFile);

	if (multiRun) {
		multiRunValue = 1;
	} else {
		multiRunValue = 0;
	}
	_stprintf(multiRunString, _T("%d"), multiRunValue);
	WritePrivateProfileString(CONFIG_SECTION, MULTI_RUN_KEY, multiRunString, iniFile);

}

/**
 * @brief 設定ファイルを読み込む
 */
void NoMeiryoUI::loadConfig(void)
{
	DWORD result;
	TCHAR pathname[MAX_PATH];
	TCHAR iniFile[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE + 1];
	TCHAR cDir[_MAX_DIR + 1];
	TCHAR fontName[33];
	HMODULE hModule;
	int read;
	int multiRunValue = 1;
	TCHAR multiRunString[8];

	// 実行ファイルのディレクトリを得る。
	hModule = GetModuleHandle(EXE_NAME);
	if (hModule == NULL) {
		return;
	}

	result = GetModuleFileName(hModule, pathname, MAX_PATH);

	_tsplitpath(pathname, drive, cDir, NULL, NULL);
	_stprintf(iniFile, _T("%s%s%s"), drive, cDir, INI_FILE);

	GetPrivateProfileString(CONFIG_SECTION, UIFONT_KEY, _T(""), fontName, 33, iniFile);

	if (fontName[0] != _T('\0')) {
		langResource[0] = fontName;
	}
	
	GetPrivateProfileString(CONFIG_SECTION, MULTI_RUN_KEY, _T("1"), multiRunString, 8, iniFile);
	multiRunValue = _ttoi(multiRunString);
	if (multiRunValue != 0) {
		multiRun = true;
	} else {
		multiRun = false;
	}

}

/**
 * @brief 複数起動関連処理
 */
void NoMeiryoUI::handleMultipleRun(void)
{
	if (multiRun) {
		return;
	}
	HWND hWnd = FindWindow(NULL, langResource[TITLE].c_str());
	if (hWnd != NULL) {
		// Already running
		ExitProcess(0);
	}
}