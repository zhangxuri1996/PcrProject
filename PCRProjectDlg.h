
// PCRProjectDlg.h : header file
//
//**************************************************************
//Message definition to Resource.h
//**************************************************************
#define WM_RegDlg_event		WM_USER + 101		
#define WM_GraDlg_event		WM_USER + 102	
#define WM_TrimDlg_event	WM_USER + 103
#define WM_ReadHID_event	WM_USER + 104
#define WM_OperDlg_event	WM_USER + 105
#define WM_ParsaveDlg_event		WM_USER + 106
#define WM_TempDlg_event	WM_USER + 107

//.......................................................................................

#if !defined(AFX_USBHIDIOCDLG_H__0B2AAA84_F5A9_11D3_9F47_0050048108EA__INCLUDED_)
#define AFX_USBHIDIOCDLG_H__0B2AAA84_F5A9_11D3_9F47_0050048108EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//....................................................................

//#pragma once
#include "GraDlg.h"
#include "RegDlg.h"
#include "TrimDlg.h"
#include "OperationDlg.h"
#include "TempDlg.h"
#include "ParsaveDlg.h"
#include "VersionShow.h"
#include "TrimReader.h"
#include "TempDlg.h"

//****************************************************
//Global variable definition
//****************************************************
#define TxNum			64		// the number of the buffer for sent data to COMX
#define RxNum			200		// the number of the buffer for received data from COMX
#define TrimRxNum	    800		// the number of the buffer for received data from COMX
#define dNum			29			// ÿ���򴮿ڷ��͵��ֽ���
#define ONCOMNUM		59		// ��λ�����ض����ֽڻ����COM���ж�

//#define HIDREPORTNUM	64		//	HID ÿ��report byte����
#define HIDREPORTNUM	130		//	HID ÿ��report byte����

#define VEDIOPITCHTIME	100			// vedio��ʾʱÿ֡��ʾ�ļ��ʱ��
//#define HIDREADTIMEOUT	500			// HID read data timeout value
#define HIDREADTIMEOUT	5000

#define TRIGERCMD		0x01			// Trigger command
#define GRACMD			0x02			// ͼ�� command 
#define READCMD			0x04			// Read command
#define TESTCMD			0x09			// ����ʵ��command (��ȡRAM����)
#define TEMPCMD			0x10			// peltier Control(temperature) command
#define PIDCMD			0x11			// PID command
#define PIDREADCFGCMD	0x12			// ��ȡPID configure��Ϣ command
#define PIDREADCYCCMD	0x13			// ��ȡPID cycle�Ȳ�����Ϣ command
#define CYCCMD			0x14			// ��ȡѭ����Ϣ command
#define POLLINGRACMD	0x15			// polling graphic HID command

#define HIDBUFSIZE		65

#define PAGENUM			2				// 1: ֻ��ʾoperation dialog
										// 2: ��ʾ����Ի���
#define	HIDNUM			1				// 1: ֻʹ��temperature hid
										// 2: ʹ��temperature��graphic����hid
#define VERSIONYEAR		2018			// �汾����ʾ�Ի�����ʾ����ֵ
#define VERSIONMONTH	8				// �汾����ʾ�Ի�����ʾ����ֵ
#define VERSIONDAY		4				// �汾����ʾ�Ի�����ʾ����ֵ

#define GRA_VID			0x0483			// Graphic HID VID
#define GRA_PID			0x5750			// Graphic HID PID
#define TEM_VID			0x0683			// Temperature HID VID
#define TEM_PID			0x5850			// Temperature HID PID

//#define GRA_VID			0x0683			// Graphic HID VID
//#define GRA_PID			0x5850			// Graphic HID PID
//#define TEM_VID			0x0483			// Temperature HID VID
//#define TEM_PID			0x5750			// Temperature HID PID

#define HIDREADCNTNUM	3				//HID��ȡ����timeout�����·��Ͷ�ȡ������

//*****************************************************************
//External variable definition
//*****************************************************************
extern BYTE TxData[TxNum];		// the buffer of sent data to COMX
extern BYTE RxData[RxNum];		// the buffer of received data from COMX

extern CString RegRecStr;				//���������ַ���buffer ʮ������
extern CString Dec_RegRecStr;			//���������ַ���buffer ʮ����
extern CString Valid_RegRecStr;			//��Ч���������ַ���buffer ʮ������
extern CString Valid_Dec_RegRecStr;		//��Ч���������ַ���buffer ʮ����

extern BOOL timerCtrFlag;						//ͼ�����¶Ȱ�HID��ȡtimer����
extern CStringArray TEMP[16];          //16·�¿�����

// CPCRProjectDlg dialog
class CPCRProjectDlg : public CDialogEx
{
// Construction
public:
	CPCRProjectDlg(CWnd* pParent = NULL);	// standard constructor

//	virtual void OnOK();
//	virtual void OnCancel();

// Dialog Data
	enum { IDD = IDD_PCRPROJECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnRegDlg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGraDlg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTrimDlg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReadHID(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOperDlg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnParsaveDlg(WPARAM wParam, LPARAM iParam);
	afx_msg LRESULT OnTempDlg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGradlgIntegral(WPARAM wParam, LPARAM lParam);
	LRESULT OnPassageway(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	BOOL DeviceNameMatch(LPARAM lParam);
	bool FindTheHID();
	LRESULT Main_OnDeviceChange(WPARAM wParam, LPARAM lParam);
	void CloseHandles();
	void DisplayInputReport();
	void DisplayReceivedData(char ReceivedByte);
	void GetDeviceCapabilities(HANDLE pHidHandle);
	void PrepareForOverlappedTransfer();
	void ReadAndWriteToDevice();
	void RegisterForDeviceNotifications();
	void GRAHID_WriteHIDOutputReport();
	void GRAHID_ReadHIDInputReport();
	void TEMHID_WriteHIDOutputReport();
	void TEMHID_ReadHIDInputReport();
	void SendHIDRead();
	void TrimReadAnalysis();

public:
//	CMscomm1 m_mscomm;
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
	CTabCtrl m_tab;
	CRegDlg m_RegDlg;
	CGraDlg m_GraDlg;
	CTrimDlg m_TrimDlg;
	COperationDlg m_OperDlg;
	CTempDlg m_TempDlg;
	CVersionShow m_VersionDlg;

	CTrimReader m_TrimReader;
	DPReader m_DPReader;


	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	void GetCom();
	CString m_StaticOpenComm;
	void CommSend (int num);
	afx_msg void OnBnClickedBtnOpencomm();
	afx_msg void OnBnClickedBtnOpenhid();
//	CString m_strBytesReceived;
//	CEdit m_BytesReceived;
	CListBox m_BytesReceived;
	CString m_strBytesReceived;
	afx_msg void OnBnClickedBtnSendhid();
	afx_msg void OnBnClickedBtnReadhid();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnBnClickedPcrBtnVer();
	afx_msg void OnStnClickedPcrStaticVer();
//	virtual void OnCancel();
	void DownLoadData(char fileName[],char saveFileName[]);
	void MerGeTrim(char trim1Name[], char trim2Name[]);
	void ClearTrim(char trimName[]);
	void DeleteTrim(char trimName[]);
	void CPCRProjectDlg::RenameTrim(char oldName[], char newName[]);
	CString QueryTrim(CString trimNumber);
};

#endif // !defined(AFX_USBHIDIOCDLG_H__0B2AAA84_F5A9_11D3_9F47_0050048108EA__INCLUDED_)