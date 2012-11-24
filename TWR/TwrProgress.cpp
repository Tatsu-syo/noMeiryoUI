#include <windows.h>

#include "AppliBase.h"
#include "TwrProgress.h"

/**
 * コンストラクタ
 */
TwrProgress::TwrProgress() : TwrWnd()
{
}

/**
 * コンストラクタ
 *
 * @param newHwnd オブジェクトに結び付けるウインドウハンドル
 */
TwrProgress::TwrProgress(HWND newHwnd) : TwrWnd()
{
	hWnd = newHwnd;
}

/**
 * プログレスバーの範囲を設定する。
 *
 * @param min 最小値
 * @param max 最大値
 */
void TwrProgress::setRange(unsigned int min, unsigned int max)
{
	::SendMessage(hWnd, PBM_SETRANGE, 0, (LPARAM)MAKELPARAM(min, max));
}

/**
 * プログレスバーの進む幅を設定する。
 *
 * @param step 進む幅
 */
void TwrProgress::setStep(int step)
{
	::SendMessage(hWnd, PBM_SETSTEP, (WPARAM)step, (LPARAM)0);
}

/**
 * プログレスバーの位置を設定する。
 *
 * @param pos 位置
 */
void TwrProgress::setPos(int pos)
{
	::SendMessage(hWnd, PBM_SETPOS, (WPARAM)pos, (LPARAM)0);
}

/**
 * プログレスバーの値を進める。
 */
void TwrProgress::stepIt(void)
{
	::SendMessage(hWnd, PBM_STEPIT, (WPARAM)0, (LPARAM)0);
}

