/*
noMeiryoUI (C) 2005,2012-2022 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#ifndef UTIL_H
#define UTIL_H

#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <windows.h>
#include <windowsx.h>
#include <vector>
#include "TWR/tstring.h"

#define PRESET_SECTION "PRESET"
#define APP_FONTSIZE 11

/**
 * @brief WindowMetric定数
*/
enum WindowMetricItem {
	BorderWidth,
	TitleWidth,
	TitleHeight,
	ScrollWidth,
	ScrollHeight,
	PaletteWidth,
	PaletteHeight,
	MenuWidth,
	MenuHeight,
	Padding,
};

enum Country {
	NoCountry,
	Japan,
	Korea
};

/** 必要なクライアント領域幅(96DPI) */
const int REQUIRED_CLIENT_WIDTH = 615;
/** 必要なクライアント領域高さ(96DPI) */
const int REQUIRED_CLIENT_HEIGHT = 400;

extern bool WIN8_SIZE;
extern std::vector<tstring> langResource;
/**
 * @brief 動作中の国
 */
extern Country runningCountry;

/** フォント名(Windows 8.x) */
extern std::vector<tstring> fontFaces8;
/** フォントサイズ(Windows 8.x) */
extern std::vector<int> fontSizes8;
/** フォント文字セット(Windows 8.x) */
extern std::vector<int> fontCharset8;
/** Windows 8のWindow Metricのプリセット */
extern double Win8PresetWindowsMetric[];

/** フォント名(Windows 10) */
extern std::vector<tstring> fontFaces10;
/** フォントサイズ(Windows 10) */
extern std::vector<int> fontSizes10;
/** フォント文字セット(Windows 10) */
extern std::vector<int> fontCharset10;
/** Windows 10のWindow Metricのプリセット */
extern double Win10PresetWindowsMetric[];

/** フォント名(Windows 11) */
extern std::vector<tstring> fontFaces11;
/** フォントサイズ(Windows 11) */
extern std::vector<int> fontSizes11;
/** フォント文字セット(Windows 11) */
extern std::vector<int> fontCharset11;
/** Windows 11のWindow Metricのプリセット */
extern double Win11PresetWindowsMetric[];

/** コードページ */
extern int codePage;

int getFontPointInt(LOGFONT *font, HWND hWnd);
double getFontPoint(LOGFONT *font, HWND hWnd);
void readResourceItem(TCHAR *file, TCHAR *key, TCHAR *fallback);
int readFontResource8(TCHAR *file);
int readFontResource10(TCHAR *file);
int readFontResource11(TCHAR* file);
void setLocationInternalCode(TCHAR langWork[85]);
void adjustCenter(RECT parentRect, HWND parentHWnd, HWND myHWnd);
void strreplace(TCHAR* buf, const TCHAR* source, const TCHAR* oldWord, const TCHAR* newWord, int bufLen);
BOOL isWin11OrLater(DWORD& buildNumber);
DWORD GetVersionForApp(DWORD &majorVersion, DWORD &minorVersion, DWORD& buildNumber);
size_t utf8toUtf16(tstring& dst, const char* src);
int getSystemDPI(void);
double getFontPointDouble(LOGFONT* font, HWND hWnd);
std::vector<tstring *> *extractArguments();
void deleteArguments(std::vector<tstring *> *arguments);

#if _MSC_VER < 1800

double round(double x);

#endif

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
	std::vector<tstring> &fontFaces,
	std::vector<int> &fontSizes,
	std::vector<int> &fontCharset,
	double *windowMetric,
	int dpiY);

#endif
