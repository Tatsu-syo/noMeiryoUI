/*
noMeiryoUI (C) 2005,2012-2015 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<tchar.h>
#include	<windows.h>
#include	"NCFileDialog.h"

/**
 * コンストラクタ
 *
 * @param bOpenFileDialog 構築するダイアログボックスの種類<br>
 * TRUE:[ファイルを開く] ダイアログボックス FALSE:[ファイル名をつけて保存]ダイアログボックス
 * @param lpszDefExt 既定の拡張子
 * @param lpszFileName 初期表示されるファイル名
 * @param dwFlags ダイアログボックスの設定用フラグ
 * @param lpszFilter フィルタ文字列
 * @param pParentWnd 親ウインドウのハンドル
 * @param dwSize OPENFILENAME構造体のサイズ
 */
NCFileDialog::NCFileDialog(
	BOOL bOpenFileDialog,
	const char *lpszDefExt,
	const TCHAR *lpszFileName,
	DWORD dwFlags,
	const TCHAR *lpszFilter,
	HWND pParentWnd,
	DWORD dwSize
)
{
	// コンストラクタからm_OpenFileDialogに情報を設定する。
	// 種別
	m_OpenFileDialog = bOpenFileDialog;

	// 構造体のサイズ
	memset(&m_ofn,0x00,sizeof(OPENFILENAME));
	if (dwSize > 0) {
		m_ofn.lStructSize = dwSize;
	}else{
		m_ofn.lStructSize = sizeof(OPENFILENAME);
	}
	// 既定の拡張子(未使用)
	m_defExt = lpszDefExt;
	// 親ウインドウ
	m_ofn.hwndOwner = pParentWnd;
	// フィルタ
	m_ofn.lpstrFilter = lpszFilter;
	m_ofn.nFilterIndex = 1;
	// フラグ
	m_ofn.Flags = dwFlags;
	// 初期ファイル名
	memset(gotFileName,0x00,sizeof(TCHAR) * _MAX_PATH);
	if (lpszFileName != NULL) {
		_tcscpy(gotFileName,lpszFileName);
	}
	m_ofn.lpstrFile = gotFileName;
	m_ofn.nMaxFile = _MAX_PATH;
	// m_ofn.lpstrTitle = "設定ファイルの指定";
}

/**
 * ダイアログを表示します。
 *
 * @return IDOK:OKボタン押下 IDCANCEL:キャンセルボタン押下
 */
int NCFileDialog::DoModal()
{
	int result;

	if (m_OpenFileDialog == FALSE){
		result = ::GetSaveFileName(&m_ofn);
	}else{
		result = ::GetOpenFileName(&m_ofn);
	}
	if (result){
		return IDOK;
	}else{
		return IDCANCEL;
	}
}
