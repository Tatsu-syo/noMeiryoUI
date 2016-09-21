/*
noMeiryoUI (C) 2005,2012-2015 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#ifndef NCFileDialog_H
#define NCFileDialog_H

class NCFileDialog {
private:
	TCHAR gotFileName[_MAX_PATH];
	BOOL m_OpenFileDialog;
	const char *m_defExt;
public:
	OPENFILENAME m_ofn;

	NCFileDialog(
		BOOL bOpenFileDialog,
		const char *lpszDefExt = NULL,
		const TCHAR *lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		const TCHAR *lpszFilter = NULL,
		HWND pParentWnd = NULL,
		DWORD dwSize = 0
	);

	int DoModal();
	TCHAR *GetPathName() { return gotFileName;};
	OPENFILENAME &GetOFN() {
		return m_ofn;
	}
};

#endif
