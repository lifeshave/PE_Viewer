#include"ImportDlg.h"
#define  _CRT_SECURE_NO_WARNINGS
#define GETTHUNK(pImportDesc) ((DWORD)                          \
	((PIMAGE_IMPORT_DESCRIPTOR)pImportDesc->OriginalFirstThunk ? \
	(PIMAGE_IMPORT_DESCRIPTOR)pImportDesc->OriginalFirstThunk:(PIMAGE_IMPORT_DESCRIPTOR)pImportDesc->FirstThunk \
	))
LRESULT CALLBACK  ImportTableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	int wmId;
	LPNMHDR pnmh;
	LPNMLISTVIEW lpnmlv;


	switch (message)
	{
	case WM_INITDIALOG:
		InitImportListCtrl(hDlg);
		InitImportFunctionsListCtrl(hDlg);
		ShowImportDescInfo(hDlg);
		return TRUE;

	case WM_NOTIFY:
		wmId = LOWORD(wParam);
		pnmh = (LPNMHDR)lParam;
		if (wmId == IDC_IMPORT_LIST)
		{
			switch (pnmh->code)
			{
			case LVN_ITEMCHANGED:
				lpnmlv = (LPNMLISTVIEW)lParam;
				if (lpnmlv->uNewState)
					ShowImportFuncsByDllIndex(hDlg, lpnmlv->iItem);


			}

		}
		return TRUE;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;;
	}
	return FALSE;
}
void InitImportListCtrl(HWND hDlg)
{
	LVCOLUMN     lvColumn;
	memset(&lvColumn, 0, sizeof(lvColumn));

	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 98;
	lvColumn.pszText = TEXT("DLL Name");
	SendDlgItemMessage(hDlg, IDC_IMPORT_LIST, LVM_INSERTCOLUMN, 0, (LPARAM)&lvColumn);

	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 98;
	lvColumn.pszText = TEXT("OriginalFirstThunk");
	SendDlgItemMessage(hDlg, IDC_IMPORT_LIST, LVM_INSERTCOLUMN, 1, (LPARAM)&lvColumn);

	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 98;
	lvColumn.pszText = TEXT("TimeDateStamp");
	SendDlgItemMessage(hDlg, IDC_IMPORT_LIST, LVM_INSERTCOLUMN, 2, (LPARAM)&lvColumn);

	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 98;
	lvColumn.pszText =TEXT( "ForwarderChin");
	SendDlgItemMessage(hDlg, IDC_IMPORT_LIST, LVM_INSERTCOLUMN, 3, (LPARAM)&lvColumn);


	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 98;
	lvColumn.pszText = TEXT("Name");
	SendDlgItemMessage(hDlg, IDC_IMPORT_LIST, LVM_INSERTCOLUMN, 4, (LPARAM)&lvColumn);

	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 98;
	lvColumn.pszText = TEXT("FirstThunk");
	SendDlgItemMessage(hDlg, IDC_IMPORT_LIST, LVM_INSERTCOLUMN, 5, (LPARAM)&lvColumn);


}
void InitImportFunctionsListCtrl(HWND hDlg)
{
	LVCOLUMN     lvColumn;
	memset(&lvColumn, 0, sizeof(lvColumn));

	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 80;
	lvColumn.pszText = TEXT("ThunkRVA");
	SendDlgItemMessage(hDlg, IDC_IMPORTFUNCTIONS_LIST, LVM_INSERTCOLUMN, 0, (LPARAM)&lvColumn);

	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 80;
	lvColumn.pszText = TEXT("ThunkValue");
	SendDlgItemMessage(hDlg, IDC_IMPORTFUNCTIONS_LIST, LVM_INSERTCOLUMN, 1, (LPARAM)&lvColumn);

	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 50;
	lvColumn.pszText = TEXT("Hint");
	SendDlgItemMessage(hDlg, IDC_IMPORTFUNCTIONS_LIST, LVM_INSERTCOLUMN, 2, (LPARAM)&lvColumn);


	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 280;
	lvColumn.pszText = TEXT("Function Name");
	SendDlgItemMessage(hDlg, IDC_IMPORTFUNCTIONS_LIST, LVM_INSERTCOLUMN, 3, (LPARAM)&lvColumn);


}
void  ShowImportDescInfo(HWND hDlg)
{
	HWND         hList;
	LVITEM       lvItem;


	LPCCH szDllName;
	//存放要显示的字符
	TCHAR pszDest[30] = { '\0' };
	size_t cchDest = 30;
	LPCTSTR pszFormat = TEXT("%08lX");	//DWORD格式显示

	PIMAGE_NT_HEADERS       pNtH = NULL;
	PIMAGE_IMPORT_DESCRIPTOR  pImportDesc = NULL;

	memset(&lvItem, 0, sizeof(lvItem));

	hList = GetDlgItem(hDlg, IDC_IMPORT_LIST);
	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT);

	pNtH = GetNtHeader(sMapFile.ImageBase);
	pImportDesc = GetFirstImportDesc(sMapFile.ImageBase);
	if (!pImportDesc)
	{
		//如果没有获得输入表信息,直接发送关闭窗口消息销毁窗口
		MessageBox(hDlg, TEXT("Can't get ImportDescriptor!"), TEXT("Prompt"), MB_OK);
		SendMessage(hDlg, WM_CLOSE,0,0);
		return;
	}

	int i = 0;
	while (pImportDesc->FirstThunk)
	{

		memset(&lvItem, 0, sizeof(lvItem));
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = i;

		szDllName = RvaToPtr(pNtH, sMapFile.ImageBase, pImportDesc->Name);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szDllName, -1, pszDest,30);
		lvItem.pszText = pszDest;
		SendDlgItemMessage(hDlg, IDC_IMPORT_LIST, LVM_INSERTITEM, 0, (LPARAM)&lvItem);

		lvItem.pszText = pszDest;
	
		StringCchPrintf(pszDest, cchDest, pszFormat, pImportDesc->OriginalFirstThunk);
		lvItem.iSubItem = 1;
		SendDlgItemMessage(hDlg, IDC_IMPORT_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);

	
		StringCchPrintf(pszDest, cchDest, pszFormat, pImportDesc->TimeDateStamp);
		lvItem.iSubItem = 2;
		SendDlgItemMessage(hDlg, IDC_IMPORT_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);

	

		StringCchPrintf(pszDest, cchDest, pszFormat, pImportDesc->ForwarderChain);
		lvItem.iSubItem = 3;
		SendDlgItemMessage(hDlg, IDC_IMPORT_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);

		
	
		StringCchPrintf(pszDest, cchDest, pszFormat, pImportDesc->Name);
		lvItem.iSubItem = 4;
		SendDlgItemMessage(hDlg, IDC_IMPORT_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);

		StringCchPrintf(pszDest, cchDest, pszFormat, pImportDesc->FirstThunk);
		lvItem.iSubItem = 5;
		SendDlgItemMessage(hDlg, IDC_IMPORT_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);

		++i;
		++pImportDesc;
	}

}
void ShowImportFuncsByDllIndex(HWND hDlg, int index)
{
	HWND         hFuncList;
	LVITEM       lvItem;      
	PTCHAR szFuncName;
	TCHAR cMemAddr[30], cOrd[30];

	DWORD        dwThunk, *pdwThunk = NULL, *pdwRVA = NULL;
	int i = 0;

	PIMAGE_NT_HEADERS         pNtH = NULL;
	PIMAGE_IMPORT_DESCRIPTOR  pFistImportDesc = NULL, pCurrentImportDesc = NULL;
	PIMAGE_IMPORT_BY_NAME     pByName = NULL;

	//TCHAR tcTEMP[30] = {'\0'};
	//存放要显示的字符
	TCHAR pszDest[30] = { '\0' };
	size_t cchDest = 30;
	LPCTSTR pszFormat = TEXT("%08lX");	//DWORD格式显示
	memset(&lvItem, 0, sizeof(lvItem));

	hFuncList = GetDlgItem(hDlg, IDC_IMPORTFUNCTIONS_LIST);
	SendMessage(hFuncList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT);
	SendMessage(hFuncList, LVM_DELETEALLITEMS, 0, 0);

	pNtH = GetNtHeader(sMapFile.ImageBase);
	pFistImportDesc = GetFirstImportDesc(sMapFile.ImageBase);
	pCurrentImportDesc = &pFistImportDesc[index];
	dwThunk = GETTHUNK(pCurrentImportDesc);

	pdwRVA = (DWORD *)dwThunk;
	pdwThunk = (DWORD*)RvaToPtr(pNtH, sMapFile.ImageBase, dwThunk);
	if (!pdwThunk)
		return;

	while (*pdwThunk)
	{
		memset(&lvItem, 0, sizeof(lvItem));
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = i;

		lvItem.pszText = pszDest;
		
		StringCchPrintf(pszDest, cchDest, pszFormat, (DWORD)pdwRVA);
		SendDlgItemMessage(hDlg, IDC_IMPORTFUNCTIONS_LIST, LVM_INSERTITEM, 0, (LPARAM)&lvItem);

	
		StringCchPrintf(pszDest, cchDest, pszFormat, (DWORD)(*pdwThunk));
		lvItem.iSubItem = 1;
		SendDlgItemMessage(hDlg, IDC_IMPORTFUNCTIONS_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);

		if (HIWORD(*pdwThunk) == 0x8000)//#define HIWORD(l)   ((WORD)((((DWORD_PTR)(*pdwThunk)) >> 16) & 0xffff))//强转4字节又移16位,强转2字节,与1安位与,判断最高位是否为1
		{
			
			strcpy_s((char*)pszDest, _countof(pszDest), "-");
			
			StringCchPrintf(pszDest, cchDest, TEXT("Ord:%08lX"), IMAGE_ORDINAL32(*pdwThunk));
			szFuncName = cOrd;
		}
		else
		{
			pByName = (PIMAGE_IMPORT_BY_NAME)RvaToPtr(pNtH, sMapFile.ImageBase, (DWORD)(*pdwThunk));
			if (pByName)
			{
				
				StringCchPrintf(pszDest, cchDest, TEXT("%04lX"), pByName->Hint);
			
				(CHAR*)szFuncName = pByName->Name;
			}
			else
			{
				strcpy_s((char*)pszDest, 2, "-");
				StringCchPrintf(cMemAddr, sizeof(cMemAddr), TEXT("MemAddr:%08lX"), (DWORD)(*pdwThunk));	
				szFuncName = cMemAddr;
			}
		}

		
		lvItem.iSubItem = 2;
		SendDlgItemMessage(hDlg, IDC_IMPORTFUNCTIONS_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);

		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCCH)szFuncName, -1, pszDest, 30);
		lvItem.pszText = pszDest;
		
		lvItem.iSubItem = 3;
		SendDlgItemMessage(hDlg, IDC_IMPORTFUNCTIONS_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);


		++i;
		++pdwRVA;
		++pdwThunk;

	}

}
