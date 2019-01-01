#include"Func.h"
#pragma comment(lib,"Dbghelp.lib")//ImageRvaToVaʹ�ô�API��Ҫ�������
//�ļ�����
BOOL LoadFile(LPTSTR lpFileName, PMAPFILE pMapFile)
{
	HANDLE hFile;
	HANDLE hMapping;
	LPVOID ImageBase;
	memset(pMapFile,0,sizeof(MAPFILE));
	//���ļ�  CreateFile����һ��ȫ���ܵ����̣��ɴ򿪺ʹ����ļ����ܵ����ʲۡ�ͨ�ŷ����豸�Լ�����̨
	hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,0);
	//�ж��ļ����Ƿ�ɹ�
	if (!hFile)
	{
		return FALSE;
	}
	//����ӳ���ļ�
	hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY,0,0,NULL);
	//�ļ�ӳ�����ӳ�䵽��ǰӦ�ó���ĵ�ַ�ռ�
	ImageBase = MapViewOfFile(hMapping, FILE_MAP_READ,0,0,0);

	//�ж��Ƿ�ӳ��ɹ�
	if (!ImageBase)
	{
		//���ɹ��͹رվ��������
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return FALSE;
	}

	//����MAPFILE�ṹ��ֵ
	pMapFile->hFile = hFile;
	pMapFile->hMapping = hMapping;
	pMapFile->ImageBase = ImageBase;
	
	return TRUE;

}

//ж���ļ�
void UnLoadFile(PMAPFILE pMapFile)
{
	if (!pMapFile->ImageBase)
	{
		//�ڵ�ǰӦ�ó�����ڴ��ַ�ռ�����һ���ļ�ӳ������ӳ��
		UnmapViewOfFile(pMapFile->hMapping);
	}
	if (!pMapFile->hMapping)
	{
		//�ر�һ���ں˶������а����ļ����ļ�ӳ�䡢���̡��̡߳���ȫ��ͬ������ȡ�
		CloseHandle(pMapFile->hMapping);
	}
	if (!pMapFile->hFile)
	{
		CloseHandle(pMapFile->hFile);
	}
}

//�ж��Ƿ�ΪPE�ļ�
BOOL IsPeFile(LPVOID ImageBase)
{
	PIMAGE_DOS_HEADER pDosHeader=NULL;
	PIMAGE_NT_HEADERS pNtHeader = NULL;
	//����Ƿ�Ϊ��Ч��ַ
	if (!ImageBase)
	{
		return FALSE;
	}
	//�ж�dosͷ��
	pDosHeader = (PIMAGE_DOS_HEADER)ImageBase;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return FALSE;
	}
	//�ж�peͷ,���ڱȽ�PE��־λ��Ҫ���ֽڹ�ǿתpDosHeader
	pNtHeader = (PIMAGE_NT_HEADERS32)((DWORD)pDosHeader + pDosHeader->e_lfanew);
	if (pNtHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}
	return TRUE;
}

//��ȡPEͷ��ָ��
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

//��ȡ�ļ�ͷָ��
PIMAGE_FILE_HEADER GetFileHeader(LPVOID ImageBase)
{
	PIMAGE_FILE_HEADER pFileHeader = NULL;
	PIMAGE_NT_HEADERS pNtHeader = GetNtHeader(ImageBase);
	pFileHeader = &pNtHeader->FileHeader;
	return pFileHeader;
}

//��ȡ��ѡͷָ��
PIMAGE_OPTIONAL_HEADER GetOptionalHeader(LPVOID ImageBase)
{
	PIMAGE_OPTIONAL_HEADER pOptionalHeader = NULL;
	PIMAGE_NT_HEADERS pNtHeader = GetNtHeader(ImageBase);
	pOptionalHeader = &pNtHeader->OptionalHeader;
	return pOptionalHeader;
}

//��ȡ��һ����ͷָ��
PIMAGE_SECTION_HEADER  GetFirstSectionHeader(LPVOID ImageBase)
{
	PIMAGE_NT_HEADERS     pNtH = NULL;
	PIMAGE_SECTION_HEADER pSH = NULL;

	pNtH = GetNtHeader(ImageBase);
	//winnt.hͷ�ļ��ж���ĺ�,����ͨ��NtH��õ�һ����ַ
	pSH = IMAGE_FIRST_SECTION(pNtH);
	return  pSH;
}

//��ȡ�����ָ��
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