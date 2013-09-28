/*
noMeiryoUI (C) 2005,2012,2013 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#ifndef MYAPPLI_H
#define MYAPPLI_H

#include "DialogAppliBase.h"

class NoMeiryoUI : public DialogAppliBase {
private:

	enum fontType {
		all,
		title,
		icon,
		palette,
		hint,
		message,
		menu
	};

	tstring allFontName;
	tstring titleFontName;
	tstring iconFontName;
	tstring paletteFontName;
	tstring hintFontName;
	tstring messageFontName;
	tstring menuFontName;

	NONCLIENTMETRICS metrics;
	LOGFONT iconFont;

	NONCLIENTMETRICS metricsAll;
	LOGFONT iconFontAll;

	TwrWnd *verInfo;

	void selectFont(enum fontType type);
	INT_PTR OnBnClickedOk();
	void OnBnClickedAll();
	void SetWinVer();

protected:
	INT_PTR OnCommand(WPARAM wParam);

public:
	BaseDialog *createBaseDialog();
	int OnAppliStart(TCHAR *lpCmdLine);
	int OnWindowShow();
	int OnAppliEnd();
	INT_PTR OnInitDialog();
	void UpdateData(bool toObj);

};

#include	<windows.h>
#include	<sddl.h>
#include	<tchar.h>

TCHAR *getSID(void);

#endif
