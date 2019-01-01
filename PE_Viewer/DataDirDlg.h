#pragma once
#ifndef _DATADIRDLG_H
#define _DATADIRDLG_H
#include<Windows.h>
#include"resource.h"
#include"Func.h"  //要用里面的结构
#include<Strsafe.h>
typedef struct
{
	UINT   ID_RVA;
	UINT   ID_SIZE;
}MYDATADIRECTORY;
extern MAPFILE sMapFile;
extern HINSTANCE hInst;
LRESULT CALLBACK  DataDirDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
//关于对话框消息处理函数
LRESULT CALLBACK  AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void ShowDataDirInfo(HWND hDlg);
#endif
