/*
noMeiryoUI (C) 2005,2012-2020 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#include "FontSel.h"
#include "util.h"

#include <algorithm>
#include <functional>

/** フォントのリスト */
std::vector<struct FontInfo> fontList;
/** 文字セットと対応するスタイルのリスト */
std::vector<struct CharsetInfo> charsetList;

static bool noMeiryoUI = false;
static bool noTahoma = false;

bool operator<(const FontInfo& left, const FontInfo& right)
{
	return (_tcscmp(left.dispName, right.dispName) > 0);
}

bool operator>(const FontInfo& left, const FontInfo& right)
{
	return (_tcscmp(left.dispName, right.dispName) < 0);
}

/**
 * TypeInfoにENUMLOGFONTEXから情報をコピーする。 
 * 
 * @param typeInfo コピー先フォント情報
 * @param lpelfe コピー元ENUMLOGFONTEX
 */
void copyTypeInfo(TypeInfo& typeInfo, ENUMLOGFONTEX* lpelfe)
{
	_tcscpy(typeInfo.typeName, _T("Standard"));
	if (lpelfe->elfStyle[0] != _T('\0')) {
		_tcscpy(typeInfo.typeName, lpelfe->elfStyle);
	}
	typeInfo.logFont = lpelfe->elfLogFont;
}

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
	ENUMLOGFONTEX* lpelfe,    // 論理的なフォントデータ
	NEWTEXTMETRICEX* lpntme,  // 物理的なフォントデータ
	DWORD FontType,           // フォントの種類
	LPARAM lParam             // アプリケーション定義のデータ
)
{
	int fonts;
	TCHAR dispBuf[32];

	struct TypeInfo fontInfo;
	copyTypeInfo(fontInfo, lpelfe);

	for (int j = 0; j < charsetList.size(); j++) {

		// 同じ文字セットか?
		if (charsetList[j].charset == lpelfe->elfLogFont.lfCharSet) {
			// 同じ文字セットで文字スタイルのみ違う
			charsetList[j].fonts.push_back(fontInfo);
			return 1;
		}
	}

	// 文字セットも異なる
	struct CharsetInfo charset;
	charset.charset = lpelfe->elfLogFont.lfCharSet;
	charset.fonts.push_back(fontInfo);
	charsetList.push_back(charset);

	return 1;

}

/**
 * Charsetに対するフォントを取得する
 * 
 * @param lf フォント名とCharsetの入ったフォント情報
 */
int getCharsetFont(LOGFONT *lf)
{
	lf->lfPitchAndFamily = 0;
	lf->lfCharSet = DEFAULT_CHARSET;

	charsetList.clear();

	HDC hDC;
	hDC = GetDC(GetDesktopWindow());

	EnumFontFamiliesEx(
		hDC,
		lf,
		(FONTENUMPROC)EnumFontCharsetProc,
		(LPARAM)0,
		0
	);

	return 0;
}

/**
 * EnumFontFamiliesExのコールバック
 *
 * @param lpelfe 論理的なフォントデータ
 * @param lpntme 物理的なフォントデータ
 * @param FontType フォントの種類
 * @param lParam アプリケーション定義のデータ
 * @return 0:列挙を中止する 1:次のフォントを列挙する。
 */
int CALLBACK EnumFontFamExProc(
  ENUMLOGFONTEX *lpelfe,    // 論理的なフォントデータ
  NEWTEXTMETRICEX *lpntme,  // 物理的なフォントデータ
  DWORD FontType,           // フォントの種類
  LPARAM lParam             // アプリケーション定義のデータ
)
{
	std::vector<int> charset;
	int fonts;
	struct FontInfo fontInfo;
	TCHAR dispBuf[32];

	if (lpelfe->elfLogFont.lfFaceName[0] == _T('@')) {
		// 縦書きフォントは飛ばす。
		return 1;
	}
	if (noMeiryoUI) {
		if (!_tcscmp(_T("Meiryo UI"), lpelfe->elfLogFont.lfFaceName)) {
			return 1;
		}
	}

	if (noTahoma) {
		if (!_tcscmp(_T("Tahoma"), lpelfe->elfLogFont.lfFaceName)) {
			return 1;
		}
	}

	fonts = fontList.size();

	for (int i = 0; i < fonts; i++) {
		// 同じ名前か?
		if (!_tcscmp(fontList[i].logFont.lfFaceName, lpelfe->elfLogFont.lfFaceName)) {
			return 1;
		}
	}

	// 見つからない場合は追加する。
	fontInfo.logFont = lpelfe->elfLogFont;
	_tcscpy(fontInfo.fullName, lpelfe->elfFullName);
	// fontInfo.charsetList.clear();

	_tcscpy(dispBuf, lpelfe->elfLogFont.lfFaceName);
	if (isKorean) {
		getKoreanFontName(dispBuf);
	}
	_tcscpy(fontInfo.dispName, dispBuf);
	fontList.push_back(fontInfo);

	return 1;
}

/**
 * フォントを取得する。
 */
int getFont()
{
	LOGFONT lf;
	HDC hDC;

	fontList.clear();

	hDC = GetDC(GetDesktopWindow());

	memset(&lf, 0x00, sizeof(LOGFONT));
	lf.lfFaceName[0] = _T('\0');
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfPitchAndFamily = 0;

	EnumFontFamiliesEx(hDC,
		&lf,
		(FONTENUMPROC)EnumFontFamExProc,
		(LPARAM)0,
		0);

	std::sort(fontList.begin(), fontList.end(), std::greater<FontInfo>());

	return 0;
}

/**
 * コンストラクタ
 */
FontSel::FontSel(HWND parent, int resource) : BaseDialog(parent, resource)
{
	displayFont = NULL;
	m_fontNameList = NULL;
	m_fontSizeList = NULL;
	m_ChersetList = NULL;
	m_styleList = NULL;
	m_underline = NULL;
	previousFont = NULL;
}

/**
 * デストラクタ
 */
FontSel::~FontSel(void)
{
	if (m_fontSizeList != NULL) {
		delete m_fontSizeList;
	}
	if (m_fontNameList != NULL) {
		delete m_fontNameList;
	}
	if (m_ChersetList != NULL) {
		delete m_ChersetList;
	}
	if (m_styleList != NULL) {
		delete m_styleList;
	}
	if (m_underline != NULL) {
		delete m_underline;
	}
	if (displayFont != NULL) {
		DeleteObject(displayFont);
	}
}

/**
 * ダイアログ初期化処理
 */
INT_PTR FontSel::OnInitDialog()
{
	getFont();
	m_fontNameList = new TwrCombobox(::GetDlgItem(hWnd, IDC_COMBO_NAME));
	m_fontSizeList = new TwrCombobox(::GetDlgItem(hWnd, IDC_COMBO_SIZE));
	m_ChersetList =  new TwrCombobox(::GetDlgItem(hWnd, IDC_COMBO_CHARSET));
	m_styleList =  new TwrCombobox(::GetDlgItem(hWnd, IDC_COMBO_STYLE));
	m_underline = new TwrCheckbox(::GetDlgItem(hWnd, IDC_CHECK_UNDERLINE));
	m_strike = new TwrCheckbox(::GetDlgItem(hWnd, IDC_CHECK_STRIKE));

	int fonts;

	fonts = fontList.size();
	for (int i = 0; i < fonts; i++) {
		m_fontNameList->addItem(fontList[i].dispName);
	}

	m_fontSizeList->addItem(_T("6"));
	m_fontSizeList->addItem(_T("7"));
	m_fontSizeList->addItem(_T("8"));
	m_fontSizeList->addItem(_T("9"));
	m_fontSizeList->addItem(_T("10"));
	m_fontSizeList->addItem(_T("11"));
	m_fontSizeList->addItem(_T("12"));
	m_fontSizeList->addItem(_T("13"));
	m_fontSizeList->addItem(_T("14"));
	m_fontSizeList->addItem(_T("15"));
	m_fontSizeList->addItem(_T("16"));
	m_fontSizeList->addItem(_T("17"));
	m_fontSizeList->addItem(_T("18"));
	m_fontSizeList->addItem(_T("19"));
	m_fontSizeList->addItem(_T("20"));
	m_fontSizeList->addItem(_T("21"));
	m_fontSizeList->addItem(_T("22"));
	m_fontSizeList->addItem(_T("23"));
	m_fontSizeList->addItem(_T("24"));
	m_fontSizeList->addItem(_T("26"));
	m_fontSizeList->addItem(_T("28"));
	m_fontSizeList->addItem(_T("36"));
	m_fontSizeList->addItem(_T("48"));
	m_fontSizeList->addItem(_T("72"));

	// 選択する。
	if (previousFont != NULL) {
		// フォントサイズ
		int count = m_fontSizeList->getCount();
		int point = getFontPointInt(previousFont, this->getHwnd());
		int selection = 0;
		for (int i = 0; i < count; i++) {
			int itemSize = _tstoi(m_fontSizeList->getItem(i).c_str());
			if (point >= itemSize) {
				selection = i;
			}
		}
		if (selection > -1) {
			m_fontSizeList->setSelectedIndex(selection);
		}

		// フォントフェイス
		for (int i = 0; i < fonts; i++) {
			if (!_tcscmp(fontList[i].dispName, previousFont->lfFaceName)) {
				m_fontNameList->setSelectedIndex(i);

				// フォントに合った文字コードセットを設定する。
				setCharset();
				int charsetCount = charsetList.size();
				int charset = 0;
				for (int j = 0; j < charsetCount; j++) {
					if (charsetList[j].charset == previousFont->lfCharSet) {
						m_ChersetList->setSelectedIndex(j);
						charset = j;
					}
				}

				// フォントに合ったスタイルを設定する。
				setStyle();

#if 0
				int style = 0;
				// イタリック
				if (previousFont->lfItalic) {
					style |= 1;
				}
				// 太字
				if (previousFont->lfWeight > 400) {
					style |= 2;
				}
				m_styleList->setSelectedIndex(style);
#endif

				// 下線
				if (previousFont->lfUnderline) {
					m_underline->setChecked(true);
				}
				// 取り消し線
				if (previousFont->lfStrikeOut) {
					m_strike->setChecked(true);
				}

				break;
			}
		}
	}

	if (useResource) {
		applyResource();
	}
	adjustPosition();

	return (INT_PTR)FALSE;
}

/**
 * ウインドウ位置を親ウインドウの中央に調整する。
 */
void FontSel::adjustPosition(void)
{
	RECT parentRect;

	GetWindowRect(getParent(), &parentRect);

	adjustCenter(parentRect, getParent(), this->hWnd);
}

/**
 * 各操作に対する処理の分岐
 *
 * @param wParam WPARAM
 * @return 0:メッセージを処理した。 0以外:メッセージを処理しなかった。
 */
INT_PTR FontSel::OnCommand(WPARAM wParam)
{
	INT_PTR result;

	switch (LOWORD(wParam)) {
		case IDC_COMBO_NAME:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				setCharset();
				setStyle();
			}
			break;
		case IDC_COMBO_CHARSET:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				setStyle();
			}
			break;
		case IDOK:
			result = onOK();
			if (result == 0) {
				return result;
			}
	}
	return BaseDialog::OnCommand(wParam);

}

/**
 * リソースを各項目に設定する。
 */
void FontSel::applyResource()
{
	HDC hDC = GetDC(this->hWnd);

	displayFont = CreateFont(
		-MulDiv(APP_FONTSIZE, GetDeviceCaps(hDC, LOGPIXELSY), 72),
		0,
		0,
		0,
		FW_NORMAL,
		FALSE,
		FALSE,
		FALSE,
		_tstoi(langResource[70].c_str()),
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, // CLEARTYPE_QUALITY,
		FIXED_PITCH | FF_MODERN,
		langResource[0].c_str());

	ReleaseDC(this->hWnd, hDC);


	// アプリタイトル
	setText(langResource[27].c_str());

	setChildText(IDC_STATIC_NAME, langResource[28].c_str());
	setChildFont(IDC_STATIC_NAME, displayFont);
	setChildText(IDC_STATIC_STYLE, langResource[29].c_str());
	setChildFont(IDC_STATIC_STYLE, displayFont);
	setChildText(IDC_STATIC_SIZE, langResource[30].c_str());
	setChildFont(IDC_STATIC_SIZE, displayFont);

	setChildText(IDC_CHECK_UNDERLINE, langResource[31].c_str());
	setChildFont(IDC_CHECK_UNDERLINE, displayFont);
	setChildText(IDC_CHECK_STRIKE, langResource[32].c_str());
	setChildFont(IDC_CHECK_STRIKE, displayFont);
	setChildText(IDC_STATIC_CHARSET, langResource[33].c_str());
	setChildFont(IDC_STATIC_CHARSET, displayFont);

	setChildText(IDOK, langResource[34].c_str());
	setChildFont(IDOK, displayFont);
	setChildText(IDCANCEL, langResource[35].c_str());
	setChildFont(IDCANCEL, displayFont);

	setChildFont(IDC_COMBO_NAME, displayFont);
	setChildFont(IDC_COMBO_STYLE, displayFont);
	setChildFont(IDC_COMBO_SIZE, displayFont);
	setChildFont(IDC_COMBO_CHARSET, displayFont);

}

/**
 * 選択したフォントに合った文字セットの選択肢を設定します。
 */
void FontSel::setCharset(void)
{
	int selected = m_fontNameList->getSelectedIndex();
	int initialIndex = 0;	// 初期選択

	if (selected > -1) {
		// 選択したフォントに対応した文字セットとスタイルを取得する
		getCharsetFont(&fontList[selected].logFont);

		m_ChersetList->clear();
		int charsetCount = charsetList.size();
		for (int i = 0; i < charsetCount; i++) {
			switch (charsetList[i].charset) {
				case ANSI_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[36].c_str());
					} else {
						m_ChersetList->addItem(_T("欧文"));
					}
					break;
				case BALTIC_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[37].c_str());
					} else {
						m_ChersetList->addItem(_T("バルト諸国言語"));
					}
					break;
				case CHINESEBIG5_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[38].c_str());
					} else {
						m_ChersetList->addItem(_T("繁体字"));
					}
					break;
				case DEFAULT_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[39].c_str());
					} else {
						m_ChersetList->addItem(_T("デフォルト言語"));
					}
					break;
				case EASTEUROPE_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[40].c_str());
					} else {
						m_ChersetList->addItem(_T("東ヨーロッパ言語"));
					}
					break;
				case GB2312_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[41].c_str());
					} else {
						m_ChersetList->addItem(_T("簡体字"));
					}
					break;
				case GREEK_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[42].c_str());
					} else {
						m_ChersetList->addItem(_T("ギリシャ文字"));
					}
					break;
				case HANGUL_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[43].c_str());
					} else {
						m_ChersetList->addItem(_T("韓国語"));
					}
					break;
				case MAC_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[44].c_str());
					} else {
						m_ChersetList->addItem(_T("Mac文字セット"));
					}
					break;
				case OEM_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[45].c_str());
					} else {
						m_ChersetList->addItem(_T("OEM文字セット"));
					}
					break;
				case RUSSIAN_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[46].c_str());
					} else {
						m_ChersetList->addItem(_T("ロシア語"));
					}
					break;
				case SHIFTJIS_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[47].c_str());
					} else {
						m_ChersetList->addItem(_T("日本語"));
					}
					initialIndex = i;
					break;
				case SYMBOL_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[48].c_str());
					} else {
						m_ChersetList->addItem(_T("シンボル"));
					}
					break;
				case TURKISH_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[49].c_str());
					} else {
						m_ChersetList->addItem(_T("トルコ語"));
					}
					break;
				case VIETNAMESE_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[50].c_str());
					} else {
						m_ChersetList->addItem(_T("ベトナム語"));
					}
					break;
				case JOHAB_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[51].c_str());
					} else {
						m_ChersetList->addItem(_T("韓国語(組合型)"));
					}
					break;
				case ARABIC_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[52].c_str());
					} else {
						m_ChersetList->addItem(_T("アラビア語"));
					}
					break;
				case HEBREW_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[53].c_str());
					} else {
						m_ChersetList->addItem(_T("ヘブライ語"));
					}
					break;
				case THAI_CHARSET:
					if (useResource) {
						m_ChersetList->addItem(langResource[54].c_str());
					} else {
						m_ChersetList->addItem(_T("タイ語"));
					}
					break;
				default:
					m_ChersetList->addItem(_T(""));
					break;
			}
		}
	}
	// 文字セットを選択状態にする。
	m_ChersetList->setSelectedIndex(initialIndex);
}

/**
 * 選択したフォントに合ったスタイルの選択肢を設定します。
 */
void FontSel::setStyle()
{
	int selected = m_fontNameList->getSelectedIndex();
	int charset = m_ChersetList->getSelectedIndex();
	int styleCount = charsetList[charset].fonts.size();

	m_styleList->clear();
	for (int i = 0; i < styleCount; i++) {
		m_styleList->addItem(charsetList[charset].fonts[i].typeName);
	}

	m_styleList->setSelectedIndex(0);
}

/**
 * OKボタンを押下した時の動作
 */
INT_PTR FontSel::onOK(void)
{
	int selectedFontIndex = m_fontNameList->getSelectedIndex();
	int selectedCharset = m_ChersetList->getSelectedIndex();
	int selectedStyle = m_styleList->getSelectedIndex();

	tstring error;
	tstring message;

	if (useResource) {
		error = langResource[63].c_str();
	} else {
		error = _T("エラー");
	}

	if (selectedFontIndex < 0) {
		if (useResource) {
			message = langResource[65].c_str();
		} else {
			message = _T("フォントを選択してください。");
		}
		MessageBox(this->hWnd,
			message.c_str(),
			error.c_str(),
			MB_ICONEXCLAMATION | MB_OK);
		return (INT_PTR)0;
	}

	if (selectedStyle < 0) {
		if (useResource) {
			message = langResource[66].c_str();
		} else {
			message = _T("スタイルを選択してください。");
		}
		MessageBox(this->hWnd,
			message.c_str(),
			error.c_str(),
			MB_ICONEXCLAMATION | MB_OK);
		return (INT_PTR)0;
	}

	if (selectedCharset < 0) {
		message = langResource[68].c_str();
		MessageBox(this->hWnd,
			message.c_str(),
			error.c_str(),
			MB_ICONEXCLAMATION | MB_OK);
		return (INT_PTR)0;
	}

	tstring size = m_fontSizeList->getSelectedText();
	if (size.length() < 1) {
		message = langResource[67].c_str();
		MessageBox(this->hWnd,
			message.c_str(),
			error.c_str(),
			MB_ICONEXCLAMATION | MB_OK);
		return (INT_PTR)0;
	}

	selectedFont = charsetList[selectedCharset].fonts[selectedStyle].logFont;
	// selectedFont.lfCharSet = fontList[selectedFontIndex].charsetList[selectedCharset].charset;

	/*
	if (selectedStyle > 1) {
		if (selectedFont.lfWeight < 600) {
			selectedFont.lfWeight = FW_BOLD;
		}
	} else {
		selectedFont.lfWeight = FW_NORMAL;
	}
	if ((selectedStyle == 1) || (selectedStyle == 3)) {
		selectedFont.lfItalic = TRUE;
	} else {
		selectedFont.lfItalic = FALSE;
	}
	*/

	int point = _tstoi(size.c_str());

	// サイズの設定
	HDC hDC = GetDC(this->hWnd);
	selectedFont.lfHeight = -MulDiv(point, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	selectedFont.lfWidth = 0;
	ReleaseDC(this->hWnd, hDC);

	// Windows 8互換の場合はピクセル数から計算したポイントが指定したポイントより小さい場合、
	// フォントの高さのピクセル数の絶対値を増やす。
	if (WIN8_SIZE) {
		if (abs(getFontPointInt(&selectedFont, this->getHwnd())) < point) {
			// ピクセル数はマイナスで指定するので1減らして絶対値を増やす。
			selectedFont.lfHeight--;
		}
	}

	if (m_underline->isChecked()) {
		selectedFont.lfUnderline = TRUE;
	} else {
		selectedFont.lfUnderline = FALSE;
	}

	if (m_strike->isChecked()) {
		selectedFont.lfStrikeOut = TRUE;
	} else {
		selectedFont.lfStrikeOut = FALSE;
	}

	return (INT_PTR)1;
}

void FontSel::setNoMeiryoUI()
{
	noMeiryoUI = true;
}

void FontSel::setNoTahoma()
{
	noTahoma = true;
}
