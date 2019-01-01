#include"PE_Main.h"


int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	//窗口实例句柄经常用到,保存到全局变量中
	hInst = hInstance;
	//第二个参数为资源ID 第四个参数声明消息处理函数 第三个参数是父窗口句柄(LPCTSTR)IDD_PROPPAGE_LARGE
	DialogBox(hInstance, (LPCTSTR)IDD_MAIN_DLG, NULL, (DLGPROC)MainDlgProc);
	
	return TRUE;
}

//消息处理函数
LRESULT CALLBACK MainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId;
	static BOOL bFlag=1;
	HDC hdc = GetDC(hWnd);
	SetTextColor(hdc,RGB(0,0,255));
	switch (message)
	{
	case WM_INITDIALOG:
		//设置图标 
		LoadIcon(hInst, (LPCWSTR)("b2.ico"));
		return 0;
	case WM_COMMAND:
		//获取命令ID 低位为命令ID
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
			//第三参数为其父窗口句柄
			DialogBox(hInst, (LPCTSTR)IDD_DATADIR_DLG,hWnd, (DLGPROC)DataDirDlgProc);
			return TRUE;

		case IDC_BUTTON_SECTION_HEADER:
			DialogBox(hInst, (LPCTSTR)IDD_SHT_DLG, hWnd, (DLGPROC)SectionTableDlgProc);
			return TRUE;

		case IDC_BUTTON_IMPORT_DESCRIPTOR://导入表
			DialogBox(hInst, (LPCTSTR)IDD_IMPORT_DLG, hWnd, (DLGPROC)ImportTableDlgProc);
			return TRUE;
		case IDC_BUTTON_EXPORT_DIRECTORY://导出表
			DialogBox(hInst, (LPCTSTR)IDD_EXPORT_DLG, hWnd, (DLGPROC)ExportTableDlgProc);
			return TRUE;
		case IDC_BUTTON_MORE_FUNCTION:
			DialogBox(hInst, (LPCTSTR)IDD_MOREFUNCTION_DLG, hWnd, (DLGPROC)MoreFunctionDlgProc);
			return TRUE;
		case IDC_BUTTON_TRANSFORM://实现中英文转换
			AlterLanguage(hWnd,bFlag);
			bFlag = !bFlag;
			return TRUE;
		}
		return TRUE;
	case WM_DROPFILES:
		/*HDROP hDrop;*///不可定义变量
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
	return FALSE;									//消息默认处理不会显示窗口
}


//打开文件并显示
BOOL OpenAndShow(HWND hWnd)
{
	ClearShowText(hWnd);
	
	//将路径发送个IDC_EDIT1控件让其显示
	SendDlgItemMessage(hWnd, IDC_EDIT1, WM_SETTEXT, 0, (LPARAM)szFilePath);
	//卸载文件(非必要,防止打开过没卸载)
	UnLoadFile(&sMapFile);
	if (!LoadFile(szFilePath, &sMapFile))
	{
		//加载失败提示
		MessageBox(hWnd, TEXT("文件路径加载错误"), TEXT("友情提示"), MB_OK);
		return FALSE;
	}
	if (!IsPeFile(sMapFile.ImageBase))
	{
		//文件不是PE文件提示
		MessageBox(hWnd, TEXT("该文件不是PE格式文件"), TEXT("友情提示"), MB_OK);
		UnLoadFile(&sMapFile);
		return FALSE;
	}
	
	ShowInfo(hWnd);
	EnableEditCtrl(hWnd, TRUE);
	return FALSE;
}



//打开文件函数
BOOL OpenFileDlg(HWND hWnd)
{
	//此函数思路为:一个文件打开对话框的出现,需要OPENFILENAME结构体,所以先填结构体在调用GetOpenFileName
	OPENFILENAME oFileName;
	//清空全局变量szFilePath
	memset(szFilePath,0,MAX_PATH);
	//结构体初始化为0
	memset(&oFileName,0,sizeof(OPENFILENAME));

	//填结构体参数
	//结构体的长度
	oFileName.lStructSize = sizeof(oFileName);
	//标识文件打开对话框为我们的对话框
	oFileName.hwndOwner = hWnd;
	//获取本程序的实例句柄  GetModuleHandle获取一个应用程序或动态链接库的模块(实例)句柄 
	//指向程序模块的名称(可以是.DLL或是.exe文件)，如果没有后缀名，则默认是.dll。若为NULL则取本程序的模块向柄
	oFileName.hInstance = GetModuleHandle(NULL);
	//指定由LpstrFile成员指向的缓冲区字符数
	oFileName.nMaxFile = MAX_PATH;
	//指向一个缓冲区，该缓冲区存放用于初始化FileName编辑控制的文件名．若不必要初始化，
	//则此缓冲区的第一个字符必须为NULL．当GetOpenFileName或GetSaveFileName函数返回时，
	//该缓冲区存放所选文件的驱动标志符，路径，文件名和扩展名
	oFileName.lpstrFile = szFilePath;
	
	oFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST ;
	//路径在此获得 调GetOpenFileName会填充结构体
	if (!GetOpenFileName(&oFileName))
	{
		return FALSE;
	}
	return TRUE;
}

//打开文件函数
BOOL OpenFileByDrag(WPARAM wParam)
{
	
	//清空全局变量szFilePath
	memset(szFilePath, 0, MAX_PATH);
	//DragQueryFile第二个参数为-1则返回拖拽的文件数量,为其他值则相当于数组下标填充文件绝对路径,第三个参数填充文件路径
	DragQueryFile((HDROP)wParam, 0, (LPTSTR)szFilePath, MAX_PATH);
	
	return TRUE;
	
}
//显示信息
void ShowInfo(HWND hWnd)
{
	//存放要显示的字符
	TCHAR pszDest[30] = {'\0'};
	size_t cchDest = 30;

	LPCTSTR pszFormat = TEXT("%04lX");	//WORD格式
	LPCTSTR pszFormat1 = TEXT("%08lX");	//DWORD格式显示
	
	//文件头指针
	PIMAGE_FILE_HEADER pFH = NULL;
	//时间存放结构体,用来存放时间以方便显示
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
	
	

	
	//-------------------------------------完美分割线---------------------------------//


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


//改变按钮状态
void    EnableEditCtrl(HWND hWnd, BOOL bTemp)
{
	//设置数据目录表按钮状态
	EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_DATA_DIRECTORY), bTemp);
	//设置数据区块表按钮状态
	EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_SECTION_HEADER), bTemp);
	//设置输入表按钮状态
	EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_IMPORT_DESCRIPTOR), bTemp);
	//设置输出表按钮状态
	EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_EXPORT_DIRECTORY), bTemp);
	
}

//本函数参考 ShowInfo 函数对应清空
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




	//-------------------------------------完美分割线---------------------------------//


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
	//每次打开文件前将其状态设置为不可操作
	EnableEditCtrl(hWnd, FALSE);
}