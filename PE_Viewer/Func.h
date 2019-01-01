#pragma once
#include<Windows.h>
#include"resource.h"
#include<Dbghelp.h>//ImageRvaToVa��API�����ͷ�ļ���
#ifndef _FUNC_H
#define _FUNC_H
typedef struct _MAP_FILE_STURCT
{
	HANDLE hFile;
	HANDLE hMapping;
	LPVOID ImageBase;
}MAPFILE, *PMAPFILE;

//�����ļ�
BOOL LoadFile(LPTSTR lpFileName,PMAPFILE pMapFile);

//ж���ļ�
void UnLoadFile(PMAPFILE pMapFile);

//�ж��Ƿ�ΪPE�ļ�
BOOL IsPeFile(LPVOID ImageBase);

//��ȡPEͷָ��
PIMAGE_NT_HEADERS GetNtHeader(LPVOID ImageBase);

//��ȡ�ļ�ͷָ��
PIMAGE_FILE_HEADER GetFileHeader(LPVOID ImageBase);

//��ȡ��ѡͷָ��
PIMAGE_OPTIONAL_HEADER GetOptionalHeader(LPVOID ImageBase);

//��ȡ��һ����ͷָ��
PIMAGE_SECTION_HEADER  GetFirstSectionHeader(LPVOID ImageBase);

//��ȡ�����ָ��
PIMAGE_IMPORT_DESCRIPTOR  GetFirstImportDesc(LPVOID ImageBase);

LPVOID RvaToPtr(PIMAGE_NT_HEADERS pNtH, LPVOID ImageBase, DWORD dwRVA);

LPVOID GetDirectoryEntryToData(LPVOID ImageBase, USHORT DirectoryEntry);

PIMAGE_EXPORT_DIRECTORY GetExportDirectory(LPVOID ImageBase);
#endif
