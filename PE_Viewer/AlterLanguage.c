#include"AlterLanguage.h"
//该函数功能完成主界面的中英文切换
void  AlterLanguage(HWND hWnd,BOOL bFlag)
{
	STATICTEXT sChinese[] = {
		{ IDC_STATIC_OH_EP, TEXT("程序执行入口:") },
		{ IDC_STATIC_OH_IMAGEBASE, TEXT("程序装入基址:") },

		{ IDC_STATIC_OH_CODEBASE, TEXT("代码区块RVA:") },
		{ IDC_STATIC_OH_DATABASE, TEXT("数据区块RVA:") },

		{ IDC_STATIC_OH_IMAGESIZE, TEXT("映像大小:") },
		{ IDC_STATIC_OH_HEADERSIZE, TEXT("PE头+区块表:") },

		{ IDC_STATIC_OH_SECTIONALIGN, TEXT("内存中对齐值:") },
		{ IDC_STATIC_OH_FILEALIGN, TEXT("文件中对齐值:") },

		{ IDC_STATIC_OH_SUBSYSTEM, TEXT("文件子系统:") },
		{ IDC_STATIC_OH_CHECKSUM, TEXT("映像校验和:") },

		{ IDC_STATIC_OH_DLLFLAGS, TEXT("显示DLL特性:") },
		{ IDC_STATIC_FH_MACHINE, TEXT("运行平台:") },

		{ IDC_STATIC_FH_NUMOFSECTIONS, TEXT("区块数目:") },
		{ IDC_STATIC_FH_TDS, TEXT("创建日期和时间:") },

		{ IDC_STATIC_FH_PTSYMBOL, TEXT("指向符号表:") },
		{ IDC_STATIC_FH_NUMOFSYM, TEXT("符号个数:") },

		{ IDC_STATIC_FH_SIZEOFOH, TEXT("可选头大小:") },
		{ IDC_STATIC_FH_CHARACTERISTICS, TEXT("文件属性:") },

		{ IDC_BUTTON_OPEN, TEXT("打开") },
		{ IDC_BUTTON_MORE_FUNCTION, TEXT("更多") },

		{ IDC_BUTTON_DATA_DIRECTORY, TEXT("数据目录表") },
		{ IDC_BUTTON_SECTION_HEADER, TEXT("区块(节)表") },
		
		{ IDC_BUTTON_IMPORT_DESCRIPTOR, TEXT("导入表") },
		{ IDC_BUTTON_EXPORT_DIRECTORY, TEXT("导出表") },

		{ IDC_BUTTON_TRANSFORM, TEXT("英文") },
		{ IDC_STATIC_PE_INFO, TEXT("基本PE信息") }
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
		SetWindowText(hWnd, TEXT("PE By 皮皮武"));
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