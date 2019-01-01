#include"AlterLanguage.h"
//�ú�������������������Ӣ���л�
void  AlterLanguage(HWND hWnd,BOOL bFlag)
{
	STATICTEXT sChinese[] = {
		{ IDC_STATIC_OH_EP, TEXT("����ִ�����:") },
		{ IDC_STATIC_OH_IMAGEBASE, TEXT("����װ���ַ:") },

		{ IDC_STATIC_OH_CODEBASE, TEXT("��������RVA:") },
		{ IDC_STATIC_OH_DATABASE, TEXT("��������RVA:") },

		{ IDC_STATIC_OH_IMAGESIZE, TEXT("ӳ���С:") },
		{ IDC_STATIC_OH_HEADERSIZE, TEXT("PEͷ+�����:") },

		{ IDC_STATIC_OH_SECTIONALIGN, TEXT("�ڴ��ж���ֵ:") },
		{ IDC_STATIC_OH_FILEALIGN, TEXT("�ļ��ж���ֵ:") },

		{ IDC_STATIC_OH_SUBSYSTEM, TEXT("�ļ���ϵͳ:") },
		{ IDC_STATIC_OH_CHECKSUM, TEXT("ӳ��У���:") },

		{ IDC_STATIC_OH_DLLFLAGS, TEXT("��ʾDLL����:") },
		{ IDC_STATIC_FH_MACHINE, TEXT("����ƽ̨:") },

		{ IDC_STATIC_FH_NUMOFSECTIONS, TEXT("������Ŀ:") },
		{ IDC_STATIC_FH_TDS, TEXT("�������ں�ʱ��:") },

		{ IDC_STATIC_FH_PTSYMBOL, TEXT("ָ����ű�:") },
		{ IDC_STATIC_FH_NUMOFSYM, TEXT("���Ÿ���:") },

		{ IDC_STATIC_FH_SIZEOFOH, TEXT("��ѡͷ��С:") },
		{ IDC_STATIC_FH_CHARACTERISTICS, TEXT("�ļ�����:") },

		{ IDC_BUTTON_OPEN, TEXT("��") },
		{ IDC_BUTTON_MORE_FUNCTION, TEXT("����") },

		{ IDC_BUTTON_DATA_DIRECTORY, TEXT("����Ŀ¼��") },
		{ IDC_BUTTON_SECTION_HEADER, TEXT("����(��)��") },
		
		{ IDC_BUTTON_IMPORT_DESCRIPTOR, TEXT("�����") },
		{ IDC_BUTTON_EXPORT_DIRECTORY, TEXT("������") },

		{ IDC_BUTTON_TRANSFORM, TEXT("Ӣ��") },
		{ IDC_STATIC_PE_INFO, TEXT("����PE��Ϣ") }
	};
	STATICTEXT sEnglish[] = {
		{ IDC_STATIC_OH_EP, TEXT("Entry Point:") },
		{ IDC_STATIC_OH_IMAGEBASE, TEXT("Image Base:") },

		{ IDC_STATIC_OH_CODEBASE, TEXT("Code Base:") },
		{ IDC_STATIC_OH_DATABASE, TEXT("Data Base:") },

		{ IDC_STATIC_OH_IMAGESIZE, TEXT("Image Size:") },
		{ IDC_STATIC_OH_HEADERSIZE, TEXT("Headers Size:") },

		{ IDC_STATIC_OH_SECTIONALIGN, TEXT("Section Alignment:") },
		{ IDC_STATIC_OH_FILEALIGN, TEXT("File Alignment:") },

		{ IDC_STATIC_OH_SUBSYSTEM, TEXT("Subsystem:") },
		{ IDC_STATIC_OH_CHECKSUM, TEXT("CheckSum:") },

		{ IDC_STATIC_OH_DLLFLAGS, TEXT("DllFlags:") },
		{ IDC_STATIC_FH_MACHINE, TEXT("Mechine:") },

		{ IDC_STATIC_FH_NUMOFSECTIONS, TEXT("Number Of Section:") },
		{ IDC_STATIC_FH_TDS, TEXT("Time Date Stamp:") },

		{ IDC_STATIC_FH_PTSYMBOL, TEXT("PTST:") },
		{ IDC_STATIC_FH_NUMOFSYM, TEXT("Number Of Symbols:") },

		{ IDC_STATIC_FH_SIZEOFOH, TEXT("Size Of OptionalHeader:") },
		{ IDC_STATIC_FH_CHARACTERISTICS, TEXT("Characteristics:") },

		{ IDC_BUTTON_OPEN, TEXT("Open") },
		{ IDC_BUTTON_MORE_FUNCTION, TEXT("More") },

		{ IDC_BUTTON_DATA_DIRECTORY, TEXT("DDT") },
		{ IDC_BUTTON_SECTION_HEADER, TEXT("Sections Table") },

		{ IDC_BUTTON_IMPORT_DESCRIPTOR, TEXT("Import Table") },
		{ IDC_BUTTON_EXPORT_DIRECTORY, TEXT("EmportTable") },

		{ IDC_BUTTON_TRANSFORM, TEXT("CN") },
		{ IDC_STATIC_PE_INFO, TEXT("Basic PE Information") }
	};
	if (bFlag)
	{
		for (int i = 0; i < 26; i++)
		{
			
			SetDlgItemText(hWnd, sChinese[i].ID_RVA, sChinese[i].pszStr);
		}
		SetWindowText(hWnd, TEXT("PE By ƤƤ��"));
	}
	else
	{
		for (int i = 0; i < 26; i++)
		{
			SetDlgItemText(hWnd, sEnglish[i].ID_RVA, sEnglish[i].pszStr);
			SetDlgItemText(hWnd, IDC_BUTTON_TRANSFORM, TEXT("CN"));
		}
		SetWindowText(hWnd, TEXT("PE By lifeshave"));
	}
}