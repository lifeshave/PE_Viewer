#include"Func.h" //����ͷ�ļ�
#include"DataDirDlg.h"
#include"SectionTable.h"
#include"ImportDlg.h"
#include"ExportDlg.h"
#include"MoreFunction.h"
#include"AlterLanguage.h"
#include<string.h>
#include<time.h>
#include<Shellapi.h>//WM_DROPFIlES��ͷ�ļ�
TCHAR szFilePath[MAX_PATH];			//���ڱ����ļ�·��
HINSTANCE hInst;					//ʵ�����

MAPFILE sMapFile = {0,0,0};
//��Ϣ������
LRESULT CALLBACK MainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//�򿪲���ʾ��Ϣ
BOOL OpenAndShow(HWND hWnd);
//�ж��ļ��򿪳ɹ����
BOOL OpenFileDlg(HWND hWnd);
//ͨ����ק���ļ�
BOOL OpenFileByDrag(WPARAM wParam);
//���ǰ����ʾ����Ϣ
void ClearShowText(HWND hWnd);
//��ʾPE��Ϣ
void ShowInfo(HWND hWnd);
//�ı䰴ť״̬
void    EnableEditCtrl(HWND hWnd, BOOL bTemp);