/*
noMeiryoUI (C) 2005,2012-2025 Tatsuhiko Shoji
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
 * @brief WindowMetric�萔
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

/** �K�v�ȃN���C�A���g�̈敝(96DPI) */
const int REQUIRED_CLIENT_WIDTH = 615;
/** �K�v�ȃN���C�A���g�̈捂��(96DPI) */
const int REQUIRED_CLIENT_HEIGHT = 400;

extern bool WIN8_SIZE;
extern std::vector<tstring> langResource;
/**
 * @brief ���쒆�̍�
 */
extern Country runningCountry;

/** �t�H���g��(Windows 8.x) */
extern std::vector<tstring> fontFaces8;
/** �t�H���g�T�C�Y(Windows 8.x) */
extern std::vector<int> fontSizes8;
/** �t�H���g�����Z�b�g(Windows 8.x) */
extern std::vector<int> fontCharset8;
/** Windows 8��Window Metric�̃v���Z�b�g */
extern double Win8PresetWindowsMetric[];

/** �t�H���g��(Windows 10) */
extern std::vector<tstring> fontFaces10;
/** �t�H���g�T�C�Y(Windows 10) */
extern std::vector<int> fontSizes10;
/** �t�H���g�����Z�b�g(Windows 10) */
extern std::vector<int> fontCharset10;
/** Windows 10��Window Metric�̃v���Z�b�g */
extern double Win10PresetWindowsMetric[];

/** �t�H���g��(Windows 11) */
extern std::vector<tstring> fontFaces11;
/** �t�H���g�T�C�Y(Windows 11) */
extern std::vector<int> fontSizes11;
/** �t�H���g�����Z�b�g(Windows 11) */
extern std::vector<int> fontCharset11;
/** Windows 11��Window Metric�̃v���Z�b�g */
extern double Win11PresetWindowsMetric[];

/** �R�[�h�y�[�W */
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
void getApplicationSettingFolder(tstring& folder);

#if _MSC_VER < 1800

double round(double x);

#endif

/**
 * �v���Z�b�g�ݒ�
 *
 * @brief �v���Z�b�g��ݒ肷��
 * @param metrics �ݒ�Ώ�NONCLIENTMETRICS
 * @param iconFont �ݒ�ΏۃA�C�R���t�H���g
 * @param fontFaces �t�H���g��
 * @param fontSizes �t�H���g�T�C�Y�ݒ�
 * @param fontCharset �t�H���g�����Z�b�g�ݒ�
 * @param windowMetric �t�H���g�ȊO��WindowMetric�̐ݒ�
 * @param dpiY ���݂�DPI
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
