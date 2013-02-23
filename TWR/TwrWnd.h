/*
This file is part of noMeiryoUI
Copyright (C) 2005,2012,2013 Tatsuhiko Shoji

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
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