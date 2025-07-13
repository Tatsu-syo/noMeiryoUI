/*
noMeiryoUI (C) 2005,2012-2025 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
// noMeiryoUI.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
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
// �_�C�A���O�x�[�X�A�v���P�[�V�����t���[�����[�N��
// ���[�U�[�����̃u���b�W���[�`��
//

// �A�v���P�[�V�����I�u�W�F�N�g
NoMeiryoUI *appObj;
static bool use7Compat = true;
bool has8Preset = true;
bool has10Preset = true;
bool has11Preset = true;
// �����N����������
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
 * �A�v���P�[�V�����I�u�W�F�N�g���쐬���܂��B
 */
DialogAppliBase *createAppli()
{
	CoInitialize(NULL);

	initializeLocale();

	// �����Ń��[�U�[�̃A�v���P�[�V�����I�u�W�F�N�g���쐬���܂��B
	appObj = new NoMeiryoUI();

	return appObj;
}

/**
 * �e����̔���Ɗe����ɍ��킹�����������s���܂��B
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

	// ���P�[���̏�����
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
 * ���\�[�X�t�@�C������ݒ肷��
 * 
 * @param langFileName ����t�@�C����
 * @param helpFileName �w���v�t�@�C����
 * @param localeName ���P�[����
 * @param iniPath ini�t�@�C���̃p�X
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
		// ����_�n��`���̃t�@�C��������ꍇ
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
			// ����̃t�@�C��������ꍇ
			_tcscpy(langFileName, findPath);

			_tcscpy(helpFileName, langWork);
			_tcscat(helpFileName, _T(".chm"));
		}
		else {
			// ����t�@�C�������݂��Ȃ��ꍇ
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
 * �_�C�A���O�v���[�V�W��
 *
 * @return TRUE:�������� FALSE:�������Ȃ�
 */
INT_PTR CALLBACK MainDialogProc(
  HWND hwndDlg,  // �_�C�A���O�{�b�N�X�̃n���h��
  UINT uMsg,     // ���b�Z�[�W
  WPARAM wParam, // �ŏ��̃��b�Z�[�W�p�����[�^
  LPARAM lParam  // 2 �Ԗڂ̃��b�Z�[�W�p�����[�^
  )
{
	// ���[�U�[�A�v���P�[�V�����̃_�C�A���O�v���[�V�W���Ɋۓ������܂��B
	return appObj->dialogProc(hwndDlg, uMsg,wParam,lParam);
}

//
// �_�C�A���O�x�[�X�A�v���P�[�V�����t���[�����[�N�̎�����
//

/**
 * �A�v���P�[�V�����E�C���h�E�N���X�̃C���X�^���X�𐶐����܂��B
 *
 * @return �A�v���P�[�V�����E�C���h�E�N���X�̃C���X�^���X
 */
BaseDialog *NoMeiryoUI::createBaseDialog()
{
	firstShow = true;

	return appObj;
}

/**
 * �A�v���P�[�V�����̊J�n���������s���܂��B
 *
 * @param lpCmdLine �R�}���h���C��
 * @return �\��
 */
int NoMeiryoUI::OnAppliStart(TCHAR *lpCmdLine)
{
	// �A�v���ŗL�̏��������s���܂��B
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

	// ���W���[�o�[�W�������擾����
	DWORD dwVersion = GetVersionForApp(majorVersion, minorVersion, buildNumber);
	// If Windows 11 Build 22621 Switch on�ɂ��悤�Ǝv��������
	// 21H2�̂��Ƃ�Insider����N���Ă��̂�21H2�̂��Ƃ͑S����������
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

	// �I�v�V�������擾����B
	getOption();

	return 0;
}

/**
 * �A�v���P�[�V�����̏I�����������s���܂��B
 *
 * @return �\��
 */
int NoMeiryoUI::OnAppliEnd()
{
	// ���̊֐����I�[�o�[���C�h���ăA�v���ŗL�̌㏈�����s���܂��B
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
// �_�C�A���O�x�[�X�A�v���P�[�V�����Ƃ��Ă�
// �A�v���P�[�V�����ŗL�����̎�������
//

/**
 * �_�C�A���O����������
 *
 * @return TRUE:�t�H�[�J�X��ݒ肵�� FALSE:�t�H�[�J�X��ݒ肵�Ȃ�
 */
INT_PTR NoMeiryoUI::OnInitDialog()
{
	// �e�N���X�̃_�C�A���O�������������ĂԁB
	DialogAppliBase::OnInitDialog();

	// �t�H���g���擾�p�\���̂̏�����
	FillMemory(&metrics, sizeof(NONCLIENTMETRICS), 0x00);
	FillMemory(&metricsAll, sizeof(NONCLIENTMETRICS), 0x00);
	FillMemory(&iconFont, sizeof(LOGFONT), 0x00);
	FillMemory(&iconFontAll, sizeof(LOGFONT), 0x00);

	// ���݂̃t�H���g�ƕt�������ʊe���̕����̏����擾���Ă����B
	getActualFont();

	// �t�H���g��ǂݍ��ޏꍇ�̏���
	if (settingFile[0] != _T('\0')) {

		BOOL loadResult = loadFontInfo(settingFile);
		if (loadResult) {
			// ���j���[�t�H���g�̏������ׂẴt�H���g�̊e�t�H���g�̏��ɂ��Ă�B
			metricsAll.lfMenuFont = metrics.lfMenuFont;
			metricsAll.lfStatusFont = metricsAll.lfMenuFont;
			metricsAll.lfMessageFont = metricsAll.lfMenuFont;
			metricsAll.lfCaptionFont = metricsAll.lfMenuFont;
			metricsAll.lfSmCaptionFont = metricsAll.lfMenuFont;
			iconFontAll = metricsAll.lfMenuFont;
		} else {
			// �ǂݍ��ݎ��s���͔O�̂��ߍēx���݂̃t�H���g���̏���
			// �擾���Ă����B
			getActualFont();
		}
	}
	return (INT_PTR)FALSE;
}

/**
 * �E�C���h�E�����������̏���
 *
 * @param wParam WPARAM
 * @param lParam lParam
 * @return 0:�������� ��0:�������Ȃ�
 */
INT_PTR NoMeiryoUI::OnWindowShown(WPARAM wParam, LPARAM lParam)
{
	DialogAppliBase::OnWindowShown(wParam, lParam);

	if (firstShow) {

		// �A�v���P�[�V�����A�C�R���̐ݒ�
		HICON hIcon;

		hIcon = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDC_MYICON), IMAGE_ICON, 16, 16, 0);
		SendMessage(this->hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

		appMenu = new TwrMenu(this->hWnd);

		if (!use7Compat) {
			// Windows 7�ȑO�̏ꍇ�̓t�H���g�T�C�Y�̎�舵�����[�h��ύX�ł��Ȃ�����B
			appMenu->setEnabled(IDM_COMPAT7, false);
		}
		appMenu->CheckMenuItem(IDM_ANOTHER, true);

		// �C�O�ł͏����ݒ�̃t�H���g���قȂ�̂Ńv���Z�b�g���j���[���
		// ����ꍇ�̂݃v���Z�b�g��L���ɂ���B
		appMenu->setEnabled(IDM_SET_8, has8Preset);
		appMenu->setEnabled(IDM_SET_10, has10Preset);
		appMenu->setEnabled(IDM_SET_11, has11Preset);

		// �����N���ݒ�
		if (!multiRun) {
			appMenu->CheckMenuItem(IDM_NO_MULTI_RUN, true);
		}

		// �攭��OS�ł̓t�H���g���Ȃ��ꍇ������̂�
		// �㔭OS�p�̃v���Z�b�g���g�p�s�Ƃ���B
		if (majorVersion < 10) {
			appMenu->setEnabled(IDM_SET_10, false);
		}
		if (majorVersion < 11) {
			appMenu->setEnabled(IDM_SET_11, false);
		}

		// UI����������\�[�X�ɍ��킹�ĕύX����B
		applyResource();

		if (setOnStart) {

			this->ShowWindow(SW_HIDE);

			if (autosetDelay > 0) {
				Sleep(autosetDelay * 1000);
			}

			// -set�I�v�V�������w�肳�ꂽ�ꍇ�̓t�H���g��ݒ肵�ă_�C�A���O�����B
			if (compatLevel > 0) {
				// Windows 11 22H2�ȍ~�̏ꍇ�A�^�C�g�������̕��ɕύX����
				set11TitlePreset(&metrics);
			}

			// �t�H���g�ύX�����{����B
			setFont(&metrics, &iconFont, true);

			ExitProcess(0);

			return (INT_PTR)TRUE;
		}

		// ���C���_�C�A���O�̃o�[�W�����\�L�ݒ�
		TCHAR buf[64];
		TCHAR verString[32];
		const TCHAR *appName;
		LoadString(hInst, IDS_VERSION, verString, 32);
		appName = langResource[1].c_str();
		_stprintf(buf, verString, appName);
		setChildText(IDC_STATIC_APP_TITLE, buf);

		// �t�H���g���\�����X�V����B
		updateDisplay();

		EnumDisplayMonitors(NULL, NULL, MonitorNearMouseCallback, 0);

		adjustCenter(myMonitorLect, HWND_TOP, this->hWnd);

		adjustWindowSize();

		firstShow = false;
	}

	return (INT_PTR)0;
}

/**
 * �_�C�A���O���\�����ꂽ��ɍs������
 *
 * @param wParam WPARAM
 * @param lParam lParam
 * @return 0:�������� ��0:�������Ȃ�
 */
INT_PTR NoMeiryoUI::OnWindowCreated(WPARAM wParam, LPARAM lParam)
{
	DialogAppliBase::OnWindowCreated(wParam, lParam);

	if (compatLevel > 0) {
		titleFontButton->EnableWindow(FALSE);
		appMenu->setEnabled(IDM_FORCE_TITLE_SET, true);

		// ���[�j���O���b�Z�[�W in Win11 22H2
		MessageBox(this->getHwnd(), langResource[MSG_WIN11_22H2RESTRICTION].c_str(),
			langResource[MSG_WARNING].c_str(), MB_OK | MB_ICONWARNING);
		//MessageBox(this->getHwnd(), _T("Windows 11�̃o�J�����["),
		//	_T("�����႟����႟"), MB_OK | MB_ICONWARNING);
	} else {
		appMenu->setEnabled(IDM_FORCE_TITLE_SET, false);
	}

	return (INT_PTR)0;
}


/**
 * �J�[�\���̂��郂�j�^�[�𔻒肷�邽�߂�EnumDisplayMonitors�̃R�[���o�b�N
 *
 * @param hMonitor ���j�^�[�̃n���h��
 * @param hdcMonitor ���j�^�[�̃f�B�X�v���C�R���e�L�X�g
 * @param lprcMonitor ���j�^�[�̍��W���
 * @param dwData EnumDisplayMonitors
 * @return TRUE:�񋓂𑱂��� FALSE:�񋓂���߁A���j�^�[�̍��W�����m�肳����
 */
BOOL CALLBACK MonitorNearMouseCallback(
	HMONITOR hMonitor,
	HDC hdcMonitor,
	LPRECT lprcMonitor,
	LPARAM dwData
)
{
	if (!firstMonitor) {
		// �f�B�X�v���C�̏�񂪉����Ȃ���Ԃ͔�����B
		myMonitorLect = *lprcMonitor;
		firstMonitor = true;
	}

	CURSORINFO cursofInfo;
	cursofInfo.cbSize = sizeof(CURSORINFO);
	BOOL result = GetCursorInfo(&cursofInfo);
	if (result == 0) {
		// �J�[�\���̏��𗘗p�ł��Ȃ��Ƃ��̓v���C�}�����j�^��
		// �J�[�\���̂��郂�j�^�����Ƃ���B
		if ((myMonitorLect.left == 0) && (myMonitorLect.top == 0)) {
			myMonitorLect = *lprcMonitor;
			return FALSE;
		} else {
			return TRUE;
		}
	}

	// �J�[�\���̂��郂�j�^���ǂ������肷��B
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
 * �E�C���h�E���\�����ꂽ�Ƃ��̏��������s���܂��B<br>
 * �E�C���h�E�x�[�X�A�v���ƃC���^�t�F�[�X�����킹�邽�߂ɗp�ӂ��Ă��܂��B
 *
 * @return �\��
 */
int NoMeiryoUI::OnWindowShow()
{

	// ���̊֐����I�[�o�[���C�h���āA����̕\�����̏������s���܂��B
	// ���̃^�C�~���O�Ń_�C�A���O�����݂���̂ŁA�����ɏ��������邱�Ƃ�
	// �_�C�A���O�������ԂŋN�����̏������������s�����Ƃ��ł��܂��B

	SetWinVer();

	// �e�L�X�g�{�b�N�X����p�Ƀ_�C�A���O�̊e�e�L�X�g�{�b�N�X���擾����B
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
 * @brief �E�C���h�E�T�C�Y�𒲐�����
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
 * �R�}���h���C���I�v�V�������擾����B
 */
void NoMeiryoUI::getOption()
{

	std::vector<tstring *> *argv = extractArguments();

	parseOption(argv);

	deleteArguments(argv);
}

/**
 * �I�v�V�����̈ʒu�ɉ�������͂��s���B
 *
 * @param param �p�����[�^
 * @param argCount �I�v�V�����̌�
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
		} else if (!(*param)[i]->compare(_T("-delay"))) {
			if ((i + 1) < param->size()) {
				autosetDelay = _ttoi((*param)[i + 1]->c_str());
				i++;
			}
		} else if (!(*param)[i]->compare(_T("-force"))) {
			forceTitleFontSet = true;
		}

	}
}

/**
 * ���ݐݒ肳��Ă���t�H���g���擾����B
 */
void NoMeiryoUI::getActualFont(void)
{
	//
	// �ʂ̃t�H���g�p�̏��擾
	//

	// �A�C�R���ȊO�̃t�H���g�����擾����B
	metrics.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metrics,
		0);

	// �A�C�R���̃t�H���g�����擾����B
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
	// ���ׂẴt�H���g�p�̏��擾
	//

	// �A�C�R���ȊO�̃t�H���g�����擾����B
	metricsAll.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metricsAll,
		0);

	// �A�C�R���̃t�H���g�����擾����B
	SystemParametersInfo(SPI_GETICONTITLELOGFONT,
		sizeof(LOGFONT),
		&iconFontAll,
		0);

	// ���j���[�t�H���g�̏��𑼂̃t�H���g�̏��ɂ��Ă�B
	metricsAll.lfStatusFont = metricsAll.lfMenuFont;
	metricsAll.lfMessageFont = metricsAll.lfMenuFont;
	metricsAll.lfCaptionFont = metricsAll.lfMenuFont;
	metricsAll.lfSmCaptionFont = metricsAll.lfMenuFont;
	iconFontAll = metricsAll.lfMenuFont;
}

/**
 * �t�H���g��񂩂�t�H���g�̃n���h�����쐬����B
 *
 * @param font �t�H���g�̏��
 * @return �t�H���g�n���h��
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
 * �_�C�A���O�R���g���[���ƃI�u�W�F�N�g�̓��e�̓��������B
 *
 * @param toObj true:�R���g���[�����I�u�W�F�N�g false:�I�u�W�F�N�g���R���g���[��
 */
void NoMeiryoUI::UpdateData(bool toObj)
{
	// �����Ƀ_�C�A���O�̃R���g���[���Ɠ�������郁���o�ϐ����L�q���܂��B
	DDX_Text(toObj,IDC_EDIT_ALL, allFontName);
	DDX_Text(toObj,IDC_EDIT_TITLE, titleFontName);
	DDX_Text(toObj,IDC_EDIT_ICON, iconFontName);
	DDX_Text(toObj,IDC_EDIT_PALETTE, paletteFontName);
	DDX_Text(toObj,IDC_EDIT_HINT, hintFontName);
	DDX_Text(toObj,IDC_EDIT_MESSAGE, messageFontName);
	DDX_Text(toObj,IDC_EDIT_MENU, menuFontName);
}

/**
 * ���\�[�X���e���ڂɐݒ肷��B
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
 * ���\�[�X���e���ڂɐݒ肷��(������)�B
 */
void NoMeiryoUI::applyDisplayText()
{
	// �A�v���^�C�g��
	setText(langResource[1].c_str());

	// ���j���[������ύX
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
 * ���\�[�X���e���ڂɐݒ肷��(�t�H���g)�B
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
 * �t�H���g�\�����X�V����B
 */
void NoMeiryoUI::updateDisplay(void)
{
	// �t�H���g���A�|�C���g���\����������쐬����B
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

	// ���j���[�ƑI������
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


	// �I�������t�H���g���e�L�X�g�{�b�N�X�ɐݒ肷��B
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
 * �e����ɑ΂��鏈���̕���
 *
 * @param wParam WPARAM
 * @return 0:���b�Z�[�W�����������B 0�ȊO:���b�Z�[�W���������Ȃ������B
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
 * @brief Windows 7���v�Z�̃g�O������
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
 * @brief �����N���t���O�؂�ւ�
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
 *  @brief Windows 11 22H2�ȍ~�ł̋����t�H���g�ݒ�؂�ւ�
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
 * �t�H���g��I������B
 *
 * @param type �ݒ肷��t�H���g�̎��
 */
void NoMeiryoUI::selectFont(enum fontType type)
{

	INT_PTR result;
	LOGFONT logfont;	// �擾�����t�H���g�̏�������\����

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
				// ���j���[�ƑI������
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
		// �I�����Ă����t�H���g���t�H���g�I���_�C�A���O�ɐݒ肷��B
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

			if ((compatLevel < 1) || (forceTitleFontSet)) {
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
			// ���j���[�ƑI������
			metrics.lfMenuFont = logfont;
			menuFontName = logfont.lfFaceName;

			DeleteObject(menuFont);
			menuFont = createFont(&metrics.lfMenuFont);
			menuFontTextBox->setFont(menuFont);
			fontPoints.menu = points;
			break;
	}

	// �\�����X�V����B
	updateDisplay();
}

/**
 * �t�@�C���_�C�A���O�p�̃t�@�C���}�X�N������쐬
 *
 * @param �t�@�C���}�X�N������o�b�t�@
 * @param �t�@�C���̎�ޖ�
 * @param �t�@�C���̃}�X�N
 * @param ���ׂẴt�@�C���̎�ޖ�
 * @param ���ׂẴt�@�C���̃}�X�N
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
 * �t�H���g�ݒ��ۑ������I���������̓���
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
		// �t�H���g�ݒ�̓ǂݍ��݂ɐ���������e�L�X�g�{�b�N�X�ɐݒ肷��B
		updateDisplay();
	}


	delete dlg;
}

/**
 * �t�H���g����ǂݍ��ށB
 *
 * @param filename ini�t�@�C����
 * @result TRUE:�ۑ����� FALSE:�ۑ����s
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

	if ((compatLevel < 1) || (forceTitleFontSet == true)) {
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
 * �t�H���g����ǂݍ��ށB
 *
 * @param filename ini�t�@�C����
 * @param category �ǂݍ��ݑΏۃt�H���g��ini�t�@�C���Z�N�V������
 * @param font �ǂݍ��ݑΏۃt�H���g��LOGFONT�\����
 * @result TRUE:�ǂݍ��ݐ��� FALSE:�ǂݍ��ݎ��s
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
 * �t�H���g�ݒ��ۑ������I���������̓���
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
 * �t�H���g���ۑ����J�n����B
 *
 * @param filename ini�t�@�C����
 * @result TRUE:�ۑ����� FALSE:�ۑ����s
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
 * �t�H���g����ۑ�����B
 *
 * @param filename ini�t�@�C����
 * @param category �ۑ��Ώۃt�H���g��ini�t�@�C���Z�N�V������
 * @param font �ۑ��Ώۃt�H���g��LOGFONT�\����
 * @param point �t�H���g�T�C�Y(�|�C���g)
 * @result TRUE:�ۑ����� FALSE:�ۑ����s
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
 * OK�{�^���������̓���(�I�������t�H���g��ݒ肷��B)
 *
 * @return TRUE:�t�H���g��ݒ肵���Ƃ� FALSE:�t�H���g�ݒ���L�����Z�������Ƃ�
 */
INT_PTR NoMeiryoUI::OnBnClickedOk()
{
#if 0
	// ����ďc�����p�t�H���g���w�肵�Ȃ��悤�₢���킹���s���B
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
			_T("�c�����p�t�H���g(���O��@�Ŏn�܂�t�H���g)��\n�w�肳��Ă��܂�����낵���ł����H"),
			_T("�m�F"),
			MB_ICONQUESTION | MB_YESNO);
		if (answer != IDYES) {
			return (INT_PTR)FALSE;
		}
	}
#endif
	if ((compatLevel > 0) && (forceTitleFontSet == false)){
		// Windows 11 22H2�ȍ~�̏ꍇ�A�^�C�g�������̕��ɕύX����
		set11TitlePreset(&metrics);

		DeleteObject(titleFont);
		titleFont = createFont(&metrics.lfCaptionFont);
		titleFontTextBox->setFont(titleFont);
		double points;
		fontPoints.title = getFontPointInt(&(metrics.lfCaptionFont), this->getHwnd());

		updateDisplay();

	}

	// �t�H���g�ύX�����{����B
	setFont(&metrics, &iconFont, appMenu->isChecked(IDM_ANOTHER));

	adjustWindowSize();
	// COLORREF ref = GetThemeSysColor(NULL, COLOR_ACTIVECAPTION);

	return (INT_PTR)TRUE;
}

/**
 * �ꊇ�ݒ�{�^���������̓���(���ׂẴt�H���g�őI�������t�H���g��ݒ肷��B)
 *
 */
void NoMeiryoUI::OnBnClickedAll()
{
#if 0
	// ����ďc�����p�t�H���g���w�肵�Ȃ��悤�₢���킹���s���B
	if (metricsAll.lfMenuFont.lfFaceName[0] == _T('@')) {
		int answer = MessageBox(hWnd,
			_T("�c�����p�t�H���g(���O��@�Ŏn�܂�t�H���g)��\n�w�肳��Ă��܂�����낵���ł����H"),
			_T("�m�F"),
			MB_ICONQUESTION | MB_YESNO);
		if (answer != IDYES) {
			return;
		}
	}
#endif
	if ((compatLevel > 0) && (forceTitleFontSet == false)) {
		set11TitlePreset(&metricsAll);

		DeleteObject(titleFont);
		titleFont = createFont(&metrics.lfCaptionFont);
		titleFontTextBox->setFont(titleFont);
		double points;
		fontPoints.title = getFontPointInt(&(metrics.lfCaptionFont), this->getHwnd());
	}

	// �t�H���g�ύX�����{����B
	setFont(&metricsAll, &iconFontAll, appMenu->isChecked(IDM_ANOTHER));

	adjustWindowSize();

	memcpy(&metrics, &metricsAll,sizeof(NONCLIENTMETRICS));
	memcpy(&iconFont, &iconFontAll,sizeof(LOGFONT));

	// �\�����X�V����B
	updateDisplay();
}

/**
 * �V�X�e����DPI���擾����B
 *
 * @return �擾����DPI
 */
int NoMeiryoUI::getDPI(void)
{
	// DPI���擾����B
	int dpiY = 96;
	HDC hdc = GetDC(NULL);

	if (hdc) {
		dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
		ReleaseDC(NULL, hdc);
	}

	return dpiY;
}

/**
 * Windows 8/8.1�̏ꍇ�̃v���Z�b�g�l��ݒ肷��B
 */
void NoMeiryoUI::OnSet8(void)
{
	// DPI���擾����B
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


	// �\�����X�V����B
	updateDisplay();

	usePreset = true;

}

/**
 * Windows 10�̏ꍇ�̃v���Z�b�g�l��ݒ肷��B
 */
void NoMeiryoUI::OnSet10(void)
{
	// DPI���擾����B
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

	// �\�����X�V����B
	updateDisplay();

	usePreset = true;

}

/**
 * Windows 11�̃^�C�g���̃v���Z�b�g������ݒ肷��B<br>
 * Windows 11 22H2�p
 * 
 * @param metrics �ݒ肷��NONCLIENTMETRICS
 */
void NoMeiryoUI::set11TitlePreset(NONCLIENTMETRICS *metrics)
{
	// DPI���擾����B
	int dpiY = getDPI();

	memset(&((*metrics).lfCaptionFont), 0, sizeof(LOGFONTW));
	_tcscpy((*metrics).lfCaptionFont.lfFaceName, fontFaces11[0].c_str());
	(*metrics).lfCaptionFont.lfHeight = -MulDiv(fontSizes11[0], dpiY, 72);
	(*metrics).lfCaptionFont.lfWeight = 400;
	(*metrics).lfCaptionFont.lfCharSet = fontCharset11[0];
	(*metrics).lfCaptionFont.lfQuality = 5;
}

/**
 * Windows 11�̏ꍇ�̃v���Z�b�g�l��ݒ肷��B
 */
void NoMeiryoUI::OnSet11(void)
{
	// DPI���擾����B
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


	// �\�����X�V����B
	updateDisplay();

	usePreset = true;

}

/**
 * �A�v���P�[�V�����t�H���g��I������B
 */
void NoMeiryoUI::OnChoiceAppFont()
{

	INT_PTR result;
	LOGFONT logfont;	// �擾�����t�H���g�̏�������\����

	FillMemory(&logfont, sizeof(LOGFONT), 0x00);

	try {

		FontSel *selector = new FontSel(this->hWnd, IDD_DIALOG_FONTSEL, TRUE);
		if (noMeiryoUI) {
			selector->setNoMeiryoUI();
		}
		if (noTahoma) {
			selector->setNoTahoma();
		}
		// �I�����Ă����t�H���g���t�H���g�I���_�C�A���O�ɐݒ肷��B
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

	// �\�����X�V����B
	updateDisplay();

	// �ݒ��ۑ�����B
	saveConfig();
}

// �ݒ肷��V�X�e���t�H���g�̏��i�[�p�\����
// �V�X�e���t�H���g�ݒ�X���b�h�Ŏg�p����B
NONCLIENTMETRICS *s_fontMetrics;

/**
 * �t�H���g�̐ݒ���s���B
 * 
 * @param fontMetrics �ݒ肷��t�H���g�ݒ�
 */
void setFontAdjusted(NONCLIENTMETRICS* fontMetrics)
{
	NONCLIENTMETRICS realMetrics;

	memcpy(&realMetrics, fontMetrics, fontMetrics->cbSize);

	if (!usePreset) {
		// Adjust caption Height
		// �����������Ȃ����߂̔z��
		int captionHeight =
			0 - realMetrics.lfCaptionFont.lfHeight + (10 * round(getSystemDPI() / 96));
		realMetrics.iCaptionHeight = captionHeight;
	}

	// Windows 11�̘g���p����
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
 * �X���b�h�ŃA�C�R���ȊO�̃t�H���g��ݒ肷��B
 *
 * @param p �X���b�h�ɓn���p�����[�^(���g�p)
 * @return 0
 */
unsigned _stdcall setOnThread(void *p)
{
	setFontAdjusted(s_fontMetrics);

	return 0;
}

/**
 * ��ʊe���̃t�H���g��ݒ肷��B
 *
 * @param fontMetrics �A�C�R���ȊO�̃t�H���g�w��pNONCLIENTMETRICS
 * @param iconLogFont �A�C�R���̃t�H���g
 * @param anotherThread �ʃX���b�h�Ŏ��s���邩
 */
void NoMeiryoUI::setFont(
	NONCLIENTMETRICS *fontMetrics,
	LOGFONT *iconLogFont,
	bool anotherThread
) {

	DWORD_PTR ptr;
	LRESULT messageResult;

	// �A�C�R���̃t�H���g�ݒ�
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

	// �A�C�R���ȊO�̃t�H���g�ݒ�
	if (anotherThread) {
		// UI�ƕʃX���b�h��SystemParametersInfo(SPI_SETNONCLIENTMETRICS��
		// ���s����B
		s_fontMetrics = fontMetrics;

		// �t�H���g�ݒ�����s����X���b�h���J�n����B
		uintptr_t startResult = _beginthreadex(NULL, 0, setOnThread, NULL, 0, NULL);
		if (startResult != 0) {
			// ����ɃX���b�h���J�n������X���b�h�I����ҋ@����B
			HANDLE handle;
			handle = (HANDLE)startResult;

			// �ꉞ5�b�قǑ҂�
			DWORD waitResult = WaitForSingleObject(handle, 5000);
			if (waitResult == WAIT_TIMEOUT) {
				// �X���b�h���I�����Ȃ��ꍇ�͂ǂ����悤���Ȃ��̂ŃX���b�h���I������B
				TerminateThread(handle, 0);
			}
			CloseHandle(handle);
		}
	} else {
		// UI�Ɠ����X���b�h��SystemParametersInfo(SPI_SETNONCLIENTMETRICS��
		// ���s����B
		setFontAdjusted(fontMetrics);
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

	// �F���Đݒ肷�邱�Ƃŉ�ʂ����t���b�V������B
	// �̂����AIObit StartMenu 8���N�����Ă���Ƃ���SetSysColors��
	// �Ăяo���Ɖ������Ȃ��Ȃ�̂ŌĂяo�����s��Ȃ����Ƃɂ����B
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
 * Windows�̃o�[�W�������擾���āA��ʂɐݒ肷��B
 */
void NoMeiryoUI::SetWinVer(void)
{
	// Windows�̓����o�[�W�����𒲂ׂ�B
	DWORD dwVersion = GetVersion();
	TCHAR buf[128];

	DWORD major = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	DWORD minor = (DWORD)(HIBYTE(LOWORD(dwVersion)));

	// �T�[�o�[���ǂ����̒ǉ������擾���邽��
	// GetVersionEx��OSVERSIONINFOEX��n���ČĂяo���B
	// Windows 98/Me,NT4�ȑO�͍l�����Ȃ��̂ŌĂѕ����͂Ȃ��B
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
 * Windows 10�̃o�[�W�������擾����B
 *
 * @param buf �o�[�W�����ԍ��i�[��
 * @param major ���W���[�o�[�W����
 * @param minor �}�C�i�[�o�[�W����
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
			if (buildNumber > 27000) {
				_tcscat_s(calledVer, _T(" Insider"));
			} else if (buildNumber >= 26200) {
				_tcscat_s(calledVer, _T(" 2025 Update"));
			} else if (buildNumber >= 26000) {
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
 * �h�L�������g�t�@�C����\������B
 *
 */
void NoMeiryoUI::showHelp(void)
{
	// ���s�t�@�C���̏��𓾂邽�߂̃o�b�t�@�Q
	TCHAR path[_MAX_PATH+1],drive[_MAX_DRIVE+1],dir[_MAX_DIR+1],commandLine[_MAX_PATH+32];

	// ���s�t�@�C���̂���Ƃ����BShelp.html�̃p�X���𐶐�����B
	::GetModuleFileName(NULL,path,_MAX_PATH);
	::_tsplitpath(path,drive,dir,NULL,NULL);
	::_stprintf(commandLine, _T("hh.exe \"%s%s%s\""), drive, dir, helpFileName);
	
	// �֘A�t����ꂽ�A�v���Ńh�L�������g�t�@�C����\������B
	// ShellExecute(hWnd,_T("open"),helpFile,NULL,NULL,SW_SHOW);

	// �E�C���h�E�ʒu�𐧌䂷�邽��CreateProcess�Ńw���v�t�@�C����\������
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

	// ���C���E�C���h�E�̂���f�B�X�v���C�̍��W�n���擾����B
	EnumDisplayMonitors(NULL, NULL, MonitorNearWindowCallback, (LPARAM)this->hWnd);

	for (int i = 0; helpMoved == false && i < 25; i++) {
		// �N�������w���v�̃E�C���h�E���������A���C���E�C���h�E�̂���f�B�X�v���C��
		// �����ɂ����Ă����B
		EnumWindows(setWindowSize, 0);
		Sleep(200);
	}

}

/**
 * �e�E�C���h�E�ɑ΂���`�F�b�N�������s���R�[���o�b�N
 *
 * @param hWnd ���������E�C���h�E�̃n���h��
 * @param lparam EnumWindows��LPARAM(���g�p)
 * @return TRUE:�񋓂𑱂��� FALSE:�񋓂��I����B
 */
BOOL CALLBACK setWindowSize(HWND hWnd, LPARAM lparam)
{
	DWORD wndPid;
	WINDOWPLACEMENT place;
	place.length = sizeof(WINDOWPLACEMENT);

	// �N�������v���O�����̃E�C���h�E��T��
	GetWindowThreadProcessId(hWnd, &wndPid);
	if (wndPid == helpPid) {
		if (IsWindowVisible(hWnd)) {
			if (IsWindow(hWnd)) {
				// ���C���E�C���h�E�̂���f�B�X�v���C�̒�����
				// �w���v�̃E�C���h�E�ʒu��ݒ肷��B
				adjustCenter(myMonitorLect, HWND_TOP, hWnd);
				helpMoved = true;

				return FALSE;
			}
		}
	}
	return TRUE;
}

/**
 * �E�C���h�E�̂��郂�j�^�[�𔻒肷�邽�߂�EnumDisplayMonitors�̃R�[���o�b�N
 *
 * @param hMonitor ���j�^�[�̃n���h��
 * @param hdcMonitor ���j�^�[�̃f�B�X�v���C�R���e�L�X�g
 * @param lprcMonitor ���j�^�[�̍��W���
 * @param dwData EnumDisplayMonitors�̃p�����[�^(���C���E�C���h�E�̃E�C���h�E�n���h��)
 * @return TRUE:�񋓂𑱂��� FALSE:�񋓂���߁A���j�^�[�̍��W�����m�肳����
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
		// �f�B�X�v���C�̏�񂪉����Ȃ���Ԃ͔�����B
		myMonitorLect = *lprcMonitor;
		firstMonitor = true;
	}

	RECT rect;
	GetWindowRect(myHwnd, &rect);

	// �J�[�\���̂��郂�j�^���ǂ������肷��B
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
 * �o�[�W�����ԍ���\������B
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

#ifdef PORTABLE

/**
 * @brief �ݒ�t�@�C����ۑ�����
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

	// ���s�t�@�C���̃f�B���N�g���𓾂�B
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
 * @brief �ݒ�t�@�C����ǂݍ���
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

	// ���s�t�@�C���̃f�B���N�g���𓾂�B
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

#else

/**
 * @brief �ݒ�t�@�C����ۑ�����
 */
void NoMeiryoUI::saveConfig(void)
{
	int multiRunValue = 1;
	TCHAR multiRunString[8];

	WIN32_FIND_DATA targetInfo;
	tstring settingsFile;

	getApplicationSettingFolder(settingsFile);

	settingsFile += _T("\\Tatsu_syo");

	memset(&targetInfo, 0, sizeof(targetInfo));

	HANDLE findHandle = FindFirstFile(settingsFile.c_str(), &targetInfo);
	if (findHandle == INVALID_HANDLE_VALUE) {
		BOOL createResult = CreateDirectory(settingsFile.c_str(), NULL);
		if (createResult == 0) {
			return;
		}
	} else {
		FindClose(findHandle);
		if ((targetInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			return;
		}
	}

	settingsFile += _T("\\NoMeiryoUI");
	memset(&targetInfo, 0, sizeof(targetInfo));
	_tcscpy_s(targetInfo.cFileName, settingsFile.c_str());
	findHandle = FindFirstFile(settingsFile.c_str(), &targetInfo);
	if (findHandle == INVALID_HANDLE_VALUE) {
		BOOL createResult = CreateDirectory(settingsFile.c_str(), NULL);
		if (createResult == 0) {
			return;
		}
	} else {
		FindClose(findHandle);
		if ((targetInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			return;
		}
	}

	settingsFile += _T("\\");
	settingsFile += INI_FILE;

	WritePrivateProfileString(CONFIG_SECTION, UIFONT_KEY, (LPCTSTR)(langResource[0].c_str()), settingsFile.c_str());

	if (multiRun) {
		multiRunValue = 1;
	} else {
		multiRunValue = 0;
	}
	_stprintf(multiRunString, _T("%d"), multiRunValue);
	WritePrivateProfileString(CONFIG_SECTION, MULTI_RUN_KEY, multiRunString, settingsFile.c_str());

}

/**
 * @brief �ݒ�t�@�C����ǂݍ���
 */
void NoMeiryoUI::loadConfig(void)
{
	TCHAR fontName[33];
	int multiRunValue = 1;
	TCHAR multiRunString[8];
	tstring settingsFile;

	getApplicationSettingFolder(settingsFile);
	settingsFile += _T("\\Tatsu_syo\\NoMeiryoUI\\");
	settingsFile += INI_FILE;

	GetPrivateProfileString(CONFIG_SECTION, UIFONT_KEY, _T(""), fontName, 33, settingsFile.c_str());

	if (fontName[0] != _T('\0')) {
		langResource[0] = fontName;
	}
	
	GetPrivateProfileString(CONFIG_SECTION, MULTI_RUN_KEY, _T("1"), multiRunString, 8, settingsFile.c_str());
	multiRunValue = _ttoi(multiRunString);
	if (multiRunValue != 0) {
		multiRun = true;
	} else {
		multiRun = false;
	}

}

#endif

/**
 * @brief �����N���֘A����
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