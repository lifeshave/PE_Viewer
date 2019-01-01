#include"MoreFunction.h"
LRESULT CALLBACK	MoreFunctionDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId;
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDC_BUTTON_OK:
			SendMessage(hDlg,WM_CLOSE,0,0);
			return FALSE;
		}
	return TRUE;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	}
		return FALSE;
	
}