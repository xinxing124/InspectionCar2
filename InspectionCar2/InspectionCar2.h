// InspectionCar2.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CInspectionCar2App:
// �йش����ʵ�֣������ InspectionCar2.cpp
//

class CInspectionCar2App : public CWinApp
{
public:
	CInspectionCar2App();
	
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CInspectionCar2App theApp;
