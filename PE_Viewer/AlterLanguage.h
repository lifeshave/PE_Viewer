#pragma once
#ifndef _ALTERLANGUAGE_H
#define _ALTERLANGUAGE_H
#include<Windows.h>
#include"resource.h"
typedef struct
{
	UINT   ID_RVA;
	PTCHAR pszStr;
}STATICTEXT;

void  AlterLanguage(HWND hWnd, BOOL bFlag);
#endif
