#include "ModelessDialog.h"

HWND modelessDialogHandle[10];
int modelessDialogHandles;

/**
 * モードレスダイアログ関連の初期化を行う。
 */
void initModelessDialog(void)
{
	for (int i = 0; i < 10; i++) {
		modelessDialogHandle[i] = NULL;
	}
	modelessDialogHandles = 0;
}

/**
 * モードレスダイアログを追加する。
 *
 * @param dialog ダイアログのウインドウハンドル
 */
void addModelessDialog(HWND dialog)
{
	bool found = false;
	int empty = -1;
	for (int i = 0; i < 10; i++) {
		if (modelessDialogHandle[i] == dialog) {
			return;
		}
		if ((modelessDialogHandle[i] == NULL) && (empty == -1)){
			empty = i;
		}
	}
	if (empty > -1) {
		modelessDialogHandle[empty] = dialog;
		modelessDialogHandles++;
	} else {
		modelessDialogHandle[modelessDialogHandles] = dialog;
		modelessDialogHandles++;
	}
}

/**
 * モードレスダイアログをアプリケーションによる制御から外す。
 *
 * @param dialog ダイアログのウインドウハンドル
 */
void delModelessDialog(HWND dialog)
{
	bool found = false;
	int empty = -1;
	for (int i = 0;i < 10; i++) {
		if (modelessDialogHandle[i] == dialog) {
			modelessDialogHandle[i] = NULL;
			modelessDialogHandles--;
			return;
		}
	}
}

/**
 * モードレスダイアログのダイアログメッセージハンドラー
 *
 * @param lpMsg ダイアログメッセージ
 * @return true:メッセージを処理した false:メッセージを処理しない
 */
bool handleDialogMessage(LPMSG lpMsg)
{
	HWND hDlg;

	for (int i = 0; i < 10; i++) {
		hDlg = modelessDialogHandle[i];
		if (hDlg == NULL) {
			continue;
		}
		if (IsDialogMessage(hDlg, lpMsg)) {
			return true;
		}
	}
	return false;
}

