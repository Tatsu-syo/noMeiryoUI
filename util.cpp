/*
noMeiryoUI (C) 2005,2012-2015 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/

#include "util.h"

/**
 * フォントサイズを整数で算出する。(Windows 8)
 *
 * @param font フォント情報
 * @param hWnd ウインドウハンドル
 * @return フォントサイズ
 */
int getFontPointInt(LOGFONT *font, HWND hWnd)
{
	double point = getFontPoint(font, hWnd);

	if (point - abs((int)point) > 0.74) {
		return (int)point + 1;
	} else {
		return (int)point;
	}
}

/**
 * フォントサイズを算出する。(Windows 7/コモンダイアログ互換)
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

