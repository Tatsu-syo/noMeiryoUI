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

		memset(&lf, 0, sizeof(LOGFONT));
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

	/**
	 * @brief 日本語版の使用するフォールバックフォントを取得する。
	 * @param settingString 言語ファイルのフォント名
	 * @return 選択されたフォント名
	*/
	tstring getJapaneseFontFallback(tstring &settingString)
	{
		int result = 1;

		_neededString = settingString;

		// Font in language file is found in your Windows?
		result = findCharsetFont();
		if (result == 0) {
			return settingString;
		}

		// In early Windows 10
		_neededString = _T("游ゴシック Medium");
		result = findCharsetFont();
		if (result == 0) {
			return _neededString;
		}

		// In Windows 8.1
		_neededString = _T("游ゴシック");
		result = findCharsetFont();
		if (result == 0) {
			return _neededString;
		}

		// なければオーソドックスなMS UI Gothic
		// メイリオは誤解を生むのであえて使わない。
		// メイリオとMeiryo UIはいろいろな意味で別物なのに混同されることがあるので。
		_neededString = _T("MS UI Gothic");

		return _neededString;

	}

}
