#include <windows.h>
#include "japan.h"

namespace japan {

	/**
	 * @brief 必要とされる文字列
	*/
	tstring _neededString;

	/**
	 * Charsetに対するEnumFontFamiliesExのコールバック
	 *
	 * @param lpelfe 論理的なフォントデータ
	 * @param lpntme 物理的なフォントデータ
	 * @param FontType フォントの種類
	 * @param lParam アプリケーション定義のデータ
	 * @return 0:列挙を中止する 1:次のフォントを列挙する。
	 */
	int CALLBACK EnumFontCharsetProc(
		ENUMLOGFONTEX *lpelfe,    // 論理的なフォントデータ
		NEWTEXTMETRICEX *lpntme,  // 物理的なフォントデータ
		DWORD FontType,           // フォントの種類
		LPARAM lParam             // アプリケーション定義のデータ
	)
	{
		if (_neededString == lpelfe->elfFullName) {
			return 0;
		}

		return 1;

	}

	/**
	 * Charsetに対するフォントを取得する
	 *
	 * @return 0:フォントが存在する 1:フォントは存在しない
	 */
	int findCharsetFont()
	{
		LOGFONT lf;

		lf.lfPitchAndFamily = 0;
		lf.lfCharSet = DEFAULT_CHARSET;

		HDC hDC;
		hDC = GetDC(GetDesktopWindow());

		int found = 1;
		found = ::EnumFontFamiliesEx(
			hDC,
			&lf,
			(FONTENUMPROC)EnumFontCharsetProc,
			(LPARAM)0,
			0
		);

		return found;
	}

	tstring getJapaneseFontFallback(tstring &settingString)
	{
		int result = 1;

		_neededString = settingString;

		result = findCharsetFont();
		if (result == 0) {
			return settingString;
		}

		_neededString = _T("游ゴシック");
		result = findCharsetFont();
		if (result == 0) {
			return _neededString;
		}

		_neededString = _T("MS UI Gothic");

		return _neededString;

	}

}
