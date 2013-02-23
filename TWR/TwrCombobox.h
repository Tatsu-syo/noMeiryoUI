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
#ifndef TWRCOMBOBOX_H
#define TWRCOMBOBOX_H

#include "TwrWnd.h"

class TwrCombobox : public TwrWnd {
private:
	int style;
public:
	TwrCombobox();
	TwrCombobox(HWND newHwnd);

	HWND create(void);
	HWND create(HWND parent);
	void addItem(const TCHAR *item);
	int getSelectedIndex(void);
	void setStyle(int newStyle) {
		style = newStyle;
	}
	void clear(void);
	void setSelectedIndex(int index);
	tstring getSelectedText(void);

};

#endif
