// InspectionCar2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "InspectionCar2.h"
#include "InspectionCar2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CInspectionCar2Dlg �Ի���
#define MODE	2
#define PI	3.1415926535898

CInspectionCar2Dlg::CInspectionCar2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInspectionCar2Dlg::IDD, pParent)
	, m_dbVoltage_1(0)
	, m_dbVoltage_2(0)
	, m_dbOldSpeed(0)
	, m_dbOldVoltage_1(0)
	, m_dbOldVoltage_2(0)
	, m_pOneSerial(NULL)
	, m_pTwoSerial(NULL)
	, m_dbSpeed(0)
	,m_iExitCount(0)
	, m_dbTotalMileage(0)
	, m_pThreeSerial(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInspectionCar2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInspectionCar2Dlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CInspectionCar2Dlg ��Ϣ�������

BOOL CInspectionCar2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	MoveWindow(0,0,480,272);//���ô����СΪ480*272
	SetCursorPos(1000,1000);
	ShowCursor(FALSE);
	

	int iFullWidth = GetSystemMetrics(SM_CXSCREEN);
	int iFullHeight = GetSystemMetrics(SM_CYSCREEN); 
	::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, iFullWidth, iFullHeight, SWP_NOOWNERZORDER|SWP_SHOWWINDOW);

	ReadParam();

	//�жϴ����Ƿ��Ѿ���
	if (m_pOneSerial != NULL)
	{
		m_pOneSerial->ClosePort();

		delete m_pOneSerial;
		m_pOneSerial = NULL;
	}
	
	//�½�����ͨѶ����
	m_pOneSerial = new CCESeries();
	m_pOneSerial->m_OnSeriesRead = OnOneSerialRead; //

	//�򿪴���
	if(m_pOneSerial->OpenPort(this,1,9600,0,8,0))
	{
		//AfxMessageBox(L"���ڴ򿪳ɹ�");
		//Sleep(200);
	}
	else
	{
		AfxMessageBox(L"����1��ʧ��");
	}

	m_pTwoSerial = new CCESeries();
	m_pTwoSerial->m_OnSeriesRead = OnTwoSerialRead; //

	//�򿪴���
	if(m_pTwoSerial->OpenPort(this,2,9600,0,8,0))
	{
		//AfxMessageBox(L"���ڴ򿪳ɹ�");
		//Sleep(200);
	}
	else
	{
		AfxMessageBox(L"����2��ʧ��");
	}

	m_pThreeSerial = new CCESeries();
	m_pThreeSerial->m_OnSeriesRead = OnThreeSerialRead; 

	//�򿪴���
	if(m_pThreeSerial->OpenPort(this,3,9600,0,8,0))
	{
		//AfxMessageBox(L"���ڴ򿪳ɹ�");
		//Sleep(200);
	}
	else
	{
		AfxMessageBox(L"����3��ʧ��");
	}


	m_BitmapMain.LoadBitmap(IDB_PIC_MAIN);//���ر���ͼƬ
	m_BitmapBattery_1.LoadBitmap(IDB_BATTERY_1);//���ر���ͼƬ
	m_BitmapBattery_2.LoadBitmap(IDB_BATTERY_2);//���ر���ͼƬ
	m_BitmapBattery_3.LoadBitmap(IDB_BATTERY_3);//���ر���ͼƬ
	m_BitmapBattery_4.LoadBitmap(IDB_BATTERY_4);//���ر���ͼƬ
	m_BitmapBattery_5.LoadBitmap(IDB_BATTERY_5);//���ر���ͼƬ
	m_BitmapBattery_6.LoadBitmap(IDB_BATTERY_6);//���ر���ͼƬ
	
	SetTimer(1,100, NULL);

	//ϵͳ�˳�
	m_RectExitShow.left=5;
	m_RectExitShow.bottom=62;
	m_RectExitShow.top=32;
	m_RectExitShow.right=100;

	//ʱ����������
	m_RectDateTimeShow.left=90;
	m_RectDateTimeShow.bottom=41;
	m_RectDateTimeShow.top=18;
	m_RectDateTimeShow.right=330;

	//��������
	m_RectMeterShow.left=41;
	m_RectMeterShow.bottom=278;
	m_RectMeterShow.top=90;
	m_RectMeterShow.right=310;

	//���1����
	m_RectBatteryShow_1.left=380;
	m_RectBatteryShow_1.bottom=117;
	m_RectBatteryShow_1.top=80;
	m_RectBatteryShow_1.right=448;

	//���2����
	m_RectBatteryShow_2.left=380;
	m_RectBatteryShow_2.bottom=235;
	m_RectBatteryShow_2.top=210;
	m_RectBatteryShow_2.right=448;

	//�ٶ�����
	m_RectSpeedShow.left=120;
	m_RectSpeedShow.bottom=266;
	m_RectSpeedShow.top=245;
	m_RectSpeedShow.right=230;

	//���������
	m_RectTotalMileageShow.left=206;
	m_RectTotalMileageShow.bottom=230;
	m_RectTotalMileageShow.top=210;
	m_RectTotalMileageShow.right=319;

	//���1��ť
	m_RectBatteryButton_1.left=399;
	m_RectBatteryButton_1.bottom=70;
	m_RectBatteryButton_1.top=30;
	m_RectBatteryButton_1.right=419;

	//���2��ť
	m_RectBatteryButton_2.left=399;
	m_RectBatteryButton_2.bottom=201;
	m_RectBatteryButton_2.top=160;
	m_RectBatteryButton_2.right=419;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CInspectionCar2Dlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_INSPECTIONCAR2_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_INSPECTIONCAR2_DIALOG));
	}
}
#endif

void CInspectionCar2Dlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

CString str = L"36.00V";
double SpeedVal=0.0;
CRect rect;
CBitmap   bBitmap  ; 
CBitmap* pOldBitmap;
CDC   hMemDC; //���ر���ͼƬ���ڴ�DC

hMemDC.CreateCompatibleDC(&dc);//��������DC
pOldBitmap = hMemDC.SelectObject(&m_BitmapMain); //ѡ�뱳��ͼƬ
dc.BitBlt( 0,0 ,480 ,272 ,&hMemDC ,0 ,0 ,SRCCOPY);

if(m_dbVoltage_1>=52.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_6); //ѡ�뱳��ͼƬ
}else if(m_dbVoltage_1>=50.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_5); //ѡ�뱳��ͼƬ
}else if(m_dbVoltage_1>=48.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_4); //ѡ�뱳��ͼƬ
}else if(m_dbVoltage_1>=46.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_3); //ѡ�뱳��ͼƬ
}else if(m_dbVoltage_1>=43.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_2); //ѡ�뱳��ͼƬ
}else if(m_dbVoltage_1<43.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_1); //ѡ�뱳��ͼƬ
}
dc.BitBlt(m_RectBatteryButton_1.left,m_RectBatteryButton_1.top ,m_RectBatteryButton_1.right,m_RectBatteryButton_1.bottom,&hMemDC ,0 ,0 ,SRCCOPY);//���1


if(m_dbVoltage_2>=52.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_6); //ѡ�뱳��ͼƬ
}else if(m_dbVoltage_2>=50.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_5); //ѡ�뱳��ͼƬ
}else if(m_dbVoltage_2>=48.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_4); //ѡ�뱳��ͼƬ
}else if(m_dbVoltage_2>=46.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_3); //ѡ�뱳��ͼƬ
}else if(m_dbVoltage_2>=43.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_2); //ѡ�뱳��ͼƬ
}else if(m_dbVoltage_2<43.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_1); //ѡ�뱳��ͼƬ
}
dc.BitBlt(m_RectBatteryButton_2.left,m_RectBatteryButton_2.top ,m_RectBatteryButton_2.right,m_RectBatteryButton_2.bottom,&hMemDC ,0 ,0 ,SRCCOPY);//���2


	CFont font;
	font.CreateFont(22, 0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, L"Times New Roman");
	//font.CreatePointFont(6,L"����"); 
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255,255,255));    //Static�ؼ�1��������ɫ-��ɫ
	dc.SelectObject(&font); 

    //CString str = L"36.00V";
	//����ʱ������
	CTime time = CTime::GetCurrentTime();       
	str.Format(L"%04d-%02d-%02d %02d:%02d:%02d",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());  
    dc.DrawText(str,m_RectDateTimeShow, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
	//���1����
	str.Format(L"%.2fV",m_dbVoltage_1);
    dc.DrawText(str,m_RectBatteryShow_1, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);
	//���2����
	str.Format(L"%.2fV",m_dbVoltage_2);
    dc.DrawText(str,m_RectBatteryShow_2, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);
	//�ٶ�����
	SpeedVal=m_dbSpeed*m_Xishu[1];
	str.Format(L"%.2fKm/h",SpeedVal);
    dc.DrawText(str,m_RectSpeedShow, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
	////���������
	//str.Format(L"%.2fKm",m_dbTotalMileage);
 //   dc.DrawText(str,m_RectTotalMileageShow, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);
	//CRect rect;
	//rect.left=m_RectTotalMileageShow.left-3; 
	//rect.bottom=m_RectTotalMileageShow.bottom+3;
	//rect.top =m_RectTotalMileageShow.top-3;
	//rect.right=m_RectTotalMileageShow.right+3;
 //   dc.FillSolidRect(&rect,RGB(255,0,0));//��ɫ���
	font.DeleteObject(); 

	CPen RectPen(PS_SOLID,5,RGB(255,0,0));
	dc.SelectObject(&RectPen); 

	dc.MoveTo(176,218);
	dc.LineTo((int)(176+cos((360-(180-(-23+SpeedVal*6.76/5*180/27)))*PI/180)*126),(int)(218+sin((360-(180-(-23+SpeedVal*6.76/5*180/27)))*PI/180)*126));
	RectPen.DeleteObject();

}

void CInspectionCar2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CRect rect;
	static int iRefresh=0;
	static int iLevelFlag=0;
	byte sendbytes[] =   {0x01,0x03,0x02,0x58,0x00,0x04,0xC4,0x62};

    byte sendfrequency[]={0x01,0x03,0x00,0x00,0x00,0x08,0x44,0x0C};
	if(nIDEvent==1)
	{
		iRefresh=(iRefresh>9?0:iRefresh+1);
		if(iRefresh==10)
		{
			m_iExitCount=0;
			InvalidateRect(&m_RectDateTimeShow);
			if(m_dbOldSpeed!=m_dbSpeed)
			{
				m_dbOldSpeed=m_dbSpeed;
				InvalidateRect(&m_RectMeterShow);
			}
			if(m_dbOldVoltage_1!=m_dbVoltage_1)
			{
				m_dbOldVoltage_1=m_dbVoltage_1;
				InvalidateRect(&m_RectBatteryShow_1);
				InvalidateRect(&m_RectBatteryButton_1);
			}
			if(m_dbOldVoltage_2!=m_dbVoltage_2)
			{
				m_dbOldVoltage_2=m_dbVoltage_2;
				InvalidateRect(&m_RectBatteryShow_2);
				InvalidateRect(&m_RectBatteryButton_2);
			}
			//Invalidate();
		}
		switch(MODE){
		case 1:
			m_pThreeSerial->WriteSyncPort(sendbytes,8);
			break;
		case 2:
			m_pThreeSerial->WriteSyncPort(sendbytes,8);
			m_pTwoSerial->WriteSyncPort(sendfrequency,8);
		break;
		}
	}


	CDialog::OnTimer(nIDEvent);
}


int CInspectionCar2Dlg::FileRead(CString filename,CString* content)
{
	int lRet;
	HANDLE hFile = INVALID_HANDLE_VALUE;	// �ļ���� 
	if(::GetFileAttributes(filename)==0xFFFFFFFF)
	{
	//::AfxMessageBox(_T("�ļ�������"));
		return -5;
	}
	// ����һ���ļ����һ���ļ�
	hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, 
					   NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox(_T("���ļ�ʧ��!"));
		return -1;
	}

	DWORD filelen,actlen;
	char *pcharbuff;
	
	filelen = GetFileSize(hFile, NULL);							// ��ȡ�ļ���С
	if (filelen == 0xFFFFFFFF)
	{
		//AfxMessageBox(_T("��ȡ�ļ���Сʧ��!"));
		return -2;	
	}

	BOOL ret = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);		// �ƶ��ļ�ָ�뵽�ļ���ͷ
	if (ret == 0xFFFFFFFF)
	{
		//AfxMessageBox(_T("���ļ�ָ�������ļ���ͷʧ��!"));
		return -3;	
	}

	pcharbuff = new char[filelen];
	ret = ReadFile(hFile, pcharbuff, filelen, &actlen, NULL);	// ���ļ��ж������� 
	if (ret == TRUE)
	{
		LPTSTR pStr = content->GetBuffer(filelen);	
		// ���ֽ�ת��Ϊ Unicode �ַ���
		MultiByteToWideChar(CP_ACP, 0, pcharbuff, filelen, pStr, filelen);
		content->ReleaseBuffer();
		//AfxMessageBox(_T("���ļ��ɹ�!"));	
		lRet=0;
	}
	else
	{
		//AfxMessageBox(_T("���ļ�ʧ��!"));	
		lRet=-4;
	}

	if (pcharbuff != NULL) 
		delete[] pcharbuff;

	CloseHandle(hFile);
	return lRet;
}

void CInspectionCar2Dlg::Split(CString source, CStringArray& dest, CString division)
{
    dest.RemoveAll();
    int pos = 0;
    int pre_pos = 0;
    while( -1 != pos ){
        pre_pos = pos;
        pos = source.Find(division,(pos));
		if(pos<0)
			dest.Add(source.Mid(pre_pos));
		else
		{
			dest.Add(source.Mid(pre_pos,(pos-pre_pos)));
			pos++;
		}
		
    }
}

void CInspectionCar2Dlg::ReadParam(void)
{
	int ret=0;
	CString strTmp;
	ret=FileRead(_T("\\ResidentFlash2\\GUI\\xishu.txt"),&strTmp);
	if(ret<0)
	{
		for(int i=0;i<2;i++)
			m_Xishu[i]=1.0;
	}
	else
	{
		CStringArray dest;
		wchar_t   *stopstring;
		Split(strTmp,dest,_T(","));
		int Count = dest.GetSize();
		for(int i=0;i<2;i++)
			m_Xishu[i]=wcstod(dest[i],&stopstring);
	}
}
void CInspectionCar2Dlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	KillTimer(1);
	//�رմ���
	//
	if (m_pOneSerial != NULL)
	{
		//�رմ���
		m_pOneSerial->ClosePort();

		//�ͷŴ��ڶ���
		delete m_pOneSerial;
		m_pOneSerial = NULL;
		//AfxMessageBox(L"���ڹرճɹ�");
	}

	if (m_pTwoSerial != NULL)
	{
		//�رմ���
		m_pTwoSerial->ClosePort();

		//�ͷŴ��ڶ���
		delete m_pTwoSerial;
		m_pTwoSerial = NULL;
		//AfxMessageBox(L"���ڹرճɹ�");
	}
	if (m_pThreeSerial != NULL)
	{
		//�رմ���
		m_pThreeSerial->ClosePort();

		//�ͷŴ��ڶ���
		delete m_pThreeSerial;
		m_pThreeSerial = NULL;
		//AfxMessageBox(L"���ڹرճɹ�");
	}
}

//���崮�ڽ������ݺ�������
void CALLBACK CInspectionCar2Dlg::OnOneSerialRead(void * pOwner,BYTE* buf,DWORD bufLen)
{
	//CString strRecv=L"";
	//BYTE *pRecvBuf = NULL; //���ջ�����
	////�õ�������ָ��
	//CPatrolCarDlg* pThis = (CPatrolCarDlg*)pOwner;

	BYTE OneRecv[1024]={0};
	static BYTE OneRecvBuf[1024]={0};
	static int OneRevPos=0;
	int j=0;
	//�õ�������ָ��
	CInspectionCar2Dlg* pThis = (CInspectionCar2Dlg*)pOwner;
	//�����յĻ�����������pRecvBuf��
	//pRecvBuf = new BYTE[bufLen];
	CopyMemory(OneRecvBuf+OneRevPos,buf,bufLen);
	OneRevPos=OneRevPos+bufLen;

	for(int i=0;i<OneRevPos;i++)
	{
		if(OneRecvBuf[i]=='#')
		{
			for(int k=i;k<OneRevPos;k++)
			{
				if(OneRecvBuf[k]=='$')
				{
					
					CopyMemory(OneRecv,OneRecvBuf+i+1,k-1);
					//strRecv=CString((char*)OneRecv);
				//	pThis->m_strInfo=CString((char*)OneRecv);
				//	pThis->InvalidateRect(&pThis->m_RectInfoShow);
					OneRevPos=0;
					break;
				}
			}
		}
	}

	//if(OneRecvBuf[0]=='#')
	//{
	//	for(int k=i;k<OneRevPos;k++)
	//	{
	//		if(OneRecvBuf[k]=='$')
	//		{
	//			
	//			CopyMemory(OneRecv,OneRecvBuf+1,k-1);
	//			//strRecv=CString((char*)OneRecv);
	//			pThis->m_strInfo=CString((char*)OneRecv);
	//			pThis->InvalidateRect(&pThis->m_RectInfoShow);
	//			OneRevPos=0;
	//			break;
	//		}
	//	}
	//}

	////�����յĻ�����������pRecvBuf��
	//strRecv=CString((char*)buf);
	
	//m_strReceive+=strRecv;
	////m_strReceive+=CString((char*)buf);
	//int iSos=m_strReceive.Find('#');
	//if(iSos>0)
	//{
	//	int iPos=m_strReceive.Find('$');
	//	if(iPos>0)
	//	{
	//		pThis->m_strInfo=m_strReceive.Mid(1,iPos-1);
	//		pThis->InvalidateRect(&pThis->m_RectInfoShow);
	//		m_strReceive=L"";
	//	}
	//}
	//else
	//{
	//	m_strReceive=L"";
	//}

}

void CALLBACK CInspectionCar2Dlg::OnTwoSerialRead(void * pOwner,BYTE* buf,DWORD bufLen)
{
	static BYTE TwoRecvBuf[1024]={0}; //���ջ�����
	static int TwoRevPos=false;
	int revint16[8];
 	int j=0;
	int iTemp=0;
	//01 03 10 00 01 00 02 00 03 00 04 00 05 00 06 00 00 00 00 xx xx
	
	//�õ�������ָ��
	CInspectionCar2Dlg* pThis = (CInspectionCar2Dlg*)pOwner;
	CopyMemory(TwoRecvBuf+TwoRevPos,buf,bufLen);	


	if(TwoRecvBuf[0]==1)
	{	
		if(TwoRevPos==0&&bufLen<21)
		{
			TwoRevPos=TwoRevPos+bufLen;
		}
		else
		{
			TwoRevPos=0;
			for(int i=3,j=0;i<19;j++,i+=2)
				revint16[j] =TwoRecvBuf[i] * 256+ TwoRecvBuf[i + 1];
			iTemp=(int)(revint16[4]+revint16[5]);
			pThis->m_dbSpeed =iTemp/2.0;
		}
	}
}

void CALLBACK CInspectionCar2Dlg::OnThreeSerialRead(void * pOwner,BYTE* buf,DWORD bufLen)
{
	//BYTE *pRecvBuf = NULL; //���ջ�����
	static BYTE ThreeRecvBuf[1024]={0};
	static int ThreeRevPos=0;
	int j=0;
	int iTemp=0;
	int revint16[4];
	//�õ�������ָ��
	CInspectionCar2Dlg* pThis = (CInspectionCar2Dlg*)pOwner;
	CopyMemory(ThreeRecvBuf+ThreeRevPos,buf,bufLen);
	
	if(ThreeRecvBuf[0]==1)
	{	
		if(ThreeRevPos==0&&bufLen<13)
		{
			ThreeRevPos=ThreeRevPos+bufLen;
		}
		else
		{
			ThreeRevPos=0;
			for(int i=3,j=0;i<11;j++,i+=2)
				revint16[j] =ThreeRecvBuf[i] * 256+ ThreeRecvBuf[i + 1];

			iTemp=(int)(revint16[0]/100.0);
			pThis->m_dbVoltage_1=iTemp/10.0;
			iTemp=(int)(revint16[1]/100.0);
			//pThis->m_dbSpeed =iTemp/10.0;
			if(MODE==1)
				pThis->m_dbSpeed =revint16[1]/1000.0;
			iTemp=(int)(revint16[2]/100.0);
			pThis->m_dbVoltage_2 =iTemp/10.0;
		}
	}
}
BOOL CInspectionCar2Dlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}
