#pragma once
#ifndef _IMPORTDLG_H
#define _IMPORTDLG_H
#include<Windows.h>
#include"resource.h"
#include"Func.h"  //Ҫ������Ľṹ
#include<Strsafe.h>
#include<Commctrl.h>//LVCOLUMN�ṹҪ�õ����ͷ�ļ�
extern MAPFILE sMapFile;
extern HINSTANCE hInst;
LRESULT CALLBACK  ImportTableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void InitImportListCtrl(HWND hDlg);
void InitImportFunctionsListCtrl(HWND hDlg);
void  ShowImportDescInfo(HWND hDlg);
void ShowImportFuncsByDllIndex(HWND hDlg, int index);
#endif