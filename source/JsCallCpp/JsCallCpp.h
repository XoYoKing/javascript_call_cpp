
// JsCallCpp.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CJsCallCppApp: 
// �йش����ʵ�֣������ JsCallCpp.cpp
//

class CJsCallCppApp : public CWinApp
{
public:
	CJsCallCppApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CJsCallCppApp theApp;