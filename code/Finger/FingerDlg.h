#include "CZKFPEngX.h"
// FingerDlg.h: 头文件
//
#include<direct.h>
#define TEMP_DIR "temp\\"
#define DB_DIR "Database\\"
#define DB_INDEX_TXT "Database\\index.txt"
#pragma once
void InitDatabase();
void InitFile(char* filename);

// CFingerDlg 对话框
class CFingerDlg : public CDialogEx
{
	

// 构造
public:
	CFingerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CZKFPEngX m_zkfpEng;
	CStatic m_staticInfo;
	CStatic m_picCtrl1;
	CStatic m_picCtrl2;
	CStatic m_picCtrl3;
	CString m_name;
	int m_minuCount;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
	afx_msg void OnImageReceivedZkfpengx(BOOL FAR* AImageValid);
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnDisconnect();
	afx_msg void OnStnClickedStaticImg1();
	afx_msg void OnBnClickedBtnCapture();
	afx_msg void OnBnClickedBtnStep1();
	afx_msg void OnBnClickedBtnDatabase();
	afx_msg void OnBnClickedBtnStep2();
	afx_msg void OnBnClickedBtnStep3();
	afx_msg void OnBnClickedBtnStep4();
	afx_msg void OnBnClickedBtnStep5();
	afx_msg void OnBnClickedBtnStep6();
	afx_msg void OnBnClickedBtnStep7();
	afx_msg void OnBnClickedBtnStep8();
	afx_msg void OnBnClickedBtnStep9();
	afx_msg void OnBnClickedBtnStep10();
	afx_msg void OnBnClickedBtnStep11();
	afx_msg void OnBnClickedBtnStep12a();
	afx_msg void OnBnClickedBtnStep12b();
	afx_msg void OnBnClickedBtnRegister();
	afx_msg void OnBnClickedBtnAddImage2();
	afx_msg void OnBnClickedBtnMatchImage();
	afx_msg void OnBnClickedBtnIdentify();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};
