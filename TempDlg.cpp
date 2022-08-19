// TheTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PCRProject.h"
#include "PCRProjectDlg.h"
#include "TempDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <afx.h>
#include <afxres.h>

//***************************************************************
//Global variable definition
//***************************************************************

int TempFlag  = 0;				// graphic dialog flag
int BusIndex  = 0;
int TempIndex = 0;

//*****************************************************************
//Own function
//*****************************************************************

//*****************************************************************
//External function
//*****************************************************************
extern BOOL g_DeviceDetected;

// CTempDlg dialog

IMPLEMENT_DYNAMIC(CTempDlg, CDialogEx)

CTempDlg::CTempDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTempDlg::IDD, pParent)
{

}

CTempDlg::~CTempDlg()
{
}

void CTempDlg::DoDataExchange(CDataExchange* pDX)
{
}


BEGIN_MESSAGE_MAP(CTempDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_DPVEDIO, &CTempDlg::OnBnClickedBtnTempStart)
	ON_BN_CLICKED(IDC_BTN_STOPVIDEO, &CTempDlg::OnBnClickedBtnTempStop)
	ON_BN_CLICKED(IDC_BTN_TEMP_SAVE, &CTempDlg::OnBnClickedBtnSaveData)
	ON_MESSAGE(UM_GRACYCPOLLPROC, OnTempCycPollProcess)
	ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL t_CycPollingFlag = false;

void CTempDlg::OnBnClickedBtnTempStart()
{
	// TODO: Add your control notification handler code here

	if (!g_DeviceDetected) {
		MessageBox("ULS24 Device Not Attached");
		return;
	}
	TempIndex = 0;
	t_CycPollingFlag = true;			// polling flag
	SetTimer(1, 50, NULL);				// 500ms polling interval
}

void CTempDlg::OnBnClickedBtnTempStop()
{
	// TODO: Add your control notification handler code here

	if (!g_DeviceDetected) {
		MessageBox("ULS24 Device Not Attached");
		return;
	}
	KillTimer(1);		// stop video
}

void CTempDlg::OnBnClickedBtnSaveData()
{
	// TODO: Add your control notification handler code here

	if (!g_DeviceDetected) {
		MessageBox("ULS24 Device Not Attached");
		return;
	}
	KillTimer(1);		// stop video
}

void CTempDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	switch (nIDEvent)
	{
	case 1:		// ³ÖÐøvideoÃüÁî·¢ËÍ¡¢Êý¾Ý¶ÁÈ¡ // Poll temperature reading
	{
		if (timerCtrFlag && t_CycPollingFlag)
		{
			TempFlag = SENDGRAMSG;

			TxData[0] = 0xaa;		//preamble code
			TxData[1] = 0x04; //  0x10;		//command Zhimin changed, use 0x14 instead
			TxData[2] = 0x02;		//data length
			TxData[3] = 0x32; //  0x02;		//data type, date edit first byte
			TxData[4] = (BYTE)BusIndex; //  ping_pong ? 0x01 : 0x02;	// ping_pong true: Lid temp.
			TxData[5] = TxData[1] + TxData[2] + TxData[3] + TxData[4];
			if (TxData[5] == 0x17)
				TxData[5] = 0x18;
			TxData[6] = 0x17;		//back code
			TxData[7] = 0x17;		//back code
									//Send message to main dialog
			TempCalMainMsg();		//
		}
		break;
	}
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CTempDlg::TempCalMainMsg()
{
	WPARAM a = 8;
	LPARAM b = 9;
	HWND hwnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
	::SendMessage(hwnd, WM_TempDlg_event, a, b);
}


LRESULT CTempDlg::OnTempCycPollProcess(WPARAM wParam, LPARAM lParam)
{

	t_CycPollingFlag = false;

	
	unsigned char cTem[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			cTem[i][j] = RxData[6 + j + 4 * i];
		float * fTem = (float *)cTem[i];
		CString sTem;
		sTem.Format("%g", *fTem);

		POINT tempPel;
		if (TempIndex >= 1024)
			TempIndex = 0;
		TEMP[BusIndex * 4 + i].Add(sTem);
		switch (BusIndex)
		{
		case 0:
		{
			switch (i)
			{
			case 0:
			{
				m_CHA1 += (sTem + ", ""\r\n");
				SetDlgItemTextA(IDC_EDIT_TEMP_A1, m_CHA1);

				GetDlgItem(IDC_EDIT_TEMP_A1)->GetScrollRange(SB_VERT, (LPINT)&tempPel.x, (LPINT)&tempPel.y);
				tempPel.x = 0;
				GetDlgItem(IDC_EDIT_TEMP_A1)->SendMessage(EM_LINESCROLL, tempPel.x, tempPel.y);

				break;
			}
			case 1:
			{
				m_CHA2 += (sTem + ", ""\r\n");
				SetDlgItemTextA(IDC_EDIT_TEMP_A2, m_CHA2);

				GetDlgItem(IDC_EDIT_TEMP_A2)->GetScrollRange(SB_VERT, (LPINT)&tempPel.x, (LPINT)&tempPel.y);
				tempPel.x = 0;
				GetDlgItem(IDC_EDIT_TEMP_A2)->SendMessage(EM_LINESCROLL, tempPel.x, tempPel.y);
				break;
			}
			case 2:
			{
				m_CHA3 += (sTem + ", ""\r\n");
				SetDlgItemTextA(IDC_EDIT_TEMP_A3, m_CHA3);
				GetDlgItem(IDC_EDIT_TEMP_A3)->GetScrollRange(SB_VERT, (LPINT)&tempPel.x, (LPINT)&tempPel.y);
				tempPel.x = 0;
				GetDlgItem(IDC_EDIT_TEMP_A3)->SendMessage(EM_LINESCROLL, tempPel.x, tempPel.y);
				break;
			}
			case 3:
			{
				m_CHA4 += (sTem + ", ""\r\n");
				SetDlgItemTextA(IDC_EDIT_TEMP_A4, m_CHA4);
				GetDlgItem(IDC_EDIT_TEMP_A4)->GetScrollRange(SB_VERT, (LPINT)&tempPel.x, (LPINT)&tempPel.y);
				tempPel.x = 0;
				GetDlgItem(IDC_EDIT_TEMP_A4)->SendMessage(EM_LINESCROLL, tempPel.x, tempPel.y);
				break;
			}
			default:
				break;
			}
			break;
		}
		case 1:
		{
			switch (i)
			{
			case 0:
			{
				m_CHA5 += (sTem + ", ""\r\n");
				SetDlgItemTextA(IDC_EDIT_TEMP_A5, m_CHA5);
				GetDlgItem(IDC_EDIT_TEMP_A5)->GetScrollRange(SB_VERT, (LPINT)&tempPel.x, (LPINT)&tempPel.y);
				tempPel.x = 0;
				GetDlgItem(IDC_EDIT_TEMP_A5)->SendMessage(EM_LINESCROLL, tempPel.x, tempPel.y);
				break;
			}
			case 1:
			{
				m_CHA6 += (sTem + ", ""\r\n");
				SetDlgItemTextA(IDC_EDIT_TEMP_A6, m_CHA6);
				GetDlgItem(IDC_EDIT_TEMP_A6)->GetScrollRange(SB_VERT, (LPINT)&tempPel.x, (LPINT)&tempPel.y);
				tempPel.x = 0;
				GetDlgItem(IDC_EDIT_TEMP_A6)->SendMessage(EM_LINESCROLL, tempPel.x, tempPel.y);
				break;
			}
			case 2:
			{
				m_CHA7 += (sTem + ", ""\r\n");
				SetDlgItemTextA(IDC_EDIT_TEMP_A7, m_CHA7);
				GetDlgItem(IDC_EDIT_TEMP_A7)->GetScrollRange(SB_VERT, (LPINT)&tempPel.x, (LPINT)&tempPel.y);
				tempPel.x = 0;
				GetDlgItem(IDC_EDIT_TEMP_A7)->SendMessage(EM_LINESCROLL, tempPel.x, tempPel.y);
				break;
			}
			case 3:
			{
				m_CHA8 += (sTem + ", ""\r\n");
				SetDlgItemTextA(IDC_EDIT_TEMP_A8, m_CHA8);
				GetDlgItem(IDC_EDIT_TEMP_A8)->GetScrollRange(SB_VERT, (LPINT)&tempPel.x, (LPINT)&tempPel.y);
				tempPel.x = 0;
				GetDlgItem(IDC_EDIT_TEMP_A8)->SendMessage(EM_LINESCROLL, tempPel.x, tempPel.y);
				break;
			}
			default:
				break;
			}
			break;
		}
		case 2:
		{
			switch (i)
			{
			case 0:
			{
				m_CHB1 += (sTem + ", ""\r\n");
				SetDlgItemTextA(IDC_EDIT_TEMP_B1, m_CHB1);
				GetDlgItem(IDC_EDIT_TEMP_B1)->GetScrollRange(SB_VERT, (LPINT)&tempPel.x, (LPINT)&tempPel.y);
				tempPel.x = 0;
				GetDlgItem(IDC_EDIT_TEMP_B1)->SendMessage(EM_LINESCROLL, tempPel.x, tempPel.y);
				break;
			}
			case 1:
			{
				m_CHB2 += (sTem + ", ""\r\n");
				SetDlgItemTextA(IDC_EDIT_TEMP_B2, m_CHB2);
				GetDlgItem(IDC_EDIT_TEMP_B2)->GetScrollRange(SB_VERT, (LPINT)&tempPel.x, (LPINT)&tempPel.y);
				tempPel.x = 0;
				GetDlgItem(IDC_EDIT_TEMP_B2)->SendMessage(EM_LINESCROLL, tempPel.x, tempPel.y);
				break;
			}
			case 2:
			{
				m_CHB3 += (sTem + ", ""\r\n");
				SetDlgItemTextA(IDC_EDIT_TEMP_B3, m_CHB3);
				GetDlgItem(IDC_EDIT_TEMP_B3)->GetScrollRange(SB_VERT, (LPINT)&tempPel.x, (LPINT)&tempPel.y);
				tempPel.x = 0;
				GetDlgItem(IDC_EDIT_TEMP_B3)->SendMessage(EM_LINESCROLL, tempPel.x, tempPel.y);
				break;
			}
			case 3:
			{
				m_CHB4 += (sTem + ", ""\r\n");
				SetDlgItemTextA(IDC_EDIT_TEMP_B4, m_CHB4);
				GetDlgItem(IDC_EDIT_TEMP_B4)->GetScrollRange(SB_VERT, (LPINT)&tempPel.x, (LPINT)&tempPel.y);
				tempPel.x = 0;
				GetDlgItem(IDC_EDIT_TEMP_B4)->SendMessage(EM_LINESCROLL, tempPel.x, tempPel.y);
				break;
			}
			default:
				break;
			}
			break;
		}
		case 3:
		{
			switch (i)
			{
			case 0:
			{
				m_CHB5 += (sTem + ", ""\r\n");
				SetDlgItemTextA(IDC_EDIT_TEMP_B5, m_CHB5);
				GetDlgItem(IDC_EDIT_TEMP_B5)->GetScrollRange(SB_VERT, (LPINT)&tempPel.x, (LPINT)&tempPel.y);
				tempPel.x = 0;
				GetDlgItem(IDC_EDIT_TEMP_B5)->SendMessage(EM_LINESCROLL, tempPel.x, tempPel.y);
				break;
			}
			case 1:
			{
				m_CHB6 += (sTem + ", ""\r\n");
				SetDlgItemTextA(IDC_EDIT_TEMP_B6, m_CHB6);
				GetDlgItem(IDC_EDIT_TEMP_B6)->GetScrollRange(SB_VERT, (LPINT)&tempPel.x, (LPINT)&tempPel.y);
				tempPel.x = 0;
				GetDlgItem(IDC_EDIT_TEMP_B6)->SendMessage(EM_LINESCROLL, tempPel.x, tempPel.y);
				break;
			}
			case 2:
			{
				m_CHB7 += (sTem + ", ""\r\n");
				SetDlgItemTextA(IDC_EDIT_TEMP_B7, m_CHB7);
				GetDlgItem(IDC_EDIT_TEMP_B7)->GetScrollRange(SB_VERT, (LPINT)&tempPel.x, (LPINT)&tempPel.y);
				tempPel.x = 0;
				GetDlgItem(IDC_EDIT_TEMP_B7)->SendMessage(EM_LINESCROLL, tempPel.x, tempPel.y);
				break;
			}
			case 3:
			{
				m_CHB8 += (sTem + ", ""\r\n");
				SetDlgItemTextA(IDC_EDIT_TEMP_B8, m_CHB8);
				GetDlgItem(IDC_EDIT_TEMP_B8)->GetScrollRange(SB_VERT, (LPINT)&tempPel.x, (LPINT)&tempPel.y);
				tempPel.x = 0;
				GetDlgItem(IDC_EDIT_TEMP_B8)->SendMessage(EM_LINESCROLL, tempPel.x, tempPel.y);
				break;
			}
			default:
				break;
			}
			break;
		}
		default:
			break;
		}

		
	}

	BusIndex = (int)RxData[5];
	if (BusIndex == 3) {
		BusIndex = 0;
		TempIndex++;
	}	
	else
		BusIndex++;

	t_CycPollingFlag = true;

	return 0;
}
