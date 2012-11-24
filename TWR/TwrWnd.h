#ifndef TWRWND_H
#define TWRWND_H

#include <windows.h>

#include "tstring.h"

/**
 * Tatsu's windows api WRapper<br>
 * Window base class
 */
class TwrWnd {
protected:
	HWND hWnd;
	tstring title;
	int left;
	int top;
	int width;
	int height;
public:
	TwrWnd();
	TwrWnd(HWND newWnd);
	virtual ~TwrWnd();

	BOOL UpdateWindow();
	BOOL ShowWindow(int nCmdShow);
	void EnableWindow(BOOL enabled);
	HWND getHwnd() {
		return hWnd;
	}
	void destroyWindow(void) {
		DestroyWindow(hWnd);
	}
	void setText(const TCHAR *text);
	void setPoint(int x, int y);
	void setSize(int w, int h);
	void setFont(HFONT font);
};

#endif