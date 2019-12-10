
// FingerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Finger.h"
#include "FingerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <fstream>
#include <iomanip>
#include <io.h>
using namespace std;
#pragma warning(disable:4996)
// CFingerDlg 对话框
#define STEP_IMG_1 "temp\\Step1_Source.bmp"
#define STEP_TXT_1 "temp\\Step1_Source.txt"
#define STEP_TXT_2 "temp\\Step2_MidFilter.txt"

#define STEP_IMG_2 "temp\\Step2_MidFilter.bmp"
int Step1_LoadBmpImage(char* info);
wchar_t* ToWideChar(char* str);
int WriteBMPImgFile(char* dstFileName, unsigned char** pusImgData);
CFingerDlg::CFingerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FINGER_DIALOG, pParent) 
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFingerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfo);
	DDX_Control(pDX, IDC_STATIC_IMG_1, m_picCtrl1);
	DDX_Control(pDX, IDC_STATIC_IMG_2, m_picCtrl2);
	DDX_Text(pDX, IDC_EDIT_Name, m_name);
	DDX_Control(pDX, IDC_ZKFPENGX, m_zkfpEng);

}

BEGIN_MESSAGE_MAP(CFingerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT_Name, &CFingerDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CFingerDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CFingerDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, &CFingerDlg::OnBnClickedBtnDisconnect)
	ON_STN_CLICKED(IDC_STATIC_IMG_1, &CFingerDlg::OnStnClickedStaticImg1)
	ON_BN_CLICKED(IDC_BTN_CAPTURE, &CFingerDlg::OnBnClickedBtnCapture)
	ON_BN_CLICKED(IDC_BTN_STEP_1, &CFingerDlg::OnBnClickedBtnStep1)
	ON_BN_CLICKED(IDC_BTN_DATABASE, &CFingerDlg::OnBnClickedBtnDatabase)
	ON_BN_CLICKED(IDC_BTN_STEP_2, &CFingerDlg::OnBnClickedBtnStep2)
	ON_BN_CLICKED(IDC_BTN_STEP_3, &CFingerDlg::OnBnClickedBtnStep3)
	ON_BN_CLICKED(IDC_BTN_STEP_4, &CFingerDlg::OnBnClickedBtnStep4)
	ON_BN_CLICKED(IDC_BTN_STEP_5, &CFingerDlg::OnBnClickedBtnStep5)
	ON_BN_CLICKED(IDC_BTN_STEP_6, &CFingerDlg::OnBnClickedBtnStep6)
	ON_BN_CLICKED(IDC_BTN_STEP_7, &CFingerDlg::OnBnClickedBtnStep7)
	ON_BN_CLICKED(IDC_BTN_STEP_8, &CFingerDlg::OnBnClickedBtnStep8)
	ON_BN_CLICKED(IDC_BTN_STEP_9, &CFingerDlg::OnBnClickedBtnStep9)
	ON_BN_CLICKED(IDC_BTN_STEP_10, &CFingerDlg::OnBnClickedBtnStep10)
	ON_BN_CLICKED(IDC_BTN_STEP_11, &CFingerDlg::OnBnClickedBtnStep11)
	ON_BN_CLICKED(IDC_BTN_STEP_12A, &CFingerDlg::OnBnClickedBtnStep12a)
	ON_BN_CLICKED(IDC_BTN_STEP_12B, &CFingerDlg::OnBnClickedBtnStep12b)
	ON_BN_CLICKED(IDC_BTN_REGISTER, &CFingerDlg::OnBnClickedBtnRegister)
	ON_BN_CLICKED(IDC_BTN_ADD_IMAGE2, &CFingerDlg::OnBnClickedBtnAddImage2)
	ON_BN_CLICKED(IDC_BTN_MATCH_IMAGE, &CFingerDlg::OnBnClickedBtnMatchImage)
	ON_BN_CLICKED(IDC_BTN_IDENTIFY, &CFingerDlg::OnBnClickedBtnIdentify)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CFingerDlg,CDialog)
	ON_EVENT(CFingerDlg,IDC_ZKFPENGX,8,OnImageReceivedZkfpengx,VTS_PBOOL)
END_EVENTSINK_MAP()


// CFingerDlg 消息处理程序

BOOL CFingerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitDatabase();//创建并初始化指纹库
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFingerDlg::OnPaint()
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
HCURSOR CFingerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFingerDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CFingerDlg::OnBnClickedBtnExit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


void CFingerDlg::OnBnClickedBtnConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	int nn = m_zkfpEng.InitEngine();
	if (nn != 0) {
		MessageBox(_T("采集器无法连接成功"), _T("提示"));
	}
	else 
	{
		MessageBox(_T("采集器连接成功"), _T("提示"));
	}
	
}


void CFingerDlg::OnImageReceivedZkfpengx(BOOL FAR* AImageValid)
{
	HDC hdc = this->GetDC()->m_hDC;
	int x = 250;
	int y = 130;
	int width = m_zkfpEng.get_ImageWidth() +100;
	int height = m_zkfpEng.get_ImageHeight()+100;
	m_zkfpEng.PrintImageAt(int(hdc), x, y, width, height);
}

int ShowImageInCtrl(CStatic& picCtrl, char* filename) {
	CImage  image;
	HRESULT hResult = image.Load(ToWideChar(filename));
	int width = image.GetWidth();
	int height = image.GetHeight();

	CRect rect;
	picCtrl.GetClientRect(&rect);
	CDC* pDc = picCtrl.GetWindowDC();
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);

	image.StretchBlt(pDc->m_hDC, rect, SRCCOPY);

	picCtrl.Invalidate(false);

	image.Destroy();
	picCtrl.ReleaseDC(pDc);
	return 0;
}
wchar_t* ToWideChar(char* str) 
{
	int num = MultiByteToWideChar(0, 0, str, -1, NULL, 0);
	wchar_t* wideStr = new wchar_t[num];
	MultiByteToWideChar(0, 0, str, -1, wideStr, num);
	return wideStr;
}

void CFingerDlg::OnBnClickedBtnDisconnect()
{
	// TODO: 在此添加控件通知处理程序代码
	m_zkfpEng.EndEngine();
}


void CFingerDlg::OnStnClickedStaticImg1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CFingerDlg::OnBnClickedBtnCapture()
{
	// TODO: 在此添加控件通知处理程序代码
	m_zkfpEng.SaveBitmap(_T("capt.bmp"));
}

void CFingerDlg::OnBnClickedBtnStep1()
{
	//初始化操作信息
	char info[MAX_PATH] = { 0 };
	//载入图像
	Step1_LoadBmpImage(info);


	m_staticInfo.SetWindowText(ToWideChar(info));

	ShowImageInCtrl(m_picCtrl1, STEP_IMG_1);

}
int ReadBMPImgFilePara(char *fileName,int &width,int&height,int &depth)
{
	CImage image;
	HRESULT hResult = image.Load(ToWideChar(fileName));
	if (FAILED(hResult) || image.IsNull())
	{
		return -1;
	}

	width = image.GetWidth();
	height = image.GetHeight();
	depth = image.GetBPP();

	if (depth != 8)
	{
		return -2;
	}

	image.Destroy();
	return 0;
}
int ReadBMPImgFileData(char* fileName, unsigned char* data)
{
	CImage image;
	HRESULT hResult = image.Load(ToWideChar(fileName));
	if (FAILED(hResult) || image.IsNull())
	{
		return -1;
	}

	int width = image.GetWidth();
	int height = image.GetHeight();
	int depth = image.GetBPP();

	if (depth != 8)
	{
		return -2;
	}

	memset(data, 0, width * height);

	int pitch = image.GetPitch();
	unsigned char* pData1 = (unsigned char*)image.GetBits();
	unsigned char* pData2 = data;
	unsigned char gray = 0;

	unsigned char* pRow1, * pRow2, * pPix1, * pPix2;
	for (int y = 0; y < height; y++)
	{
		pRow1 = pData1 + pitch * y;
		pRow2 = pData2 + width * y;
		for (int x = 0; x < width; x++)
		{
			pPix1 = pRow1 + x;
			gray = *pPix1;

			pPix2 = pRow2 + x;
			*pPix2 = gray;
		}
	}
	image.Destroy();
	return 0;
}
int SaveDataToTextFile(char* dstFile, unsigned char* data, int width, int height)
{
	ofstream fout(dstFile, ios::out);
	if (!fout)
	{
		return -1;
	}
	int space = 5;
	for (int i = 0; i < height * width; i++)
	{
		fout << setw(space) << int(data[i]);
		if (i % width == (width - 1))
		{
			fout << endl;
		}
	}
	fout.close();
	return 0;
}
int SaveDataToTextFile(char* dstFile, float* data, int width, int height)
{
	ofstream fout(dstFile, ios::out);
	if (!fout)
	{
		return -1;
	}
	int preci = 6;
	int space = 16;
	fout.precision(preci);
	for (int i = 0; i < height * width; i++)
	{
		fout << "  " << setw(space) << data[i];
		if (i % width == (width - 1))
		{
			fout << endl;
		}
	}
	fout.close();
	return 0;
}

int Step1_LoadBmpImage(char* info)
{
	char filename[MAX_PATH] = "capt.bmp";

	CopyFile(ToWideChar(filename), ToWideChar(STEP_IMG_1), false);

	int iWidth, iHeight, iDepth;
	int flag = ReadBMPImgFilePara(filename, iWidth, iHeight, iDepth);
	if (flag != 0)
	{
		sprintf(info, "图像加载失败.");
		return -1;
	}
	unsigned char* data = new unsigned char[iWidth * iHeight];
	flag = ReadBMPImgFileData(filename, data);
	if (flag != 0)
	{
		sprintf(info, "图像数据读取失败");
		delete[] data;
		return -2;
	}
	flag = SaveDataToTextFile(STEP_TXT_1, data, iWidth, iHeight);
	if (flag != 0)
	{
		sprintf(info, "数据保存失败.");
		delete[] data;
		return -3;
	}

	sprintf(info, "源图[%s],宽度[%d],高度[%d],深度[%d b]", filename, iWidth, iHeight, iDepth);

	delete[] data;
	return 0;
}
void InitDatabase()
{
	_mkdir(TEMP_DIR);
	_mkdir(DB_DIR);
	InitFile(DB_INDEX_TXT);
}
void InitFile(char* filename)
{
	FILE* index = fopen(filename, "w");
	fclose(index);
}
void GetDatabaseInfo(char* info)
{
	int pNo = 0;
	char name[MAX_PATH] = { 0 };//登记人姓名
	char srcFile[MAX_PATH] = { 0 };//指纹图像文件
	char mdlFile[MAX_PATH] = { 0 };//指纹特征文件
	FILE* index = fopen(DB_INDEX_TXT, "r");
	while (!feof(index))
	{
		fscanf(index, "%d  %s  %s  %s\n", &pNo, srcFile, mdlFile, name);
	}
	fclose(index);

	sprintf(info, "当前指纹库中共有 %d 条记录", pNo);
}

void CFingerDlg::OnBnClickedBtnDatabase()
{
	// TODO: 在此添加控件通知处理程序代码
	char info[MAX_PATH] = { 0 };
	GetDatabaseInfo(info);
	m_staticInfo.SetWindowText(ToWideChar(info));
}
int ReadDataFromTextFile(char* srcFile, unsigned char* data, int iWidth, int iHeight)
{
	ifstream fin(srcFile, ios::in);
	if (!fin)
	{
		return -1;
	}

	int d = 0;
	for (int i = 0; i < iHeight * iWidth; i++)
	{
		fin >> d;
		data[i] = (unsigned char)d;
	}

	fin.close();

	return 0;
}
int ReadDataFromTextFile(char* srcFile, float* data, int iWidth, int iHeight)
{
	ifstream fin(srcFile, ios::in);
	if (!fin)
	{
		return -1;
	}
	for (int i = 0; i < iHeight * iWidth; i++)
	{
		
		fin >> data[i];
	}

	fin.close();

	return 0;
	
}
void Sort(unsigned char* data, int dsize)
{
	unsigned char temp = 0;
	//方法1
	int i = dsize;
	int j;
	while (i > 0)//升序
	{
		int lastExchangeIndex = 0;
		for (j = 1; j < i; j++)
		{
			if (data[j] < data[j - 1])
			{
				temp = data[j];
				data[j] = data[j - 1];
				data[j - 1] = temp;
				lastExchangeIndex = j; //记下进行交换的记录位置
			}
		}
		i = lastExchangeIndex; // 本趟进行过交换的// 最后一个记录的位置
	}
	//方法2
	/*for (int i = 0; i < dsize; i++)//升序
	{
		for (int j = dsize - 1; j > i; j--)
		{
			if (data[j] < data[j - 1])
			{
				temp = data[j];
				data[j] = data[j - 1];
				data[j - 1] = temp;
			}
		}
	}*/
	/*
	for (int k = 0; k < dsize / 2; k++)//倒序
	{
		temp = data[k];
		data[k] = data[dsize-1-k];
		data[dsize - 1 - k] = temp;
	}*/
}
int MidFilter(unsigned char* ucImg, unsigned char* ucDstImg, int iWidth, int iHeight)
{
	memset(ucDstImg, 0, iWidth * iHeight);

	unsigned char* pUp, * pDown, * pImg;
	unsigned char x[9];

	for (int i = 1; i < iHeight - 1; i++)
	{
		pUp = ucImg + (i - 1) * iWidth;
		pImg = ucImg + i * iWidth;
		pDown = ucImg + (i + 1) * iWidth;

		for (int j = 1; j < iWidth - 1; j++)
		{
			pUp++;
			pImg++;
			pDown++;

			x[0] = *(pUp - 1);
			x[1] = *(pImg - 1);
			x[2] = *(pDown - 1);

			x[3] = *pUp;
			x[4] = *pImg;
			x[5] = *pDown;

			x[6] = *(pUp + 1);
			x[7] = *(pImg + 1);
			x[8] = *(pDown + 1);

			Sort(x, 9);

			*(ucDstImg + i * iWidth + j) = x[4];

		}
	}
	pDown = ucImg + iWidth;
	for (int j = 1; j < iWidth - 1; j++)
	{
		x[0] = *(ucImg +j - 1);
		x[1] = *(ucImg + j);
		x[2] = *(ucImg +j + 1);

		x[3] = *(pDown + j -1);
		x[4] = *(pDown + j);
		x[5] = *(pDown+ j + 1);

		Sort(x, 6);

		*(ucDstImg + j) = x[3];
	}

	pUp = ucImg + iWidth * (iHeight - 2);
	pDown = ucImg + iWidth * (iHeight - 1);
	for (int j = 1; j < iWidth - 1; j++)
	{
		x[0] = *(pDown + j - 1);
		x[1] = *(pDown + j);
		x[2] = *(pDown + j + 1);

		x[3] = *(pUp + j - 1);
		x[4] = *(pUp + j);
		x[5] = *(pUp + j + 1);

		Sort(x, 6);

		*(ucDstImg + iWidth * (iHeight-1) + j) = x[3];
	}
	//左上角
	x[0] = *(ucImg);
	x[1] = *(ucImg + 1);
	x[2] = *(ucImg + iWidth);
	x[3] = *(ucImg + iWidth + 1);
	Sort(x, 4);
	*(ucDstImg) = x[2];

	//右上角
	x[0] = *(ucImg + iWidth - 1);
	x[1] = *(ucImg + iWidth - 2);
	x[2] = *(ucImg + 2 * iWidth - 1);
	x[3] = *(ucImg + 2 * iWidth - 2);
	Sort(x, 4);

	*(ucDstImg + iWidth - 1) = x[2];

	//左下角
	x[0] = *(ucImg + iWidth*(iHeight-1));
	x[1] = *(ucImg + iWidth * (iHeight - 2));
	x[2] = *(ucImg + iWidth * (iHeight - 1)+1);
	x[3] = *(ucImg + iWidth * (iHeight - 2)+1);
	Sort(x, 4);

	*(ucDstImg + iWidth * (iHeight - 1)) = x[2];


	//右下角
	x[0] = *(ucImg + iWidth * (iHeight - 0)-1);
	x[1] = *(ucImg + iWidth * (iHeight - 1)-1);
	x[2] = *(ucImg + iWidth * (iHeight - 0) -2);
	x[3] = *(ucImg + iWidth * (iHeight - 1) -2);
	Sort(x, 4);

	*(ucDstImg + iWidth * (iHeight - 0)-1) = x[2];

	return 0;
 }
 

 int SaveDataToImageFile(char* srcFile, char* dstFile, unsigned char* data)
 {
	 CopyFile(ToWideChar(srcFile), ToWideChar(dstFile), false);
	 WriteBMPImgFile(dstFile, &data);
	 return 0;
 }
 int SaveDataToImageFile(char* srcFile, char* dstFile, float* data, float scale)
 {
	 int iWidth, iHeight, iDepth;
	 ReadBMPImgFilePara(srcFile, iWidth, iHeight, iDepth);

	 CopyFile(ToWideChar(srcFile), ToWideChar(dstFile), false);
	 unsigned char* tmpData = new unsigned char[iWidth * iHeight];
	 for (int i = 0; i <int(iWidth * iHeight); i++)
	 {
		 tmpData[i] = unsigned char((scale * data[i]));
	 }

	 WriteBMPImgFile(dstFile, &tmpData);

	 delete[] tmpData;

	 return 0;
 }
 int WriteBMPImgFile(char* dstFileName, unsigned char** pusImgData)
 {
	 FILE* fp = fopen(dstFileName, "r+b");
	 if (!fp)
	 {
		 return -1;
	 }
	 int imgType, iWidth, iHeight;
	 int iStartPos = 0;
	 fseek(fp, 10L, SEEK_SET);
	 fread((char*)(&iStartPos), 4, 1, fp);
	 fseek(fp, 18L, SEEK_SET);
	 fread((char*)(&iWidth), 4, 1, fp);
	 fread((char*)(&iHeight), 4, 1, fp);
	 unsigned short temp;
	 fseek(fp, 28L, SEEK_SET);
	 fread((char*)(&temp), 2, 1, fp);
	 imgType = temp;
	 if (imgType != 8)
	 {
		 return -2;
	 }
	 unsigned char* usImgData = *pusImgData;
	 int iWidthInFile = 0;
	 if (iWidth % 4 > 0)
	 {
		 iWidthInFile = iWidth - iWidth % 4 + 4;
	 }
	 else
	 {
		 iWidthInFile = iWidth;

	 }
	 for (int i = iHeight - 1; i >= 0; i--)
	 {
		 fseek(fp, iStartPos, SEEK_SET);
		 fwrite((usImgData + i * iWidth), 1, iWidth, fp);
		 iStartPos += iWidthInFile;
	 }
	 fclose(fp);
	 return 0;
 }
 int Step2_MidFilter(char* info)
 {
	 //设置输入输出文件名
	 char srcTxtFile[MAX_PATH] = { STEP_TXT_1 };//源数据文件名
	 char srcImgFile[MAX_PATH] = { STEP_IMG_1 };//源图文件名
	 char dstTxtFile[MAX_PATH] = { STEP_TXT_2 };//结果数据文件名
	 char dstImgFile[MAX_PATH] = { STEP_IMG_2 };//结果图文件名
	 //读取图像参数
	 int iWidth, iHeight, iDepth;//图像宽度/高度/深度
	 ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	 //读取源数据
	 unsigned char* imagel = new unsigned char[iWidth * iHeight];
	 ReadDataFromTextFile(srcTxtFile, imagel, iWidth, iHeight);

	 //中值滤波
	 unsigned char* image2 = new unsigned char[iWidth * iHeight];
	 MidFilter(imagel, image2, iWidth, iHeight);

	 //保存结果数据（TXT文件）

	 SaveDataToTextFile(dstTxtFile, image2, iWidth, iHeight);


	 //保存结果图像（BMP文件，仅用于显示）
	 SaveDataToImageFile(srcImgFile, dstImgFile, image2);

	 //释放内存空间
	 delete[] imagel;
	 delete[] image2;

	 //返回操作成败状态
	 sprintf(info, "完成中值滤波.");

	 return 0;
 }

 void CFingerDlg::OnBnClickedBtnStep2()
 {
	 char info[MAX_PATH] = { 0 };

	 //中值滤波
	 Step2_MidFilter(info);

	 //显示操作结果信息
	 m_staticInfo.SetWindowText(ToWideChar(info));

	 //显示图像
	 ShowImageInCtrl(m_picCtrl1, STEP_IMG_1);//显示输入图像
	 ShowImageInCtrl(m_picCtrl2, STEP_IMG_2);//显示输出图像
	 // TODO: 在此添加控件通知处理程序代码
 }
 int HistoNormalize(unsigned char* ucImg, unsigned char* ucNormImg, int iWidth, int iHeight) {
	 unsigned int Histogram[256];

	 memset(Histogram, 0, 256 * sizeof(int));
	 for (int i = 0; i < iHeight; i++) {
		 for (int j = 0; j < iWidth; j++) {
			 Histogram[ucImg[i * iWidth + j]]++;
		 }
	 }
	 double dMean = 0;
	 for (int i = 1; i < 255; i++) {
		 dMean += i * Histogram[i];
	 }
	 dMean = int(dMean / (iWidth * iHeight));
	 double dSigma = 0;
	 for (int i = 0; i < 255; i++) {
		 dSigma += Histogram[i] * (i - dMean) * (i - dMean);
	 }
	 dSigma /= (iWidth * iHeight);
	 dSigma = sqrt(dSigma);

	 double dMean0 = 128, dSigma0 = 128;
	 double dCoeff = dSigma0 / dSigma;
	 for (int i = 0; i < iHeight; i++) {
		 for (int j = 0; j < iWidth; j++) {
			 double dVal = ucImg[i * iWidth + j];
			 dVal = dMean0 + dCoeff * (dVal - dMean0);
			 if (dVal < 0) {
				 dVal = 0;
			 }
			 else if (dVal > 255) {
				 dVal = 255;
			 }
			 ucNormImg[i * iWidth + j] = (unsigned char)dVal;
		 }
	 }
	 return 0;
 }
#define STEP_TXT_3 "temp\\Step3_Normalize.txt"
#define STEP_IMG_3 "temp\\Step3_Normalize.bmp"

 //直方图均衡化单步测试
 //参数：info（返回操作成功或失败提示信息）
 int Step3_Normalize(char* info)
 {
	 //设置输入输出文件名

	 char srcTxtFile[MAX_PATH] = { STEP_TXT_2 };//源数据文件名

	 char srcImgFile[MAX_PATH] = { STEP_IMG_2 };//源图文件名

	 char dstTxtFile[MAX_PATH] = { STEP_TXT_3 };//结果数据文件名

	 char dstImgFile[MAX_PATH] = { STEP_IMG_3 };//结果图文件名

	 //读取图像参数

	 int iWidth, iHeight, iDepth;//图像宽度/高度/深度

	 ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	 //读取源数据

	 unsigned char* image1 = new unsigned char[iWidth * iHeight];

	 ReadDataFromTextFile(srcTxtFile, image1, iWidth, iHeight);

	 //直方图均衡化

	 unsigned char* image2 = new unsigned char[iWidth * iHeight];
	 HistoNormalize(image1, image2, iWidth, iHeight);

	 //保存结果数据（TXT文件）

	 SaveDataToTextFile(dstTxtFile, image2, iWidth, iHeight);

	 //保存结果图像（BMP文件，仅用于显示）
	 SaveDataToImageFile(srcImgFile, dstImgFile, image2);

	 //释放空间
	 delete[] image1;
	 delete[] image2;

	 //返回操作成败状态
	 sprintf(info, "完成直方图均衡化");
	 return 0;

 }

 void CFingerDlg::OnBnClickedBtnStep3()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 //初始化操作结果信息
	 char info[MAX_PATH] = { 0 };

	 //直方图均衡化
	 Step3_Normalize(info);

	 //显示操作结果信息
	 m_staticInfo.SetWindowText(ToWideChar(info));

	 //显示图像
	 ShowImageInCtrl(m_picCtrl1, STEP_IMG_2);//显示输入图像
	 ShowImageInCtrl(m_picCtrl2, STEP_IMG_3);//显示输出图像
 }
 int ImgDirection(unsigned char* ucImg, float* fDirc, int iWidth, int iHeight)
 {
	 //定义变量
	 const int SEMISIZ = 7;//领域窗口区域半径
	 int dx[SEMISIZ * 2 + 1][SEMISIZ * 2 + 1];
	 int dy[SEMISIZ * 2 + 1][SEMISIZ * 2 + 1];
	 float fx, fy;

	 //结果初始化
	 memset(fDirc, 0, iWidth * iHeight * sizeof(float));
	 //计算每一像素的脊线方向值
	 for (int y = SEMISIZ + 1; y < iHeight - SEMISIZ - 1; y++)//逐行遍历（除去边缘）
	 {
		 for (int x = SEMISIZ + 1; x < iWidth - SEMISIZ - 1; x++)//逐列遍历（除去边缘）
		 {
			 //计算以当前像素为中心的领域窗口区每一像素的梯度
			 for (int j = 0; j < SEMISIZ * 2 + 1; j++)
			 {
				 for (int i = 0; i < SEMISIZ * 2 + 1; i++)
				 {
					 int index1 = (y + j - SEMISIZ) * iWidth + x + i - SEMISIZ;
					 int index2 = (y + j - SEMISIZ) * iWidth + x + i - SEMISIZ - 1;
					 int index3 = (y + j - SEMISIZ - 1) * iWidth + x + i - SEMISIZ;
					 dx[i][j] = int(ucImg[index1] - ucImg[index2]);
					 dy[i][j] = int(ucImg[index1] - ucImg[index3]);
				 }
			 }
			 //计算当前像素的脊线方向值
			 fx = 0.0;
			 fy = 0.0;
			 for (int j = 0; j < SEMISIZ * 2 + 1; j++)
			 {
				 for (int i = 0; i < SEMISIZ * 2 + 1; i++)
				 {
					 fx += 2 * dx[i][j] * dy[i][j];

					 fy += (dx[i][j] * dx[i][j] - dy[i][j] * dy[i][j]);
				 }
			 }
			 fDirc[y * iWidth + x] = atan2(fx, fy);
		 }
	 }

	 return 0;
 }
 int DircLowPass(float* fDirc, float* fFitDirc, int iWidth, int iHeight)
 {
	 //定义变量
	 const int DIR_FILTER_SIZE = 2;
	 int blocksize = 2 * DIR_FILTER_SIZE + 1;
	 int imgsize = iWidth * iHeight;

	 float* filter = new float[blocksize * blocksize];//使用5*5滤波器
	 float* phix = new float[imgsize];
	 float* phiy = new float[imgsize];
	 float* phi2x = new float[imgsize];
	 float* phi2y = new float[imgsize];

	 //结果初始化

	 memset(fFitDirc, 0, sizeof(float) * iWidth * iHeight);

	 //设置5*5高斯低通滤波器模板
	 float tempSum = 0.0;
	 for (int y = 0; y < blocksize; y++)
	 {
		 for (int x = 0; x < blocksize; x++)
		 {
			 filter[y * blocksize + x] = (float)(blocksize - (abs(DIR_FILTER_SIZE - x) + abs(DIR_FILTER_SIZE - y)));
			 tempSum += filter[y * blocksize + x];

		 }
	 }

	 for (int y = 0; y < blocksize; y++)
	 {
		 for (int x = 0; x < blocksize; x++)
		 {
			 filter[y * blocksize + x] /= tempSum;
		 }
	 }

	 //计算各像素点的方向正弦值和余弦值
	 for (int y = 0; y < iHeight; y++)
	 {
		 for (int x = 0; x < iWidth; x++)
		 {
			 phix[y * iWidth + x] = cos(fDirc[y * iWidth + x]);
			 phiy[y * iWidth + x] = sin(fDirc[y * iWidth + x]);

		 }
	 }


	 //对所有像素进行方向低通滤波

	 memset(phi2x, 0, sizeof(float) * imgsize);
	 memset(phi2y, 0, sizeof(float) * imgsize);
	 float nx, ny;
	 int val;
	 for (int y = 0; y < iHeight - blocksize; y++)//逐行遍历（除去边缘区段）
	 {
		 for (int x = 0; x < iWidth - blocksize; x++)//逐列遍历（除去边缘区段）
		 {
			 //对以当前像素为中心的滤波窗口内的所有像素值进行加权累加
			 nx = 0.0;
			 ny = 0.0;
			 for (int j = 0; j < blocksize; j++)
			 {
				 for (int i = 0; i < blocksize; i++)
				 {
					 val = (x + i) + (j + y) * iWidth;
					 nx += filter[j * blocksize + i] * phix[val];//方向余弦加权累加
					 ny += filter[j * blocksize + i] * phiy[val];//方向正弦加权累加

				 }
			 }
			 //将累加结果作为当前像素的新的方向正弦值和余弦值
			 val = x + y * iWidth;
			 phi2x[val] = nx;
			 phi2y[val] = ny;

		 }
	 }
	 //根据加权累加结果，计算各像素的方向滤波结果值
	 for (int y = 0; y < iHeight - blocksize; y++)
	 {
		 for (int x = 0; x < iWidth - blocksize; x++)
		 {
			 val = x + y * iWidth;
			 fFitDirc[val] = atan2(phi2y[val], phi2x[val]) * 0.5;

		 }
	 }
	 delete[]phi2y;
	 delete[]phi2x;
	 delete[]phiy;
	 delete[]phix;

	 return 0;
 }
#define STEP_IMG_4 "temp\\Step4_Direction.bmp"
#define STEP_TXT_4 "temp\\Step4_Direction.txt"

 int Step4_Direction(char* info)
 {
	 char srcTxtFile[MAX_PATH] = { STEP_TXT_3 };
	 char srcImgFile[MAX_PATH] = { STEP_IMG_3 };
	 char dstTxtFile[MAX_PATH] = { STEP_TXT_4 };
	 char dstImgFile[MAX_PATH] = { STEP_IMG_4 };

	 int iWidth, iHeight, iDepth;
	 ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	 unsigned char* image1 = new unsigned char[iWidth * iHeight];
	 ReadDataFromTextFile(srcTxtFile, image1, iWidth, iHeight);

	 float* tmpDirections = new float[iWidth * iHeight];
	 ImgDirection(image1, tmpDirections, iWidth, iHeight);

	 float* directions = new float[iWidth * iHeight];
	 DircLowPass(tmpDirections, directions, iWidth, iHeight);

	 SaveDataToTextFile(dstTxtFile, directions, iWidth,iHeight);

	 const int DIRECTION_SCALE = 100;
	 SaveDataToImageFile(srcImgFile, dstImgFile, directions, DIRECTION_SCALE);

	 delete[] image1;
	 delete[] tmpDirections;
	 delete[] directions;


	 sprintf(info, "完成方向计算.");
	 return 0;

 }

 void CFingerDlg::OnBnClickedBtnStep4()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 char info[MAX_PATH] = { 0 };

	 Step4_Direction(info);
	 m_staticInfo.SetWindowText(ToWideChar(info));

	 ShowImageInCtrl(m_picCtrl1, STEP_IMG_3);
	 ShowImageInCtrl(m_picCtrl2, STEP_IMG_4);
 }
 //3.12.1指纹脊线频率计算
//参考函数Frequency() 如下：
//核心代码如下：

//指纹脊线频率计算
//ucImg(图源数据),fDirection(脊线方向数据),fFrequency(脊线频率结果数据),iWidth(图像宽度),iHeight(图像高度)
 int Frequency(unsigned char* ucImg, float* fDirection, float* fFrequency, int iWidth, int iHeight)
 {
	 //窗口大小
	 const  int SIZE_L = 32;
	 const int SIZE_W = 16;
	 const  int SIZE_L2 = 16;
	 const int SIZE_W2 = 8;

	 //正弦波峰值点
	 int peak_pos[SIZE_L];
	 int peak_cnt;
	 float peak_freq;
	 float Xsig[SIZE_L];

	 //方向
	 float dir = 0.0;
	 float cosdir = 0.0;
	 float sindir = 0.0;

	 float maxPeak, minPeak;

	 //结果初始化
	 float* frequency1 = new float[iWidth * iHeight];
	 memset(fFrequency, 0, sizeof(float) * iWidth * iHeight);
	 memset(frequency1, 0, sizeof(float) * iWidth * iHeight);

	 int x, y;
	 int d, k;
	 int u, v;

	 for (y = SIZE_L2; y < iHeight - SIZE_L2; y++)//逐行遍历
	 {
		 for (x = SIZE_L2; x < iWidth - SIZE_L2; x++)//逐列遍历
		 {
			 //当前像素的脊线方向
			 dir = fDirection[(y + SIZE_W2) * iWidth + (x + SIZE_W2)];
			 cosdir = -sin(dir);
			 sindir = cos(dir);

			 //计算以当前像素为中心的L*W邻域窗口的幅值序列，X[0]..X[L-1]
			 for (k = 0; k < SIZE_L; k++)
			 {
				 Xsig[k] = 0.0;
				 for (d = 0; d < SIZE_W; d++)
				 {
					 u = (int)(x + (d - SIZE_W2) * cosdir + (k - SIZE_L2) * sindir);
					 v = (int)(y + (d - SIZE_W2) * sindir + (k - SIZE_L2) * cosdir);

					 //边界点处理
					 if (u < 0)
					 {
						 u = 0;
					 }
					 else if (u > iWidth - 1)
					 {
						 u = iWidth - 1;
					 }
					 if (v < 0)
					 {
						 v = 0;
					 }
					 else if (v > iHeight - 1)
					 {
						 v = iHeight - 1;
					 }
					 Xsig[k] += ucImg[u + v * iWidth];
				 }
				 Xsig[k] /= SIZE_W;

			 }

			 //确定幅值序列变化范围
			 maxPeak = minPeak = Xsig[0];
			 for (k = 0; k < SIZE_L; k++)
			 {
				 if (minPeak > Xsig[k])
				 {
					 minPeak = Xsig[k];
				 }
				 if (maxPeak < Xsig[k])
				 {
					 maxPeak = Xsig[k];
				 }
			 }

			 //确定峰点的位置
			 peak_cnt = 0;
			 if ((maxPeak - minPeak) > 64)
			 {
				 for (k = 0; k < SIZE_L; k++)
				 {
					 if ((Xsig[k - 1] < Xsig[k]) && (Xsig[k] >= Xsig[k + 1]))
					 {
						 peak_pos[peak_cnt++] = k;
					 }
				 }
			 }
			 //计算峰值点平均值
			 peak_freq = 0.0;
			 if (peak_cnt >= 2)

			 {
				 for (k = 0; k < peak_cnt; k++) {
					 peak_freq += (peak_pos[k + 1] - peak_pos[k]);

				 }
				 peak_freq /= peak_cnt - 1;

			 }

			 //计算当前像素频率
			 if (peak_freq < 3.0 || peak_freq>25.0)
			 {
				 frequency1[x + y * iWidth] = 0.0;
			 }
			 else
			 {
				 frequency1[x + y * iWidth] = 1.0 / peak_freq;
			 }
		 }
	 }

	 //对频率进行均值滤波
	 for (y = SIZE_L2; y < iHeight - SIZE_L2; y++)//逐行遍历（出去边缘区段
	 {
		 for (x = SIZE_L2; x < iWidth - SIZE_L2; x++)//逐列遍历 除去边缘区段
		 {
			 k = x + y * iWidth;//当前像素位置
			 peak_freq = 0.0;
			 //使用一当前像素为中心的5程5邻域窗口进项均值滤波
			 for (v = -2; v <= 2; v++)
			 {
				 for (u = -2; u <= 2; u++) {

					 peak_freq += frequency1[(x + u) + (y + v) * iWidth];//求频率累加和
				 }
			 }
			 fFrequency[k] = peak_freq / 25;//当前像素频率等于窗口内频率平均值
		 }
	 }
	 delete[] frequency1;
	 return 0;
 }
 //3.12.2指纹脊线频率计算单步测试

//参考函数Step5_Frequency()
//核心代码如下:
#define STEP_TXT_5 "temp\\Step5_Frequency.txt"
# define STEP_IMG_5 "temp\\Step5_Frequency.bmp"
//指纹脊线频率计算单步测试
//参数:info(返回操作成功或失败提示信息)
 int Step5_Frequency(char* info)
 {
	 //设置输入输出文件名
	 char srcTxtFile_Img[MAX_PATH] = { STEP_TXT_3 };//图像源数据文件名
	 char srcTxtFile_Dir[MAX_PATH] = { STEP_TXT_4 };//方向源数据文件名
	 char srcImgFile[MAX_PATH] = { STEP_IMG_4 };//源图文件名
	 char dstTxtFile[MAX_PATH] = { STEP_TXT_5 };//结果数据文件名
	 char dstImgFile[MAX_PATH] = { STEP_IMG_5 };//结果图文件名

	 //读取图像参数  ReadBMPImgFilePara未知？？？？
	 int iWidth, iHeight, iDepth;
	 int flag = ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	 //读取图像源数据
	 unsigned char* image1 = new unsigned char[iWidth * iHeight];
	 ReadDataFromTextFile(srcTxtFile_Img, image1, iWidth, iHeight);

	 //读取方向源数据
	 float* direction = new float[iWidth * iHeight];
	 ReadDataFromTextFile(srcTxtFile_Dir, direction, iWidth, iHeight);

	 //频率计算
	 float* frequency = new float[iWidth * iHeight];
	 Frequency(image1, direction, frequency, iWidth, iHeight);


	 SaveDataToTextFile(dstTxtFile, frequency, iWidth, iHeight);


	 //保存结果数据(TXT文件)
	 const int FREQUENCY_SCALE = 1000;
	 SaveDataToImageFile(srcImgFile, dstImgFile, frequency, FREQUENCY_SCALE);

	 //释放内存空间
	 delete[] image1;
	 delete[] direction;
	 delete[] frequency;

	 //返回操作失败状态
	 sprintf(info, "完成频率计算.");
	 return 0;
 }
 void CFingerDlg::OnBnClickedBtnStep5()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 char info[MAX_PATH] = { 0 };

	 //指纹脊线频率计算
	 Step5_Frequency(info);

	 //显示操作结果信息
	 m_staticInfo.SetWindowText(ToWideChar(info));

	 //显示图像
	 ShowImageInCtrl(m_picCtrl1, STEP_IMG_4);//显示输入图像
	 ShowImageInCtrl(m_picCtrl2, STEP_IMG_5);//显示输出图像	
 }
 int GetMask(unsigned char* ucImg, float* fDirection, float* fFrequency, unsigned char* ucMask, int iWidth, int iHeight)
 {
	 //第1步：阈值分割（像素频率位于指定范围之内则设为前景点，否则设为背景点）
	 float freqMin = 1.0 / 25.0;
	 float freqMax = 1.0 / 3.0;
	 int x, y, k;
	 int pos, posout;
	 int expand_counts = 4;// iHeight * 2;//重复膨胀次数 
	 int erosion_counts = 20;// iHeight * 2; //重复腐蚀次数

	 memset(ucMask, 0, iWidth * iHeight);//结果初始化（全为背景点）

	 for (y = 0; y < iHeight; y++)//逐行遍历
	 {
		 for (x = 0; x < iWidth; x++)//逐列遍历
		 {
			 pos = x + y * iWidth;
			 posout = x + y * iWidth;
			 ucMask[posout] = 0;

			 //if (fFrequency[pos] >= freqMin && fFrequency[pos] <= freqMax)
			 //{
				//ucMask[posout] = 255;//频率位于指定范围之内则设为前景点
			 //}

			 if (fFrequency[pos] != 0)
			 {
				 ucMask[posout] = 255;//频率不为0设为前景点
			 }
		 }
	 }

	 //第2步：填充孔洞（利用形态学的膨胀原理）
	 for (k = 0; k < expand_counts; k++)//重复膨胀次数（次数自定）
	 {
		 //标记前景点
		 for (y = 1; y < iHeight - 1; y++)//逐行遍历
		 {
			 for (x = 1; x < iWidth - 1; x++)//逐列遍历
			 {
				 //前景点的上下左右 上左右，下左右 八个相邻点都标记为前景点（将8位像素值的最高位设为1予以标记）
				 if (ucMask[x + y * iWidth == 0xFF])//前景点
				 {
					 ucMask[x - 1 + y * iWidth] |= 0x80;//正上
					 ucMask[x + 1 + y * iWidth] |= 0x80;//正下
					 ucMask[x + (y - 1) * iWidth] |= 0x80;//正左
					 ucMask[x + (y + 1) * iWidth] |= 0x80;//正右
					 
					 ucMask[x - 1 + (y - 1) * iWidth] |= 0x80;//上左
					 ucMask[x - 1 + (y + 1) * iWidth] |= 0x80;//上右
					 ucMask[x + 1 + (y - 1) * iWidth] |= 0x80;//下左
					 ucMask[x + 1 + (y - 1) * iWidth] |= 0x80;//下右   

				 }
			 }
		 }
		 for (x = 1; x < iHeight - 1; x++)//逐行遍历
		 {
			 for (y = 1; y < iWidth - 1; y++)//逐列遍历
			 {
				 //前景点的上下左右 上左右，下左右 八个相邻点都标记为前景点（将8位像素值的最高位设为1予以标记）
				 if (ucMask[y + x * iWidth == 0xFF])//前景点
				 {
					 ucMask[y - 1 + x * iWidth] |= 0x80;//正上
					 ucMask[y + 1 + x * iWidth] |= 0x80;//正下
					 ucMask[y + (x - 1) * iWidth] |= 0x80;//正左
					 ucMask[y + (x + 1) * iWidth] |= 0x80;//正右
                     
					 ucMask[x - 1 + (y - 1) * iWidth] |= 0x80;//上左
					 ucMask[x - 1 + (y + 1) * iWidth] |= 0x80;//上右
					 ucMask[x + 1 + (y - 1) * iWidth] |= 0x80;//下左
					 ucMask[x + 1 + (y - 1) * iWidth] |= 0x80;//下右   

				 }
			 }
		 }
		 //判断和设置前景点
		 for (y = 1; y < iHeight - 1; y++)
		 {
			 for (x = 1; x < iWidth - 1; x++)
			 {
				 //将标记为前景点的像素都设为前景点
				 if (ucMask[x + y * iWidth])
				 {
					 ucMask[x + y * iWidth] = 0xFF;//设为前景点（像素值为255）

				 }
			 }
		 }
	 }

	 //第3步：去除边缘和孤立点（利用形态学的腐蚀原理）
	 for (k = 0; k < erosion_counts; k++)//重复腐蚀多次（次数自定）
	 {
		 //标记背景点
		 for (y = 1; y < iHeight - 1; y++)//逐行遍历
		 {
			 for (x = 1; x < iWidth - 1; x++)//逐列遍历
			 {
				 //背景点的上下左右 上左右，下左右 八个相邻点都标记为潜在的背景点（将8位像素值的后7位设为0予以标记）
				 if (ucMask[x + y * iWidth] == 0x0)//背景点
				 {
					 ucMask[x - 1 + y * iWidth] |= 0x80;//正上
					 ucMask[x + 1 + y * iWidth] |= 0x80;//正下
					 ucMask[x + (y - 1) * iWidth] |= 0x80;//正左
					 ucMask[x + (y + 1) * iWidth] |= 0x80;//正右

					 ucMask[x - 1 + (y - 1) * iWidth] |= 0x80;//上左
					 ucMask[x - 1 + (y + 1) * iWidth] |= 0x80;//上右
					 ucMask[x + 1 + (y - 1) * iWidth] |= 0x80;//下左
					 ucMask[x + 1 + (y - 1) * iWidth] |= 0x80;//下右

				 }
			 }
		 }
		 //判断和设置背景点
		 for (y = 1; y < iHeight - 1; y++)//逐行遍历
		 {
			 for (x = 1; x < iWidth - 1; x++)//逐列遍历
			 {
				 //只要不是前景点的所有像素都设为背景点
				 if (ucMask[x + y * iWidth] != 0xFF)//非前景点
				 {
					 ucMask[x + y * iWidth] = 0x0;//设为背景点（像素值为0）

				 }
			 }
		 }
	 }
	 return 0;
 }
#define STEP_TXT_6 "temp\\Step6_Mask.txt"
#define STEP_IMG_6 "temp\\Step6_Mask.bmp"
 //指纹掩码计算单步测试
 //参数：info（返回操作成功或失败提示信息）
 int Step6_Mask(char* info)
 {
	 //设置输入输出文件名
	 char srcTxtFile_Img[MAX_PATH] = { STEP_TXT_3 };//图像源数据文件名
	 char srcTxtFile_Dir[MAX_PATH] = { STEP_TXT_4 };//方向源数据文件名
	 char srcTxtFile_Fre[MAX_PATH] = { STEP_TXT_5 };//频率源数据文件名
	 char srcImgFile[MAX_PATH] = { STEP_IMG_5 };//源图文件名
	 char dstTxtFile[MAX_PATH] = { STEP_TXT_6 };//结果数据文件名
	 char dstImgFile[MAX_PATH] = { STEP_IMG_6 };//结果图文件名

	 //读取图像参数
	 int iWidth, iHeight, iDepth;
	 ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	 //读取图像源数据
	 unsigned char* image1 = new unsigned char[iWidth * iHeight];
	 ReadDataFromTextFile(srcTxtFile_Img, image1, iWidth, iHeight);

	 //读取方向源数据
	 float* direction = new float[iWidth * iHeight];
	 ReadDataFromTextFile(srcTxtFile_Dir, direction, iWidth, iHeight);

	 //读取频率源数据
	 float* frequency = new float[iWidth * iHeight];
	 ReadDataFromTextFile(srcTxtFile_Fre, frequency, iWidth, iHeight);

	 //掩码计算
	 unsigned char* mask = new unsigned char[iWidth * iHeight];
	 GetMask(image1, direction, frequency, mask, iWidth, iHeight);

	 //保存结果数据（TXT文件）
	 SaveDataToTextFile(dstTxtFile, mask, iWidth, iHeight);

	 //保存结果图像（BMP文件，仅用于显示）
	 SaveDataToImageFile(srcImgFile, dstImgFile, mask);

	 //释放空间
	 delete[] image1;
	 delete[] mask;
	 delete[] direction;
	 delete[] frequency;

	 //返回操作成功状态
	 sprintf(info, "完成掩码计算.");

	 return 0;

 }

 void CFingerDlg::OnBnClickedBtnStep6()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 char info[MAX_PATH] = { 0 };

	 //指纹掩码计算
	 Step6_Mask(info);

	 //显示操作结果信息
	 m_staticInfo.SetWindowText(ToWideChar(info));
	 //显示图像
	 ShowImageInCtrl(m_picCtrl1, STEP_IMG_5);//显示输入图像
	 ShowImageInCtrl(m_picCtrl2, STEP_IMG_6);//显示输出图像
 }
 //3.14.1指纹图像Gabor滤波增强

//参考函数GaborEnhance()
//核心代码如下:
//Gabor滤波增强
//ucImg(源图数据),fDirecton(脊线方向数据),fFrequency(脊线频率数据),ucMask(掩码数据),ucImgEnhanced(滤波增强结果数据),iWidth(图像宽度),iHeight(图像高度)
 int GaborEnhance(unsigned char* ucImg, float* fDirecton, float* fFrequency, unsigned char* ucMask, unsigned char* ucImgEnhanced, int iWidth, int iHeight)
 {
	 //定义变量
	 const float PI = 3.141592654;
	 int i, j, u, v;
	 int wg2 = 5;//使用11*11的Gabor滤波器，半边长为5
	 float sum, f, g;
	 float x2, y2;
	 float dx2 = 1.0 / (4.0 * 4.0);//沿x轴方向的高斯包络常量
	 float dy2 = 1.0 / (4.0 * 4.0);//沿y轴方向的高斯包络常量

	 //结果初始化
	 memset(ucImgEnhanced, 0, iWidth * iHeight);

	 //Gabor滤波增强
	 for (j = wg2; j < iHeight - wg2; j++)
	 {
		 for (i = wg2; i < iWidth - wg2; i++)
		 {
			 //跳过背景点
			 if (ucMask[i + j * iWidth] == 0)//掩码为0表示背景点
			 {
				 continue;
			 }

			 //获取当前图像的方向和频率
			 g = fDirecton[i + j * iWidth];//当前像素的脊线方向
			 f = fFrequency[i + j * iWidth];//当前像素的脊线频率
			 g += PI / 2;//垂直方向

			 //对当前像素进行Gabor滤波
			 sum = 0.0;
			 for (v = -wg2; v <= wg2; v++)//滤波窗口逐行遍历
			 {
				 for (u = -wg2; u <= wg2; u++)//滤波窗口逐列遍历
				 {
					 x2 = -u * sin(g) + v * cos(g);//x坐标旋转
					 y2 = u * cos(g) + v * sin(g);//y坐标旋转
					 sum += exp(-0.5 * (x2 * x2 * dx2 + y2 * y2 * dy2)) * cos(2 * PI * x2 * f) * ucImg[(i - u) + (j - v) * iWidth];//窗口内滤波值累加

				 }
			 }

			 //边界值处理
			 if (sum > 255.0)
			 {
				 sum = 255.0;
			 }
			 if (sum < 0.0)
			 {
				 sum = 0.0;
			 }

			 //得到当前像素的滤波结果
			 ucImgEnhanced[i + j * iWidth] = (unsigned char)sum;
		 }
	 }

	 return 0;
 }
 //3.14.2图像Gabor滤波增强单步测试
//参考函数Step7_GaborEnhance()
//核心代码如下:
#define STEP_TXT_7 "temp\\Step7_GaborEnhance.txt"
#define STEP_IMG_7 "temp\\Step7_GaborEnhance.bmp"
//Gabor滤波增强单步测试
//参数:info(返回操作成功或失败提示信息)
 int Step7_GaborEnhance(char* info)
 {
	 //设置输入输出文件名
	 char srcTxtFile_Img[MAX_PATH] = { STEP_TXT_3 };//图像源数据文件名
	 char srcTxtFile_Dir[MAX_PATH] = { STEP_TXT_4 };//方向源数据文件名
	 char srcTxtFile_Fre[MAX_PATH] = { STEP_TXT_5 };//频率源数据文件名
	 char srcTxtFile_Msk[MAX_PATH] = { STEP_TXT_6 };//掩码源数据文件名
	 char srcImgFile[MAX_PATH] = { STEP_IMG_6 };//源图文件名
	 char dstTxtFile[MAX_PATH] = { STEP_TXT_7 };//结果数据文件名
	 char dstImgFile[MAX_PATH] = { STEP_IMG_7 };//结果图文件名


	 //读取图像参数
	 int iWidth, iHeight, iDepth;
	 ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	 //读取图像源数据
	 unsigned char* image1 = new unsigned char[iWidth * iHeight];
	 ReadDataFromTextFile(srcTxtFile_Img, image1, iWidth, iHeight);

	 //读取方向源数据
	 float* direction = new float[iWidth * iHeight];
	 ReadDataFromTextFile(srcTxtFile_Dir, direction, iWidth, iHeight);

	 //读取频率源数据
	 float* frequency = new float[iWidth * iHeight];
	 ReadDataFromTextFile(srcTxtFile_Fre, frequency, iWidth, iHeight);

	 //读取掩码源数据
	 unsigned char* mask = new unsigned char[iWidth * iHeight];
	 ReadDataFromTextFile(srcTxtFile_Msk, mask, iWidth, iHeight);

	 //Gabor 滤波增强
	 unsigned char* image2 = new unsigned char[iWidth * iHeight];
	 GaborEnhance(image1, direction, frequency, mask, image2, iWidth, iHeight);

	 //保存结果数据(TXT文件)
	 SaveDataToTextFile(dstTxtFile, image2, iWidth, iHeight);

	 //保存结果图像(BMP文件，仅用于显示)
	 SaveDataToImageFile(srcImgFile, dstImgFile, image2);

	 //释放内存空间
	 delete[] image1;
	 delete[] direction;
	 delete[] frequency;
	 delete[] mask;
	 delete[] image2;

	 //返回操作失败状态
	 sprintf(info, "完成Gabor滤波增强.");
	 return 0;
 }


 void CFingerDlg::OnBnClickedBtnStep7()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 //初始化操作结果信息
	 char info[MAX_PATH] = { 0 };

	 //Gabor滤波增强
	 Step7_GaborEnhance(info);

	 //显示操作结果信息
	 m_staticInfo.SetWindowText(ToWideChar(info));

	 //显示图像
	 ShowImageInCtrl(m_picCtrl1, STEP_IMG_6);//显示输入图像
	 ShowImageInCtrl(m_picCtrl2, STEP_IMG_7);//显示输出图像	
 }
 //3.16.1图像二值化

//核心代码如下:

//ucImage(源图数据),ucBinImage(结果图数据),uThreshold(二值化灰度阈值),iWidth(图像宽度),iHeight(图像高度)
 int BinaryImg(unsigned char* ucImage, unsigned char* ucBinImage, int iWidth, int iHeight, unsigned char uThreshold)
 {
	 //分别定义指向源图数据和结果图数据的数据指针
	 unsigned char* pStart = ucImage, * pEnd = ucImage + iWidth * iHeight;
	 unsigned char* pDest = ucBinImage;

	 //逐一遍历所有像素
	 while (pStart < pEnd)
	 {
		 *pDest = (*pStart) > uThreshold ? 1 : 0;//二值化
		 pStart++;//源图数据指针后移
		 pDest++;//结果图数据指针后移
	 }

	 return 0;
 }
 //3.16.2二值图显示转换

//二值图显示转换([0,1] -> [0,255])

//ucBinImg(源图数据),ucGrayImg(结果图数据),iWidth(图像宽度),iHeight(图像高度)
 int BinaryToGray(unsigned char* ucBinImg, unsigned char* ucGrayImg, int iWidth, int iHeight)
 {
	 //分别定义指向源图数据和结果图数据的数据指针
	 unsigned char* pStart = ucBinImg, * pEnd = ucBinImg + iWidth * iHeight;
	 unsigned char* pDest = ucGrayImg;

	 //逐一遍历所有像素
	 while (pStart < pEnd)
	 {
		 *pDest = (*pStart) > 0 ? 255 : 0;//取值转换([0,1] -> [0,255])
		 pStart++;//源图数据指针后移
		 pDest++;//结果图数据指针后移
	 }

	 return 0;
 }
 //3.16.3图像二值化单步测试

//参考函数Step8_Binary()
//核心代码如下:
#define STEP_TXT_8 "temp\\Step8_Binary.txt"
#define STEP_IMG_8 "temp\\Step8_Binary.bmp"
//二值化单步测试
//参数:info(返回操作成功或失败提示信息)
 int Step8_Binary(char* info)
 {
	 //设置输入输出文件名
	 char srcTxtFile[MAX_PATH] = { STEP_TXT_7 };//源数据文件名
	 char srcImgFile[MAX_PATH] = { STEP_IMG_7 };//源图文件名
	 char dstTxtFile[MAX_PATH] = { STEP_TXT_8 };//结果数据文件名
	 char dstImgFile[MAX_PATH] = { STEP_IMG_8 };//结果图文件名


	 //读取图像参数
	 int iWidth, iHeight, iDepth;
	 ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	 //读取源数据
	 unsigned char* image1 = new unsigned char[iWidth * iHeight];
	 ReadDataFromTextFile(srcTxtFile, image1, iWidth, iHeight);

	 //二值化
	 unsigned char* image2 = new unsigned char[iWidth * iHeight];
	 BinaryImg(image1, image2, iWidth, iHeight,128);

	 //保存结果数据(TXT文件)
	 SaveDataToTextFile(dstTxtFile, image2, iWidth, iHeight);

	 //保存结果图像(BMP文件，仅用于显示)
	 BinaryToGray(image2, image1, iWidth, iHeight);//二值化显示转换 
	 SaveDataToImageFile(srcImgFile, dstImgFile, image1);

	 //释放内存空间
	 delete[] image1;
	 delete[] image2;

	 //返回操作失败状态
	 sprintf(info, "完成二值化.");
	 return 0;
 }

 void CFingerDlg::OnBnClickedBtnStep8()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 //初始化操作结果信息
	 char info[MAX_PATH] = { 0 };

	 //图像二值化
	 Step8_Binary(info);

	 //显示操作结果信息
	 m_staticInfo.SetWindowText(ToWideChar(info));

	 //显示图像
	 ShowImageInCtrl(m_picCtrl1, STEP_IMG_7);//显示输入图像
	 ShowImageInCtrl(m_picCtrl2, STEP_IMG_8);//显示输出图像	
 }
 int Thinning(unsigned char* ucBindedImg, unsigned char* ucThinnedImage, int iWidth, int iHeight, int iIterativeLimit)
 {
	 //定义变量
	 unsigned char x1, x2, x3, x4, x5, x6, x7, x8, xp;
	 unsigned char g1, g2, g3, g4;
	 unsigned char b1, b2, b3, b4;
	 unsigned char np1, np2, npm;
	 unsigned char* pUp, * pDown, * pImg;
	 int iDeletedPoints = 0;

	 //结果初始化

	 memcpy(ucThinnedImage, ucBindedImg, iWidth * iHeight);
	 for (int it = 0; it < iIterativeLimit; it++)//重复执行，一直到无点可删位置
	 {
		 iDeletedPoints = 0;//初始化本次迭代所删点数
		 for (int i = 1; i < iHeight - 1; i++)//逐行遍历
		 {
			 //初始化邻域指针
			 pUp = ucBindedImg + (i - 1) * iWidth;
			 pImg = ucBindedImg + i * iWidth;
			 pDown = ucBindedImg + (i + 1) * iWidth;

			 for (int j = 1; j < iWidth - 1; j++)//逐列遍历
			 {
				 //调整邻域指针
				 pUp++;
				 pImg++;
				 pDown++;

				 //跳过背景点
				 if (!*pImg)
				 {
					 continue;

				 }
				 //获取3*3邻域窗口内所有9个像素的灰度值
				 x6 = *(pUp - 1);
				 x5 = *(pImg - 1);
				 x4 = *(pDown - 1);

				 x7 = *pUp;
				 xp = *pImg;
				 x3 = *pDown;

				 x8 = *(pUp + 1);
				 x1 = *(pImg + 1);
				 x2 = *(pDown + 1);

				 //判断条件G1
				 b1 = !x1 && (x2 == 1 || x3 == 1);
				 b2 = !x3 && (x4 == 1 || x5 == 1);
				 b3 = !x5 && (x6 == 1 || x7 == 1);
				 b4 = !x7 && (x8 == 1 || x1 == 1);

				 g1 = (b1 + b2 + b3 + b4) == 1;

				 //判断条件G2
				 np1 = x1 || x2;
				 np1 += x3 || x4;
				 np1 += x5 || x6;
				 np1 += x7 || x8;
				 np2 = x2 || x3;
				 np2 += x4 || x5;
				 np2 += x6 || x7;
				 np2 += x8 || x1;

				 npm = np1 > np2 ? np2 : np1;
				 g2 = npm >= 2 && npm <= 3;

				 //判断条件G3和G4
				 g3 = (x1 && (x2 || x3 || !x8)) == 0;
				 g4 = (x5 && (x6 || x7 || !x4)) == 0;

				 //组合判断(使用条件G1&G2&G3)
				 if (g1 && g2 && g3)
				 {
					 ucThinnedImage[iWidth * i + j] = 0;//删掉当前像素（置为背景点）
					 ++iDeletedPoints;//本次迭代所删点数加1
				 }
			 }//end for(j)列
		 }//end for(i)行

		 //结果同步
		 memcpy(ucBindedImg, ucThinnedImage, iWidth * iHeight);

		 //本次迭代内第二次遍历(使用条件G1&G2&G4)
		 for (int i = 1; i < iHeight - 1; i++)//逐行遍历
		 {
			 //初始化邻域指针
			 pUp = ucBindedImg + (i - 1) * iWidth;
			 pImg = ucBindedImg + i * iWidth;
			 pDown = ucBindedImg + (i + 1) * iWidth;


			 for (int j = 1; j < iWidth - 1; j++)//逐列遍历
			 {
				 //调整邻域指针
				 pUp++;
				 pImg++;
				 pDown++;

				 //跳过背景点
				 if (!*pImg)
				 {
					 continue;

				 }

				 //获取3*3邻域窗口内所有9个像素的灰度值
				 x6 = *(pUp - 1);
				 x5 = *(pImg - 1);
				 x4 = *(pDown - 1);

				 x7 = *pUp;
				 xp = *pImg;
				 x3 = *pDown;

				 x8 = *(pUp + 1);
				 x1 = *(pImg + 1);
				 x2 = *(pDown + 1);

				 //判断条件G1
				 b1 = !x1 && (x2 == 1 || x3 == 1);
				 b2 = !x3 && (x4 == 1 || x5 == 1);
				 b3 = !x5 && (x6 == 1 || x7 == 1);
				 b4 = !x7 && (x8 == 1 || x1 == 1);

				 g1 = (b1 + b2 + b3 + b4) == 1;

				 //判断条件G2
				 np1 = x1 || x2;
				 np1 += x3 || x4;
				 np1 += x5 || x6;
				 np1 += x7 || x8;
				 np2 = x2 || x3;
				 np2 += x4 || x5;
				 np2 += x6 || x7;
				 np2 += x8 || x1;

				 npm = np1 > np2 ? np2 : np1;
				 g2 = npm >= 2 && npm <= 3;

				 //判断条件G3和G4
				 g3 = (x1 && (x2 || x3 || !x8)) == 0;
				 g4 = (x5 && (x6 || x7 || !x4)) == 0;

				 //组合判断(使用条件G1&G2&G4)
				 if (g1 && g2 && g4)
				 {
					 ucThinnedImage[iWidth * i + j] = 0;//删掉当前像素（置为背景点）
					 ++iDeletedPoints;//本次迭代所删点数加1
				 }
			 }//end for(j)列
		 }//end for(i)行

		 //结果同步
		 memcpy(ucBindedImg, ucThinnedImage, iWidth * iHeight);

		 //如果本次迭代已经无点可删，则停止迭代
		 if (iDeletedPoints == 0)
		 {
			 break;
		 }
	 }

	 //清除边缘区段
	 for (int i = 0; i < iHeight; i++)
	 {
		 for (int j = 0; j < iWidth; j++)
		 {
			 if (i < 16)//上边缘
			 {
				 ucThinnedImage[i * iWidth + j] = 0;
			 }
			 else if (i >= iHeight - 16)//下边缘
			 {
				 ucThinnedImage[i * iWidth + j] = 0;
			 }
			 else if (j < 16)//左边缘
			 {
				 ucThinnedImage[i * iWidth + j] = 0;
			 }
			 else if (j >= iWidth - 16)//右边缘
			 {
				 ucThinnedImage[i * iWidth + j] = 0;
			 }
		 }
	 }
	 return 0;
 }
#define STEP_TXT_9 "temp\\Step9_Thinning.txt"
#define STEP_IMG_9 "temp\\Step9_Thinning.bmp"
 //细化单步测试
 //参数：info（返回操作成功或失败提示信息）
 int Step9_Thinning(char* info)
 {
	 //设置输入输出文件名
	 char srcTxtFile[MAX_PATH] = { STEP_TXT_8 };//源数据文件名
	 char srcImgFile[MAX_PATH] = { STEP_IMG_8 };//源图文件名
	 char dstTxtFile[MAX_PATH] = { STEP_TXT_9 };//结果数据文件名
	 char dstImgFile[MAX_PATH] = { STEP_IMG_9 };//结果图文件名

	 //读取图像参数
	 int iWidth, iHeight, iDepth;
	 ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	 //读取源数据
	 unsigned char* image1 = new unsigned char[iWidth * iHeight];
	 ReadDataFromTextFile(srcTxtFile, image1, iWidth, iHeight);


	 unsigned char* image2 = new unsigned char[iWidth * iHeight];
	 Thinning(image1, image2, iWidth, iHeight, 200);

	 //保存结果数据（TXT文件）
	 SaveDataToTextFile(dstTxtFile, image2, iWidth, iHeight);

	 //保存结果图像（BMP文件，仅用于显示）
	 BinaryToGray(image2, image1, iWidth, iHeight); 
	 SaveDataToImageFile(srcImgFile, dstImgFile, image1);

	 //释放内存空间
	 delete[] image1;
	 delete[] image2;

	 //返回操作成败状态
	 sprintf(info, "完成细化.");
	 return 0;
 }

 void CFingerDlg::OnBnClickedBtnStep9()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 char info[MAX_PATH] = { 0 };

	 //图像细化
	 Step9_Thinning(info);

	 //显示操作结果信息
	 m_staticInfo.SetWindowText(ToWideChar(info));

	 //显示图像
	 ShowImageInCtrl(m_picCtrl1, STEP_IMG_8);//显示输入图像
	 ShowImageInCtrl(m_picCtrl2, STEP_IMG_9);//显示输出图像
 }
 //3.18.1指纹特征提取
//参考函数Extract()
//核心代码如下:

//指纹特征提取
//ucThinImg(源图数据),ucMinuImg(结果图数据),iWidth(图像宽度),iHeight(图像高度)
 int Extract(unsigned char* ucThinImg, unsigned char* ucMinuImg, int iWidth, int iHeight)
 {
	 //定义变量
	 unsigned char* pDest = ucMinuImg;//结果图数据指针
	 unsigned char* pUp, * pDown, * pImg;//源图邻域指针
	 unsigned char x1, x2, x3, x4, x5, x6, x7, x8;//八邻点
	 unsigned char nc;//八邻点中黑点数
	 int iMinuCount = 0;//特征点数量

	 //结果初始化(全设为0，表示都是非特征点)
	 memset(pDest, 0, iWidth * iHeight);

	 //遍历源图以提取指纹特征
	 for (int i = 1; i < iHeight - 1; i++)//逐行遍历
	 {
		 //初始化邻域指针
		 pUp = ucThinImg + (i - 1) * iWidth;
		 pImg = ucThinImg + i * iWidth;
		 pDown = ucThinImg + (i + 1) * iWidth;

		 for (int j = 1; j < iWidth - 1; j++)//逐列遍历
		 {
			 //调整邻域指针
			 pUp++;
			 pImg++;
			 pDown++;

			 //跳过背景点(背景点像素值为0)
			 if (!*pImg)
			 {
				 continue;
			 }

			 //获取3*3邻域窗口内所有8个邻点像素的灰度值
			 x6 = *(pUp - 1);
			 x5 = *(pImg - 1);
			 x4 = *(pDown - 1);

			 x7 = *pUp;
			 x3 = *pDown;

			 x8 = *(pUp + 1);
			 x1 = *(pImg + 1);
			 x2 = *(pDown + 1);

			 //统计八邻点中黑点数量
			 nc = x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8;

			 //特征点判断
			 if (nc == 1)//端点
			 {
				 pDest[i * iWidth + j] = 1;//结果图中对应像素点设为1(表示端点)
				 ++iMinuCount;//特征点数量加1
			 }
			 else if (nc == 3)//分叉点
			 {
				 pDest[i * iWidth + j] = 3;//结果图中对应像素点设为3(表示分叉点)
				 ++iMinuCount;//特征点数量加1
			 }
		 }
	 }
	 return iMinuCount;//返回特征点数量
 }
 //3.18.2指纹特征提取单步测试

//参考函数Step10_MinuExtract()
//核心代码如下:
#define STEP_TXT_10 "temp\\Step10_MinuExtract.txt"
#define STEP_IMG_10 "temp\\Step10_MinuExtract.bmp"
//指纹特征提取单步测试
//参数:minuCount(返回特征点数量),info(返回操作成功或失败提示信息)
 int Step10_MinuExtract(int& minuCount, char* info)
 {
	 //结果初始化
	 minuCount = 0;

	 //设置输入输出文件名
	 char srcTxtFile[MAX_PATH] = { STEP_TXT_9 };//源数据文件名
	 char srcImgFile[MAX_PATH] = { STEP_IMG_9 };//源图文件名
	 char dstTxtFile[MAX_PATH] = { STEP_TXT_10 };//结果数据文件名
	 char dstImgFile[MAX_PATH] = { STEP_IMG_10 };//结果图文件名


	 //读取图像参数
	 int iWidth, iHeight, iDepth;
	 ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	 //读取源数据
	 unsigned char* image1 = new unsigned char[iWidth * iHeight];
	 ReadDataFromTextFile(srcTxtFile, image1, iWidth, iHeight);

	 //特征提取
	 unsigned char* image2 = new unsigned char[iWidth * iHeight];
	 minuCount = Extract(image1, image2, iWidth, iHeight);

	 //保存结果数据(TXT文件)
	 SaveDataToTextFile(dstTxtFile, image2, iWidth, iHeight);

	 //保存结果图像(BMP文件，仅用于显示)
	 BinaryToGray(image2, image1, iWidth, iHeight);//二值图显示转换 
	 SaveDataToImageFile(srcImgFile, dstImgFile, image1);

	 //释放内存空间
	 delete[] image1;
	 delete[] image2;

	 //返回操作失败状态
	 sprintf(info, "完成特征提取.");
	 return 0;
 }

 void CFingerDlg::OnBnClickedBtnStep10()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 char info[MAX_PATH] = {0};
	
	//特征提取
	Step10_MinuExtract(m_minuCount,info);
	
	//显示操作结果信息
	m_staticInfo.SetWindowText(ToWideChar(info));
	
	//显示图像
	ShowImageInCtrl(m_picCtrl1,STEP_IMG_9);//显示输入图像
	ShowImageInCtrl(m_picCtrl2,STEP_IMG_10);//显示输出图像	
 }
 struct NEIGHBOR
 {
	 int x;
	 int y;
	 int type;
	 float Theta;
	 float Theta2Ridge;
	 float ThetaThisNibor;
	 int distance;
 };
 struct MINUTIAE
 {
	 int x;
	 int y;
	 int type;
	 float theta;
	 NEIGHBOR* neibors;
 };
 //去除边缘特征点
//minutiaes(特征点数组),count(特征点数量),ucImg(源图数据),iWidth(图像宽度),iHeight(图像高度)
 int CutEdge(MINUTIAE* minutiaes, int count, unsigned char* ucImg, int iWidth, int iHeight)
 {
	 //定义变量
	 int minuCount = count;
	 int x, y, type;
	 bool del;

	 //初始化标记数组
	 int* pFlag = new int[minuCount];//标记数组(标记值:0-保留,1-删除)
	 memset(pFlag, 0, sizeof(int) * minuCount);//初始化(全0全保留)

	 //遍历所有特征点
	 for (int i = 0; i < minuCount; i++)
	 {
		 //获取当前特征点信息
		 y = minutiaes[i].y - 1;//纵坐标（行坐标）
		 x = minutiaes[i].x - 1;//横坐标（列坐标）
		 type = minutiaes[i].type;//特征点类型(1-端点,3-分叉点)

		 //将当前特征点的删除标记初始化为true
		 del = true;

		 //根据当前特征点的位置判断其是否边缘特征点
		 if (x < iWidth / 2)//如果位于图像左半图
		 {
			 if (abs(iWidth / 2 - x) > abs(iHeight / 2 - y))//如果位于图像左半图的左侧
			 {
				 //在特征图中查找当前特征点同一行左侧是否还有其他特征点
				 while (--x >= 0)//逐一左移查找
				 {
					 //如果在左侧存在其他特征点，则说明当前特征点不是边缘特征点

					 if (ucImg[x + y * iWidth] > 0)//特征图像素值(0-非特征点，1-端点，3-分叉点)
					 {
						 del = false;//删除标记置为false
						 break;//停止当前特征点的左移查找过程
					 }
				 }
			 }
			 else//如果位于图像左半图的右侧
			 {
				 if (y > iHeight / 2)//如果位于当前左半图的右下侧
				 {
					 //在特征图中查找当前特征点同一列下侧是否还有其他特征点
					 while (++y < iHeight)//逐一下移查找
					 {
						 //如果在下侧存在其他特征点，则说明当前特征点不是边缘特征点
						 if (ucImg[x + y * iWidth] > 0)//特征图像像素值(0-非特征点，1-端点，3-分叉点)
						 {
							 del = false;//删除标记置为false
							 break;//停止当前特征点的下移查找过程
						 }
					 }
				 }
				 else//如果位于图像左半图的右上侧
				 {

					 //在特征图中查找当前特征点同一列上侧是否还有其他特征点
					 while (--y >= 0)//逐一上移查找
					 {
						 //如果在上侧存在其他特征点，则说明当前特征点不是边缘特征点，就无须删除
						 if (ucImg[x + y * iWidth] > 0)//特征图像像素值(0-非特征点，1-端点，3-分叉点)
						 {
							 del = false;//删除标记置为false
							 break;//停止当前特征点的上移查找过程
						 }
					 }

				 }
			 }
		 }
		 else//如果位于图像右半图
		 {
			 if (abs(iWidth / 2 - x) > abs(iHeight / 2 - y))//如果位于图像左半图的左侧
			 {
				 //在特征图中查找当前特征点同一行左侧是否还有其他特征点
				 while (++x <= 0)//逐一右移查找
				 {
					 //如果在右侧存在其他特征点，则说明当前特征点不是边缘特征点

					 if (ucImg[x + y * iWidth] > 0)//特征图像素值(0-非特征点，1-端点，3-分叉点)
					 {
						 del = false;//删除标记置为false
						 break;//停止当前特征点的左移查找过程
					 }
				 }
			 }
			 else//如果位于图像右半图的左侧
			 {
				 if (y > iHeight / 2)//如果位于当前右半图的左下侧
				 {
					 //在特征图中查找当前特征点同一列下侧是否还有其他特征点
					 while (++y < iHeight)//逐一下移查找
					 {
						 //如果在下侧存在其他特征点，则说明当前特征点不是边缘特征点
						 if (ucImg[x + y * iWidth] > 0)//特征图像像素值(0-非特征点，1-端点，3-分叉点)
						 {
							 del = false;//删除标记置为false
							 break;//停止当前特征点的下移查找过程
						 }
					 }
				 }
				 else//如果位于图像右半图的左上侧
				 {

					 //在特征图中查找当前特征点同一列上侧是否还有其他特征点
					 while (--y >= 0)//逐一上移查找
					 {
						 //如果在上侧存在其他特征点，则说明当前特征点不是边缘特征点，就无须删除
						 if (ucImg[x + y * iWidth] > 0)//特征图像像素值(0-非特征点，1-端点，3-分叉点)
						 {
							 del = false;//删除标记置为false
							 break;//停止当前特征点的上移查找过程
						 }
					 }

				 }
			 }

		 }
		 //如果当前特征点是边缘特征点，则予以删除（在标记数组中将其标记设置为1）
		 if (del)//如果当前特征点的删除标记为true,则表明其为边缘特征点,应予以删除
		 {
			 pFlag[i] = 1;//在标记数组中将其设置为1（表示删除）
			 continue;//继续判断结构数组中的下一个特征点
		 }
	 }

	 //重组特征点结构数组（在当前结构数组中将其所有有效特征点前移）
	 int newCount = 0;//有效特征点数量（同时也是重组后有效特征点数组下标）
	 for (int i = 0; i < minuCount; i++)
	 {
		 if (pFlag[i] == 0)//如果当前特征点需要保留（标记值：0-保留，1-删除）
		 {
			 memcpy(&minutiaes[newCount], &minutiaes[i], sizeof(MINUTIAE));//特征点结构体整体复制
			 newCount++;//有效特征点下标后移(有效特征点数量+1)
		 }
	 }
	 delete[] pFlag;
	 pFlag = NULL;
	 //返回有效特征点数量
	 return newCount;
 }
 //特征过滤
//minuData(特征图数据),thinData(细化图数据),minutiaes(特征点数组),minuCount(特征点数量),iWidth(图像宽度),iHeight(图像高度)
 int MinuFilter(unsigned char* minuData, unsigned char* thinData, MINUTIAE* minutiaes, int& minuCount, int iWidth, int iHeight)
 {
	 //第一步：计算细化图中个点的方向
	 float* dir = new float[iWidth * iHeight];
	 memset(dir, 0, iWidth * iHeight * sizeof(float));

	 ImgDirection(thinData, dir, iWidth, iHeight);//计算脊线方向

	 //第二步：从特征图中提取特征点数据
	 unsigned char* pImg;
	 unsigned char val;
	 int temp = 0;
	 for (int i = 1; i < iHeight - 1; i++)//逐行遍历
	 {
		 pImg = minuData + i * iWidth;
		 for (int j = 1; j < iWidth - 1; j++)//逐列遍历
		 {
			 //获取特征图数据
			 ++pImg;//特征图指针后移
			 val = *pImg;//特征图像素值（0-非特征点，1-端点，3-分叉点）
			 //提取特征点数据
			 if (val > 0)
			 {
				 minutiaes[temp].x = j + 1;//横坐标（从1开始）
				 minutiaes[temp].y = i + 1;//纵坐标（从1开始）
				 minutiaes[temp].theta = dir[i * iWidth + j];//脊线方向
				 minutiaes[temp].type = int(val);//特征点类型(1-端点，3-分叉点)
				 ++temp;//特征点数组指针后移

			 }
		 }
	 }
	 delete[] dir;

	 //第三步：去除边缘特征点
	 minuCount = CutEdge(minutiaes, minuCount, thinData, iWidth, iHeight);

	 //第四步：去除毛刺/小孔/间断等伪特征点

	 //初始化标记数组
	 int* pFlag = new int[minuCount];//标记数组(标记值：0-保留，1-删除)
	 memset(pFlag, 0, sizeof(int) * minuCount);

	 //遍历所有特征点
	 int x1, x2, y1, y2, type1, type2;
	 for (int i = 0; i < minuCount; i++)//特征点1遍历
	 {
		 //获取特征点1的信息
		 x1 = minutiaes[i].x;
		 y1 = minutiaes[i].y;
		 type1 = minutiaes[i].type;//特征点类型(1-端点，3-分叉点)

		 for (int j = i + 1; j < minuCount; j++)//特征点2遍历
		 {
			 //跳过已删特征点
			 if (pFlag[j] == 1)
			 {
				 continue;
			 }

			 //获取特征点2的信息
			 x2 = minutiaes[j].x;
			 y2 = minutiaes[j].y;
			 type2 = minutiaes[j].type; // 特征点类型（1 - 端点，3 - 分叉点）

				 //计算两点间距
			 int r = (int)sqrt(float((y1 - y2) * (y1 - y2) + (x1 - x2) * (x1 - x2)));

			 //删除间距过小的特征点
			 if (r <= 4)//如果间距不大于4则认为间距过小（间距阈值可自定）
			 {
				 if (type1 == type2)//如果两点类型相同
				 {
					 if (type1 == 1)//如果两点都是端点，则认为为"短线或纹线间断"
					 {
						 pFlag[i] = pFlag[j] = 1;//同时删掉两点
					 }
					 else//如果两点都是分叉点，则认定为“小孔”
					 {
						 pFlag[j] = 1;//只删掉点2
					 }
				 }
				 else if (type1 == 1)//如果点1是端点而点2是分叉点，则认为1为“毛刺”
				 {
					 pFlag[i] = 1;//只删掉点1
				 }
				 else//如果点1是分叉点而点2是端点，则认定点2位“毛刺”
				 {
					 pFlag[j] = 1;//只删掉点2
				 }
			 }
		 }
	 }

	 //重组特征点结构数组（在当前结构数组中将所有有效特征点前移）
	 int newCount = 0;//有效特征点数量（同时也是重组后的有效特征点数组下标）
	 for (int i = 0; i < minuCount; i++)
	 {
		 if (pFlag[i] == 0)//如果当前特征点需要保留（标记值：0-保留，1-删除）
		 {
			 memcpy(&minutiaes[newCount], &minutiaes[i], sizeof(MINUTIAE));//特征结构整体复制

			 newCount++;//有效特征点下标后移（有特征点数量加1）
		 }
	 }
	 delete[] pFlag;
	 minuCount = newCount;//保存有效特征点数量

	 //返回结果

	 return 0;

 }
 int SaveMinutiae(MINUTIAE* minutiaes, int count, char* fileName)
 {
	 //打开文件(二进制写入方式)
	 FILE* fp = fopen(fileName, "wb");
	 if (!fp)
	 {
		 return -1;
	 }

	 //将所有特征点的结构体数据整体写入文件
	 const static int TemplateFileFlag = 0x3571027f;
	 fwrite(&TemplateFileFlag, sizeof(int), 1, fp);//写入特征模板文件标记
	 fwrite(&count, sizeof(int), 1, fp);//写入特征点数量

	 for (int i = 0; i < count; i++)//逐一写入所有特征点
	 {
		 fwrite(&(minutiaes[i]), sizeof(MINUTIAE), 1, fp);//将特征点结构整体写入
	 }

	 //关闭文件
	 fclose(fp);
	 return 0;
 }
#define STEP_TXT_11 "temp\\Step11_MinuFilter.txt"
#define STEP_IMG_11 "temp\\Step11_MinuFilter.bmp"
#define STEP_IMG_11_MDL "temp\\Step11_MinuFilter_MDL.mdl"

 //特征过滤单步测试
 //参数：minuCount(特征点数量),info(返回操作成功或失败提示信息)
 int Step11_MinuFilter(int& minuCount, char* info)
 {
	 //参数预检
	 const int MINU_COUNT_THRED = 4;//特征点数量阈值（小于阈值则认为指纹特征无效）
	 if (minuCount < MINU_COUNT_THRED)
	 {
		 sprintf(info, "输入参数无效");
		 return-1;
	 }

	 //设置输入输出文件名
	 char srcTxtFile_Minu[MAX_PATH] = { STEP_TXT_10 };//特征图源数据文件名
	 char srcTxtFile_Thin[MAX_PATH] = { STEP_TXT_9 };//细化图源数据文件名
	 char srcImgFile[MAX_PATH] = { STEP_IMG_10 };//源图文件名
	 char dstTxtFile[MAX_PATH] = { STEP_TXT_11 };//结果数据文件名
	 char dstImgFile[MAX_PATH] = { STEP_IMG_11 };//结果图文件名
	 char dstMdlFile[MAX_PATH] = { STEP_IMG_11_MDL };//结果模板文件名

	 //读取图像参数
	 int iWidth, iHeight, iDepth;
	 ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	 //读取特征图像源数据
	 unsigned char* image1 = new unsigned char[iWidth * iHeight];
	 ReadDataFromTextFile(srcTxtFile_Minu, image1, iWidth, iHeight);

	 //读取细化图源数据
	 unsigned char* thin = new unsigned char[iWidth * iHeight];
	 ReadDataFromTextFile(srcTxtFile_Thin, thin, iWidth, iHeight);

	 //特征过滤
	 MINUTIAE* minutiaes = new MINUTIAE[minuCount];
	 memset(minutiaes, sizeof(MINUTIAE), minuCount);
	 MinuFilter(image1, thin, minutiaes, minuCount, iWidth, iHeight);

	 //保存模板文件
	 SaveMinutiae(minutiaes, minuCount, dstMdlFile);
	 //生成结果图像
	 unsigned char* image2 = new unsigned char[iWidth * iHeight];
	 memset(image2, 0, iWidth * iHeight);
	 for (int i = 0; i < minuCount; i++)
	 {
		 image2[(minutiaes[i].y - 1) * iWidth + (minutiaes[i].x - 1)] = 0xff;
	 }

	 //保存结果数据(TXT文件)
	 SaveDataToTextFile(dstTxtFile, image2, iWidth, iHeight);

	 //保存结果图像（BMP文件，仅用于显示）
	 SaveDataToImageFile(srcImgFile, dstImgFile, image2);

	 //释放空间
	 delete[] image1;
	 delete[] thin;
	 delete[] minutiaes;
	 delete[] image2;

	 //返回操作成败状态
	 sprintf(info, "完成特征过滤.");

	 return 0;

 }
 //第11步：特征过滤

 void CFingerDlg::OnBnClickedBtnStep11()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 char info[MAX_PATH] = { 0 };

	 //特征过滤
	 Step11_MinuFilter(m_minuCount, info);

	 //显示操作结果信息
	 m_staticInfo.SetWindowText(ToWideChar(info));

	 //显示图像
	 ShowImageInCtrl(m_picCtrl1, STEP_IMG_10);//显示输入图像
	 ShowImageInCtrl(m_picCtrl2, STEP_IMG_11);//显示输出图像
 }
 //获得新指纹编号
 int GetNewIndexInDB()
 {
	 //从index 文件中读取最后一条记录的编号
	 int  sNo = 0;
	 char name[MAX_PATH] = { 0 }, srcFile[MAX_PATH] = { 0 }, mdlFile[MAX_PATH] = { 0 };
	 FILE* index = fopen(DB_INDEX_TXT, "r");
	 while (!feof(index))
	 {
		 fscanf(index, "%d %s %s %s\n", &sNo, srcFile, mdlFile, name);
	 }
	 fclose(index);
	 //生成新编号
	 sNo = sNo + 1;

	 //返回新编号
	 return sNo;
 }
 //指纹特征入库单步测试
//参数:userName(登记人姓名),info(返回操作成功或失败提示信息)
 int Step12_Enroll(char* userName, char* info)
 {

	 //设置输入文件名
	 char srcImgFile[MAX_PATH] = { STEP_IMG_1 };//源图文件名
	 char srcMdlFile[MAX_PATH] = { STEP_IMG_11_MDL };//模板文件名

	 //获取数据库内新指纹记录编号
	 int sNo = GetNewIndexInDB();

	 //设置用户名/图源文件名/模板文件名等需要存入数据库的指纹登记信息
	 char regName[MAX_PATH] = { 0 };//登记人姓名
	 char dstImgFile[MAX_PATH] = { 0 };//图源文件
	 char dstMdlFile[MAX_PATH] = { 0 };//源模板文件
	 sprintf(regName, userName);//直接复制界面输入内容
	 sprintf(dstImgFile, "%s%d.bmp", DB_DIR, sNo);//存入数据库文件夹内，已编号命名
	 sprintf(dstMdlFile, "%s%d.mdl", DB_DIR, sNo);//存入数据库文件夹内，已编号命名

	 //保存原始图像
	 CopyFile(ToWideChar(srcImgFile), ToWideChar(dstImgFile), false);

	 //保存模板文件
	 CopyFile(ToWideChar(srcMdlFile), ToWideChar(dstMdlFile), false);

	 //指纹登记信息写入数据库索引文件
	 FILE* index = fopen(DB_INDEX_TXT, "a");//追加模式写入
	 fprintf(index, "%d %s %s %s\n", sNo, dstImgFile, dstMdlFile, regName);
	 fclose(index);

	 //返回操作失败状态
	 sprintf(info, "完成特征入库.");
	 return 0;
 }
 char* ToChar(wchar_t* str)
 {
	 int num = WideCharToMultiByte(0, 0, str, -1, NULL, 0, NULL, false);
	 char* chStr = new char[num];
	 WideCharToMultiByte(0, 0, str, -1, chStr, num, NULL, false);
	 return chStr;
 }



 void CFingerDlg::OnBnClickedBtnStep12a()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 char info[MAX_PATH] = { 0 };

	 //获取界面输入内容
	 UpdateData(true);

	 //特征入库
	 Step12_Enroll(ToChar(m_name.GetBuffer()), info);

	 //显示操作结果信息
	 m_staticInfo.SetWindowText(ToWideChar(info));

	 //显示图像
	 ShowImageInCtrl(m_picCtrl1, STEP_IMG_1);//显示原始指纹图像
	 ShowImageInCtrl(m_picCtrl2, STEP_IMG_11);//显示指纹特征图像	
 }
 //其中，Angle2Points() 用于计算链接两个坐标点的线段倾斜度,定义如下:
//计算线段倾斜度
//x1(端点1横坐标),y1(端点1纵坐标),x2(端点2横坐标),y2(端点2纵坐标)
 float Angle2Points(int x1, int y1, int x2, int y2)
 {
	 const float PI = 3.141592654;
	 float diffY, diffX;
	 float theta = 0.0;
	 diffY = y2 - y1;
	 diffX = x2 - x1;

	 if (diffY < 0 && diffX>0)
	 {
		 theta = atan2(-1 * diffY, diffX);
	 }
	 else if (diffY < 0 && diffX < 0)
	 {
		 theta = PI - atan2(-1 * diffY, -1 * diffX);
	 }
	 else if (diffY > 0 && diffX < 0)
	 {
		 theta = atan2(diffY, -1 * diffX);
	 }
	 else if (diffY > 0 && diffX > 0)
	 {
		 theta = PI - atan2(diffY, diffX);
	 }
	 else if (diffX == 0)
	 {
		 theta = PI / 2;
	 }
	 else
	 {
		 theta = 0.0;
	 }

	 return theta;
 }

 //参考函数BuildNabors(),核心代码如下：

 //构建特征点相邻关系
 //minutiae(特征点数组),minuCount(特征点数量)
 int BuildNabors(MINUTIAE* minutiae, int minuCount)
 {
	 //定义变量
	 const int MAX_NEIGHBOR_EACH = 10;//每个特征点最多保存10个相邻特征点
	 int x1, x2, y1, y2;
	 int* pFlag = new int[minuCount];//相邻标记数组(标记值：0-不相邻,1-相邻)

	 //遍历特征点数组
	 for (int i = 0; i < minuCount; i++)
	 {
		 //获取当前特征点信息
		 x1 = minutiae[i].x;
		 y1 = minutiae[i].y;

		 //初始化当前特征点的相邻标记数组
		 memset(pFlag, 0, sizeof(int) * minuCount);//初始化为全0(不相邻)
		 pFlag[i] = 1;//将自身标记为"相邻"

		 //为当前特征点创建并初始化相邻特征点结构数组
		 minutiae[i].neibors = new NEIGHBOR[MAX_NEIGHBOR_EACH];//创建相邻特征点结构数组
		 if (minutiae[i].neibors == NULL)
		 {
			 return -1;
		 }
		 memset(minutiae[i].neibors, 0, sizeof(NEIGHBOR) * MAX_NEIGHBOR_EACH);//初始化数组

		 //查找和保存10个相邻特征点
		 for (int neighborNo = 0; neighborNo < MAX_NEIGHBOR_EACH; neighborNo++)
		 {
			 //初始化最小间距和对应特征点下标
			 int minDistanse = 1000;//最小间距
			 int minNo = 0;//最小间距对应的特征点下标

			 //查找相邻特征点之外的最近的不相邻特征点
			 for (int j = 0; j < minuCount; j++)//每次都遍历所有特征点
			 {
				 //跳过已找到的相邻特征点
				 if (pFlag[j] == 1)//(标记值：0-不相邻,1-相邻)
				 {
					 continue;
				 }

				 //获取特征点2的信息
				 x2 = minutiae[j].x;
				 y2 = minutiae[j].y;

				 //计算两点间距
				 int r = (int)sqrt(float((y1 - y2) * (y1 - y2) + (x1 - x2) * (x1 - x2)));

				 //查找最小间距
				 if (r < minDistanse)
				 {
					 minNo = j;
					 minDistanse = r;
				 }
			 }

			 //保存查找结果
			 pFlag[minNo] = 1;//将找到的最近的不相邻特征点的标记设置为"相邻"
			 minutiae[i].neibors[neighborNo].x = minutiae[minNo].x;//横坐标(列坐标)
			 minutiae[i].neibors[neighborNo].y = minutiae[minNo].y;//纵坐标(行坐标)
			 minutiae[i].neibors[neighborNo].type = minutiae[minNo].type;//特征点类型(1-端点,3-分叉点)

			 minutiae[i].neibors[neighborNo].Theta = Angle2Points(minutiae[minNo].x, minutiae[minNo].y, x1, y1);//两点连线角度(弧度)
			 minutiae[i].neibors[neighborNo].Theta2Ridge = minutiae[minNo].theta - minutiae[i].theta;//两点脊线方向夹角(弧度)
			 minutiae[i].neibors[neighborNo].ThetaThisNibor = minutiae[minNo].theta;//相邻特征点的脊线方向(弧度)
			 minutiae[i].neibors[neighborNo].distance = minDistanse;//两点距离		
		 }
	 }

	 delete[] pFlag;

	 return 0;
 }
 //指纹特征匹配相似度计算
//iwidth(图像宽度),iHeight(图像高度),minutiae1(指纹一的特征点数组),account1(指纹一的特征点数量),minutiae2(指纹二的特征点数组),account2(指纹二的特征点数量)
 float MinuSimilarity(int iwidth, int iHeight, MINUTIAE* minutiae1, int count1, MINUTIAE* minutiae2, int count2)
 {
	 const int MAX_SIMILAR_PAIR = 100;//最多保存100对配对相似特征点
	 const int MAX_NEIGHBOR_EACH = 10;//每个特征点最多保存10对相邻特征点

	 //构建特征点相邻关系
	 BuildNabors(minutiae1, count1);
	 BuildNabors(minutiae2, count2);

	 //初始化特征点匹配结果(相似特征点点对)
	 int simiarPair[MAX_SIMILAR_PAIR][2];
	 memset(simiarPair, 0, 100 * 2 * sizeof(int));

	 //选择基准特征和参考特征
	 MINUTIAE* baseMinutiae;//基准特征点数组(含参考点较少)
	 MINUTIAE* refMinutiae;//参考特征点数组(含参考点较多)
	 int baseAccount, refAccount;
	 if (count1 < count2)
	 {
		 baseMinutiae = minutiae1;
		 baseAccount = count1;
		 refMinutiae = minutiae2;
		 refAccount = count2;
	 }
	 else
	 {
		 baseMinutiae = minutiae2;
		 baseAccount = count2;
		 refMinutiae = minutiae1;
		 refAccount = count1;
	 }

	 //方便起见，下文中“特征点”均简称为“点”，例如“基准点/参考点/相邻点/相似点”
	 NEIGHBOR* baseNeighbors = NULL;//基准点的相邻点指针
	 NEIGHBOR* refNeighbors = NULL;//参考点点的相邻点指针
	 int similarMinutiae = 0;//相邻点对数量(数组下表)
	 float baseTheta, refTheta;//基准方向和参考方向

	 //特征点匹配(在参考点数组中寻找与每一个基准点对应的最相似的参考点)
	 for (int i = 0; i < baseAccount; i++)//逐一遍历基准点数组
	 {
		 //获取当前基准点信息
		 baseNeighbors = baseMinutiae[i].neibors;//基准点的相邻点数组
		 baseTheta = baseMinutiae[i].theta;//基准点的脊线方向

		 //在参考点数组中寻找与当前基准点对应的最相似的参考点
		 int refSimilarNo = 0;
		 int maxSimilarNeibors = 0;
		 for (int j = 0; j < refAccount; j++)
		 {
			 //跳过与当前基准点类型不同的参考点
			 if (refMinutiae[j].type != baseMinutiae[i].type)
			 {
				 continue;
			 }
			 //获取当前参考点信息
			 refNeighbors = refMinutiae[i].neibors;//参考点的相邻点数组
			 refTheta = refMinutiae[i].theta;//参考点的脊线方向

			 //统计相似相邻点数量
			 int thisSimilarNeigbors = 0;//相似相邻点数量初始化为0
			 for (int m = 0; m < MAX_NEIGHBOR_EACH; m++)//逐一遍历当前基准点的相邻点数组
			 {
				 //在当前参考点的相邻数组中查找与当前基准点的当前相邻点相似的相邻点
				 for (int n = 0; n < MAX_NEIGHBOR_EACH; n++)//逐一遍历当前参考点的相邻点数组
				 {
					 //跳过与当前基准点类型不同的参考点
					 if (baseMinutiae[m].type != refMinutiae[m].type)
					 {
						 continue;
					 }
					 //计算两个相邻点之间的距离差和不同角度差(弧度)
					 int dist = abs(int(baseNeighbors[m].distance - refNeighbors[n].distance));
					 float theta1 = abs(float((baseNeighbors[m].Theta - baseTheta) - (refNeighbors[n].Theta - refTheta)));
					 float theta2 = abs(float(baseNeighbors[m].Theta2Ridge - refNeighbors[n].Theta2Ridge));
					 float theta3 = abs(float((baseNeighbors[m].Theta - baseNeighbors[m].ThetaThisNibor) - (refNeighbors[n].Theta - refNeighbors[n].ThetaThisNibor)));

					 //如果距离差小于指定阈值(此处取4)并且角度差均小于指定阈值(此处取0.15),则认为这两个相邻点相似
					 if (dist < 4 && theta1 < 0.15f && theta2 < 0.15f && theta3 < 0.15f)
					 {
						 ++thisSimilarNeigbors;//相似相邻点数量加1
						 break;//转向当前基准点的下一个相邻点继续查找

					 }
				 }
			 }

			 //如果3对以上相邻点相似，则认为当前基准点与当前参考点相似，保存匹配结果
			 if ((thisSimilarNeigbors >= MAX_NEIGHBOR_EACH * 3 / 10) && (similarMinutiae < MAX_SIMILAR_PAIR))
			 {
				 simiarPair[similarMinutiae][0] = i;//保存当前基准点下标
				 simiarPair[similarMinutiae][1] = refSimilarNo;//保存当前参考点下标
				 ++similarMinutiae;//相似点对数量加1(数组下标后移)
			 }
		 }
	 }

	 //计算特征匹配相似度
	 float similarity = similarMinutiae / 8.0f;
	 //如果相似特征点数量小于指定下限阈值(此处取2)，则认为两个指纹完全不匹配
	 similarity = similarMinutiae < 2 ? 0.0f : similarity;//边界值处理
	 //如果相似特征点数量大于指定上限阈值(此处取8)，则认为两个指纹完全匹配
	 similarity = similarMinutiae > 8 ? 1.0f : similarity;//边界值处理

	 //返回特征匹配相似度
	 return similarity;

 }
 //读取特征模板文件
 int ReadMinutiae(char* fileName, MINUTIAE** minutiae)
 {
	 //打开文件(二进制读取方式)
	 FILE* fp = fopen(fileName, "rb");
	 if (!fp)
	 {
		 return -1;
	 }

	 //逐一整体读取所有特征点的结构体数据
	 const static int TemplateFileFlag = 0x3571027f;
	 int flag;
	 fread(&flag, sizeof(int), 1, fp);//读取特征模板文件标记
	 if (flag != TemplateFileFlag)
	 {
		 return -2;
	 }

	 int account;
	 fread(&account, sizeof(int), 1, fp);//读取特征点数量

	 *minutiae = new MINUTIAE[account];//创建特征点结构数组
	 if (*minutiae == NULL)
	 {
		 return -3;
	 }

	 for (int i = 0; i < account; i++)//逐一遍历所有特征点
	 {
		 fread(&((*minutiae)[i]), sizeof(MINUTIAE), 1, fp);//读取整体特征点结构
	 }

	 //关闭文件
	 fclose(fp);
	 return account;
 }
#define STEP_IMG_12 "temp\\Step12_Result.bmp"
 //指纹特征匹配单步测试
 //参数:info(返回操作成功或失败提示信息)
 int Step12_Match(char* info)
 {

	 //设置输入文件名
	 char srcImgFile[MAX_PATH] = { STEP_IMG_1 };//源图文件名
	 char srcMdlFile[MAX_PATH] = { STEP_IMG_11_MDL };//模板文件名
	 char dstImgFile[MAX_PATH] = { STEP_IMG_12 };//结果图文件名

	 //读取图像参数
	 int iWidth, iHeight, iDepth;
	 ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	 //读取指纹特征数据
	 MINUTIAE* minu = NULL;
	 int minuAccount = ReadMinutiae(srcMdlFile, &minu);

	 //特征匹配(此处仅测试自身与自身匹配)
	 float similar = MinuSimilarity(iWidth, iHeight, minu, minuAccount, minu, minuAccount);

	 //释放空间
	 delete[] minu;

	 //保存匹配结果图像
	 CopyFile(ToWideChar(srcImgFile), ToWideChar(dstImgFile), false);//复制文件


	 //返回操作失败状态
	 const float SIMILAR_THRED = 0.1;//匹配相似度阈值(小于阈值则认为不匹配);
	 if (similar < SIMILAR_THRED)
	 {
		 sprintf(info, "匹配失败.");
		 return -3;
	 }
	 sprintf(info, "匹配成功:相似度[%.2f].", similar);
	 return 0;
 }
 //指纹库预检(判断指纹库是否为空)
bool EmptyDB()
{
	//打开文件
	char indexFile[MAX_PATH]={ DB_INDEX_TXT }; //指纹库索引文件
	FILE* pFile = fopen(indexFile, "r");
	if (!pFile)//若索引文件不存在或存在异常，则指纹库为空
	{
		return true;
	}
	//判断指纹库是否为空(索引文件内容是否为空)
	int size = filelength(fileno(pFile));
	bool bEmpty=(size < 1)?true : false;//若索引文件内容为空，则指纹库为空
	//关闭文件
	fclose(pFile);
	//返回判断结果
	return bEmpty;
}
//参数:info(返回操作成功或失败提示信息)
int Step12_Identify(char* info)
{
	//设置输入输出文件名
	char srcImgFile[MAX_PATH]={ STEP_IMG_1 };//源图文件名
	char srcMDlFile[MAX_PATH]={ STEP_IMG_11_MDL };//源图指纹特征文件名
	char dstImgFile[MAX_PATH]={ STEP_IMG_12 };//结果图文件名
	char dstMDlFile[MAX_PATH]={ 0 };//结果图指纹特征文件名
	//指纹库预检(判断指纹库是否为空)
	EmptyDB();
	//读取图像参数
	int iWidth, iHeight, iDepth;
	ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);
	//读取指纹特征数据
	MINUTIAE* minu1 = NULL, * minu2 = NULL;
	int minuAccount1 = 0, minuAccount2 = 0;
	minuAccount1 = ReadMinutiae(srcMDlFile, &minu1);
	//特征识别
	int id = 0;
	char name[MAX_PATH] = { 0 }, src[MAX_PATH] = { 0 }, mdl[MAX_PATH] = { 0 };
	float maxSimilar = -1;
	int maxID = -1;
	char maxName[MAX_PATH] = { 0 }, maxSrc[MAX_PATH] = { 0 };
	FILE* index = fopen(DB_INDEX_TXT, "r");
	while (!feof(index))
	{
		//读取指纹记录信息(指纹编号指纹图像文件名特征模板文件名登记人姓名)
		fscanf(index, "%d %s %s %s\n", &id, src, mdl, name);
		sprintf(dstMDlFile,mdl);
		//从特征模板文件中读取指纹特征
		minuAccount2 = ReadMinutiae(dstMDlFile, &minu2);
		//特征匹配
		float similar = MinuSimilarity(iWidth, iHeight, minu1, minuAccount1, minu2, minuAccount2);
		//保存相似度最高的指纹记录信息
		if (similar > maxSimilar)
		{
			maxSimilar = similar;
			maxID = id;
			sprintf(maxName, name);
			sprintf(maxSrc, src);
		}
	}
	fclose(index);//关闭文件

	//释放内存空间
	delete[] minu1;
	delete[] minu2;

	//返回识别结果
	const float SIMLAR_THRED = 0.1;
	if (maxID < 0 || maxSimilar < SIMLAR_THRED)
	{
		sprintf(info, "识别失败");
		return -2;
	}

	CopyFile(ToWideChar(maxSrc), ToWideChar(dstImgFile), false);
	sprintf(info, "识别成功.识别结果：姓名[%s],目标指纹[%s],相似度[%2f].",
		maxName, maxSrc, maxSimilar);
	return 0;
}


void CFingerDlg::OnBnClickedBtnStep12b()
{
	// TODO: 在此添加控件通知处理程序代码
	char info[MAX_PATH] = { 0 };

	//特征提取
	//Step12_Match(info);
	Step12_Identify(info);//指纹识别

	//显示操作结果信息
	m_staticInfo.SetWindowText(ToWideChar(info));

	//显示图像
	ShowImageInCtrl(m_picCtrl1, STEP_IMG_1);//显示原始指纹图像
	ShowImageInCtrl(m_picCtrl2, STEP_IMG_12);//显示指纹特征图像	
}
int Enroll(char* regName, char* srcImgFile, char* outImgFile, char* info, bool bSaveImg)
{
	//第0步：设置中间结果文件名
	//中间结果数据文件名
	char stepTxtFile1[MAX_PATH] = { STEP_TXT_1 };
	char stepTxtFile2[MAX_PATH] = { STEP_TXT_2 };
	char stepTxtFile3[MAX_PATH] = { STEP_TXT_3 };
	char stepTxtFile4[MAX_PATH] = { STEP_TXT_4 };
	char stepTxtFile5[MAX_PATH] = { STEP_TXT_5 };
	char stepTxtFile6[MAX_PATH] = { STEP_TXT_6 };
	char stepTxtFile7[MAX_PATH] = { STEP_TXT_7 };
	char stepTxtFile8[MAX_PATH] = { STEP_TXT_8 };
	char stepTxtFile9[MAX_PATH] = { STEP_TXT_9 };
	char stepTxtFile10[MAX_PATH] = { STEP_TXT_10 };
	char stepTxtFile11[MAX_PATH] = { STEP_TXT_11 };

	//中间结果图像文件名
	char stepImgFile1[MAX_PATH] = { STEP_IMG_1 };
	char stepImgFile2[MAX_PATH] = { STEP_IMG_2 };
	char stepImgFile3[MAX_PATH] = { STEP_IMG_3 };
	char stepImgFile4[MAX_PATH] = { STEP_IMG_4 };
	char stepImgFile5[MAX_PATH] = { STEP_IMG_5 };
	char stepImgFile6[MAX_PATH] = { STEP_IMG_6 };
	char stepImgFile7[MAX_PATH] = { STEP_IMG_7 };
	char stepImgFile8[MAX_PATH] = { STEP_IMG_8 };
	char stepImgFile9[MAX_PATH] = { STEP_IMG_9 };
	char stepImgFile10[MAX_PATH] = { STEP_IMG_10 };
	char stepImgFile11[MAX_PATH] = { STEP_IMG_11 };
	char stepImgFile12[MAX_PATH] = { STEP_IMG_12 };

	//指纹特征模板文件名
	char stepMdlFile11[MAX_PATH] = { STEP_IMG_11_MDL };

	//第一步：图像载入，读取源图数据和参数信息
	int iWidth, iHeight, iDepth;
	ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);//读取源图参数信息
	unsigned char* image1 = new unsigned char[iWidth * iHeight];
	unsigned char* image2 = new unsigned char[iWidth * iHeight];
	ReadBMPImgFileData(srcImgFile, image1);//读取源图数据
	memset(image2, 0, iWidth * iHeight);//结果图初始化
	if (bSaveImg)
	{
		CopyFile(ToWideChar(srcImgFile), ToWideChar(stepImgFile1), false);
		SaveDataToTextFile(stepTxtFile1, image1, iWidth, iHeight);
	}
	//第二步：中值滤波
	MidFilter(image1, image2, iWidth, iHeight);//中值滤波
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile2, image2);
		SaveDataToTextFile(stepTxtFile2, image2, iWidth, iHeight);
	}

	//第三步：直方图均衡化
	memcpy(image1, image2, sizeof(unsigned char) * iWidth * iHeight);//以第二步滤波结果为源数据
	HistoNormalize(image1, image2, iWidth, iHeight);//直方图均衡化
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile3, image2);
		SaveDataToTextFile(stepTxtFile3, image2, iWidth, iHeight);
	}

	//第四步：方向计算
	memcpy(image1, image2, sizeof(unsigned char) * iWidth * iHeight);//以第三步均衡化结果为源图数据
	float* tmpDirection = new float[iWidth * iHeight];
	float* direction = new float[iWidth * iHeight];
	ImgDirection(image1, tmpDirection, iWidth, iHeight);//方向计算
	DircLowPass(tmpDirection, direction, iWidth, iHeight);//方向低通滤波
	if (bSaveImg)
	{
		const int DIRECTION_SCALE = 100;//方向结果转换比例(仅用于结果显示)
		SaveDataToImageFile(srcImgFile, stepImgFile4, direction,DIRECTION_SCALE);
		SaveDataToTextFile(stepTxtFile4, direction, iWidth, iHeight);
	}
	delete[] tmpDirection;

	//第五步：频率计算
	memcpy(image1, image2, sizeof(unsigned char) * iWidth * iHeight);//以第三步均衡化结果为源图数据
	float* frequency = new float[iWidth * iHeight];
	Frequency(image1, direction, frequency, iWidth, iHeight);//频率计算
	if (bSaveImg)
	{
		const int FREQUENCY_SCALE = 1000;//频率结果转化比例(仅用于结果显示)
		SaveDataToImageFile(srcImgFile, stepImgFile5, frequency,FREQUENCY_SCALE);
		SaveDataToTextFile(stepTxtFile5, frequency, iWidth, iHeight);
	}

	//第六步：掩码计算
	memcpy(image1, image2, sizeof(unsigned char) * iWidth*iHeight);//以第三步均衡化结果为源图数据
	unsigned char* mask = new unsigned char[iWidth * iHeight];
	GetMask(image1, direction, frequency, mask, iWidth, iHeight);//频率计算
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile6, mask);
		SaveDataToTextFile(stepTxtFile6, mask, iWidth, iHeight);
	}

	//第七步：Gabor滤波增强
	memcpy(image1, image2, sizeof(unsigned char) * iWidth * iHeight);//以第三步均衡化结果为源图数据

	GaborEnhance(image1, direction, frequency, mask, image2, iWidth, iHeight);//增强
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile7, image2);
		SaveDataToTextFile(stepTxtFile7, image2, iWidth, iHeight);
	}

	delete[] direction;
	delete[] frequency;
	delete[] mask;

	//第八步：二值化
	memcpy(image1, image2, sizeof(unsigned char) * iWidth * iHeight);//以第7步Gabor滤波结果为源图数据

	BinaryImg(image1, image2, iWidth, iHeight, 128);//二值化
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile8, image2);
		SaveDataToTextFile(stepTxtFile8, image2, iWidth, iHeight);
	}

	//第九步：细化
	memcpy(image1, image2, sizeof(unsigned char) * iWidth * iHeight);//以第8步二值化结果为源图数据

	Thinning(image1, image2, iWidth, iHeight, 128);//二值化
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile9, image2);
		SaveDataToTextFile(stepTxtFile9, image2, iWidth, iHeight);
	}
	unsigned char* thin = new unsigned char[iWidth * iHeight];
	memcpy(thin, image2, sizeof(unsigned char) * iWidth * iHeight);//保存细化结果

	//第十步：特征提取
	memcpy(image1, image2, sizeof(unsigned char) * iWidth * iHeight);//以第9步细化结果为源图数据

	int minuCount = Extract(image1, image2, iWidth, iHeight);//特征提取
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile10, image2);
		SaveDataToTextFile(stepTxtFile10, image2, iWidth, iHeight);
	}

	//第十一步:特征过滤
	memcpy(image1, image2, sizeof(unsigned char) * iWidth * iHeight);//以第10步特征提取结果为源图数据
	MINUTIAE* minutiaes = new MINUTIAE[minuCount];
	memset(minutiaes, sizeof(MINUTIAE), minuCount);
	MinuFilter(image1, thin, minutiaes, minuCount, iWidth, iHeight);//特征过滤
	delete[] thin;
	SaveMinutiae(minutiaes, minuCount, stepMdlFile11);//保存特征数据文件
	if (true)//bSaveImg)
	{
		memset(image2, 0, iWidth * iHeight);
		for (int i = 0; i < minuCount; i++)
		{
			image2[(minutiaes[i].y - 1) * iWidth + (minutiaes[i].x - 1)] = 0xff;
		}
		SaveDataToImageFile(srcImgFile, stepImgFile11, image2);
		SaveDataToTextFile(stepTxtFile11, image2, iWidth, iHeight);
	}
	strcpy(outImgFile, stepImgFile11);//保存特征结果图文件名

	//第十二步：特征入库
	int sNo = GetNewIndexInDB();//获得数据库新指纹记录编号
	char dbImgFile[MAX_PATH] = { 0 };//数据库中源图文件名
	char dbMdlFile[MAX_PATH] = { 0 };//数据库中模板文件名
	sprintf(dbImgFile, "%s%d.bmp", DB_DIR, sNo);//设置源图文件名（存入数据库文件夹内，以编号命名）
	sprintf(dbMdlFile, "%s%d.mdl", DB_DIR, sNo);//设置模板文件名（存入数据库文件夹内，以编号命名）

	if (minuCount > 0)
	{
		//保存原始指纹图像
		CopyFile(ToWideChar(srcImgFile), ToWideChar(dbImgFile), false);

		//保存指纹特征模板文件
		CopyFile(ToWideChar(stepMdlFile11), ToWideChar(dbMdlFile), false);

		//指纹登记信息写入数据库索引文件
		FILE* index = fopen(DB_INDEX_TXT, "a");//追加模式写入
		fprintf(index, "%d %s %s %s\n", sNo, dbImgFile, dbMdlFile, regName);
		fclose(index);
	}
	//释放空间
	delete[] image1;
	delete[] image2;
	delete[] minutiaes;

	sprintf(info, "指纹登记成功.");
	return 0;
}

void CFingerDlg::OnBnClickedBtnRegister()
{
	// TODO: 在此添加控件通知处理程序代码
	char info[MAX_PATH] = { 0 };

	//获得界面输入内容(登记人姓名)
	UpdateData(true);

	//实时采集图像
	char srcImgFile[MAX_PATH] = "capt.bmp";
	m_zkfpEng.SaveBitmap(ToWideChar(srcImgFile));

	//指纹登记
	bool bSaveImg = false;//是否保存中间结果图像
	char outImgFile[MAX_PATH] = { 0 };//指纹特征结果图像文件名
	Enroll(ToChar(m_name.GetBuffer()), srcImgFile, outImgFile, info, bSaveImg);//指纹登记

	//显示操作结果信息
	m_staticInfo.SetWindowText(ToWideChar(info));

	//显示图像
	ShowImageInCtrl(m_picCtrl1, srcImgFile);//显示原始指纹图像
	ShowImageInCtrl(m_picCtrl2, outImgFile);//显示指纹特征图像
}


void CFingerDlg::OnBnClickedBtnAddImage2()
{
	// TODO: 在此添加控件通知处理程序代码
	char info[MAX_PATH] = { 0 };

	//获得界面输入内容（登记人姓名）
	UpdateData(true);

	//选择图像
	char srcImgFile[MAX_PATH] = "capt.bmp";

	//指纹登记
	bool bSaveImg = false;//是否保存中间结果图像
	char outImgFile[MAX_PATH] = { 0 };//指纹特征结果图像文件名
	Enroll(ToChar(m_name.GetBuffer()), srcImgFile, outImgFile, info, bSaveImg);//指纹登记

	//显示操作结果信息
	m_staticInfo.SetWindowText(ToWideChar(info));

	//显示图像
	ShowImageInCtrl(m_picCtrl1, srcImgFile);//显示原始指纹图像
	ShowImageInCtrl(m_picCtrl2, outImgFile);//显示指纹特征图像
}
int Identify(char* srcImgFile, char* outImgFile, char* info, bool bSaveImg)
{
	//第0步:设置中间结果文件名
	//中间结果数据文件名
	char stepTxtFile1[MAX_PATH] = { STEP_TXT_1 };
	char stepTxtFile2[MAX_PATH] = { STEP_TXT_2 };
	char stepTxtFile3[MAX_PATH] = { STEP_TXT_3 };
	char stepTxtFile4[MAX_PATH] = { STEP_TXT_4 };
	char stepTxtFile5[MAX_PATH] = { STEP_TXT_5 };
	char stepTxtFile6[MAX_PATH] = { STEP_TXT_6 };
	char stepTxtFile7[MAX_PATH] = { STEP_TXT_7 };
	char stepTxtFile8[MAX_PATH] = { STEP_TXT_8 };
	char stepTxtFile9[MAX_PATH] = { STEP_TXT_9 };
	char stepTxtFile10[MAX_PATH] = { STEP_TXT_10 };
	char stepTxtFile11[MAX_PATH] = { STEP_TXT_11 };

	//中间结果图像文件名
	char stepImgFile1[MAX_PATH] = { STEP_IMG_1 };
	char stepImgFile2[MAX_PATH] = { STEP_IMG_2 };
	char stepImgFile3[MAX_PATH] = { STEP_IMG_3 };
	char stepImgFile4[MAX_PATH] = { STEP_IMG_4 };
	char stepImgFile5[MAX_PATH] = { STEP_IMG_5 };
	char stepImgFile6[MAX_PATH] = { STEP_IMG_6 };
	char stepImgFile7[MAX_PATH] = { STEP_IMG_7 };
	char stepImgFile8[MAX_PATH] = { STEP_IMG_8 };
	char stepImgFile9[MAX_PATH] = { STEP_IMG_9 };
	char stepImgFile10[MAX_PATH] = { STEP_IMG_10 };
	char stepImgFile11[MAX_PATH] = { STEP_IMG_11 };
	char stepImgFile12[MAX_PATH] = { STEP_IMG_12 };

	//指纹特征模板文件名
	char stepMdlFile11[MAX_PATH] = { STEP_IMG_11_MDL };

	//指纹库预检(判断指纹库是否为空)
	EmptyDB();

	//第1步:图像载入,读取源图数据和参数信息
	int iWidth, iHeight, iDepth;
	ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);//读取源图参数信息
	unsigned char* image1 = new unsigned char[iWidth * iHeight];
	unsigned char* image2 = new unsigned char[iWidth * iHeight];
	ReadBMPImgFileData(srcImgFile, image1);//读取源图数据
	memset(image2, 0, iWidth * iHeight);//结果图初始化
	if (bSaveImg)
	{
		CopyFile(ToWideChar(srcImgFile), ToWideChar(stepImgFile1), false);
		SaveDataToTextFile(stepTxtFile1, image1, iWidth, iHeight);
	}

	//第2步:中值滤波
	MidFilter(image1, image2, iWidth, iHeight);//中值滤波
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile2, image2);
		SaveDataToTextFile(stepTxtFile2, image2, iWidth, iHeight);
	}

	//第3步:直方图均衡化
	memcpy(image1, image2, sizeof(unsigned char) * iWidth * iHeight);//以第2步滤波结果为源图数据
	HistoNormalize(image1, image2, iWidth, iHeight);//直方图均衡化
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile3, image2);
		SaveDataToTextFile(stepTxtFile3, image2, iWidth, iHeight);
	}

	//第4步:方向计算
	memcpy(image1, image2, sizeof(unsigned char) * iWidth * iHeight);//以第3步均衡化结果为源图数据
	float* tmpDirection = new float[iWidth * iHeight];
	float* direction = new float[iWidth * iHeight];
	ImgDirection(image1, tmpDirection, iWidth, iHeight);//方向计算
	DircLowPass(tmpDirection, direction, iWidth, iHeight);//方向低通滤波
	if (bSaveImg)
	{
		const int DIRECTION_SCALE = 100;//方向结果转换比例(仅用于显示)
		SaveDataToImageFile(srcImgFile, stepImgFile4, direction, DIRECTION_SCALE);
		SaveDataToTextFile(stepTxtFile4, direction, iWidth, iHeight);
	}
	delete[] tmpDirection;

	//第5步:频率计算
	memcpy(image1, image2, sizeof(unsigned char) * iWidth * iHeight);//以第3步均衡化结果为源图数据
	float* frequency = new float[iWidth * iHeight];
	Frequency(image1, direction, frequency, iWidth, iHeight);//频率计算
	if (bSaveImg)
	{
		const int FREQUENCY_SCALE = 1000;//频率结果转换比例(仅用于显示)
		SaveDataToImageFile(srcImgFile, stepImgFile5, frequency, FREQUENCY_SCALE);
		SaveDataToTextFile(stepTxtFile5, frequency, iWidth, iHeight);
	}

	//第6步:掩码计算
	memcpy(image1, image2, sizeof(unsigned char) * iWidth * iHeight);//以第3步均衡化结果为源图数据
	unsigned char* mask = new unsigned char[iWidth * iHeight];
	GetMask(image1, direction, frequency, mask, iWidth, iHeight);//掩码计算
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile6, mask);
		SaveDataToTextFile(stepTxtFile6, mask, iWidth, iHeight);
	}

	//第7步:Gabor滤波增强
	memcpy(image1, image2, sizeof(unsigned char) * iWidth * iHeight);//以第3步均衡化结果为源图数据
	GaborEnhance(image1, direction, frequency, mask, image2, iWidth, iHeight);//增强
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile7, image2);
		SaveDataToTextFile(stepTxtFile7, image2, iWidth, iHeight);
	}
	delete[] direction;
	delete[] frequency;
	delete[] mask;

	//第8步:二值化
	memcpy(image1, image2, sizeof(unsigned char) * iWidth * iHeight);//以第7步Gabor滤波增强结果为源图数据
	BinaryImg(image1, image2, iWidth, iHeight, 128);//二值化
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile8, image2);
		SaveDataToTextFile(stepTxtFile8, image2, iWidth, iHeight);
	}

	//第9步:细化
	memcpy(image1, image2, sizeof(unsigned char) * iWidth * iHeight);//以第8步二值化结果为源图数据
	Thinning(image1, image2, iWidth, iHeight, 128);//细化
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile9, image2);
		SaveDataToTextFile(stepTxtFile9, image2, iWidth, iHeight);
	}
	unsigned char* thin = new unsigned char[iWidth * iHeight];
	memcpy(thin, image2, sizeof(unsigned char) * iWidth * iHeight);//保存细化结果

	//第10步:特征提取
	memcpy(image1, image2, sizeof(unsigned char) * iWidth * iHeight);//以第9步细化结果为源图数据
	int minuCount = Extract(image1, image2, iWidth, iHeight);//特征提取
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile10, image2);
		SaveDataToTextFile(stepTxtFile10, image2, iWidth, iHeight);
	}

	//第11步:特征过滤
	memcpy(image1, image2, sizeof(unsigned char) * iWidth * iHeight);//以第10步特征提取结果为源图数据
	MINUTIAE* minutiaes = new MINUTIAE[minuCount];
	memset(minutiaes, sizeof(MINUTIAE), minuCount);
	MinuFilter(image1, thin, minutiaes, minuCount, iWidth, iHeight);//特征过滤
	delete[] thin;
	SaveMinutiae(minutiaes, minuCount, stepMdlFile11);//保存特征数据文件
	if (bSaveImg)
	{
		memset(image2, 0, iWidth * iHeight);
		for (int i = 0; i < minuCount; i++)
		{
			image2[(minutiaes[i].y - 1) + (minutiaes[i].x - 1)] = 0xff;
		}
		SaveDataToImageFile(srcImgFile, stepImgFile11, image2);
		SaveDataToTextFile(stepTxtFile11, image2, iWidth, iHeight);
	}
	delete[] image1;
	delete[] image2;

	//第12步:特征识别
	int id = 0;
	char name[MAX_PATH] = { 0 }, src[MAX_PATH] = { 0 }, mdl[MAX_PATH] = { 0 };
	float maxSimilar = -1;
	int maxID = -1;
	char maxName[MAX_PATH] = { 0 }, maxSrc[MAX_PATH] = { 0 };
	MINUTIAE* minutiaes2 = NULL;
	int minuCount2 = 0;

	FILE* index = fopen(DB_INDEX_TXT, "r");//打开指纹库索引文件

	while (!feof(index))
	{
		//读取指纹记录信息(指纹编号/指纹图像文件名/指纹特征模板文件名/登记人姓名)
		fscanf(index, "%d %s %s %s\n", &id, src, mdl, name);

		//从特征模板文件中读取指纹特征
		minuCount2 = ReadMinutiae(mdl, &minutiaes2);

		//指纹匹配
		float similar = MinuSimilarity(iWidth, iHeight, minutiaes, minuCount, minutiaes2, minuCount2);

		//保存相似度最高的指纹记录信息
		if (similar > maxSimilar)
		{
			maxSimilar = similar;
			maxID = id;
			sprintf(maxName, name);
			sprintf(maxSrc, src);
		}
	}

	fclose(index);//关闭文件

	delete[] minutiaes;
	delete[] minutiaes2;

	//返回识别结果
	const float SIMILAR_THRED = 0.1;//匹配相似度阈值(小于阈值则认为不匹配)
	if (maxID<0 || maxSimilar < SIMILAR_THRED)
	{
		sprintf(info, "识别失败.");
		return -12;
	}

	strcpy(outImgFile, maxSrc);//保存识别结果图像文件名
	if (bSaveImg)
	{
		CopyFile(ToWideChar(maxSrc), ToWideChar(stepImgFile12), false);//复制识别结果图像
	}
	sprintf(info, "识别成功. 识别结果: 姓名[%s],目标指纹[%s],相似度[%0.2f].", maxName, maxSrc, maxSimilar);
	return 0;

}



void CFingerDlg::OnBnClickedBtnMatchImage()
{
	// TODO: 在此添加控件通知处理程序代码
	char info[MAX_PATH] = { 0 };

	//选择图像
	char srcImgFile[MAX_PATH] = "capt.bmp";

	//指纹识别
	bool bSaveImg = false;//是否保存中间结果图像
	char outImgFile[MAX_PATH] = { 0 };//识别结果图像文件名
	Identify(srcImgFile, outImgFile, info, bSaveImg);//指纹识别

	//显示操作结果信息
	m_staticInfo.SetWindowText(ToWideChar(info));

	//显示图像
	ShowImageInCtrl(m_picCtrl1, srcImgFile);//显示原始指纹图像
	ShowImageInCtrl(m_picCtrl2, outImgFile);//显示识别结果图像	
}


void CFingerDlg::OnBnClickedBtnIdentify()
{
	// TODO: 在此添加控件通知处理程序代码
	char info[MAX_PATH] = { 0 };

	//选择图像
	char srcImgFile[MAX_PATH] = "capt.bmp";
	m_zkfpEng.SaveBitmap(ToWideChar(srcImgFile));

	//指纹识别
	bool bSaveImg = false;//是否保存中间结果图像
	char outImgFile[MAX_PATH] = { 0 };//识别结果图像文件名
	Identify(srcImgFile, outImgFile, info, bSaveImg);//指纹识别

	//显示操作结果信息
	m_staticInfo.SetWindowText(ToWideChar(info));

	//显示图像
	ShowImageInCtrl(m_picCtrl1, srcImgFile);//显示原始指纹图像
	ShowImageInCtrl(m_picCtrl2, outImgFile);//显示识别结果图像	
}
