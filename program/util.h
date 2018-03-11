/*
noMeiryoUI (C) 2005,2012-2017 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <vector>
#include "TWR/tstring.h"

#define PRESET_SECTION "PRESET"

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

int getFontPointInt(LOGFONT *font, HWND hWnd);
double getFontPoint(LOGFONT *font, HWND hWnd);
void readResourceFile(TCHAR *file);
int readFontResource8(TCHAR *file);
int readFontResource10(TCHAR *file);
int setFontResourceJa8(void);
int setFontResourceJa10(void);
void adjustCenter(RECT parentRect, HWND parentHWnd, HWND myHWnd);

#endif
