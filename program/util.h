/*
noMeiryoUI (C) 2005,2012-2020 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#ifndef UTIL_H
#define UTIL_H

#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

extern bool WIN8_SIZE;
extern std::vector<tstring> langResource;
/** フォント名(Windows 8.x) */
extern std::vector<tstring> fontFaces8;
/** フォントサイズ(Windows 8.x) */
extern std::vector<int> fontSizes8;
/** フォント文字セット(Windows 8.x) */
extern std::vector<int> fontCharset8;
/** フォント名(Windows 10) */
extern std::vector<tstring> fontFaces10;
/** フォントサイズ(Windows 8.x) */
extern std::vector<int> fontSizes10;
/** フォント文字セット(Windows 10) */
extern std::vector<int> fontCharset10;
/** コードページ */
extern int codePage;
/** 韓国語環境かどうか */
extern bool isKorean;

int getFontPointInt(LOGFONT *font, HWND hWnd);
double getFontPoint(LOGFONT *font, HWND hWnd);
void readResourceFile(TCHAR *file);
int readFontResource8(TCHAR *file);
int readFontResource10(TCHAR *file);
int setFontResourceJa8(void);
int setFontResourceJa10(void);
void adjustCenter(RECT parentRect, HWND parentHWnd, HWND myHWnd);
void getKoreanFontName(TCHAR *dispBuf);
void strreplace(TCHAR* buf, const TCHAR* source, const TCHAR* oldWord, const TCHAR* newWord, int bufLen);

#endif
