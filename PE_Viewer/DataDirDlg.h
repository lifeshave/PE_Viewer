#pragma once
#ifndef _DATADIRDLG_H
#define _DATADIRDLG_H
#include<Windows.h>
#include"resource.h"
#include"Func.h"  //Ҫ������Ľṹ
#include<Strsafe.h>
typedef struct
{
	UINT   ID_RVA;
	UINT   ID_SIZE;
}MYDATADIRECTORY;
extern MAPFILE sMapFile;
extern HINSTANCE hInst;
LRESULT CALLBACK  DataDirDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
//���ڶԻ�����Ϣ������
LRESULT CALLBACK  AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void ShowDataDirInfo(HWND hDlg);
#endif
