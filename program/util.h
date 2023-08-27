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

#define DLG_STYLE_NORMAL 55
#define DLG_STYLE_ITALIC 56
#define DLG_STYLE_BOLD 57
#define DLG_STYLE_BOLD_ITALIC 58
#define DLG_STYLE_MEDIUM 74
#define DLG_STYLE_SEMI_BOLD 75
#define DLG_STYLE_BOOK 76
#define DLG_STYLE_OBLIQUE 77
#define DLG_STYLE_LIGHT 78
#define DLG_STYLE_EXTRA_LIGHT 79
#define DLG_STYLE_SEMI_LIGHT 80
#define DLG_STYLE_SLANTED 81
#define DLG_CHECK_BOLD 82
#define DLG_CHECK_ITALIC 83
#define MSG_WARNING 84
#define MSG_WIN11_22H2RESTRICTION 85

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


/** 必要なクライアント領域幅(96DPI) */
const int REQUIRED_CLIENT_WIDTH = 615;
/** 必要なクライアント領域高さ(96DPI) */
const int REQUIRED_CLIENT_HEIGHT = 400;

extern bool WIN8_SIZE;
extern std::vector<tstring> langResource;

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
/** 韓国語環境かどうか */
extern bool isKorean;

int getFontPointInt(LOGFONT *font, HWND hWnd);
double getFontPoint(LOGFONT *font, HWND hWnd);
void readResourceFile(TCHAR *file);
int readFontResource8(TCHAR *file);
int readFontResource10(TCHAR *file);
int readFontResource11(TCHAR* file);
void adjustCenter(RECT parentRect, HWND parentHWnd, HWND myHWnd);
void getKoreanFontName(TCHAR *dispBuf);
void strreplace(TCHAR* buf, const TCHAR* source, const TCHAR* oldWord, const TCHAR* newWord, int bufLen);
BOOL isWin11OrLater(DWORD& buildNumber);
DWORD GetVersionForApp(DWORD &majorVersion, DWORD &minorVersion, DWORD& buildNumber);
size_t utf8toUtf16(tstring& dst, const char* src);
int getSystemDPI(void);
double getFontPointDouble(LOGFONT* font, HWND hWnd);

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
