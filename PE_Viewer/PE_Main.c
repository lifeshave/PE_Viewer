#include"PE_Main.h"


int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	//����ʵ����������õ�,���浽ȫ�ֱ�����
	hInst = hInstance;
	//�ڶ�������Ϊ��ԴID ���ĸ�����������Ϣ������ �����������Ǹ����ھ��(LPCTSTR)IDD_PROPPAGE_LARGE
	DialogBox(hInstance, (LPCTSTR)IDD_MAIN_DLG, NULL, (DLGPROC)MainDlgProc);
	
	return TRUE;
}

//��Ϣ������
LRESULT CALLBACK MainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId;
	static BOOL bFlag=1;
	HDC hdc = GetDC(hWnd);
	SetTextColor(hdc,RGB(0,0,255));
	switch (message)
	{
	case WM_INITDIALOG:
		//����ͼ�� 
		LoadIcon(hInst, (LPCWSTR)("b2.ico"));
		return 0;
	case WM_COMMAND:
		//��ȡ����ID ��λΪ����ID
		wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDC_BUTTON_OPEN:
			if (!OpenFileDlg(hWnd))
			{
				return FALSE;
			}
			if (OpenAndShow(hWnd))
			{
				return TRUE;
			}
			return FALSE;
		case IDC_BUTTON_DATA_DIRECTORY:
			//��������Ϊ�丸���ھ��
			DialogBox(hInst, (LPCTSTR)IDD_DATADIR_DLG,hWnd, (DLGPROC)DataDirDlgProc);
			return TRUE;

		case IDC_BUTTON_SECTION_HEADER:
			DialogBox(hInst, (LPCTSTR)IDD_SHT_DLG, hWnd, (DLGPROC)SectionTableDlgProc);
			return TRUE;

		case IDC_BUTTON_IMPORT_DESCRIPTOR://�����
			DialogBox(hInst, (LPCTSTR)IDD_IMPORT_DLG, hWnd, (DLGPROC)ImportTableDlgProc);
			return TRUE;
		case IDC_BUTTON_EXPORT_DIRECTORY://������
			DialogBox(hInst, (LPCTSTR)IDD_EXPORT_DLG, hWnd, (DLGPROC)ExportTableDlgProc);
			return TRUE;
		case IDC_BUTTON_MORE_FUNCTION:
			DialogBox(hInst, (LPCTSTR)IDD_MOREFUNCTION_DLG, hWnd, (DLGPROC)MoreFunctionDlgProc);
			return TRUE;
		case IDC_BUTTON_TRANSFORM://ʵ����Ӣ��ת��
			AlterLanguage(hWnd,bFlag);
			bFlag = !bFlag;
			return TRUE;
		}
		return TRUE;
	case WM_DROPFILES:
		/*HDROP hDrop;*///���ɶ������
		/*DragQueryFile((HDROP)wParam, 0, (LPTSTR)szFilePath, MAX_PATH);*/
		if (!OpenFileByDrag(wParam))
		{
			return FALSE;
		}
		if (OpenAndShow(hWnd))
		{
			return TRUE;
		}
		return FALSE;
	
	case WM_CLOSE:
		EndDialog(hWnd,0);
		return TRUE;
	}
	ReleaseDC(hWnd,hdc);
	return FALSE;									//��ϢĬ�ϴ�������ʾ����
}


//���ļ�����ʾ
BOOL OpenAndShow(HWND hWnd)
{
	ClearShowText(hWnd);
	
	//��·�����͸�IDC_EDIT1�ؼ�������ʾ
	SendDlgItemMessage(hWnd, IDC_EDIT1, WM_SETTEXT, 0, (LPARAM)szFilePath);
	//ж���ļ�(�Ǳ�Ҫ,��ֹ�򿪹�ûж��)
	UnLoadFile(&sMapFile);
	if (!LoadFile(szFilePath, &sMapFile))
	{
		//����ʧ����ʾ
		MessageBox(hWnd, TEXT("�ļ�·�����ش���"), TEXT("������ʾ"), MB_OK);
		return FALSE;
	}
	if (!IsPeFile(sMapFile.ImageBase))
	{
		//�ļ�����PE�ļ���ʾ
		MessageBox(hWnd, TEXT("���ļ�����PE��ʽ�ļ�"), TEXT("������ʾ"), MB_OK);
		UnLoadFile(&sMapFile);
		return FALSE;
	}
	
	ShowInfo(hWnd);
	EnableEditCtrl(hWnd, TRUE);
	return FALSE;
}



//���ļ�����
BOOL OpenFileDlg(HWND hWnd)
{
	//�˺���˼·Ϊ:һ���ļ��򿪶Ի���ĳ���,��ҪOPENFILENAME�ṹ��,��������ṹ���ڵ���GetOpenFileName
	OPENFILENAME oFileName;
	//���ȫ�ֱ���szFilePath
	memset(szFilePath,0,MAX_PATH);
	//�ṹ���ʼ��Ϊ0
	memset(&oFileName,0,sizeof(OPENFILENAME));

	//��ṹ�����
	//�ṹ��ĳ���
	oFileName.lStructSize = sizeof(oFileName);
	//��ʶ�ļ��򿪶Ի���Ϊ���ǵĶԻ���
	oFileName.hwndOwner = hWnd;
	//��ȡ�������ʵ�����  GetModuleHandle��ȡһ��Ӧ�ó����̬���ӿ��ģ��(ʵ��)��� 
	//ָ�����ģ�������(������.DLL����.exe�ļ�)�����û�к�׺������Ĭ����.dll����ΪNULL��ȡ�������ģ�����
	oFileName.hInstance = GetModuleHandle(NULL);
	//ָ����LpstrFile��Աָ��Ļ������ַ���
	oFileName.nMaxFile = MAX_PATH;
	//ָ��һ�����������û�����������ڳ�ʼ��FileName�༭���Ƶ��ļ�����������Ҫ��ʼ����
	//��˻������ĵ�һ���ַ�����ΪNULL����GetOpenFileName��GetSaveFileName��������ʱ��
	//�û����������ѡ�ļ���������־����·�����ļ�������չ��
	oFileName.lpstrFile = szFilePath;
	
	oFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST ;
	//·���ڴ˻�� ��GetOpenFileName�����ṹ��
	if (!GetOpenFileName(&oFileName))
	{
		return FALSE;
	}
	return TRUE;
}

//���ļ�����
BOOL OpenFileByDrag(WPARAM wParam)
{
	
	//���ȫ�ֱ���szFilePath
	memset(szFilePath, 0, MAX_PATH);
	//DragQueryFile�ڶ�������Ϊ-1�򷵻���ק���ļ�����,Ϊ����ֵ���൱�������±�����ļ�����·��,��������������ļ�·��
	DragQueryFile((HDROP)wParam, 0, (LPTSTR)szFilePath, MAX_PATH);
	
	return TRUE;
	
}
//��ʾ��Ϣ
void ShowInfo(HWND hWnd)
{
	//���Ҫ��ʾ���ַ�
	TCHAR pszDest[30] = {'\0'};
	size_t cchDest = 30;

	LPCTSTR pszFormat = TEXT("%04lX");	//WORD��ʽ
	LPCTSTR pszFormat1 = TEXT("%08lX");	//DWORD��ʽ��ʾ
	
	//�ļ�ͷָ��
	PIMAGE_FILE_HEADER pFH = NULL;
	//ʱ���Žṹ��,�������ʱ���Է�����ʾ
	struct tm tmTemp;
	
	pFH = GetFileHeader(sMapFile.ImageBase);
	if (!pFH)
	{
		MessageBox(hWnd, TEXT("Can't get File Header ! "), TEXT("PEInfo"), MB_OK);
		return;
	}
	PIMAGE_OPTIONAL_HEADER pOH = NULL;
	pOH = GetOptionalHeader(sMapFile.ImageBase);
	if (!pOH)
	{
		MessageBox(hWnd, TEXT("Can't get Optional Header !"), TEXT("PEInfo"), MB_OK);
		return;
	}
	StringCchPrintf(pszDest, cchDest, pszFormat, pFH->Machine);
	SetDlgItemText(hWnd, IDC_EDIT_FH_MACHINE, pszDest);

	
	StringCchPrintf(pszDest, cchDest, pszFormat, pFH->NumberOfSections);
	SetDlgItemText(hWnd, IDC_EDIT_FH_NUMOFSECTIONS, pszDest);

	
	gmtime_s(&tmTemp,(time_t*)(&pFH->TimeDateStamp));
	
	LPCTSTR pszFormatTemp = TEXT("%d-%02d-%02d %02d:%02d:%2d");
	StringCchPrintf(pszDest, cchDest, pszFormatTemp, tmTemp.tm_year + 1990, tmTemp.tm_mon + 1, tmTemp.tm_mday,
		tmTemp.tm_hour,tmTemp.tm_min,tmTemp.tm_sec);
	SetDlgItemText(hWnd, IDC_EDIT_FH_TDS, pszDest);


	StringCchPrintf(pszDest, cchDest, pszFormat, pFH->PointerToSymbolTable);
	SetDlgItemText(hWnd, IDC_EDIT_FH_PTSYMBOL, pszDest);

	
	StringCchPrintf(pszDest, cchDest, pszFormat, pFH->NumberOfSymbols);
	SetDlgItemText(hWnd, IDC_EDIT_FH_NUMOFSYM, pszDest);

	
	StringCchPrintf(pszDest, cchDest, pszFormat, pFH->SizeOfOptionalHeader);
	SetDlgItemText(hWnd, IDC_EDIT_FH_SIZEOFOH, pszDest);


	StringCchPrintf(pszDest, cchDest, pszFormat, pFH->Characteristics);
	SetDlgItemText(hWnd, IDC_EDIT_FH_CHARACTERISTICS, pszDest);


	
	StringCchPrintf(pszDest, cchDest, pszFormat, pOH->DllCharacteristics);
	SetDlgItemText(hWnd, IDC_EDIT_OH_DLLFLAGS, pszDest);
	
	

	
	//-------------------------------------�����ָ���---------------------------------//


	StringCchPrintf(pszDest, cchDest, pszFormat1, pOH->AddressOfEntryPoint);
	SetDlgItemText(hWnd, IDC_EDIT_OH_EP, pszDest);

	
	StringCchPrintf(pszDest, cchDest, pszFormat1, pOH->ImageBase);
	SetDlgItemText(hWnd, IDC_EDIT_OH_IMAGEBASE, pszDest);


	StringCchPrintf(pszDest, cchDest, pszFormat1, pOH->BaseOfCode);
	SetDlgItemText(hWnd, IDC_EDIT_OH_CODEBASE, pszDest);

	
	StringCchPrintf(pszDest, cchDest, pszFormat1, pOH->BaseOfData);
	SetDlgItemText(hWnd, IDC_EDIT_OH_DATABASE, pszDest);


	StringCchPrintf(pszDest, cchDest, pszFormat1, pOH->SizeOfImage);
	SetDlgItemText(hWnd, IDC_EDIT_OH_IMAGESIZE, pszDest);

	
	StringCchPrintf(pszDest, cchDest, pszFormat1, pOH->SizeOfHeaders);
	SetDlgItemText(hWnd, IDC_EDIT_OH_HEADERSIZE, pszDest);

	
	StringCchPrintf(pszDest, cchDest, pszFormat1, pOH->SectionAlignment);
	SetDlgItemText(hWnd, IDC_EDIT_OH_SECTIONALIGN, pszDest);

	StringCchPrintf(pszDest, cchDest, pszFormat1, pOH->FileAlignment);
	SetDlgItemText(hWnd, IDC_EDIT_OH_FILEALIGN, pszDest);

	
	StringCchPrintf(pszDest, cchDest, pszFormat1, pOH->Subsystem);
	SetDlgItemText(hWnd, IDC_EDIT_OH_SUBSYSTEM, pszDest);

	
	StringCchPrintf(pszDest, cchDest, pszFormat1, pOH->CheckSum);
	SetDlgItemText(hWnd, IDC_EDIT_OH_CHECKSUM, pszDest);

	
}


//�ı䰴ť״̬
void    EnableEditCtrl(HWND hWnd, BOOL bTemp)
{
	//��������Ŀ¼��ť״̬
	EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_DATA_DIRECTORY), bTemp);
	//�������������ť״̬
	EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_SECTION_HEADER), bTemp);
	//���������ť״̬
	EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_IMPORT_DESCRIPTOR), bTemp);
	//���������ť״̬
	EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_EXPORT_DIRECTORY), bTemp);
	
}

//�������ο� ShowInfo ������Ӧ���
void ClearShowText(HWND hWnd)
{
	
	SetDlgItemText(hWnd, IDC_EDIT_FH_MACHINE, TEXT(""));

	SetDlgItemText(hWnd, IDC_EDIT_FH_NUMOFSECTIONS, TEXT(""));

	SetDlgItemText(hWnd, IDC_EDIT_FH_TDS, TEXT(""));

	SetDlgItemText(hWnd, IDC_EDIT_FH_PTSYMBOL, TEXT(""));

	SetDlgItemText(hWnd, IDC_EDIT_FH_NUMOFSYM, TEXT(""));

	SetDlgItemText(hWnd, IDC_EDIT_FH_SIZEOFOH, TEXT(""));

	SetDlgItemText(hWnd, IDC_EDIT_FH_CHARACTERISTICS, TEXT(""));

	SetDlgItemText(hWnd, IDC_EDIT_OH_DLLFLAGS, TEXT(""));




	//-------------------------------------�����ָ���---------------------------------//


	SetDlgItemText(hWnd, IDC_EDIT_OH_EP, TEXT(""));


	SetDlgItemText(hWnd, IDC_EDIT_OH_IMAGEBASE, TEXT(""));


	SetDlgItemText(hWnd, IDC_EDIT_OH_CODEBASE, TEXT(""));


	SetDlgItemText(hWnd, IDC_EDIT_OH_DATABASE, TEXT(""));


	SetDlgItemText(hWnd, IDC_EDIT_OH_IMAGESIZE, TEXT(""));

	SetDlgItemText(hWnd, IDC_EDIT_OH_HEADERSIZE, TEXT(""));


	SetDlgItemText(hWnd, IDC_EDIT_OH_SECTIONALIGN, TEXT(""));

	SetDlgItemText(hWnd, IDC_EDIT_OH_FILEALIGN, TEXT(""));

	SetDlgItemText(hWnd, IDC_EDIT_OH_SUBSYSTEM, TEXT(""));

	SetDlgItemText(hWnd, IDC_EDIT_OH_CHECKSUM, TEXT(""));
	//ÿ�δ��ļ�ǰ����״̬����Ϊ���ɲ���
	EnableEditCtrl(hWnd, FALSE);
}