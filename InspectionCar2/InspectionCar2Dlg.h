// InspectionCar2Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "CESeries.h"

// CInspectionCar2Dlg �Ի���
class CInspectionCar2Dlg : public CDialog
{
// ����
public:
	CInspectionCar2Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_INSPECTIONCAR2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	CCESeries *m_pOneSerial;
	CCESeries *m_pTwoSerial;
	CCESeries* m_pThreeSerial;
	int		m_iThreeSerialMode;
	double m_Xishu[2];
	//���崮�ڽ������ݺ�������
	static void CALLBACK OnOneSerialRead(void * pOwner,BYTE* buf,DWORD bufLen);
	static void CALLBACK OnTwoSerialRead(void * pOwner,BYTE* buf,DWORD bufLen);
	static void CALLBACK OnThreeSerialRead(void * pOwner,BYTE* buf,DWORD bufLen);
	int FileRead(CString filename,CString* content);
	void Split(CString source, CStringArray& dest, CString division);
	void ReadParam(void);

	CRect m_RectExitShow;
	CRect m_RectMeterShow;
	CRect m_RectDateTimeShow;
	CRect m_RectBatteryShow_1;
	CRect m_RectBatteryShow_2;
	CRect m_RectBatteryButton_1;
	CRect m_RectBatteryButton_2;
	CRect m_RectSpeedShow;
	CRect m_RectTotalMileageShow;


	BYTE m_cRecvBuf[1024];
	CString m_strInfo;
	CString m_strTempMileage;

	int m_iExitCount;
	double m_dbVoltage_1;
	double m_dbVoltage_2;
	double m_dbSpeed;
	double m_dbOldVoltage_1;
	double m_dbOldVoltage_2;
	double m_dbOldSpeed;
	double m_dbTotalMileage;

	CBitmap m_BitmapMain;
	CBitmap m_BitmapBattery_1;
	CBitmap m_BitmapBattery_2;
	CBitmap m_BitmapBattery_3;
	CBitmap m_BitmapBattery_4;
	CBitmap m_BitmapBattery_5;
	CBitmap m_BitmapBattery_6;
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
