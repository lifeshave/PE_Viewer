#include"DataDirDlg.h"
LRESULT CALLBACK  DataDirDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	switch (message)
	{
	case WM_INITDIALOG:
		ShowDataDirInfo(hDlg);
		return TRUE;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{

		case IDC_BUTTON_OK:
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			return TRUE;

		case IDC_BUTTON_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTDLG, hDlg, (DLGPROC)AboutDlgProc);
			return TRUE;
		
		}
		return TRUE;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return  TRUE;
	}


	return FALSE;

}

void ShowDataDirInfo(HWND hDlg)
{
	//定义一个数组,方便显示数据目录表的时候使用循环来设置
	MYDATADIRECTORY myDataArray[] =
	{
		{ IDC_EDIT_DD_RVA_EXPORT, IDC_EDIT_DD_SIZE_EXPORT },
		{ IDC_EDIT_DD_RVA_IMPORT, IDC_EDIT_DD_SIZE_IMPORT },
		{ IDC_EDIT_DD_RVA_RES, IDC_EDIT_DD_SZIE_RES },
		{ IDC_EDIT_DD_RVA_EXCEPTION, IDC_EDIT_DD_SZIE_EXCEPTION },
		{ IDC_EDIT_DD_RVA_SECURITY, IDC_EDIT_DD_SIZE_SECURITY },
		{ IDC_EDIT_DD_RVA_RELOC, IDC_EDIT_DD_SIZE_RELOC },
		{ IDC_EDIT_DD_RVA_DEBUG, IDC_EDIT_DD_SIZE_DEBUG },
		{ IDC_EDIT_DD_RVA_COPYRIGHT, IDC_EDIT_DD_SIZE_COPYRIGHT },
		{ IDC_EDIT_DD_RVA_GP, IDC_EDIT_DD_SIZE_GP },
		{ IDC_EDIT_DD_RVA_TLS, IDC_EDIT_DD_SIZE_TLS },
		{ IDC_EDIT_DD_RVA_LOADCONFIG, IDC_EDIT_DD_SIZE_LOADCONFIG },
		{ IDC_EDIT_DD_RVA_IAT, IDC_EDIT_DD_SIZE_IAT },
		{ IDC_EDIT_DD_RVA_BOUND, IDC_EDIT_DD_SIZE_BOUND },
		{ IDC_EDIT_DD_RVA_COM, IDC_EDIT_DD_SIZE_COM },
		{ IDC_EDIT_DD_RVA_DELAYIMPORT, IDC_EDIT_DD_SIZE_DELAYIMPORT },
		{ IDC_EDIT_DD_RVA_NOUSE, IDC_EDIT_DD_SIZE_NOUSE }
	};
	//存放要显示的字符
	TCHAR pszDest[30] = { '\0' };
	size_t cchDest = 30;

	LPCTSTR pszFormat = TEXT("%08lX");	//DWORD格式显示
	PIMAGE_OPTIONAL_HEADER pOH = NULL;

	pOH = GetOptionalHeader(sMapFile.ImageBase);
	if (!pOH)
		return;

	for (int i = 0; i<16; i++)
	{
		
		StringCchPrintf(pszDest, cchDest, pszFormat, pOH->DataDirectory[i].VirtualAddress);
		SetDlgItemText(hDlg, myDataArray[i].ID_RVA, pszDest);

		
		StringCchPrintf(pszDest, cchDest, pszFormat, pOH->DataDirectory[i].Size);
		SetDlgItemText(hDlg, myDataArray[i].ID_SIZE, pszDest);
	}

}
LRESULT CALLBACK  AboutDlgProc(HWND hAbout, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		EndDialog(hAbout, 0);
		return  TRUE;
	}
	return FALSE;
}