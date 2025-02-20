/*
noMeiryoUI (C) 2005,2012-2024 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#pragma warning(disable : 4996)

#include <math.h>
#include "util.h"
#include "iniReader.h"

/** Windows 8のフォントサイズ算出式を使用するか？ */
bool WIN8_SIZE = true;
/** 言語リソース */
std::vector<tstring> langResource;

/** フォント名(Windows 8.x) */
std::vector<tstring> fontFaces8;
/** フォントサイズ(Windows 8.x) */
std::vector<int> fontSizes8;
/** フォント文字セット(Windows 8.x) */
std::vector<int> fontCharset8;

/** フォント名(Windows 10) */
std::vector<tstring> fontFaces10;
/** フォントサイズ(Windows 10) */
std::vector<int> fontSizes10;
/** フォント文字セット(Windows 10) */
std::vector<int> fontCharset10;

/** フォント名(Windows 11) */
std::vector<tstring> fontFaces11;
/** フォントサイズ(Windows 11) */
std::vector<int> fontSizes11;
/** フォント文字セット(Windows 11) */
std::vector<int> fontCharset11;

int codePage = 0;
Country runningCountry = NoCountry;

// 
double Win8PresetWindowsMetric[] = {
	1,
	36,
	22,
	17,
	17,
	22,
	22,
	19,
	19,
	4 };

double Win10PresetWindowsMetric[] = {
	1,
	36,
	22,
	17,
	17,
	22,
	22,
	19,
	19,
	4 };

double Win11PresetWindowsMetric[] = {
	1,
	36,
	22,
	17,
	17,
	22,
	22,
	19,
	19,
	4 };

#if _MSC_VER < 1800

double round(double x)
{
	if (x > 0.0){
		return floor(x + 0.5);
	} else {
		return ceil(x - 0.5);
	}
}
#endif

/**
 * フォントのピクセル数に対応するポイント数を整数で算出する。(Windows 8)
 *
 * @param font フォント情報
 * @param hWnd ウインドウハンドル
 * @return フォントサイズ
 */
int getFontPointInt(LOGFONT *font, HWND hWnd)
{
	double point = getFontPoint(font, hWnd);

	if (WIN8_SIZE) {
		// Windows 8ディスプレイコントロールパネル互換
		if ((point > 10) || (point < 8)) {
			return (int)point;
		} else {
			// 10ptまではWindows 7と同様に計算する。
			// Windows 7以前互換 
			if (point - abs((int)point) > 0.49) {
				return (int)point + 1;
			} else {
				return (int)point;
			}
		}
	} else {
		// Windows 7以前互換 
		if (point - abs((int)point) > 0.49) {
			return (int)point + 1;
		} else {
			return (int)point;
		}
	}
}

/**
 * フォントのピクセル数に対応するポイント数を整数で算出する。(Windows 8)
 *
 * @param font フォント情報
 * @param hWnd ウインドウハンドル
 * @return フォントサイズ
 */
double getFontPointDouble(LOGFONT* font, HWND hWnd)
{
	double point = getFontPoint(font, hWnd);

	if (WIN8_SIZE) {
		// Windows 8ディスプレイコントロールパネル互換
		if ((point > 10) || (point < 8)) {
			return point;
		}
		else {
			// 10ptまではWindows 7と同様に計算する。
			// Windows 7以前互換 
			if (point - abs((int)point) > 0.49) {
				return point + 1;
			}
			else {
				return point;
			}
		}
	}
	else {
		// Windows 7以前互換 
		if (point - abs((int)point) > 0.49) {
			return point + 1;
		}
		else {
			return point;
		}
	}
}

/**
 * フォントのピクセル数に対応するポイント数を算出する。
 * (Windows 7/コモンダイアログ互換)
 *
 * @param font フォント情報
 * @param hWnd ウインドウハンドル
 * @return フォントサイズ
 */
double getFontPoint(LOGFONT *font, HWND hWnd)
{
	// フォントを作成する。
	HFONT hFont = CreateFontIndirect(font);
	// 自身のウインドウハンドルから作成したデバイスコンテキストに
	// フォントを設定する。
	HDC dc = GetDC(hWnd);
	SelectFont(dc, hFont);

	// デバイスコンテキストからTEXTMETRICを取得する。
	TEXTMETRIC metric;
	GetTextMetrics(dc, &metric);

	int logPixelY = GetDeviceCaps(dc, LOGPIXELSY);

	ReleaseDC(hWnd, dc);
	DeleteObject(hFont);

	int height;
	if (font->lfHeight < 0) {
		// 負の場合はlfHeightはフォント自体の高さ。
		height = 0 - font->lfHeight;
	} else if (font->lfHeight > 0) {
		// 正の場合はすでにInternal Leadingを含んでいるのでその分を引く。
		height = font->lfHeight - metric.tmInternalLeading;
	} else {
		// 0の場合はデフォルトの大きさを作成したフォントから取得する。
		height = metric.tmAscent + metric.tmDescent - metric.tmInternalLeading;
	}

	double point = (double)height * 72 / logPixelY;

	return point;
}

/**
 * リソースの読み込みを行う。
 *
 * @param file リソースファイル名
 * @param key キー名
 * @param 読み込めなかった時の文字列
 */
void readResourceItem(TCHAR *file, TCHAR *key, TCHAR *fallback)
{
	tstring buf;
	int len;

	// INIファイルを読み込む。Unicode版のAPIでもファイルが非Unicodeの場合は
	// 各言語の文字コードのファイルとして読んでくれる。
	len = GetPrivateProfileStringExT("RESOURCE", key, _T(""), buf, file);

	if (len > 0) {
		langResource.push_back(buf);
	} else {
		langResource.push_back(fallback);
	}

}

/**
 * リソースの読み込みを行う(フォント名用)。
 *
 * @param buffer 格納先
 * @param file リソースファイル名
 * @param key キー名
 */
int readFontFace(std::vector<tstring> &buffer, TCHAR *file, TCHAR *key)
{
	tstring buf;
	int len;

	// INIファイルを読み込む。
	len = GetPrivateProfileStringExT(PRESET_SECTION, key, _T(""), buf, file);

	buffer.push_back(buf);

	return len;
}

/**
 * リソースの読み込みを行う(フォントサイズ用)。
 *
 * @param buffer 格納先
 * @param file リソースファイル名
 * @param key キー名
 */
int readFontSize(std::vector<int> &buffer, TCHAR *file, TCHAR *key)
{
	int size;

	// INIファイルを読み込む。Unicode版のAPIでもファイルが非Unicodeの場合は
	// 各言語の文字コードのファイルとして読んでくれる。
	size = GetPrivateProfileInt(_T(PRESET_SECTION), key, 0, file);
	buffer.push_back(size);

	return size;
}

/**
 * リソースの読み込みを行う(フォント文字セット用)。
 *
 * @param buffer 格納先
 * @param file リソースファイル名
 * @param key キー名
 */
int readFontCharset(std::vector<int> &buffer, TCHAR *file, TCHAR *key)
{
	int size;

	// INIファイルを読み込む。Unicode版のAPIでもファイルが非Unicodeの場合は
	// 各言語の文字コードのファイルとして読んでくれる。
	size = GetPrivateProfileInt(_T(PRESET_SECTION), key, 1, file);
	buffer.push_back(size);

	return size;
}

/**
 * Windows 8のフォントプリセットリソースの読み込みを行う
 *
 * @param file リソースファイル名
 * @return 0:設定失敗 1:設定成功
 */
int readFontResource8(TCHAR *file)
{
	int result;

	// フォント名
	result = readFontFace(fontFaces8, file, _T("CAPTION_FACE_8"));
	if (result == 0) {
		return 0;
	}
	result = readFontFace(fontFaces8, file, _T("ICON_FACE_8"));
	if (result == 0) {
		return 0;
	}
	result = readFontFace(fontFaces8, file, _T("SMALLCAPTION_FACE_8"));
	if (result == 0) {
		return 0;
	}
	result = readFontFace(fontFaces8, file, _T("STATUS_FACE_8"));
	if (result == 0) {
		return 0;
	}
	result = readFontFace(fontFaces8, file, _T("MESSAGE_FACE_8"));
	if (result == 0) {
		return 0;
	}
	result = readFontFace(fontFaces8, file, _T("MENU_FACE_8"));
	if (result == 0) {
		return 0;
	}

	// 文字サイズ
	result = readFontSize(fontSizes8, file, _T("CAPTION_SIZE_8"));
	if (result == 0) {
		return 0;
	}
	result = readFontSize(fontSizes8, file, _T("ICON_SIZE_8"));
	if (result == 0) {
		return 0;
	}
	result = readFontSize(fontSizes8, file, _T("SMALLCAPTION_SIZE_8"));
	if (result == 0) {
		return 0;
	}
	result = readFontSize(fontSizes8, file, _T("STATUS_SIZE_8"));
	if (result == 0) {
		return 0;
	}
	result = readFontSize(fontSizes8, file, _T("MESSAGE_SIZE_8"));
	if (result == 0) {
		return 0;
	}
	result = readFontSize(fontSizes8, file, _T("MENU_SIZE_8"));
	if (result == 0) {
		return 0;
	}

	// 文字セット
	readFontCharset(fontCharset8, file, _T("CAPTION_CHARSET_8"));
	readFontCharset(fontCharset8, file, _T("ICON_CHARSET_8"));
	readFontCharset(fontCharset8, file, _T("SMALLCAPTION_CHARSET_8"));
	readFontCharset(fontCharset8, file, _T("STATUS_CHARSET_8"));
	readFontCharset(fontCharset8, file, _T("MESSAGE_CHARSET_8"));
	readFontCharset(fontCharset8, file, _T("MENU_CHARSET_8"));

	return 1;
}

/**
 * Windows 10のフォントプリセットリソースの読み込みを行う
 *
 * @param file リソースファイル名
 * @return 0:設定失敗 1:設定成功
 */
int readFontResource10(TCHAR *file)
{
	int result;

	// フォント名
	result = readFontFace(fontFaces10, file, _T("CAPTION_FACE_10"));
	if (result == 0) {
		return 0;
	}
	result = readFontFace(fontFaces10, file, _T("ICON_FACE_10"));
	if (result == 0) {
		return 0;
	}
	result = readFontFace(fontFaces10, file, _T("SMALLCAPTION_FACE_10"));
	if (result == 0) {
		return 0;
	}
	result = readFontFace(fontFaces10, file, _T("STATUS_FACE_10"));
	if (result == 0) {
		return 0;
	}
	result = readFontFace(fontFaces10, file, _T("MESSAGE_FACE_10"));
	if (result == 0) {
		return 0;
	}
	result = readFontFace(fontFaces10, file, _T("MENU_FACE_10"));
	if (result == 0) {
		return 0;
	}

	// 文字サイズ
	result = readFontSize(fontSizes10, file, _T("CAPTION_SIZE_10"));
	if (result == 0) {
		return 0;
	}
	result = readFontSize(fontSizes10, file, _T("ICON_SIZE_10"));
	if (result == 0) {
		return 0;
	}
	result = readFontSize(fontSizes10, file, _T("SMALLCAPTION_SIZE_10"));
	if (result == 0) {
		return 0;
	}
	result = readFontSize(fontSizes10, file, _T("STATUS_SIZE_10"));
	if (result == 0) {
		return 0;
	}
	result = readFontSize(fontSizes10, file, _T("MESSAGE_SIZE_10"));
	if (result == 0) {
		return 0;
	}
	result = readFontSize(fontSizes10, file, _T("MENU_SIZE_10"));
	if (result == 0) {
		return 0;
	}

	// 文字セット
	readFontCharset(fontCharset10, file, _T("CAPTION_CHARSET_10"));
	readFontCharset(fontCharset10, file, _T("ICON_CHARSET_10"));
	readFontCharset(fontCharset10, file, _T("SMALLCAPTION_CHARSET_10"));
	readFontCharset(fontCharset10, file, _T("STATUS_CHARSET_10"));
	readFontCharset(fontCharset10, file, _T("MESSAGE_CHARSET_10"));
	readFontCharset(fontCharset10, file, _T("MENU_CHARSET_10"));

	return 1;
}

/**
 * Windows 11のフォントプリセットリソースの読み込みを行う
 *
 * @param file リソースファイル名
 * @return 0:設定失敗 1:設定成功
 */
int readFontResource11(TCHAR* file)
{
	int result;

	// フォント名
	result = readFontFace(fontFaces11, file, _T("CAPTION_FACE_11"));
	if (result == 0) {
		return 0;
	}
	result = readFontFace(fontFaces11, file, _T("ICON_FACE_11"));
	if (result == 0) {
		return 0;
	}
	result = readFontFace(fontFaces11, file, _T("SMALLCAPTION_FACE_11"));
	if (result == 0) {
		return 0;
	}
	result = readFontFace(fontFaces11, file, _T("STATUS_FACE_11"));
	if (result == 0) {
		return 0;
	}
	result = readFontFace(fontFaces11, file, _T("MESSAGE_FACE_11"));
	if (result == 0) {
		return 0;
	}
	result = readFontFace(fontFaces11, file, _T("MENU_FACE_11"));
	if (result == 0) {
		return 0;
	}

	// 文字サイズ
	result = readFontSize(fontSizes11, file, _T("CAPTION_SIZE_11"));
	if (result == 0) {
		return 0;
	}
	result = readFontSize(fontSizes11, file, _T("ICON_SIZE_11"));
	if (result == 0) {
		return 0;
	}
	result = readFontSize(fontSizes11, file, _T("SMALLCAPTION_SIZE_11"));
	if (result == 0) {
		return 0;
	}
	result = readFontSize(fontSizes11, file, _T("STATUS_SIZE_11"));
	if (result == 0) {
		return 0;
	}
	result = readFontSize(fontSizes11, file, _T("MESSAGE_SIZE_11"));
	if (result == 0) {
		return 0;
	}
	result = readFontSize(fontSizes11, file, _T("MENU_SIZE_11"));
	if (result == 0) {
		return 0;
	}

	// 文字セット
	readFontCharset(fontCharset11, file, _T("CAPTION_CHARSET_11"));
	readFontCharset(fontCharset11, file, _T("ICON_CHARSET_11"));
	readFontCharset(fontCharset11, file, _T("SMALLCAPTION_CHARSET_11"));
	readFontCharset(fontCharset11, file, _T("STATUS_CHARSET_11"));
	readFontCharset(fontCharset11, file, _T("MESSAGE_CHARSET_11"));
	readFontCharset(fontCharset11, file, _T("MENU_CHARSET_11"));

	return 1;
}

/**
 * ウインドウを親ウインドウの中央に配置する。
 *
 * @param parentRect 中心に入れる対象の領域
 * @param parentHWnd 親ウインドウハンドル
 * @param myHWnd 中央寄せする要素のウインドウハンドル
 */
void adjustCenter(RECT parentRect, HWND parentHWnd, HWND myHWnd)
{
	int parentWidth, parentHeight;
	int myWidth, myHeight;
	int newTop, newLeft;
	RECT myRect;

	GetWindowRect(myHWnd, &myRect);

	parentWidth = parentRect.right - parentRect.left + 1;
	parentHeight = parentRect.bottom - parentRect.top + 1;

	myWidth = myRect.right - myRect.left + 1;
	myHeight = myRect.bottom - myRect.top + 1;

	double scale = (double)getSystemDPI() / 96;
	int targetWidth = REQUIRED_CLIENT_WIDTH * scale;
	int targetHeight = REQUIRED_CLIENT_HEIGHT * scale;

	if (myWidth >= parentWidth) {
		newLeft = parentRect.left + 1;
	} else {
		newLeft = parentRect.left + MulDiv((parentWidth - myWidth), 45, 100);
	}
	if (myHeight >= parentHeight) {
		newTop = parentRect.top + 1;
	} else {
		newTop = parentRect.top + MulDiv((parentHeight - myHeight), 45, 100);
	}

	SetWindowPos(myHWnd, parentHWnd, newLeft, newTop, myWidth, myHeight, SWP_SHOWWINDOW);

}

/**
 * 文字列を置き換える
 *
 * @param buf 置き換えられた文字列
 * @param source 置き換える対象の文字列
 * @param oldWord 置換される文字列
 * @param newWord 置換する文字列
 * @param bufLen 置き換え先文字列のバッファサイズ
 *
 */
void strreplace(TCHAR* buf, const TCHAR* source, const TCHAR* oldWord, const TCHAR* newWord, int bufLen)
{
	int p = 0;
	int newP = 0;
	int oldLen;
	int newLen;
	int copyLen;

	oldLen = _tcslen(oldWord);
	newLen = _tcslen(newWord);

	while (*(source + p) != _T('\0')) {
		if (_tcsncmp(source + p, oldWord, oldLen) == 0) {
			if ((newP + newLen) < bufLen) {
				_tcscpy(buf + newP, newWord);
				newP += newLen;
			} else {
				copyLen = bufLen - newP;
				_tcsncpy(buf + newP, newWord, copyLen - 1);
				newP += (copyLen - 1);
			}
			p += oldLen;
		} else {
			if (newP >= bufLen) {
				return;
			} else if (newP == (bufLen - 1)) {
				*(buf + newP) = _T('\0');
				return;
			} else {
				*(buf + newP) = *(source + p);
				newP++;
				p++;
			}
		}
	}
	if (newP < bufLen) {
		*(buf + newP) = _T('\0');
	}
}

/**
 * Windows 11かどうか判別する。
 *
 * @param buildNumber ビルド番号格納先
 * @return TRUE:Windows 11 or later FALSE:Windows 10
 */
BOOL isWin11OrLater(DWORD &buildNumber)
{
	OSVERSIONINFOEX versionInfo;
	DWORDLONG dwlConditionMask = 0;
	int op = VER_GREATER_EQUAL;

	// Initialize the OSVERSIONINFOEX structure.

	ZeroMemory(&versionInfo, sizeof(OSVERSIONINFOEX));
	versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	versionInfo.dwMajorVersion = 10;
	versionInfo.dwMinorVersion = 0;
	versionInfo.wServicePackMajor = 0;
	versionInfo.wServicePackMinor = 0;
	versionInfo.dwBuildNumber = 22000;

#if 0
	// Initialize the condition mask.

	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, op);
	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, op);
	VER_SET_CONDITION(dwlConditionMask, VER_SERVICEPACKMAJOR, op);
	VER_SET_CONDITION(dwlConditionMask, VER_SERVICEPACKMINOR, op);
	VER_SET_CONDITION(dwlConditionMask, VER_BUILDNUMBER, op);

	// Perform the test.

	return VerifyVersionInfo(
		&osvi,
		VER_MAJORVERSION | VER_MINORVERSION |
		VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR | VER_BUILDNUMBER,
		dwlConditionMask);
#endif

	GetVersionEx((OSVERSIONINFO *)&versionInfo);

	buildNumber = versionInfo.dwBuildNumber;
	if (versionInfo.dwBuildNumber >= 22000) {	// 22000 is first version(21H2)
		return TRUE;
	} else{
		return FALSE;
	}
}

/**
 * アプリケーションが実用するWindowsのバージョンを返す(Windows 11対応)<br>
 * Windows 11はメジャーバージョン11扱いとする。その後のWindowsがメジャーバージョン11返したらその時は10.xx扱いにする。<br>
 * Windows 7(6.1)/8(6.2)/8.1(6.3)は既存コードとの兼ね合いからそのままとする。
 * 
 * @return 上位16ビット：メジャーバージョン 下位16ビット：マイナーバージョン
 */
DWORD GetVersionForApp(DWORD& majorVersion, DWORD& minorVersion, DWORD &buildNumber)
{
	DWORD dwVersion = GetVersion();

	majorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	minorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

	if (majorVersion == 10 && isWin11OrLater(buildNumber)) {
		// Windows 11が普通にやると10返すのでここで１１と判別
		majorVersion = 11;
	}

	return (majorVersion << 16) | minorVersion;
}

/**
 * charをunsignedとみなしてintに変換する
 * 
 * @param c 変換するchar
 * @return cをunsignedとみなして変換した値
 */
inline int charToInt(char c)
{
	if (c < 0) {
		return 256 + c;
	}
	else {
		return c;
	}

}

/**
 * UTF-8文字列をUTF-16文字列に変換する
 *
 * @param src 変換元UTF-8文字列
 * @param dst 変換先領域(NULL可能)
 * @return 必要となるwchar_tの個数
 */
size_t utf8toUtf16(tstring &dst, const char* src)
{
	wchar_t wc;
	int shift_count;
	int i;

	assert(src != NULL);

	while (*src) {
		// Extruct to Unicode
		int c = charToInt(*src);

		wc = 0;
		if (c < 0x80) {
			// 1byte(7bit)
			wc = c;
			shift_count = 0;
		}
		else if (c >= 0xfc) {
			// 6bytes(27-32bit:no use)
			wc = c & 0x01;
			shift_count = 5;
		}
		else if (c >= 0xf8) {
			// 5bytes(22-26bit:no use)
			wc = c & 0x03;
			shift_count = 4;
		}
		else if (c >= 0xf0) {
			// 4bytes(17-21bit)
			wc = c & 0x07;
			shift_count = 3;
		}
		else if (c >= 0xe0) {
			// 3bytes(12-16bit)
			wc = c & 0x0f;
			shift_count = 2;
		}
		else if (c >= 0xc0) {
			// 2bytes(8-11bit)
			wc = c & 0x1f;
			shift_count = 1;
		}
		else {
			break;
		}

		src++;
		for (i = 0; i < shift_count; i++) {
			if (*src == '\0') {
				wc = L'\0';
				break;
			}
			c = charToInt(*src);

			wc = wc << 6;
			wc = wc | (c & 0x3f);
			src++;
		}

		if (c >= 0x10000) {
			// Windows uses UTF-16 LE
			int c1 = (c & 0x3ff) | 0xdc00;
			int c2_1 = (c >> 10) | 0x3f;
			int c2_2 = ((c >> 16) & 0x1f) - 1;
			int c2 = 0xd800 | c2_2 | c2_1;

			dst += c1;
			dst += c2;
		}
		else {
			dst += wc;
		}
	}

	return dst.length();
}

/**
 * システムのDPIを取得する。
 * 
 * @return システムのDPI
 */
int getSystemDPI(void)
{
	HDC hDc = GetDC(GetDesktopWindow());
	int dpi = GetDeviceCaps(hDc, LOGPIXELSY);

	return dpi;
}

/**
 * プリセット設定
 * 
 * @brief プリセットを設定する
 * @param metrics 設定対象NONCLIENTMETRICS
 * @param iconFont 設定対象アイコンフォント
 * @param fontFaces フォント名
 * @param fontSizes フォントサイズ設定
 * @param fontCharset フォント文字セット設定
 * @param windowMetric フォント以外のWindowMetricの設定
 * @param dpiY 現在のDPI
 */
void setPreset(
	NONCLIENTMETRICS *metrics,
	LOGFONTW *iconFont,
	std::vector<tstring>& fontFaces,
	std::vector<int>& fontSizes,
	std::vector<int>& fontCharset,
	double *windowMetric,
	int dpiY)
{
	// フォント以外のNONCLIENTMETRICSの現在値を保持するため、
	// NONCLIENTMETRICSの内容を取得しなおす。
	FillMemory(metrics, sizeof(NONCLIENTMETRICS), 0x00);
	metrics->cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metrics,
		0);

	memset(&metrics->lfCaptionFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics->lfCaptionFont.lfFaceName, fontFaces[0].c_str());
	metrics->lfCaptionFont.lfHeight = -MulDiv(fontSizes[0], dpiY, 72);
	metrics->lfCaptionFont.lfWeight = 400;
	metrics->lfCaptionFont.lfCharSet = fontCharset[0];
	metrics->lfCaptionFont.lfQuality = 5;

	memset(iconFont, 0, sizeof(LOGFONTW));
	_tcscpy(iconFont->lfFaceName, fontFaces[1].c_str());
	iconFont->lfHeight = -MulDiv(fontSizes[1], dpiY, 72);
	iconFont->lfWeight = 400;
	iconFont->lfCharSet = fontCharset[1];
	iconFont->lfQuality = 5;

	memset(&(metrics->lfSmCaptionFont), 0, sizeof(LOGFONTW));
	_tcscpy(metrics->lfSmCaptionFont.lfFaceName, fontFaces[2].c_str());
	metrics->lfSmCaptionFont.lfHeight = -MulDiv(fontSizes[2], dpiY, 72);
	metrics->lfSmCaptionFont.lfWeight = 400;
	metrics->lfSmCaptionFont.lfCharSet = fontCharset[2];
	metrics->lfSmCaptionFont.lfQuality = 5;

	memset(&(metrics->lfStatusFont), 0, sizeof(LOGFONTW));
	_tcscpy(metrics->lfStatusFont.lfFaceName, fontFaces[3].c_str());
	metrics->lfStatusFont.lfHeight = -MulDiv(fontSizes[3], dpiY, 72);
	metrics->lfStatusFont.lfWeight = 400;
	metrics->lfStatusFont.lfCharSet = fontCharset[3];
	metrics->lfStatusFont.lfQuality = 5;

	memset(&(metrics->lfMessageFont), 0, sizeof(LOGFONTW));
	_tcscpy(metrics->lfMessageFont.lfFaceName, fontFaces[4].c_str());
	metrics->lfMessageFont.lfHeight = -MulDiv(fontSizes[4], dpiY, 72);
	metrics->lfMessageFont.lfWeight = 400;
	metrics->lfMessageFont.lfCharSet = fontCharset[4];
	metrics->lfMessageFont.lfQuality = 5;

	memset(&(metrics->lfMenuFont), 0, sizeof(LOGFONTW));
	_tcscpy(metrics->lfMenuFont.lfFaceName, fontFaces[5].c_str());
	metrics->lfMenuFont.lfHeight = -MulDiv(fontSizes[5], dpiY, 72);
	metrics->lfMenuFont.lfWeight = 400;
	metrics->lfMenuFont.lfCharSet = fontCharset[5];
	metrics->lfMenuFont.lfQuality = 5;

	// 枠線等のメトリックも元に戻す。その際、DPIを反映する必要がある。
	metrics->iBorderWidth = round(windowMetric[BorderWidth] * ((double)dpiY / 96.0));
	metrics->iCaptionWidth = round(windowMetric[TitleWidth] * ((double)dpiY / 96.0));
	metrics->iCaptionHeight = round(windowMetric[TitleHeight] * ((double)dpiY / 96.0));
	metrics->iScrollWidth = round(windowMetric[ScrollWidth] * ((double)dpiY / 96.0));
	metrics->iScrollHeight = round(windowMetric[ScrollHeight] * ((double)dpiY / 96.0));
	metrics->iSmCaptionWidth = round(windowMetric[PaletteWidth] * ((double)dpiY / 96.0));
	metrics->iSmCaptionHeight = round(windowMetric[PaletteHeight] * ((double)dpiY / 96.0));
	metrics->iMenuWidth = round(windowMetric[MenuWidth] * ((double)dpiY / 96.0));
	metrics->iMenuHeight = round(windowMetric[MenuHeight] * ((double)dpiY / 96.0));
	metrics->iPaddedBorderWidth = round(windowMetric[Padding] * ((double)dpiY / 96.0));

}

/**
 * @brief コマンドライン引数を展開する
 * @return コマンドライン引数配列
 */
std::vector<tstring *> *extractArguments()
{
	TCHAR *argument = GetCommandLine();
	std::vector<tstring *> *argv = new std::vector<tstring *>();

	TCHAR delim = _T('\0');
	tstring *splitted = nullptr;

	while (*argument) {
		if (delim == _T('\0')) {
			// Quote not started
			if (_istspace(*argument)) {
				if (splitted != nullptr) {
					// Argument end
					argv->push_back(splitted);
					splitted = nullptr;
				}
			}
			else {
				switch (*argument) {
					case _T('\''):
						// Quote start
						if (splitted == nullptr) {
							delim = *argument;
						} else {
							splitted->push_back(*argument);
						}
						break;
					case _T('\"'):
						// Quote start
						if (splitted == nullptr) {
							delim = *argument;
						} else {
							splitted->push_back(*argument);
						}
						break;
					default:
						if (splitted == nullptr) {
							// argument start
							splitted = new tstring();
						}
						splitted->push_back(*argument);
						break;
				}
			}
		} else {
			if (*argument == delim) {
				if (splitted == nullptr) {
					// argument start
					splitted = new tstring();
				}
				argv->push_back(splitted);
				splitted = nullptr;
				delim = _T('\0');
			} else {
				if (splitted == nullptr) {
					// argument start
					splitted = new tstring();
				}
				splitted->push_back(*argument);
			}
		}
		argument++;
	}
	if (splitted != nullptr) {
		argv->push_back(splitted);
	}

	return argv;
}

/**
 * @brief 引数vectorを解放する
 * @param arguments 引数vector
 */
void deleteArguments(std::vector<tstring *> *arguments)
{
	for (size_t i = 0; i < arguments->size(); i++) {
		if ((*arguments)[i] != nullptr) {
			delete (*arguments)[i];
		}
	}

	delete arguments;
}
