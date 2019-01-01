#include"SectionTable.h"
LRESULT CALLBACK  SectionTableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		InitSectionListCtrl(hDlg);
		ShowSectionHeaderInfo(hDlg);
		return TRUE;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;;
	}
	return FALSE;
}

void  InitSectionListCtrl(HWND hDlg)
{
	HWND         hList;
	LVCOLUMN     lvColumn;
	memset(&lvColumn, 0, sizeof(lvColumn));

	hList = GetDlgItem(hDlg, IDC_SECTIONLIST);
	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT);

	//依次填写表头
	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 80;
	lvColumn.pszText = TEXT("Name");
	SendDlgItemMessage(hDlg, IDC_SECTIONLIST, LVM_INSERTCOLUMN, 0, (LPARAM)&lvColumn);

	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 80;
	lvColumn.pszText = TEXT("VAddress");
	SendDlgItemMessage(hDlg, IDC_SECTIONLIST, LVM_INSERTCOLUMN, 1, (LPARAM)&lvColumn);

	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 80;
	lvColumn.pszText = TEXT("VSize");
	SendDlgItemMessage(hDlg, IDC_SECTIONLIST, LVM_INSERTCOLUMN, 2, (LPARAM)&lvColumn);

	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 80;
	lvColumn.pszText = TEXT("ROffset");
	SendDlgItemMessage(hDlg, IDC_SECTIONLIST, LVM_INSERTCOLUMN, 3, (LPARAM)&lvColumn);


	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 80;
	lvColumn.pszText = TEXT("RSize");
	SendDlgItemMessage(hDlg, IDC_SECTIONLIST, LVM_INSERTCOLUMN, 4, (LPARAM)&lvColumn);

	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 80;
	lvColumn.pszText = TEXT("Flags");
	SendDlgItemMessage(hDlg, IDC_SECTIONLIST, LVM_INSERTCOLUMN, 5, (LPARAM)&lvColumn);

}
void ShowSectionHeaderInfo(HWND hDlg)
{
	//由于LVITEM结构体中的pszText字段在unicode被定义时定义的是wchar类型,所以显示名字的时候需要进行转码
	//将ASCALL码转为UNICODE编码
	LVITEM                  lvItem;
	WORD                    i;
	PIMAGE_FILE_HEADER       pFH = NULL;
	PIMAGE_SECTION_HEADER	 pSH = NULL;


	//存放要显示的字符
	TCHAR pszDest[30] = { '\0' };
	size_t cchDest = 30;
	char cBuff[9] = {'\0'};//用来读取名字用的
	LPCTSTR pszFormat = TEXT("%08lX");	//DWORD格式显示




	pFH = GetFileHeader(sMapFile.ImageBase);
	if (!pFH)return;

	pSH = GetFirstSectionHeader(sMapFile.ImageBase);

	for (i = 0; i < pFH->NumberOfSections; i++)
	{
		memset(&lvItem, 0, sizeof(lvItem));
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = i;

		memset(pszDest, 0, sizeof(pszDest));

		//此处实现转码
		memcpy(cBuff, pSH->Name, 9);
		for (int i = 0; i < 10;i++)
		{
			pszDest[i] = cBuff[i];
		}

		//将结构体pszText字段指针指向pszDest
		lvItem.pszText = pszDest;

		SendDlgItemMessage(hDlg, IDC_SECTIONLIST, LVM_INSERTITEM, 0, (LPARAM)&lvItem);

		StringCchPrintf(pszDest, cchDest, pszFormat, pSH->VirtualAddress);
		lvItem.iSubItem = 1;
		SendDlgItemMessage(hDlg, IDC_SECTIONLIST, LVM_SETITEM, 0, (LPARAM)&lvItem);

		
		StringCchPrintf(pszDest, cchDest, pszFormat, pSH->Misc.VirtualSize);
		lvItem.iSubItem = 2;
		SendDlgItemMessage(hDlg, IDC_SECTIONLIST, LVM_SETITEM, 0, (LPARAM)&lvItem);

	
		StringCchPrintf(pszDest, cchDest, pszFormat, pSH->PointerToRawData);
		lvItem.iSubItem = 3;
		SendDlgItemMessage(hDlg, IDC_SECTIONLIST, LVM_SETITEM, 0, (LPARAM)&lvItem);

		
		StringCchPrintf(pszDest, cchDest, pszFormat, pSH->SizeOfRawData);
		lvItem.iSubItem = 4;
		SendDlgItemMessage(hDlg, IDC_SECTIONLIST, LVM_SETITEM, 0, (LPARAM)&lvItem);

		
		StringCchPrintf(pszDest, cchDest, pszFormat, pSH->Characteristics);
		lvItem.iSubItem = 5;
		SendDlgItemMessage(hDlg, IDC_SECTIONLIST, LVM_SETITEM, 0, (LPARAM)&lvItem);

		++pSH;
	}


}