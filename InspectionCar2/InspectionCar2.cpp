// InspectionCar2.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "InspectionCar2.h"
#include "InspectionCar2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInspectionCar2App

BEGIN_MESSAGE_MAP(CInspectionCar2App, CWinApp)
END_MESSAGE_MAP()


// CInspectionCar2App ����
CInspectionCar2App::CInspectionCar2App()
	: CWinApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CInspectionCar2App ����
CInspectionCar2App theApp;

// CInspectionCar2App ��ʼ��

BOOL CInspectionCar2App::InitInstance()
{

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CInspectionCar2Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
