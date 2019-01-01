#pragma once
#ifndef _EXPORTDLG_H
#define _EXPORTDLG_H
#include<Windows.h>
#include"resource.h"
#include"Func.h"  //要用里面的结构
#include<Strsafe.h>
#include<Commctrl.h>//LVCOLUMN结构要用到这个头文件
extern MAPFILE sMapFile;
extern HINSTANCE hInst;
LRESULT CALLBACK  ExportTableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void InitExportListCtrl(HWND hDlg);
void   ShowExportDirInfo(HWND hDlg);
void   ShowExportFuncsInfo(HWND hDlg);
#endif