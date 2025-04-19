/*
noMeiryoUI (C) 2005,2012-2024 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#include <string.h>
#include <tchar.h>
#include <tstring.h>

namespace korea {

	/**
	 * 韓国語のフォント名を置き換える(韓国語専用)
	 * 
	 * @param dispBuf 韓国語のフォント名
	 */
	void getKoreanFontName(TCHAR *dispBuf)
	{
		if (!_tcscmp(dispBuf, _T("Gulim"))) {
			_tcscpy(dispBuf, _T("굴림"));
		}

		if (!_tcscmp(dispBuf, _T("GulimChe"))) {
			_tcscpy(dispBuf, _T("굴림체"));
		}

		if (!_tcscmp(dispBuf, _T("Gungsuh"))) {
			_tcscpy(dispBuf, _T("궁서"));
		}

		if (!_tcscmp(dispBuf, _T("GungsuhChe"))) {
			_tcscpy(dispBuf, _T("궁서체"));
		}

		if (!_tcscmp(dispBuf, _T("Dotum"))) {
			_tcscpy(dispBuf, _T("돋움"));
		}

		if (!_tcscmp(dispBuf, _T("DotumChe"))) {
			_tcscpy(dispBuf, _T("돋움체"));
		}

		if (!_tcscmp(dispBuf, _T("Malgun Gothic"))) {
			_tcscpy(dispBuf, _T("맑은 고딕"));
		}

		if (!_tcscmp(dispBuf, _T("Batang"))) {
			_tcscpy(dispBuf, _T("바탕"));
		}

		if (!_tcscmp(dispBuf, _T("BatangChe"))) {
			_tcscpy(dispBuf, _T("바탕체"));
		}

	}

}

