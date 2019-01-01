#include"ExportDlg.h"
LRESULT CALLBACK  ExportTableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		InitExportListCtrl(hDlg);
		ShowExportDirInfo(hDlg);
		ShowExportFuncsInfo(hDlg);
		return TRUE;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	}
	return FALSE;
}

void InitExportListCtrl(HWND hDlg)
{
	LVCOLUMN     lvColumn;
	memset(&lvColumn, 0, sizeof(lvColumn));

	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 80;
	lvColumn.pszText = TEXT("Ordinal");
	SendDlgItemMessage(hDlg, IDC_EXPORT_LIST, LVM_INSERTCOLUMN, 0, (LPARAM)&lvColumn);

	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 80;
	lvColumn.pszText = TEXT("RVA");
	SendDlgItemMessage(hDlg, IDC_EXPORT_LIST, LVM_INSERTCOLUMN, 1, (LPARAM)&lvColumn);

	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 280;
	lvColumn.pszText = TEXT("Function Name");
	SendDlgItemMessage(hDlg, IDC_EXPORT_LIST, LVM_INSERTCOLUMN, 2, (LPARAM)&lvColumn);

}


void   ShowExportDirInfo(HWND hDlg)
{
	 PTCHAR szName;

	//存放要显示的字符
	TCHAR pszDest[30] = { '\0' };
	size_t cchDest = 30;
	LPCTSTR pszFormat = TEXT("%08lX");	//DWORD格式显示

	PIMAGE_NT_HEADERS       pNtH = NULL;
	PIMAGE_EXPORT_DIRECTORY pExportDir = NULL;
	pNtH = GetNtHeader(sMapFile.ImageBase);
	pExportDir = GetExportDirectory(sMapFile.ImageBase);
	if (!pExportDir)
	{
		MessageBox(hDlg, TEXT("Can't get Export Directory!"), TEXT("PE"), MB_OK);
		SendMessage(hDlg,WM_CLOSE,0,0);
		return;
	}

	
	StringCchPrintf(pszDest, cchDest, pszFormat, pExportDir->Characteristics);
	SetDlgItemText(hDlg, IDC_EDIT_EXPDIR_CHARACTERISTICS, pszDest);


	StringCchPrintf(pszDest, cchDest, pszFormat, pExportDir->TimeDateStamp);
	SetDlgItemText(hDlg, IDC_EDIT_EXPDIR_TDS, pszDest);


	StringCchPrintf(pszDest, cchDest, pszFormat, pExportDir->Base);
	SetDlgItemText(hDlg, IDC_EDIT_EXPDIR_BASE, pszDest);


	StringCchPrintf(pszDest, cchDest, pszFormat, pExportDir->Name);
	SetDlgItemText(hDlg, IDC_EDIT_EXPDIR_NAME, pszDest);

	StringCchPrintf(pszDest, cchDest, pszFormat, pExportDir->NumberOfFunctions);
	SetDlgItemText(hDlg, IDC_EDIT_EXPDIR_NUMOFFUNCS, pszDest);


	StringCchPrintf(pszDest, cchDest, pszFormat, pExportDir->NumberOfNames);
	SetDlgItemText(hDlg, IDC_EDIT_EXPDIR_NUMOFNAMES, pszDest);

	
	StringCchPrintf(pszDest, cchDest, pszFormat, pExportDir->AddressOfFunctions);
	SetDlgItemText(hDlg, IDC_EDIT_EXPDIR_ADDROFFUNCS, pszDest);

	
	StringCchPrintf(pszDest, cchDest, pszFormat, pExportDir->AddressOfNames);
	SetDlgItemText(hDlg, IDC_EDIT_EXPDIR_ADDROFNAMES, pszDest);

	StringCchPrintf(pszDest, cchDest, pszFormat, pExportDir->AddressOfNameOrdinals);
	SetDlgItemText(hDlg, IDC_EDIT_EXPDIR_ADDROFNAMEORD, pszDest);

	szName = RvaToPtr(pNtH, sMapFile.ImageBase, pExportDir->Name);
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCCH)szName, -1, pszDest, 30);
	SetDlgItemText(hDlg, IDC_EDIT_EXPDIR_DLLNAME, pszDest);
}

void   ShowExportFuncsInfo(HWND hDlg)
{
	HWND         hList;
	LVITEM       lvItem;
	PTCHAR       szFuncName;

	UINT                    iNumOfName = 0;
	PDWORD                  pdwRvas, pdwNames;
	PWORD                   pwOrds;
	UINT                    i = 0, j = 0, k = 0;
	BOOL                    bIsByName = FALSE;;


	//存放要显示的字符
	TCHAR pszDest[30] = { '\0' };
	size_t cchDest = 30;
	LPCTSTR pszFormat = TEXT("%08lX");	//DWORD格式显示



	PIMAGE_NT_HEADERS       pNtH = NULL;
	PIMAGE_EXPORT_DIRECTORY pExportDir = NULL;

	pNtH = GetNtHeader(sMapFile.ImageBase);
	if (!pNtH)
		return;
	pExportDir = (PIMAGE_EXPORT_DIRECTORY)GetExportDirectory(sMapFile.ImageBase);
	if (!pExportDir)
		return;


	pwOrds = (PWORD)RvaToPtr(pNtH, sMapFile.ImageBase, pExportDir->AddressOfNameOrdinals);
	pdwRvas = (PDWORD)RvaToPtr(pNtH, sMapFile.ImageBase, pExportDir->AddressOfFunctions);
	pdwNames = (PDWORD)RvaToPtr(pNtH, sMapFile.ImageBase, pExportDir->AddressOfNames);

	if (!pdwRvas)
		return;

	hList = GetDlgItem(hDlg, IDC_EXPORT_LIST);
	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT);


	iNumOfName = pExportDir->NumberOfNames;

	for (i = 0; i<pExportDir->NumberOfFunctions; i++)
	{
		if (*pdwRvas)
		{
			for (j = 0; j<iNumOfName; j++)
			{
				if (i == pwOrds[j])
				{
					bIsByName = TRUE;
					szFuncName = RvaToPtr(pNtH, sMapFile.ImageBase, pdwNames[j]);
					break;
				}

				bIsByName = FALSE;
			}

			//show funcs to listctrl

			memset(&lvItem, 0, sizeof(lvItem));
			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = k;

			
			lvItem.pszText = pszDest;
			StringCchPrintf(pszDest, cchDest, TEXT("%04lX"), (UINT)(pExportDir->Base + i));
			SendDlgItemMessage(hDlg, IDC_EXPORT_LIST, LVM_INSERTITEM, 0, (LPARAM)&lvItem);

		
			StringCchPrintf(pszDest, cchDest, pszFormat, (*pdwRvas));
			lvItem.iSubItem = 1;
			SendDlgItemMessage(hDlg, IDC_EXPORT_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);

			if (bIsByName)
			{
				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCCH)szFuncName, -1, pszDest, 30);
				lvItem.pszText = pszDest;
			}
				
			else
			{
				lvItem.pszText = TEXT("-");
			}
				

			lvItem.iSubItem = 2;
			SendDlgItemMessage(hDlg, IDC_EXPORT_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);

			++k;

		}

		++pdwRvas;

	}


}
