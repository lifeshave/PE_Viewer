#pragma once
#ifndef _SECTIONTABLE_H
#define _SECTIONTABLE_H
#include<Windows.h>
#include"resource.h"
#include"Func.h"  //Ҫ������Ľṹ
#include<Strsafe.h>
#include<Commctrl.h>//LVCOLUMN�ṹҪ�õ����ͷ�ļ�
extern MAPFILE sMapFile;
extern HINSTANCE hInst;
LRESULT CALLBACK	SectionTableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void InitSectionListCtrl(HWND);
void ShowSectionHeaderInfo(HWND hDlg);

#endif
