/*
noMeiryoUI (C) 2005,2012-2015 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#ifndef MYAPPLI_H
#define MYAPPLI_H

#include "DialogAppliBase.h"
#include "TwrMenu.h"

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

	bool noMeiryoUI;
	TwrWnd *verInfo;

	TwrMenu *appMenu;

	void OnLoad();
	BOOL startLoadFont(TCHAR *filename);
	BOOL loadFont(TCHAR *filename, TCHAR *section, LOGFONT *font);
	void OnSave();
	BOOL startSaveFont(TCHAR *filename);
	BOOL saveFont(TCHAR *filename, TCHAR *section, LOGFONT *font);
	void selectFont(enum fontType type);
	void updateDisplay(void);

	INT_PTR OnBnClickedOk();
	void OnBnClickedAll();
	void OnSet8();
	void SetWinVer();

	int getFontPointInt(LOGFONT *font);
	double getFontPoint(LOGFONT *font);
	void setFont(
		NONCLIENTMETRICS *fontMetrics,
		LOGFONT *iconLogFont
	);
	void showHelp(void);

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
