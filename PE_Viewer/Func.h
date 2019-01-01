#pragma once
#include<Windows.h>
#include"resource.h"
#include<Dbghelp.h>//ImageRvaToVa此API在这个头文件中
#ifndef _FUNC_H
#define _FUNC_H
typedef struct _MAP_FILE_STURCT
{
	HANDLE hFile;
	HANDLE hMapping;
	LPVOID ImageBase;
}MAPFILE, *PMAPFILE;

//加载文件
BOOL LoadFile(LPTSTR lpFileName,PMAPFILE pMapFile);

//卸载文件
void UnLoadFile(PMAPFILE pMapFile);

//判断是否为PE文件
BOOL IsPeFile(LPVOID ImageBase);

//获取PE头指针
PIMAGE_NT_HEADERS GetNtHeader(LPVOID ImageBase);

//获取文件头指针
PIMAGE_FILE_HEADER GetFileHeader(LPVOID ImageBase);

//获取可选头指针
PIMAGE_OPTIONAL_HEADER GetOptionalHeader(LPVOID ImageBase);

//获取第一区块头指针
PIMAGE_SECTION_HEADER  GetFirstSectionHeader(LPVOID ImageBase);

//获取输入表指针
PIMAGE_IMPORT_DESCRIPTOR  GetFirstImportDesc(LPVOID ImageBase);

LPVOID RvaToPtr(PIMAGE_NT_HEADERS pNtH, LPVOID ImageBase, DWORD dwRVA);

LPVOID GetDirectoryEntryToData(LPVOID ImageBase, USHORT DirectoryEntry);

PIMAGE_EXPORT_DIRECTORY GetExportDirectory(LPVOID ImageBase);
#endif
