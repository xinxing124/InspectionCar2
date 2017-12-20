// InspectionCar2Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "CESeries.h"

// CInspectionCar2Dlg 对话框
class CInspectionCar2Dlg : public CDialog
{
// 构造
public:
	CInspectionCar2Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_INSPECTIONCAR2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	//定义串口接收数据函数类型
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
