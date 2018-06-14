// InspectionCar2Dlg.cpp : 实现文件
//
#include "stdafx.h"
#include "InspectionCar2.h"
#include "InspectionCar2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CInspectionCar2Dlg 对话框
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
	, m_CurrAmount(0)
	, m_iCurrCount(0)
	, m_iLastCount(0)
	, m_LiCheng(0)
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


// CInspectionCar2Dlg 消息处理程序

BOOL CInspectionCar2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	MoveWindow(0,0,480,272);//设置窗体大小为480*272
	SetCursorPos(1000,1000);
	ShowCursor(FALSE);
	

	int iFullWidth = GetSystemMetrics(SM_CXSCREEN);
	int iFullHeight = GetSystemMetrics(SM_CYSCREEN); 
	::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, iFullWidth, iFullHeight, SWP_NOOWNERZORDER|SWP_SHOWWINDOW);

	ReadParam();
	ReadLiCheng();

	//判断串口是否已经打开
	if (m_pOneSerial != NULL)
	{
		m_pOneSerial->ClosePort();

		delete m_pOneSerial;
		m_pOneSerial = NULL;
	}
	
	//新建串口通讯对象
	m_pOneSerial = new CCESeries();
	m_pOneSerial->m_OnSeriesRead = OnOneSerialRead; //

	//打开串口
	if(m_pOneSerial->OpenPort(this,1,9600,0,8,0))
	{
		//AfxMessageBox(L"串口打开成功");
		//Sleep(200);
	}
	else
	{
		AfxMessageBox(L"串口1打开失败");
	}

	m_pTwoSerial = new CCESeries();
	m_pTwoSerial->m_OnSeriesRead = OnTwoSerialRead; //

	//打开串口
	if(m_pTwoSerial->OpenPort(this,2,9600,0,8,0))
	{
		//AfxMessageBox(L"串口打开成功");
		//Sleep(200);
	}
	else
	{
		AfxMessageBox(L"串口2打开失败");
	}

	m_pThreeSerial = new CCESeries();
	m_pThreeSerial->m_OnSeriesRead = OnThreeSerialRead; 

	//打开串口
	if(m_pThreeSerial->OpenPort(this,3,9600,0,8,0))
	{
		//AfxMessageBox(L"串口打开成功");
		//Sleep(200);
	}
	else
	{
		AfxMessageBox(L"串口3打开失败");
	}


	m_BitmapMain.LoadBitmap(IDB_PIC_MAIN);//加载背景图片
	m_BitmapBattery_1.LoadBitmap(IDB_BATTERY_1);//加载背景图片
	m_BitmapBattery_2.LoadBitmap(IDB_BATTERY_2);//加载背景图片
	m_BitmapBattery_3.LoadBitmap(IDB_BATTERY_3);//加载背景图片
	m_BitmapBattery_4.LoadBitmap(IDB_BATTERY_4);//加载背景图片
	m_BitmapBattery_5.LoadBitmap(IDB_BATTERY_5);//加载背景图片
	m_BitmapBattery_6.LoadBitmap(IDB_BATTERY_6);//加载背景图片
	
	SetTimer(1,100, NULL);
	SetTimer(2,50, NULL);
	SetTimer(3,3000, NULL);

	//byte sendfrequency[]={0x01,0x03,0x00,0x00,0x00,0x08,0x44,0x0C};
	//m_pTwoSerial->WriteSyncPort(sendfrequency,8);

	//系统退出
	m_RectExitShow.left=5;
	m_RectExitShow.bottom=62;
	m_RectExitShow.top=32;
	m_RectExitShow.right=100;

	//时间日期区域
	m_RectDateTimeShow.left=90;
	m_RectDateTimeShow.bottom=41;
	m_RectDateTimeShow.top=18;
	m_RectDateTimeShow.right=330;

	//表盘区域
	m_RectMeterShow.left=41;
	m_RectMeterShow.bottom=278;
	m_RectMeterShow.top=90;
	m_RectMeterShow.right=310;

	//电池1区域
	m_RectBatteryShow_1.left=380;
	m_RectBatteryShow_1.bottom=117;
	m_RectBatteryShow_1.top=80;
	m_RectBatteryShow_1.right=448;

	//电池2区域
	m_RectBatteryShow_2.left=380;
	m_RectBatteryShow_2.bottom=235;
	m_RectBatteryShow_2.top=210;
	m_RectBatteryShow_2.right=448;

	//速度区域
	m_RectSpeedShow.left=120;
	m_RectSpeedShow.bottom=266;
	m_RectSpeedShow.top=245;
	m_RectSpeedShow.right=230;

	//总里程区域
	m_RectTotalMileageShow.left=206;
	m_RectTotalMileageShow.bottom=230;
	m_RectTotalMileageShow.top=210;
	m_RectTotalMileageShow.right=319;

	//电池1按钮
	m_RectBatteryButton_1.left=399;
	m_RectBatteryButton_1.bottom=70;
	m_RectBatteryButton_1.top=30;
	m_RectBatteryButton_1.right=419;

	//电池2按钮
	m_RectBatteryButton_2.left=399;
	m_RectBatteryButton_2.bottom=201;
	m_RectBatteryButton_2.top=160;
	m_RectBatteryButton_2.right=419;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

CString str = L"36.00V";
double SpeedVal=0.0;
CRect rect;
CBitmap   bBitmap  ; 
CBitmap* pOldBitmap;
CDC   hMemDC; //加载背景图片的内存DC

hMemDC.CreateCompatibleDC(&dc);//创建兼容DC
pOldBitmap = hMemDC.SelectObject(&m_BitmapMain); //选入背景图片
dc.BitBlt( 0,0 ,480 ,272 ,&hMemDC ,0 ,0 ,SRCCOPY);

if(m_dbVoltage_1>=52.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_6); //选入背景图片
}else if(m_dbVoltage_1>=50.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_5); //选入背景图片
}else if(m_dbVoltage_1>=48.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_4); //选入背景图片
}else if(m_dbVoltage_1>=46.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_3); //选入背景图片
}else if(m_dbVoltage_1>=43.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_2); //选入背景图片
}else if(m_dbVoltage_1<43.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_1); //选入背景图片
}
dc.BitBlt(m_RectBatteryButton_1.left,m_RectBatteryButton_1.top ,m_RectBatteryButton_1.right,m_RectBatteryButton_1.bottom,&hMemDC ,0 ,0 ,SRCCOPY);//电池1


if(m_dbVoltage_2>=52.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_6); //选入背景图片
}else if(m_dbVoltage_2>=50.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_5); //选入背景图片
}else if(m_dbVoltage_2>=48.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_4); //选入背景图片
}else if(m_dbVoltage_2>=46.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_3); //选入背景图片
}else if(m_dbVoltage_2>=43.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_2); //选入背景图片
}else if(m_dbVoltage_2<43.0)
{
	pOldBitmap = hMemDC.SelectObject(&m_BitmapBattery_1); //选入背景图片
}
dc.BitBlt(m_RectBatteryButton_2.left,m_RectBatteryButton_2.top ,m_RectBatteryButton_2.right,m_RectBatteryButton_2.bottom,&hMemDC ,0 ,0 ,SRCCOPY);//电池2


	CFont font;
	font.CreateFont(22, 0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, L"Times New Roman");
	//font.CreatePointFont(6,L"宋体"); 
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255,255,255));    //Static控件1的字体颜色-红色
	dc.SelectObject(&font); 

    //CString str = L"36.00V";
	//日期时间区域
	CTime time = CTime::GetCurrentTime();       
	str.Format(L"%04d-%02d-%02d %02d:%02d:%02d",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());  
    dc.DrawText(str,m_RectDateTimeShow, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
	//电池1区域
	str.Format(L"%.2fV",m_dbVoltage_1);
    dc.DrawText(str,m_RectBatteryShow_1, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);
	//电池2区域
	str.Format(L"%.2fV",m_dbVoltage_2);
    dc.DrawText(str,m_RectBatteryShow_2, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);
	//速度区域
	SpeedVal=m_dbSpeed*m_Xishu[1];
	str.Format(L"%.2fKm/h",SpeedVal);
    dc.DrawText(str,m_RectSpeedShow, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);

	//str.Format(L"%.3f",m_LiCheng);
    //dc.DrawText(str,m_RectSpeedShow, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);

	////总里程区域
	//str.Format(L"%.2fKm",m_dbTotalMileage);
 //   dc.DrawText(str,m_RectTotalMileageShow, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);
	//CRect rect;
	//rect.left=m_RectTotalMileageShow.left-3; 
	//rect.bottom=m_RectTotalMileageShow.bottom+3;
	//rect.top =m_RectTotalMileageShow.top-3;
	//rect.right=m_RectTotalMileageShow.right+3;
 //   dc.FillSolidRect(&rect,RGB(255,0,0));//红色填充
	font.DeleteObject(); 

	SpeedVal=m_dbSpeed*m_Xishu[1];
	CPen RectPen(PS_SOLID,5,RGB(255,0,0));
	dc.SelectObject(&RectPen); 
	
	dc.MoveTo(176,218);
	dc.LineTo((int)(176+cos((360-(180-(-23+SpeedVal*6.76/5*180/27)))*PI/180)*126),(int)(218+sin((360-(180-(-23+SpeedVal*6.76/5*180/27)))*PI/180)*126));
	RectPen.DeleteObject();

}

void CInspectionCar2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRect rect;
	static int iRefresh=0;
	static int iLevelFlag=0;
	byte sendbytes[] =   {0x01,0x03,0x02,0x58,0x00,0x04,0xC4,0x62};
	//#02KS\r  
	byte sendState[]={'#','0','1','K','S','\r'};
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
			//m_pTwoSerial->WriteSyncPort(sendfrequency,8);
			m_pOneSerial->WriteSyncPort(sendState,6);

			//if(m_iLastCount<m_iCurrCount)
			//{
			//	m_LiCheng=(m_iCurrCount-m_iLastCount)*m_Xishu[3]+m_LiCheng;
			//	m_iLastCount=m_iCurrCount;
			//	WriteParam();

			//}
			//else if(m_iLastCount>m_iCurrCount)
			//{
			//	m_LiCheng=(m_iCurrCount+65535-m_iLastCount)*m_Xishu[3]+m_LiCheng;
			//	m_iLastCount=m_iCurrCount;
			//	WriteParam();
			//}
		break;
		}
	}
	else if(nIDEvent==2)
	{
		//m_pOneSerial->WriteSyncPort(sendState,6);
		m_pTwoSerial->WriteSyncPort(sendfrequency,8);
	}
	else if(nIDEvent==3)
	{
		switch(MODE){
		case 2:
			//m_pOneSerial->WriteSyncPort(sendState,6);

			if(m_iLastCount<m_iCurrCount)
			{
				m_LiCheng=(m_iCurrCount-m_iLastCount)*m_Xishu[3]+m_LiCheng;
				m_iLastCount=m_iCurrCount;
				WriteParam();

			}
			else if(m_iLastCount>m_iCurrCount)
			{
				m_LiCheng=(m_iCurrCount+65536-m_iLastCount)*m_Xishu[3]+m_LiCheng;
				m_iLastCount=m_iCurrCount;
				WriteParam();
			}
		break;
		}
	}



	CDialog::OnTimer(nIDEvent);
}

int CInspectionCar2Dlg::FileWrite(CString filename,char* lpBuffer,DWORD  nNumberOfBytesWritten)
{
	int lRet=0;
	HANDLE hFile = INVALID_HANDLE_VALUE;	// 文件句柄 

	// 创建一个文件或打开一个文件
	hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, 
					   NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox(_T("打开文件失败!"));
		return -1;
	}

	BOOL ret = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);		// 移动文件指针到文件开头
	if (ret == 0xFFFFFFFF)
	{
		//AfxMessageBox(_T("将文件指针移至文件开头失败!"));
		return -3;	
	}

	DWORD actlen;
	ret=WriteFile(hFile, lpBuffer,nNumberOfBytesWritten, &actlen, NULL); 
	if(!ret)
	{
		lRet=-4;
	}

	CloseHandle(hFile);
	return lRet;
}

int CInspectionCar2Dlg::FileRead(CString filename,CString* content)
{
	int lRet;
	HANDLE hFile = INVALID_HANDLE_VALUE;	// 文件句柄 
	if(::GetFileAttributes(filename)==0xFFFFFFFF)
	{
	//::AfxMessageBox(_T("文件不存在"));
		return -5;
	}
	// 创建一个文件或打开一个文件
	hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, 
					   NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox(_T("打开文件失败!"));
		return -1;
	}

	DWORD filelen,actlen;
	char *pcharbuff;
	
	filelen = GetFileSize(hFile, NULL);							// 获取文件大小
	if (filelen == 0xFFFFFFFF)
	{
		//AfxMessageBox(_T("获取文件大小失败!"));
		return -2;	
	}

	BOOL ret = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);		// 移动文件指针到文件开头
	if (ret == 0xFFFFFFFF)
	{
		//AfxMessageBox(_T("将文件指针移至文件开头失败!"));
		return -3;	
	}

	pcharbuff = new char[filelen];
	ret = ReadFile(hFile, pcharbuff, filelen, &actlen, NULL);	// 从文件中读出数据 
	if (ret == TRUE)
	{
		LPTSTR pStr = content->GetBuffer(filelen);	
		// 将字节转化为 Unicode 字符串
		MultiByteToWideChar(CP_ACP, 0, pcharbuff, filelen, pStr, filelen);
		content->ReleaseBuffer();
		//AfxMessageBox(_T("读文件成功!"));	
		lRet=0;
	}
	else
	{
		//AfxMessageBox(_T("读文件失败!"));	
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

void CInspectionCar2Dlg::WriteParam(void)
{
	int ret=0;
	char Tmp[100];
	int aLen=sprintf_s(Tmp,100,"%f",m_LiCheng);
	if(aLen>0)
	{
		ret=FileWrite(_T("\\ResidentFlash2\\GUI\\licheng.txt"),Tmp,aLen);
		
	}
}

void CInspectionCar2Dlg::ReadLiCheng(void)
{	
	int ret=0;
	CString strTmp;
	wchar_t   *stopstring;

	ret=FileRead(_T("\\ResidentFlash2\\GUI\\licheng.txt"),&strTmp);
	if(ret<0)
	{
		m_LiCheng=0.0;
	}
	else
	{
		m_LiCheng=wcstod(strTmp,&stopstring);
	}
	
}
void CInspectionCar2Dlg::ReadParam(void)
{
	int ret=0;
	CString strTmp;
	ret=FileRead(_T("\\ResidentFlash2\\GUI\\xishu.txt"),&strTmp);
	if(ret<0)
	{
		for(int i=0;i<8;i++)
			m_Xishu[i]=1.0;
	}
	else
	{
		for(int i=0;i<8;i++)
			m_Xishu[i]=1.0;

		CStringArray dest;
		wchar_t   *stopstring;
		Split(strTmp,dest,_T(","));
		int Count = dest.GetSize();
		m_XishuNum=Count;
		for(int i=0;i<Count;i++)
			m_Xishu[i]=wcstod(dest[i],&stopstring);
	}
}
void CInspectionCar2Dlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	KillTimer(1);
	//关闭串口
	//
	if (m_pOneSerial != NULL)
	{
		//关闭串口
		m_pOneSerial->ClosePort();

		//释放串口对象
		delete m_pOneSerial;
		m_pOneSerial = NULL;
		//AfxMessageBox(L"串口关闭成功");
	}

	if (m_pTwoSerial != NULL)
	{
		//关闭串口
		m_pTwoSerial->ClosePort();

		//释放串口对象
		delete m_pTwoSerial;
		m_pTwoSerial = NULL;
		//AfxMessageBox(L"串口关闭成功");
	}
	if (m_pThreeSerial != NULL)
	{
		//关闭串口
		m_pThreeSerial->ClosePort();

		//释放串口对象
		delete m_pThreeSerial;
		m_pThreeSerial = NULL;
		//AfxMessageBox(L"串口关闭成功");
	}
}

//定义串口接收数据函数类型
void CALLBACK CInspectionCar2Dlg::OnOneSerialRead(void * pOwner,BYTE* buf,DWORD bufLen)
{
	//CString strRecv=L"";
	//BYTE *pRecvBuf = NULL; //接收缓冲区
	////得到父对象指针
	//CPatrolCarDlg* pThis = (CPatrolCarDlg*)pOwner;
	char ch[14];
	BYTE OneRecv[1024]={0};
	static BYTE OneRecvBuf[1024]={0};
	static int OneRevPos=0;
	static double CurrSetVal=1.6;
	double TempSetVal=1.6;
	int Count=0;
	double TargetSpeed=0.0;
	double SpeedVal=0.0;
	int j=0;
	//得到父对象指针
	CInspectionCar2Dlg* pThis = (CInspectionCar2Dlg*)pOwner;
	//将接收的缓冲区拷贝到pRecvBuf种
	//pRecvBuf = new BYTE[bufLen];
	CopyMemory(OneRecvBuf+OneRevPos,buf,bufLen);
	//OneRevPos=OneRevPos+bufLen;

	//#02KS\r  
	if(OneRecvBuf[0]=='>')
	{
		switch(OneRecvBuf[4])
		{
		case 0:
			TargetSpeed=0.0;
			break;
		case 'E'://3
			TargetSpeed=3.0;
			TempSetVal=1.65;
			break;
		case 'D'://6
			TargetSpeed=6.0;
			TempSetVal=1.8;
			break;
		case 'B'://9
			TargetSpeed=9.0;
			TempSetVal=2.0;
			break;
		case '7'://12
			TargetSpeed=12.0;
			TempSetVal=2.2;
			break;
		case '3'://15
			TargetSpeed=15.0;
			TempSetVal=2.4;
			break;
		case 'C'://20
			TargetSpeed=20.0;
			TempSetVal=2.8;
			break;
		case 'F':
			TargetSpeed=0.0;
			CurrSetVal=1.6;
			sprintf_s(ch,13,"#01D0+%2.3f\r",CurrSetVal);
			pThis->m_pOneSerial->WriteSyncPort((BYTE*)ch,13);
			break;
		}
			
		if(TargetSpeed!=0)
		{
			SpeedVal=pThis->m_dbSpeed*pThis->m_Xishu[1];
			
			//if(SpeedVal==0)
			//{
			//	Count++;
			//	if(Count>6000) 
			//	{
			//		Count=0;
			//		CurrSetVal=TempSetVal;
			//	}
			//}
			//if(fabs(TargetSpeed-SpeedVal)<0.25)
			//	return;

			if(TargetSpeed-SpeedVal>=9.0)
			{
				//#01D0+01.000/r
				CurrSetVal+=0.1;
				CurrSetVal=(CurrSetVal>4.8?4.8:CurrSetVal);
				sprintf_s(ch,13,"#01D0+%2.3f\r",CurrSetVal);
				pThis->m_pOneSerial->WriteSyncPort((BYTE*)ch,13);
			}
			else if(TargetSpeed-SpeedVal>=3.0)
			{
				CurrSetVal+=0.05;
				CurrSetVal=(CurrSetVal>4.8?4.8:CurrSetVal);
				sprintf_s(ch,13,"#01D0+%2.3f\r",CurrSetVal);
				pThis->m_pOneSerial->WriteSyncPort((BYTE*)ch,13);
			}
			else if(TargetSpeed-SpeedVal>=0.8)
			{
				CurrSetVal+=0.01;
				CurrSetVal=(CurrSetVal>4.8?4.8:CurrSetVal);
				sprintf_s(ch,13,"#01D0+%2.3f\r",CurrSetVal);
				pThis->m_pOneSerial->WriteSyncPort((BYTE*)ch,13);
			}
			else if(TargetSpeed-SpeedVal>=0.5)
			{
				CurrSetVal+=0.0025;
				CurrSetVal=(CurrSetVal>4.8?4.8:CurrSetVal);
				sprintf_s(ch,13,"#01D0+%2.3f\r",CurrSetVal);
				pThis->m_pOneSerial->WriteSyncPort((BYTE*)ch,13);
			}
			else if(TargetSpeed-SpeedVal<=-9.0)
			{
				CurrSetVal-=0.1;
				CurrSetVal=(CurrSetVal<0.0?0.0:CurrSetVal);
				sprintf_s(ch,13,"#01D0+%2.3f\r",CurrSetVal);
				pThis->m_pOneSerial->WriteSyncPort((BYTE*)ch,13);
			}
			else if(TargetSpeed-SpeedVal<=-3.0)
			{
				CurrSetVal-=0.025;
				CurrSetVal=(CurrSetVal<0.0?0.0:CurrSetVal);
				sprintf_s(ch,13,"#01D0+%2.3f\r",CurrSetVal);
				pThis->m_pOneSerial->WriteSyncPort((BYTE*)ch,13);
			}
			else if(TargetSpeed-SpeedVal<=-0.8)
			{
				CurrSetVal-=0.005;
				CurrSetVal=(CurrSetVal<0.0?0.0:CurrSetVal);
				sprintf_s(ch,13,"#01D0+%2.3f\r",CurrSetVal);
				pThis->m_pOneSerial->WriteSyncPort((BYTE*)ch,13);
			}
			else if(TargetSpeed-SpeedVal<=-0.5)
			{
				CurrSetVal-=0.0025;
				CurrSetVal=(CurrSetVal<0.0?0.0:CurrSetVal);
				sprintf_s(ch,13,"#01D0+%2.3f\r",CurrSetVal);
				pThis->m_pOneSerial->WriteSyncPort((BYTE*)ch,13);
			}
		}
	}
	//for(int i=0;i<OneRevPos;i++)
	//{
	//	if(OneRecvBuf[i]=='#')
	//	{
	//		for(int k=i;k<OneRevPos;k++)
	//		{
	//			if(OneRecvBuf[k]=='$')
	//			{
	//				
	//				CopyMemory(OneRecv,OneRecvBuf+i+1,k-1);
	//				//strRecv=CString((char*)OneRecv);
	//			//	pThis->m_strInfo=CString((char*)OneRecv);
	//			//	pThis->InvalidateRect(&pThis->m_RectInfoShow);
	//				OneRevPos=0;
	//				break;
	//			}
	//		}
	//	}
	//}

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

	////将接收的缓冲区拷贝到pRecvBuf种
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
	static BYTE TwoRecvBuf[1024]={0}; //接收缓冲区
	static int TwoRevPos=false;
	int revint16[8];
 	int j=0;
	int iTemp=0;
	byte sendfrequency[]={0x01,0x03,0x00,0x00,0x00,0x08,0x44,0x0C};

	//得到父对象指针
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
			pThis->m_iCurrCount=revint16[0];
			pThis->m_dbSpeed =revint16[4];
			//iTemp=(int)(revint16[4]+revint16[5]);
			//pThis->m_dbSpeed =iTemp/2.0;

			
			//pThis->m_pTwoSerial->WriteSyncPort(sendfrequency,8);
		}
	}
}

void CALLBACK CInspectionCar2Dlg::OnThreeSerialRead(void * pOwner,BYTE* buf,DWORD bufLen)
{
	//BYTE *pRecvBuf = NULL; //接收缓冲区
	static BYTE ThreeRecvBuf[1024]={0};
	static int ThreeRevPos=0;
	int j=0;
	int iTemp=0;
	int revint16[4];
	//得到父对象指针
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
	else if(ThreeRecvBuf[17]==1)
	{
		if(ThreeRevPos==0&&bufLen<30)
		{
			ThreeRevPos=ThreeRevPos+bufLen;
		}
		else
		{
			ThreeRevPos=0;
			for(int i=(17+3),j=0;i<(17+11);j++,i+=2)
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}
