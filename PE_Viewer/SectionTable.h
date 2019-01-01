#pragma once
#ifndef _SECTIONTABLE_H
#define _SECTIONTABLE_H
#include<Windows.h>
#include"resource.h"
#include"Func.h"  //要用里面的结构
#include<Strsafe.h>
#include<Commctrl.h>//LVCOLUMN结构要用到这个头文件
extern MAPFILE sMapFile;
extern HINSTANCE hInst;
LRESULT CALLBACK	SectionTableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void InitSectionListCtrl(HWND);
void ShowSectionHeaderInfo(HWND hDlg);

#endif
