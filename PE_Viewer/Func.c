#include"Func.h"
#pragma comment(lib,"Dbghelp.lib")//ImageRvaToVa使用此API需要导库进来
//文件加载
BOOL LoadFile(LPTSTR lpFileName, PMAPFILE pMapFile)
{
	HANDLE hFile;
	HANDLE hMapping;
	LPVOID ImageBase;
	memset(pMapFile,0,sizeof(MAPFILE));
	//打开文件  CreateFile这是一个全功能的例程，可打开和创建文件、管道、邮槽、通信服务、设备以及控制台
	hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,0);
	//判断文件打开是否成功
	if (!hFile)
	{
		return FALSE;
	}
	//创建映射文件
	hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY,0,0,NULL);
	//文件映射对象映射到当前应用程序的地址空间
	ImageBase = MapViewOfFile(hMapping, FILE_MAP_READ,0,0,0);

	//判断是否映射成功
	if (!ImageBase)
	{
		//不成功就关闭句柄并返回
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return FALSE;
	}

	//保存MAPFILE结构中值
	pMapFile->hFile = hFile;
	pMapFile->hMapping = hMapping;
	pMapFile->ImageBase = ImageBase;
	
	return TRUE;

}

//卸载文件
void UnLoadFile(PMAPFILE pMapFile)
{
	if (!pMapFile->ImageBase)
	{
		//在当前应用程序的内存地址空间解除对一个文件映射对象的映射
		UnmapViewOfFile(pMapFile->hMapping);
	}
	if (!pMapFile->hMapping)
	{
		//关闭一个内核对象。其中包括文件、文件映射、进程、线程、安全和同步对象等。
		CloseHandle(pMapFile->hMapping);
	}
	if (!pMapFile->hFile)
	{
		CloseHandle(pMapFile->hFile);
	}
}

//判断是否为PE文件
BOOL IsPeFile(LPVOID ImageBase)
{
	PIMAGE_DOS_HEADER pDosHeader=NULL;
	PIMAGE_NT_HEADERS pNtHeader = NULL;
	//检查是否为有效地址
	if (!ImageBase)
	{
		return FALSE;
	}
	//判断dos头部
	pDosHeader = (PIMAGE_DOS_HEADER)ImageBase;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return FALSE;
	}
	//判断pe头,由于比较PE标志位需要四字节故强转pDosHeader
	pNtHeader = (PIMAGE_NT_HEADERS32)((DWORD)pDosHeader + pDosHeader->e_lfanew);
	if (pNtHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}
	return TRUE;
}

//获取PE头的指针
PIMAGE_NT_HEADERS GetNtHeader(LPVOID ImageBase)
{
	if (!IsPeFile(ImageBase))
	{
		return FALSE;
	}
	PIMAGE_DOS_HEADER pDosHeader = NULL;
	PIMAGE_NT_HEADERS pNtHeader = NULL;
	pDosHeader = (PIMAGE_DOS_HEADER)ImageBase;
	pNtHeader = (PIMAGE_NT_HEADERS32)((DWORD)pDosHeader + pDosHeader->e_lfanew);
	return pNtHeader;
}

//获取文件头指针
PIMAGE_FILE_HEADER GetFileHeader(LPVOID ImageBase)
{
	PIMAGE_FILE_HEADER pFileHeader = NULL;
	PIMAGE_NT_HEADERS pNtHeader = GetNtHeader(ImageBase);
	pFileHeader = &pNtHeader->FileHeader;
	return pFileHeader;
}

//获取可选头指针
PIMAGE_OPTIONAL_HEADER GetOptionalHeader(LPVOID ImageBase)
{
	PIMAGE_OPTIONAL_HEADER pOptionalHeader = NULL;
	PIMAGE_NT_HEADERS pNtHeader = GetNtHeader(ImageBase);
	pOptionalHeader = &pNtHeader->OptionalHeader;
	return pOptionalHeader;
}

//获取第一区块头指针
PIMAGE_SECTION_HEADER  GetFirstSectionHeader(LPVOID ImageBase)
{
	PIMAGE_NT_HEADERS     pNtH = NULL;
	PIMAGE_SECTION_HEADER pSH = NULL;

	pNtH = GetNtHeader(ImageBase);
	//winnt.h头文件中定义的宏,可以通过NtH获得第一块表地址
	pSH = IMAGE_FIRST_SECTION(pNtH);
	return  pSH;
}

//获取输入表指针
PIMAGE_IMPORT_DESCRIPTOR  GetFirstImportDesc(LPVOID ImageBase)
{
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
	pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)GetDirectoryEntryToData(ImageBase, IMAGE_DIRECTORY_ENTRY_IMPORT);
	if (!pImportDesc)
		return NULL;
	return  pImportDesc;
}
//
LPVOID RvaToPtr(PIMAGE_NT_HEADERS pNtH, LPVOID ImageBase, DWORD dwRVA)
{
	return ImageRvaToVa(pNtH, ImageBase, dwRVA, NULL);
}

LPVOID GetDirectoryEntryToData(LPVOID ImageBase, USHORT DirectoryEntry)
{
	DWORD dwDataStartRVA;
	LPVOID pDirData = NULL;
	PIMAGE_NT_HEADERS     pNtH = NULL;
	PIMAGE_OPTIONAL_HEADER pOH = NULL;

	pNtH = GetNtHeader(ImageBase);
	if (!pNtH)
		return NULL;
	pOH = GetOptionalHeader(ImageBase);
	if (!pOH)
		return NULL;
	dwDataStartRVA = pOH->DataDirectory[DirectoryEntry].VirtualAddress;
	if (!dwDataStartRVA)
		return NULL;

	pDirData = RvaToPtr(pNtH, ImageBase, dwDataStartRVA);
	if (!pDirData)
		return NULL;
	return  pDirData;
}

PIMAGE_EXPORT_DIRECTORY  GetExportDirectory(LPVOID ImageBase)
{

	PIMAGE_EXPORT_DIRECTORY pExportDir = NULL;
	pExportDir = (PIMAGE_EXPORT_DIRECTORY)GetDirectoryEntryToData(ImageBase, IMAGE_DIRECTORY_ENTRY_EXPORT);
	if (!pExportDir)
		return NULL;
	return  pExportDir;
}