#include"Func.h" //功能头文件
#include"DataDirDlg.h"
#include"SectionTable.h"
#include"ImportDlg.h"
#include"ExportDlg.h"
#include"MoreFunction.h"
#include"AlterLanguage.h"
#include<string.h>
#include<time.h>
#include<Shellapi.h>//WM_DROPFIlES的头文件
TCHAR szFilePath[MAX_PATH];			//用于保存文件路径
HINSTANCE hInst;					//实例句柄

MAPFILE sMapFile = {0,0,0};
//消息处理函数
LRESULT CALLBACK MainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//打开并显示信息
BOOL OpenAndShow(HWND hWnd);
//判断文件打开成功与否
BOOL OpenFileDlg(HWND hWnd);
//通过拖拽打开文件
BOOL OpenFileByDrag(WPARAM wParam);
//清空前面显示的信息
void ClearShowText(HWND hWnd);
//显示PE信息
void ShowInfo(HWND hWnd);
//改变按钮状态
void    EnableEditCtrl(HWND hWnd, BOOL bTemp);