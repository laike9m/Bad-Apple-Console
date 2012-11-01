
// Bad Apple ConsoleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Bad Apple Console.h"
#include "Bad Apple ConsoleDlg.h"
#include "afxdialogex.h"
#include "MyTimer.h"
#include "mmsystem.h"
#include <afxwin.h>
#include <iostream>
#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

using namespace std;

void play();
static int time_already_play;
static MCI_OPEN_PARMS mciOpen;
#define NUM 5450;           //图片的总数
int NUM_x = NUM;
clock_t startclk,endclk;

//5图对应x轴1点
int* sync = new int[NUM_x/5];            //保存实际进度中的每个点

void NewSleep(int);   
int background = 0;

MyTimer timer;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBadAppleConsoleDlg 对话框




CBadAppleConsoleDlg::CBadAppleConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBadAppleConsoleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CBadAppleConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBadAppleConsoleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_WM_TIMER()
END_MESSAGE_MAP()


// CBadAppleConsoleDlg 消息处理程序

BOOL CBadAppleConsoleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//窗口风格设置+最小化
	WINDOWPLACEMENT lwndpl;
	WINDOWPLACEMENT * lpwndpl;
	lpwndpl=&lwndpl;
	GetWindowPlacement(lpwndpl);
	lpwndpl->showCmd=SW_SHOWMINIMIZED;
	SetWindowPlacement(lpwndpl);
	this->ModifyStyle(0,WS_OVERLAPPEDWINDOW);
	this->SetWindowText("BadApple Console v1.0");

	//console 设置
	::AllocConsole();    // 打开控件台资源
    freopen("CONOUT$", "w+t", stdout);    // 申请写	
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); // 获取标准输出设备句柄
	SetConsoleTitle("Bad Apple CMD v1.0");       //设置console标题
	SMALL_RECT rc = {0,0, 80-1, 30}; // 重置窗口位置和大小，横向超过79的话设置就会失败
	SetConsoleWindowInfo(hOut,true ,&rc);

	PlayGameSound("Bad Apple.mp3");
	play();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBadAppleConsoleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBadAppleConsoleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBadAppleConsoleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void play(){
	//加载资源IDR_ZIP1
	HINSTANCE hInstance=GetModuleHandle(0);
	HRSRC hrsrc = FindResource(NULL,MAKEINTRESOURCE(IDR_ZIP2),"ZIP");  //第二个参数里面是资源ID，第三个是类型
	HANDLE hglob = LoadResource(hInstance,hrsrc);	//加载资源
	void *zipbuf = LockResource(hglob);	//得到指向资源的指针
	unsigned int ziplen=SizeofResource(hInstance,hrsrc);//读取资源的尺寸
	
	//zip相关操作PART_1
	HZIP hz;	
	ZIPENTRY ze;
	hz = OpenZip(zipbuf, ziplen, 0);  
  	GetZipItem(hz,-1,&ze);  //第二个参数是-1代表获取zip中的所有文件
	int numitems = ze.index;	//获取zip中包含的文件的个数，为下一步循环读取图片做准备

	CImage img;
	string frame_output=""; //一帧输出
	//--------------------------------用加入的资源文件Bad Apple pics.zip完成--------------------------
	int count = 0;
	for (int zi=0; zi<numitems; zi++)
	{ 
		//zip相关操作PART_2
		ZIPENTRY ze; 
		GetZipItem(hz,zi,&ze); //fetch individual details,第二个参数是序号代表获取zip中的第几个文件
		string a("C:\\temp\\");
		string k = ze.name;
		a += k;
		UnzipItem(hz, zi, a.c_str());  //解压图片到路径C:\temp\XXX.jpg
		count++;
		img.Load(a.c_str());
		DeleteFile(a.c_str());//迅速删除图片使不留痕迹
		int i, j; 
		//图像每一行的字节数
		int nRowBytes = img.GetPitch(); 						
		//如果nRowBytes>0  则从开始到结尾
		//如果nRowBytes<0, 则从结尾到开始
		int nWidth = img.GetWidth(); 
		int nHeight = img.GetHeight(); 
		//每个像素所占的字节数
		int nClrCount = img.GetBPP() / 8;     //BPP=bits per pixel
		BYTE* p; 
		p = (BYTE*)img.GetBits(); 
		//if (nRowBytes<0)                             //使p指向第一行的起始像素
		//	p += nRowBytes*(nHeight-1);
		for(i = 0; i < nHeight; i++) { 			
			for(j = 0; j < nWidth; j++) { 
				//存储形式RGBRGBRGB...
				int average;       //平均饱和度
				average = ( p[j*nClrCount]+p[j*nClrCount+1]+p[j*nClrCount+2] )/3;
				//*(start+average) = *(start+average) + 1;     //数组的这一项+1
				if (average>200){
					frame_output += " ";
				}
				else
					frame_output+="#";
			} 
			if (i<nHeight-1)              //最后一行就不用再换行
				p += nRowBytes;        //换行
		}  			
		img.Destroy();	//Destroy了下一次才能读取
		if (count == 1){
			while(1){
				if(time_already_play>200)
					break;
			}
			startclk = clock();                     //计时开始
		}
		//printf("%s",frame_output.c_str());     //输出动画的一帧		
		cout<<frame_output.c_str();
		frame_output = "";      //清空输出字符串

		//移动光标至顶部
		COORD coord;
		coord.X=0;
		coord.Y=10;		
		HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(hout,coord);
		
		//实现精确延时，根据目前进度和标准进度的时间差决定延时长短，并且以sleep(1)为单位减小误差
		endclk = clock();
		float std_time = 218000*float(count)/NUM;
		while (endclk - startclk < std_time){
			timer.Sleep(1);
			endclk = clock();
		}	
	}
	CloseZip(hz);
}

CBadAppleConsoleDlg::~CBadAppleConsoleDlg(){
	FreeConsole();                      // 释放控制台资源
}

UINT CBadAppleConsoleDlg::ThreadPlaySound(LPVOID pFileName)
{
    //srand((DWORD)time(NULL));
    //MCIDEVICEID  dev[20]={0};

    MCI_PLAY_PARMS mciPlay;
    try
    {    
        mciOpen.lpstrDeviceType = (LPCSTR)MCI_DEVTYPE_WAVEFORM_AUDIO;
        mciOpen.lpstrElementName = (LPCSTR)pFileName;
        mciOpen.wDeviceID = 1;
        if(mciSendCommand(NULL,MCI_OPEN, MCI_OPEN_ELEMENT,(DWORD_PTR)&mciOpen)!=0)
        {    
            return 0 ;
        }
        mciPlay.dwFrom = 0;
        mciPlay.dwTo = 100;
        mciSendCommand(1,MCI_PLAY,MCI_FROM|MCI_NOTIFY,(DWORD_PTR)(LPVOID)&mciPlay);
		while(1){
			MCI_STATUS_PARMS StatusParms; 
			StatusParms.dwItem = MCI_STATUS_POSITION;
			mciSendCommand (1, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&StatusParms);
			time_already_play = int(StatusParms.dwReturn);
		}
    }
    catch (...)
    {

    }
    if(pFileName!=NULL)
        delete pFileName;
    return 0 ; 

}

//播放声音,static
void CBadAppleConsoleDlg::PlayGameSound(LPCTSTR pszSoundName)
{        
    //创建动态指针,记得要删除
    size_t size = (strlen(pszSoundName)+1)*sizeof(char);
    char* pChar = (char*)malloc(size);
    memset(pChar,0,size);
    strcpy_s(pChar,size,pszSoundName);
    CWinThread* pTh = AfxBeginThread(ThreadPlaySound,(LPVOID)pChar,0,0,NULL);
}

