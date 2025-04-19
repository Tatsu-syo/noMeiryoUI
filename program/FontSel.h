/*
noMeiryoUI (C) 2005,2012-2022 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#pragma once

#include <vector>
#include <windows.h>
#include "Resource.h"
#include "basedialog.h"
#include "TwrCombobox.h"
#include "TwrCheckbox.h"

/** スタイルの情報 */
struct TypeInfo {
	TCHAR typeName[128];
	LOGFONT logFont;
};

/** 文字セットと対応するスタイルの情報 */
struct CharsetInfo {
	int charset;
	std::vector<struct TypeInfo> fonts;
};

/** フォントの情報 */
struct FontInfo {
	LOGFONT logFont;
	// std::vector<struct CharsetInfo> charsetList;
	TCHAR dispName[32];
	TCHAR fullName[LF_FULLFACESIZE];
};

class FontSel :
	public BaseDialog
{
private:
	TwrCombobox *m_fontNameList;
	TwrCombobox *m_fontSizeList;
	TwrCombobox *m_ChersetList;
	TwrCombobox *m_styleList;
	TwrCheckbox* m_bold;
	TwrCheckbox* m_italic;
	TwrCheckbox *m_underline;
	TwrCheckbox *m_strike;

	HFONT displayFont;

	LOGFONT selectedFont;
	LOGFONT *previousFont;
	double m_point;
	bool m_fontonly;

	void adjustPosition(void);
	void applyResource();
	void setCharset(void);
	void setStyle();
	INT_PTR onOK(void);
	void disableWithoutFontselect(void);

protected:
	INT_PTR OnInitDialog();
	INT_PTR OnCommand(WPARAM wParam);

public:
	FontSel(HWND parent, int resource, bool fontOnly = false);
	virtual ~FontSel(void);

	LOGFONT getSelectedFont() {
		return selectedFont;
	}
	void setPreviousFont(LOGFONT *item) {
		previousFont = item;
	}
	void setNoMeiryoUI();
	void setNoTahoma();

	/**
	 * ポイント数を取得する。
	 * 
	 * @return 選択されたポイント数
	 */
	double getPoint(void) {
		return m_point;
	}

	/**
	 * ポイント数を設定する。(初期表示用)
	 * 
	 * @param point ポイント数
	 */
	void setPoint(double point) {
		m_point = point;
	}

};

void copyTypeInfo(TypeInfo& typeInfo, ENUMLOGFONTEX* lpelfe);
