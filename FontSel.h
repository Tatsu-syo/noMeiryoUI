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
#pragma once

#include <vector>
#include <windows.h>
#include "Resource.h"
#include "basedialog.h"
#include "TwrCombobox.h"
#include "TwrCheckbox.h"

struct FontInfo {
	LOGFONT logFont;
	std::vector<int> charsetList;
};

class FontSel :
	public BaseDialog
{
private:
	TwrCombobox *m_fontNameList;
	TwrCombobox *m_fontSizeList;
	TwrCombobox *m_ChersetList;
	TwrCombobox *m_styleList;
	TwrCheckbox *m_underline;
	TwrCheckbox *m_strike;

	LOGFONT selectedFont;

	void setCharset(void);
	void setStyle(void);
	INT_PTR onOK(void);

protected:
	INT_PTR OnInitDialog();
	INT_PTR OnCommand(WPARAM wParam);

public:
	FontSel(HWND parent, int resource);
	virtual ~FontSel(void);

	LOGFONT getSelectedFont() {
		return selectedFont;
	}
	void setNoMeiryoUI();

};
