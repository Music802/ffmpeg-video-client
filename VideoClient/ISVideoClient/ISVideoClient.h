
// ISVideoClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CISVideoClientApp: 
// �йش����ʵ�֣������ ISVideoClient.cpp
//

class CISVideoClientApp : public CWinApp
{
public:
	CISVideoClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CISVideoClientApp theApp;