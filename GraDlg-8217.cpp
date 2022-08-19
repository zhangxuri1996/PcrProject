// GraDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PCRProject.h"
#include "PCRProjectDlg.h"
#include "GraDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <afx.h>
#include <afxres.h>
#include "CurveShow.h"
#include<vector>
#include<regex>
using namespace std;

#include "OperationDlg.h"

//***************************************************************
//Global variable definition
//***************************************************************

int GraFlag = 0;				// graphic dialog flag
bool Gra_pageFlag = false;		// graphic dialog ��ҳѭ����־
bool Gra_videoFlag = false;		// graphic dialog videoѭ����־
BYTE GraBuf[GRADATANUM];		// graphic dialog transmitted data buffer
int RecdataFlag = 0;			// receive data recode flag

byte PCRType = 0;				// PCR select type
int PCRCycCnt = 0;				// PCR ���η��ͼ���
int PCRNum = 0;					// ��PCR���

BYTE PCRTypeFilterClass = 0;	// Graphic command����type ������֣��续�С�ҳ��vedio������type byte�ĵ�4λ��
BYTE PCRTypeFilterNum = 0;		// Graphic command����type ��PCR���֣�����type byte�ĸ�4λ��

int Page12_24Flag = 0;			// 12��/24�л�ҳ��־(0:12�У�1:24��)
int Vedio12_24Flag = 0;			// 12��/24��Vedio��־(0:12�У�1:24��)

int xOffset = 0;				// ��PCR��ͼƫ����
int yOffset = 0;				// ��PCR��ͼƫ����

CBitmap bBmp;						// ��̨��ͼBMP
CBitmap *oldBMP;					// ԭBMPָ��
CDC bgDC;							// ��̨DC
CDC * dispalyDC;					// ǰ̨��ʾDC
CRect displayRect;					// ��ʾ��rect

CBitmap bBmp_Row;					// ��̨����BMP
CBitmap *oldBMP_Row;				// ԭBMPָ��
CDC bgDC_Row;						// ��̨DC
CDC *RowDC;							// ǰ̨��ʾDC

int OnPaint_Flag = 0;				// OnPaint���Ʊ�־
BOOL DrawPageStart = FALSE;			// �ж��ǲ��ǵ�һ�ΰ���ͼ��ť(Capture)
BOOL DrawRowStart = FALSE;			// �ж��ǲ��ǵ�һ�ΰ����а�ť

CString sPCRdata;
CString sADCRecdata;

//CFile m_recdataFile(_T("PRCdata.txt"),CFile::modeCreate|CFile::modeWrite);			// receive data file
//CFile m_adcdataFile(_T("ADCdata.txt"),CFile::modeCreate|CFile::modeWrite);			// ADC data file
CFile m_recdataFile;			// receive data file
CFile m_adcdataFile;			// ADC data file

CTime sysTime_Gra;					// system time

POINT pt;			// �༭����ʾλ��ָ��
int typePixelSize = 0;		// 12�к�24�����ֱ�־

BOOL PollingGraTimerFlag = FALSE;		// GraDlg polling timer����ָ�������־
BOOL PollingGraMode = FALSE;			// ��ͼʱ�ж��Ƿ�Ϊpollingģʽ

BOOL PollingPCRMask = TRUE;				// �ж���ѡPCRʱpolling��־ 

int pixelNum = 12;						// Default 12X12 frame

//*****************************************************************
//Own function
//*****************************************************************


//*****************************************************************
//External function
//*****************************************************************


extern float int_time; // zd add, in ms
extern int gain_mode;  // zd add, 0 high gain, 1 low gain
int adc_result[24];    // zd add
int frame_data[24][24];		// A whole frame of data
int rn;

extern BOOL g_DeviceDetected;

//=====HDR Support==========

int pdata_0[24][24];	// first pass
int pdata_1[24][24];	// second pass
int hdr_phase = 0;

int contrast = 100;

extern CTrimDlg *g_pTrimDlg;
extern CTrimReader *g_pTrimReader;

#define DARK_LEVEL 100

#define ADC_CORRECT
#define DARK_MANAGE

// #define SAVE_DATA_FILE

CCurveShow *pCVS = NULL;		// Zhimin added. This is for modeless implementation of the curve show dialog.

//------------------

// CGraDlg dialog

IMPLEMENT_DYNAMIC(CGraDlg, CDialogEx)

CGraDlg::CGraDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGraDlg::IDD, pParent)
	, m_GainMode(0)				// Low gain default
	, m_FrameSize(0)
	, m_OrigOut(FALSE)
{

	m_EditReadRow = _T("");
	m_ShowAllDataInt = 0;
	m_ReadHexInt = 0;
	m_PixelData = _T("");
	m_ADCRecdata = _T("");

//	m_GainMode = 1;
	m_FrameSize = 0;
	m_PixOut = true;
	m_OrigOut = false;

	for(int i=0; i<24; i++) {
		adc_result[i] = 0;
	}

	for(int i=0;i<4;i++)
		m_PositionChip[i].empty();
}

CGraDlg::~CGraDlg()
{
}

void CGraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_READROW, m_EditReadRow);
	DDX_Control(pDX, IDC_RADIO_SHOWALLDATA, m_ShowAllData);
	DDX_Radio(pDX, IDC_RADIO_SHOWALLDATA, m_ShowAllDataInt);
	DDX_Control(pDX, IDC_RADIO_READHEX, m_ReadHex);
	DDX_Radio(pDX, IDC_RADIO_READHEX, m_ReadHexInt);
	DDX_Control(pDX, IDC_RADIO_READDEC, m_ReadDec);
	DDX_Control(pDX, IDC_RADIO_ADCDATA, m_ADCData);
	DDX_Control(pDX, IDC_RADIO_SHOWVALIDDATA, m_ShowValidData);
	DDX_Text(pDX, IDC_EDIT_RecData, m_PixelData);
	DDX_Text(pDX, IDC_EDIT_ADCDATA, m_ADCRecdata);
	DDX_Radio(pDX, IDC_RADIOLOWGAIN, m_GainMode);
	DDX_Radio(pDX, IDC_RADIO_12, m_FrameSize);
	DDX_Check(pDX, IDC_CHECK_PIXOUT, m_PixOut);
	DDX_Check(pDX, IDC_CHECK_ORIGOUT, m_OrigOut);
}


BEGIN_MESSAGE_MAP(CGraDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_READROW, &CGraDlg::OnClickedBtnReadrow)
	ON_MESSAGE(UM_GRAPROCESS,OnGraProcess)
	ON_MESSAGE(UM_GRAPOLLINGPROCESS,OnGraPollingProcess)
	ON_BN_CLICKED(IDC_BTN_DPROW24, &CGraDlg::OnBnClickedBtnDprow24)
	ON_BN_CLICKED(IDC_BTN_DPPAGE12, &CGraDlg::OnBnClickedBtnDppage12)
	ON_BN_CLICKED(IDC_BTN_DPPAGE24, &CGraDlg::OnBnClickedBtnDppage24)
	ON_BN_CLICKED(IDC_BTN_DPVEDIO, &CGraDlg::OnBnClickedBtnDpvedio)
	ON_BN_CLICKED(IDC_BTN_STOPVIDEO, &CGraDlg::OnBnClickedBtnStopvideo)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CGraDlg::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_ADCCONVERT, &CGraDlg::OnBnClickedBtnAdcconvert)
	ON_BN_CLICKED(IDC_BTN_CLEARADC, &CGraDlg::OnBnClickedBtnClearadc)
	ON_BN_CLICKED(IDC_BUTTON1, &CGraDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CGraDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BTN_DPVIDEO24, &CGraDlg::OnBnClickedBtnDpvideo24)
	ON_BN_CLICKED(IDC_RADIO_ADCDATA, &CGraDlg::OnBnClickedRadioAdcdata)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_RADIOLOWGAIN, &CGraDlg::OnClickedRadiolowgain)
	ON_COMMAND(IDC_RADIOHIGHGAIN, &CGraDlg::OnRadiohighgain)
	ON_COMMAND(IDC_RADIOHDR, &CGraDlg::OnRadiohdr)
	ON_BN_CLICKED(IDC_RADIO_12, &CGraDlg::OnClickedRadio12)
	ON_COMMAND(IDC_RADIO_24, &CGraDlg::OnRadio24)
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE, &CGraDlg::OnBnClickedButtonCapture)
	ON_BN_CLICKED(IDC_CHECK_PIXOUT, &CGraDlg::OnClickedCheckPixout)
	ON_BN_CLICKED(IDC_CHECK_ORIGOUT, &CGraDlg::OnClickedCheckOrigout)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_GraDlg_Check_AutoSnap, &CGraDlg::OnClickedGradlgCheckAutosnap)
	ON_BN_CLICKED(IDC_LED_INDV_EN, &CGraDlg::OnBnClickedLedIndvEn)
	ON_BN_CLICKED(IDC_LED_SWITCH, &CGraDlg::OnBnClickedLedSwitch)
	ON_BN_CLICKED(IDC_BTN_COPY, &CGraDlg::OnBnClickedBtnCopy)
	ON_BN_CLICKED(IDC_PCRCURVE, &CGraDlg::OnBnClickedPcrcurve)
END_MESSAGE_MAP()


//*********************************************************************
//Own function
//*********************************************************************

//�������Ի����Ӧ��Ϣ������
void CGraDlg::GraCalMainMsg()
{
	WPARAM a = 8;
	LPARAM b = 9;
	HWND hwnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
	::SendMessage(hwnd,WM_GraDlg_event,a,b);
}


void CGraDlg::MakeGraPacket(byte pCmd, byte pType, byte pData)
{
	TxData[0] = 0xaa;		//preamble code
	TxData[1] = pCmd;		//command
	TxData[2] = 0x0C;		//data length
	TxData[3] = pType;		//data type, date edit first byte
	TxData[4] = pData;		//real data
	TxData[5] = 0x00;		//Ԥ��λ
	TxData[6] = 0x00;
	TxData[7] = 0x00;
	TxData[8] = 0x00;
	TxData[9] = 0x00;
	TxData[10] = 0x00;
	TxData[11] = 0x00;
	TxData[12] = 0x00;
	TxData[13] = 0x00;
	TxData[14] = 0x00;
	TxData[15] = TxData[1]+TxData[2]+TxData[3]+TxData[4]+TxData[5]+TxData[6]+TxData[7]+TxData[8]+TxData[9]
	+TxData[10]+TxData[11]+TxData[12]+TxData[13]+TxData[14];		//check sum
	if (TxData[15]==0x17)
		TxData[15]=0x18;
	else
		TxData[15]=TxData[15];
	TxData[16] = 0x17;		//back code
	TxData[17] = 0x17;		//back code
}

void CGraDlg::DrawRaw(CDC *pBufDC, CRect pRect,CBrush pBrush[RowNum24][ColNum24], int pRowNum, int pColNum, int pPixelSize, int pPCRNum)
{
	int xOffset = 0;
	int yOffset = 0;
	switch (pPCRNum)
	{
	case 1:
		if (pPixelSize == pixelsize12)		// 12�����
		{
			xOffset = 0;
			yOffset = 0;
		}
		else		// 24�����
		{
			xOffset = 0;
			yOffset = 0;
		}
		break;
	case 2:
		if (pPixelSize == pixelsize12)
		{
			xOffset = 60;		// offset = pixelsize * rowNum		
			yOffset = 0;
		}
		else
		{
			xOffset = 72;
			yOffset = 0;
		}
		break;
	case 3:
		if (pPixelSize == pixelsize12)
		{
			xOffset = 0;
			yOffset = 60;
		}
		else
		{
			xOffset = 0;
			yOffset = 72;
		}
		break;
	case 4:
		if (pPixelSize == pixelsize12)
		{
			xOffset = 60;
			yOffset = 60;
		}
		else
		{
			xOffset = 72;
			yOffset = 72;
		}
		break;
	default:
		break;
	}

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pBufDC,pRowNum*pPixelSize,pColNum*pPixelSize);		// ���õ�Ļ��С

	CDC bDC; 
	bDC.CreateCompatibleDC(pBufDC);	

	bDC.SelectObject(&bmp);		

	for(int l=0; l<pColNum; l++)		// l�����кţ�rxdata[5]���������к�
	{
		pRect.SetRect((pPixelSize*l + xOffset),(pPixelSize*RxData[5] + yOffset),(pPixelSize*(l+1) + xOffset),(pPixelSize*(RxData[5]+1) + yOffset));
		bDC.Rectangle(pRect);
		bDC.FillRect(&pRect,&pBrush[RxData[5]][l]);
	}
	pBufDC->BitBlt(0,0,900, 380, &bDC, 0, 0, SRCCOPY);
}

void CGraDlg::DrawPage(CDC *pBufDC, CRect pRect, CBrush pBrush[RowNum24][ColNum24], int pPixelSize, int pPCRNum)
{
	switch (pPCRNum)
	{
	case 1:
		if (pPixelSize == pixelsize12)		// 12�����
		{
			xOffset = 0;
			yOffset = 0;
		}
		else		// 24�����
		{
			xOffset = 0;
			yOffset = 0;
		}
		break;
	case 2:
		if (pPixelSize == pixelsize12)
		{
			xOffset = pixelsize12*12;		// offset = pixelsize * rowNum		
			yOffset = 0;
		}
		else
		{
			xOffset = pixelsize24*24;
			yOffset = 0;
		}
		break;
	case 3:
		if (pPixelSize == pixelsize12)
		{
			xOffset = 0;
			yOffset = pixelsize12*12;
		}
		else
		{
			xOffset = 0;
			yOffset = pixelsize24*24;
		}
		break;
	case 4:
		if (pPixelSize == pixelsize12)
		{
			xOffset = pixelsize12*12;
			yOffset = pixelsize12*12;
		}
		else
		{
			xOffset = pixelsize24*24;
			yOffset = pixelsize24*24;
		}
		break;
	default:
		break;
	}

	if (pPixelSize == pixelsize12)
	{
		for(int l=0; l<ColNum12; l++)		// l�����к�
		{
			if(m_GainMode <= 1) {
				pRect.SetRect((pPixelSize*l + xOffset),(pPixelSize*RxData[5] + yOffset),(pPixelSize*(l+1) + xOffset),(pPixelSize*(RxData[5]+1) + yOffset));

				pBufDC->Rectangle(pRect);
				pBufDC->FillRect(&pRect,&pBrush[RxData[5]][l]);
			}
			else {

				//zd add

				if(!hdr_phase) {
					pdata_0[rn][l] = adc_result[l];
				}
				else {
					pdata_1[rn][l] = adc_result[l];
				}
			}
		}
	}
	else
	{
		for(int l=0; l<ColNum24; l++)		// l�����к�
		{
			if(m_GainMode <= 1) {
				pRect.SetRect((pPixelSize*l + xOffset),(pPixelSize*RxData[5] + yOffset),(pPixelSize*(l+1) + xOffset),(pPixelSize*(RxData[5]+1) + yOffset));
				pBufDC->Rectangle(pRect);
				pBufDC->FillRect(&pRect,&pBrush[RxData[5]][l]);
			}
			else {

				//zd add

				if(!hdr_phase) {
					pdata_0[rn][l] = adc_result[l];
				}
				else {
					pdata_1[rn][l] = adc_result[l];
				}
			}
		}
	}

}

void DrawOverTime(CDC *pBufDC, CRect pRect, CBrush pBrush[RowNum24][ColNum24], int pPixelSize, int pPCRNum)
{
	int xOffset = 0;
	int yOffset = 0;
	switch (pPCRNum)
	{
	case 1:
		if (pPixelSize == pixelsize12)		// 12�����
		{
			xOffset = 0;
			yOffset = 0;
		}
		else		// 24�����
		{
			xOffset = 0;
			yOffset = 0;
		}
		break;
	case 2:
		if (pPixelSize == pixelsize12)
		{
			xOffset = 60;		// offset = pixelsize * rowNum		
			yOffset = 0;
		}
		else
		{
			xOffset = 72;
			yOffset = 0;
		}
		break;
	case 3:
		if (pPixelSize == pixelsize12)
		{
			xOffset = 0;
			yOffset = 60;
		}
		else
		{
			xOffset = 0;
			yOffset = 72;
		}
		break;
	case 4:
		if (pPixelSize == pixelsize12)
		{
			xOffset = 60;
			yOffset = 60;
		}
		else
		{
			xOffset = 72;
			yOffset = 72;
		}
		break;
	default:
		break;
	}

	for(int l=0; l<ColNum12; l++)		// l�����к�
	{
		pRect.SetRect((pPixelSize*l + xOffset),(pPixelSize*RxData[5] + yOffset),(pPixelSize*(l+1) + xOffset),(pPixelSize*(RxData[5]+1) + yOffset));
		pBufDC->Rectangle(pRect);
		pBufDC->FillRect(&pRect,&pBrush[RxData[5]][l]);
	}
}

void CGraDlg::ReceiveDataProcess()
{
	//���ݴ�����ʾ

	// ���ݷ��ص�type��������ʾ12�����ݻ���24������
	switch(PCRTypeFilterClass)
	{
	case dprow12:		// 12�л���
	case dppage12:		// 12�л�ҳ
	case dpvideo12:		// 12�л�vedio
	case 0x0c:			// ����ʱ�䳬��10ms
		pixelNum = 12;
		break;

	case dprow24:		// 24�л���
	case dppage24:		// 24�л�ҳ
	case dpvideo24:		// 24�л�vedio
		pixelNum = 24;
		break;

	default: break;
	}

	WORD * DecData = new WORD [pixelNum];
	int NumData = 0;
	int iDecVal = 0;
	CString TmHex;
	TmHex.Empty();
	CString sDecData;	// �����ʾ��ʮ�����ַ���
	sDecData.Empty();
	char sDecVal[10];
	BYTE lByte = 0x00;
	CString sRownum;
	sRownum.Empty();
	sRownum.Format(" %d",RxData[5]);

	for (NumData=0; NumData<pixelNum; NumData++)			//��ÿ����byte���ϳ�һ��word
	{
		lByte=RxData[NumData*2+6];				//ȡ����4λbyte
		lByte = lByte<<4;						//����4λbyte����4λ
		DecData[NumData] = RxData[NumData*2+7];		//����8λbyte��ֵ��word����
		DecData[NumData] <<= 8;						//word buffer����8λ������8λbyte���ݷ��õ���8λ
		DecData[NumData] |= lByte;				//����4λbyte�ŵ�word buffer��8λ
		DecData[NumData] >>= 4;						//��word buffer��������4λ�������Ч12λ����
	}

	for (NumData=0; NumData<pixelNum; NumData++)			//��һ���е�ÿ��Pixelʮ��������Ч����ת��ʮ����
	{
		TmHex.Format("%2X",DecData[NumData]);		//��ÿ��word bufferת��ʮ�������ַ���
		iDecVal = ChangeNum(TmHex, TmHex.GetLength());		//��ÿ��ʮ�������ַ���ת����Чʮ������
		gcvt(iDecVal,4,sDecVal);		//��ʮ���Ƹ�����ת�����ַ���
		//�ڶ�����������ʮ�������ж���λ
		//����������������Ϊchar *, �������sDecVal�Ķ���

		sDecData += sDecVal;				//��char* ��ֵ��CString��������ʾ
		sDecData += "  ";					//ÿ�����ݼ�ӿո�
	}

	sDecData += sRownum;
	delete[] DecData;

	// ����������Ϊ0x14ʱ��ֻ��ʾ�����ַ���
	if (RxData[2] == 0x14)
	{
		sDecData = "";
		sDecData.Format("the cycle number is %d",RxData[3]);
	}

	int result = 0;
	char fstrbuf[9];

	CString sADCData;	// �����ʾ��ADC�����ַ���
	sADCData.Empty();

	int flag;		// For ADC Correct

	if(RxData[5] == 0) {
		if(PCRNum == 1) 
			sADCData += "\r\n";
		
		sADCData += "Chip#";
		itoa (PCRNum, fstrbuf, 9);		//�����ת���ַ�����ʾ
		sADCData += fstrbuf;
		sADCData += "\r\n";
	}

	for (NumData=0; NumData<pixelNum; NumData++)
	{
		if(g_pTrimReader) {		
			result = g_pTrimReader->ADCCorrection(NumData, RxData[NumData*2+7], RxData[NumData*2+6], pixelNum, PCRNum, gain_mode, &flag);
		}
		else {
			result = 0;
		}

		adc_result[NumData] = result; // zd add

		frame_data[RxData[5]][NumData] = result;		// RxData is the row number.

		itoa (result,fstrbuf,10);		//�����ת���ַ�����ʾ
		sADCData += fstrbuf;
		sADCData += " ";
	}

	sADCData += sRownum;

	// ����������Ϊ0x14ʱ��ֻ��ʾ�����ַ���
	if (RxData[2] == 0x14)
	{
		sADCData = "";
		sADCData.Format("the cycle number is %d",RxData[3]);
	}

	if (m_OrigOut && m_ReadHex.GetCheck())			//��ʮ������
	{
		if (m_ShowAllData.GetCheck())	//��ʾ��������
		{
			//			m_PixelData = RegRecStr + m_PixelData;		//���������ڱ༭���һ����ʾ
			m_PixelData += (RegRecStr+"\r\n");					//���������ڱ༭�����һ����ʾ
			SetDlgItemText(IDC_EDIT_RecData,m_PixelData);

			sPCRdata = (RegRecStr+"\r\n");

			//�༭��ֱ�������׶�			
			GetDlgItem(IDC_EDIT_RecData)->GetScrollRange(SB_VERT,(LPINT)&pt.x,(LPINT)&pt.y);
			pt.x=0;
			GetDlgItem(IDC_EDIT_RecData)->SendMessage(EM_LINESCROLL,pt.x,pt.y);
			RegRecStr = "";
		}

		if (m_ShowValidData.GetCheck())	//��ʾ��Ч����
		{
			//			m_PixelData = Valid_RegRecStr + m_PixelData;	//���������ڱ༭���һ����ʾ
			m_PixelData += (Valid_RegRecStr+"\r\n");					//���������ڱ༭�����һ����ʾ
			SetDlgItemText(IDC_EDIT_RecData,m_PixelData);

			sPCRdata = (Valid_RegRecStr+"\r\n");

			//�༭��ֱ�������׶�
			//			POINT pt;
			GetDlgItem(IDC_EDIT_RecData)->GetScrollRange(SB_VERT,(LPINT)&pt.x,(LPINT)&pt.y);
			pt.x=0;
			GetDlgItem(IDC_EDIT_RecData)->SendMessage(EM_LINESCROLL,pt.x,pt.y);
			Valid_RegRecStr = "";
		}		
	}

	if (m_OrigOut && m_ReadDec.GetCheck())			//��ʮ������ʾ
	{
		//		m_PixelData = sTest + m_PixelData +"\r\n";			//ÿ�����ݽ��ӻس�
		//���������ڱ༭���һ����ʾ
		m_PixelData += (sDecData+"\r\n");						
		SetDlgItemText(IDC_EDIT_RecData,m_PixelData);		//ÿ�����ݽ��ӻس�
		//���������ڱ༭�����һ����ʾ	

		sPCRdata = (sDecData+"\r\n");

		//�༭��ֱ�������׶�
		//		POINT pt;
		GetDlgItem(IDC_EDIT_RecData)->GetScrollRange(SB_VERT,(LPINT)&pt.x,(LPINT)&pt.y);
		pt.x=0;
		GetDlgItem(IDC_EDIT_RecData)->SendMessage(EM_LINESCROLL,pt.x,pt.y);
	}

	if (m_PixOut && m_GainMode <= 1)		//ADC������ʾ
	{
		m_ADCRecdata += (sADCData+"\r\n");
		SetDlgItemText(IDC_EDIT_ADCDATA,m_ADCRecdata);		//ÿ�����ݽ��ӻس�
		//���������ڱ༭�����һ����ʾ

		sADCRecdata = (sADCData+"\r\n");

		//�༭��ֱ�������׶�
		//		POINT pt;
		GetDlgItem(IDC_EDIT_ADCDATA)->GetScrollRange(SB_VERT,(LPINT)&pt.x,(LPINT)&pt.y);
		pt.x=0;
		GetDlgItem(IDC_EDIT_ADCDATA)->SendMessage(EM_LINESCROLL,pt.x,pt.y);

	}

	if(RxData[5] == 11 && pixelNum == 12) {			
		UpdatePCRCurve(PCRNum, pixelNum);
	}
	else if(RxData[5] == 23 && pixelNum == 24) {
		UpdatePCRCurve(PCRNum, pixelNum);
	}
}

std::vector<double> m_yData[4][4];	//handled data��4 channels��4 frames

void CGraDlg::UpdatePCRCurve(int PCRNum, int pixelNum)
{
	// This is called when a frame of image data is captured. 
	// PCRNum is the chip number. It is 1 based, i.e. 1,2,3,4.
	// pixelNum is the frame size, 12 for 12X12 frames and 24 for 24x24 frames
	// The frame data is available in the global variable int frame_data[24][24].
	
	//calculate yData 
	if(pixelNum==24)
	{
		for(int row=0;row<24;row=row+6)			// for every amount from 6 rows and 6 columns to produce 1 singal
		{
			int sum=0;					// record the frame
			for(int innerrow=row;innerrow<row+6;innerrow++)	//handle 6 rows 
			{
				for(int column=6;column<12;column++)
				{   //handle 6 columns in every row, every number subtract 100 to get rid of noice.And plus them.
					int value=frame_data[innerrow][column]-100;
					sum+=abs(value)<10?0:value;
				}
			}
			m_yData[PCRNum-1][row/6].push_back(sum);
		}	
	}else if(pixelNum==12)
	{
		//int sum=0;					// record the frame
		//switch(PCRNum)
		//{
		//case 1://chip1
		//	sum=frame_data[2][5]+frame_data[2][6];//F2+G2
		//	m_yData[PCRNum-1][0].push_back(sum);

		//	sum=frame_data[5][5]+frame_data[5][6];//F5+G5
		//	m_yData[PCRNum-1][1].push_back(sum);

		//	sum=frame_data[8][5]+frame_data[8][6];//F8+G8
		//	m_yData[PCRNum-1][2].push_back(sum);

		//	sum=frame_data[10][5]+frame_data[11][5];//F10+F11
		//	m_yData[PCRNum-1][3].push_back(sum);
		//	break;
		//case 2://chip2
		//	sum=frame_data[1][5]+frame_data[1][6]+frame_data[2][5];//F1+G1+F2
		//	m_yData[PCRNum-1][0].push_back(sum);

		//	sum=frame_data[4][5]+frame_data[4][6];//F4+G4
		//	m_yData[PCRNum-1][1].push_back(sum);

		//	sum=frame_data[7][5]+frame_data[7][6];//F7+G7
		//	m_yData[PCRNum-1][2].push_back(sum);

		//	sum=frame_data[10][5]+frame_data[10][6];	//F10+G10
		//	m_yData[PCRNum-1][3].push_back(sum);
		//	break;
		//case 3://chip3
		//	sum=frame_data[1][5]+frame_data[1][6]+
		//		frame_data[2][5]+frame_data[2][6];//F1+G1+F2+G2
		//	m_yData[PCRNum-1][0].push_back(sum);

		//	sum=frame_data[4][5]+frame_data[4][6]+
		//		frame_data[5][5]+frame_data[5][6];//F4+G4+F5+G5
		//	m_yData[PCRNum-1][1].push_back(sum);

		//	sum=frame_data[7][5]+frame_data[7][6]+
		//		frame_data[8][5]+frame_data[8][6];//F7+G7+F8+G8
		//	m_yData[PCRNum-1][2].push_back(sum);

		//	sum=frame_data[10][5]+frame_data[10][6];//F10+G10
		//	m_yData[PCRNum-1][3].push_back(sum);
		//	break;
		//case 4://chip4
		//	m_yData[PCRNum-1][0].push_back(0);
		//	m_yData[PCRNum-1][1].push_back(0);
		//	m_yData[PCRNum-1][2].push_back(0);
		//	m_yData[PCRNum-1][3].push_back(0);
		//	break;
		//}

		for(int frame=0;frame<4;frame++)
		{
			int j=0;
			int column[10],row[10];
			memset(column,0,10*sizeof(int));
			memset(row,0,10*sizeof(int));
			CString temp(m_PositionChip[PCRNum-1].at(frame).c_str());
			CString number;
			for(int i=0;i<m_PositionChip[PCRNum-1].at(frame).size();i++)
			{
				char tt=temp.GetAt(i);
				if(tt>='A'&& tt<='Z'){
					column[j]=tt-'A';
				}else if(tt=='+'){
					row[j]=atoi(number);
					j++;
					number="";
				}else if(tt>='0'&& tt<='9'){
					number+=tt;
				}
			}
			row[j]=atoi(number);
			number="";
			double sum=0;
			for(int i=0;i<=j;i++)
			{
				sum+=frame_data[row[i]][column[i]];
			}
			m_yData[PCRNum-1][frame].push_back(sum);
		}
	}
}


LRESULT CGraDlg::OnGraProcess(WPARAM wParam, LPARAM lParam)
{
	//���ݴ���
	ReceiveDataProcess();

#ifdef SAVE_DATA_FILE

	//�༭���ļ�����	
	// receive data save file
	m_recdataFile.Write(sPCRdata,sPCRdata.GetLength());

	// ADC data save file
	m_adcdataFile.Write(sADCRecdata,sADCRecdata.GetLength());

	// ������һ�ΰ������ļ�
	if ((PCRMask == 0) & (Gra_pageFlag == false))
	{
		m_recdataFile.Close();
		m_adcdataFile.Close();
	}

#endif

	// Drawing ����
	GraDlgDrawPattern();
/*
	// ��ҳʱÿ�ζ�ȡһ�У����ݴ�����ٶ���һ��
	if (Gra_pageFlag)
	{
		GraFlag = READGRAMSG;
		GraCalMainMsg();
	}
*/
	return 0;
}


LRESULT CGraDlg::OnGraPollingProcess(WPARAM wParam, LPARAM lParam)
{
	PollingGraMode = TRUE;
	PollingGraTimerFlag = TRUE;
	SetTimer(2,PGRATIMERVALUE,NULL);		// start polling

	return 0;
}


void CGraDlg::GraDlgDrawPattern()
{

	CBrush brush[RowNum24][ColNum24];	// �»�ˢ

	int i=0;
	int l=0;
	int gray_level=0;

	// zd comment RxData[5] is the row number.

    rn = RxData[5];

	for (i=0; i<ColNum24; i++)
	{
		gray_level = adc_result[i]/16;
		if(gray_level > 255) gray_level = 255;
		else if(gray_level < 0) gray_level = 0;

		brush[RxData[5]][i].CreateSolidBrush(RGB(gray_level, gray_level, gray_level));				 // zd mod use corrected adc data with green tint
//		brush[RxData[5]][i].CreateSolidBrush(RGB(255,0,0));		// ȡ��byteΪ��Чλ
//		brush[RxData[5]][i].CreateSolidBrush(RGB(RxData[i*2+6],RxData[i*2+6],RxData[i*2+6]));		// ȡ��byteΪ��Чλ
	}

	switch (PCRNum)
	{
	case 1:
		if (typePixelSize == 12)		// 12�����
		{
			xOffset = 0;
			yOffset = 0;
		}
		else		// 24�����
		{
			xOffset = 0;
			yOffset = 0;
		}
		break;
	case 2:
		if (typePixelSize == 12)
		{
			xOffset = pixelsize12*12;		// offset = pixelsize * rowNum		
			yOffset = 0;
		}
		else
		{
			xOffset = pixelsize24*24;
			yOffset = 0;
		}
		break;
	case 3:
		if (typePixelSize == 12)
		{
			xOffset = 0;
			yOffset = pixelsize12*12;
		}
		else
		{
			xOffset = 0;
			yOffset = pixelsize24*24;
		}
		break;
	case 4:
		if (typePixelSize == 12)
		{
			xOffset = pixelsize12*12;
			yOffset = pixelsize12*12;
		}
		else
		{
			xOffset = pixelsize24*24;
			yOffset = pixelsize24*24;
		}
		break;
	default:
		break;
	}

	switch(PCRTypeFilterClass)
	{
	case dprow12:	
		{
			for(int l=0; l<ColNum12; l++)		// l�����кţ�rxdata[5]���������к�
			{
				displayRect.SetRect((pixelsize12*l + xOffset),(pixelsize12*RxData[5] + yOffset),(pixelsize12*(l+1) + xOffset),(pixelsize12*(RxData[5]+1) + yOffset));
				bgDC_Row.Rectangle(displayRect);
				bgDC_Row.FillRect(&displayRect,&brush[RxData[5]][l]);
			}

			if (PCRMask == 0)	// ��ѡPCR��ȫ������
			{
				PCRNum = 0;

				OnPaint_Flag = DRAWROW;
				Invalidate();
			}
			else
			{
				//���ݴ���buffer����
				memset(RxData,0,sizeof(RxData));

				cycleRow12();		// ������һ��PCR����ָ��
			}	

			break;
		}
	case dppage12:	
		{
			for(int l=0; l<ColNum12; l++)		// l�����к�
			{
				if(m_GainMode <= 1) 
				{
					displayRect.SetRect((pixelsize12*l + xOffset),(pixelsize12*RxData[5] + yOffset),(pixelsize12*(l+1) + xOffset),(pixelsize12*(RxData[5]+1) + yOffset));
					bgDC.Rectangle(displayRect);
					bgDC.FillRect(&displayRect,&brush[RxData[5]][l]);
				}
				else
				{
					//zd add
					if(!hdr_phase) {
						pdata_0[rn][l] = adc_result[l];
					}
					else {
						pdata_1[rn][l] = adc_result[l];
					}
				}
			}

			// ��ҳʱÿ�ζ�ȡһ�У����ݴ�����ٶ���һ��
/*			if (Gra_pageFlag)	// ��ǰҳ��δ����	
			{
				//���ݴ���buffer����
				memset(RxData,0,sizeof(RxData));
				
				// ��ȡ��һ��
				GraFlag = READGRAMSG;
				GraCalMainMsg();
			} 
			else	// ��ǰҳ�ѻ���
			{
				if (PCRMask == 0)	// ������ѡPCR�ѻ���
				{
					PCRNum = 0;
					OnPaint_Flag = DRAWPAGE;
					Invalidate();

					Gra_videoFlag = true;		// videoģʽʱ����ʼ������һ��

					if (PollingGraMode)		// polling mode
					{
						// update PCRMask status
						GraFlag = GETPCRMASK;
						GraCalMainMsg();

						ReceiveTemCycNum++;
						GraFlag = STARTTEMHID;
						GraCalMainMsg();
					}
				}
				else	// ����һ��PCR
				{
					RxData [0] = 0xbb;
					//���ݴ���buffer����
					memset(RxData,0,sizeof(RxData));

					cyclePage12();	// ������һ����ҳָ��
				}
			}		 */	
			break;
		}
	case 0x0c:		// ����ʱ�䳬��10ms
		{
			//���ݴ���buffer����
			memset(RxData,0,sizeof(RxData));

			break;
		}
	case dprow24:	
		{
			for(int l=0; l<ColNum24; l++)		// l�����кţ�rxdata[5]���������к�
			{
				displayRect.SetRect((pixelsize24*l + xOffset),(pixelsize24*RxData[5] + yOffset),(pixelsize24*(l+1) + xOffset),(pixelsize24*(RxData[5]+1) + yOffset));
				bgDC_Row.Rectangle(displayRect);
				bgDC_Row.FillRect(&displayRect,&brush[RxData[5]][l]);
			}

			if (PCRMask == 0)		// ��ѡPCR��ȫ������
			{
				PCRNum = 0;

				OnPaint_Flag = DRAWROW;
				Invalidate();
			}
			else
			{
				//���ݴ���buffer����
				memset(RxData,0,sizeof(RxData));

				cycleRow24();		// ������һ��PCR����ָ��
			}
			break;
		}
	case dppage24:	
		{
			for(int l=0; l<ColNum24; l++)		// l�����к�
			{
				if(m_GainMode <= 1) 
				{
					displayRect.SetRect((pixelsize24*l + xOffset),(pixelsize24*RxData[5] + yOffset),(pixelsize24*(l+1) + xOffset),(pixelsize24*(RxData[5]+1) + yOffset));
					bgDC.Rectangle(displayRect);
					bgDC.FillRect(&displayRect,&brush[RxData[5]][l]);
				}
				else
				{
					//zd add
					if(!hdr_phase) {
						pdata_0[rn][l] = adc_result[l];
					}
					else {
						pdata_1[rn][l] = adc_result[l];
					}
				}
			}
			
			// ��ҳʱÿ�ζ�ȡһ�У����ݴ�����ٶ���һ��
/*			if (Gra_pageFlag)	// ��ǰҳ��δ����	
			{
				//���ݴ���buffer����
				memset(RxData,0,sizeof(RxData));

				// ��ȡ��һ��
				GraFlag = READGRAMSG;
				GraCalMainMsg();
			}
			else	// ��ǰҳ�ѻ���
			{
				if (PCRMask == 0)	// ������ѡPCR�ѻ���
				{
					PCRNum = 0;
					OnPaint_Flag = DRAWPAGE;
					Invalidate();

					Gra_videoFlag = true;		// videoģʽʱ����ʼ������һ��

					if (PollingGraMode)		// polling mode
					{
						// update PCRMask status
						GraFlag = GETPCRMASK;
						GraCalMainMsg();

						ReceiveTemCycNum++;
						GraFlag = STARTTEMHID;
						GraCalMainMsg();
					}
				}
				else
				{
					//���ݴ���buffer����
					memset(RxData,0,sizeof(RxData));
					cyclePage24();	// ������һ����ҳָ��
				}
			}	*/
			break;
		}
	case dpvideo12:	//video12
		{
			//���ݴ���buffer����
			memset(RxData,0,sizeof(RxData));

			break;
		}
	case dpvideo24:	//video24
		{
			//���ݴ���buffer����
			memset(RxData,0,sizeof(RxData));

			break;
		}

	default:
		break;
	}

	// release memeory
	for (int i=0; i<RowNum24; i++)
	{
		for (int j=0; j<ColNum24; j++)
			brush[i][j].DeleteObject();
	}

}


// CGraDlg message handlers


void CGraDlg::OnClickedBtnReadrow()
{
	// TODO: Add your control notification handler code here

	// update PCRMask status
	GraFlag = GETPCRMASK;
	GraCalMainMsg();

	if (PCRMask == 0)
	{
		AfxMessageBox("Please check the PCR mask");
		return;
	}
	else
	{
		// initial cycle number
		PCRCycCnt =0;
		
		// polling PCR mask
		int i=0;
		while(PollingPCRMask)
		{
			if (PCRMask & (0x01 << i))
			{
				PollingPCRMask = FALSE;		// stop polling
				PCRCycCnt = i;				// ��������λ������PCRType(ע�⣺��λ����PCR����Ŵ�0��ʼ)
				PCRMask &= 0xFF << i+1;		// ��ѡPCRλ����
			}
			else
				i++;
		}
		PollingPCRMask = TRUE;		// polling��־��λ

		// �ж��ǲ��ǵ�һ�ΰ����а�ť
		if (DrawRowStart)
		{
			bBmp_Row.DeleteObject();
			bgDC_Row.SelectObject(oldBMP_Row);
			bgDC_Row.DeleteDC();
		}
		DrawRowStart = TRUE;

		CString sRownum;
		GetDlgItemText(IDC_EDIT_READROW,sRownum);
		GraBuf[0] = atoi(sRownum);

		// initial background DC
		RowDC = GetDlgItem(IDC_PCR2BMP) -> GetDC();
		bgDC_Row.CreateCompatibleDC(RowDC);

		bBmp_Row.CreateCompatibleBitmap(RowDC,240,240);
		oldBMP_Row = bgDC_Row.SelectObject(&bBmp_Row);
		bgDC_Row.SetBkColor(RGB(0,0,0));

		// start draw row
		PCRType = (PCRCycCnt<<4)|(0x01);	// �ֱ�����4��PCR��type

		GraFlag = SENDGRAMSG;
		MakeGraPacket(0x02,PCRType,GraBuf[0]);
		//Send message to main dialog
		GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����

	}

	
/*
	for(PCRCycCnt=0; PCRCycCnt<4; PCRCycCnt++)	// 4��PCR���η���
	{
		PCRType = (PCRCycCnt<<4)|(0x01);	// �ֱ�����4��PCR��type

		GraFlag = SENDGRAMSG;
		MakeGraPacket(0x02,PCRType,GraBuf[0]);
		//Send message to main dialog
		GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����
	}
*/	
}

void CGraDlg::cycleRow12(void)
{
	// initial cycle number
	PCRCycCnt =0;

	// polling PCR mask
	int i=0;
	while(PollingPCRMask)
	{
		if (PCRMask & (0x01 << i))
		{
			PollingPCRMask = FALSE;		// stop polling
			PCRCycCnt = i;				// ��������λ������PCRType(ע�⣺��λ����PCR����Ŵ�0��ʼ)
			PCRMask &= 0xFF << i+1;		// ��ѡPCRλ����
		}
		else
			i++;
	}
	PollingPCRMask = TRUE;		// polling��־��λ

	PCRType = (PCRCycCnt<<4)|(0x01);	// �ֱ�����4��PCR��type

	GraFlag = SENDGRAMSG;
	MakeGraPacket(0x02,PCRType,GraBuf[0]);
	//Send message to main dialog
	GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����
}


void CGraDlg::OnBnClickedBtnDprow24()
{
	// TODO: Add your control notification handler code here

	// update PCRMask status
	GraFlag = GETPCRMASK;
	GraCalMainMsg();
	
	if (PCRMask == 0)
	{
		AfxMessageBox("Please check the PCR mask");
		return;
	}
	else
	{
		// initial cycle number
		PCRCycCnt =0;

		// polling PCR mask
		int i=0;
		while(PollingPCRMask)
		{
			if (PCRMask & (0x01 << i))
			{
				PollingPCRMask = FALSE;		// stop polling
				PCRCycCnt = i;				// ��������λ������PCRType(ע�⣺��λ����PCR����Ŵ�0��ʼ)
				PCRMask &= 0xFF << i+1;		// ��ѡPCRλ����
			}
			else
				i++;
		}
		PollingPCRMask = TRUE;		// polling��־��λ

		// �ж��ǲ��ǵ�һ�ΰ����а�ť
		if (DrawRowStart)
		{
			bBmp_Row.DeleteObject();
			bgDC_Row.SelectObject(oldBMP_Row);
			bgDC_Row.DeleteDC();
		}
		DrawRowStart = TRUE;

		CString sRownum;
		GetDlgItemText(IDC_EDIT_READROW,sRownum);
		GraBuf[0] = atoi(sRownum);

		// initial background DC
		RowDC = GetDlgItem(IDC_PCR2BMP) -> GetDC();
		bgDC_Row.CreateCompatibleDC(RowDC);

		bBmp_Row.CreateCompatibleBitmap(RowDC,240,240);
		oldBMP_Row = bgDC_Row.SelectObject(&bBmp_Row);
		bgDC_Row.SetBkColor(RGB(0,0,0));

		// start draw row
		PCRType = (PCRCycCnt<<4)|(0x07);	// �ֱ�����4��PCR��type

		GraFlag = SENDGRAMSG;
		MakeGraPacket(0x02,PCRType,GraBuf[0]);
		//Send message to main dialog
		GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����

	}	
/*
	for(PCRCycCnt=0; PCRCycCnt<4; PCRCycCnt++)	// 4��PCR���η���
	{
		PCRType = (PCRCycCnt<<4)|(0x07);	// �ֱ�����4��PCR��type

		GraFlag = SENDGRAMSG;
		MakeGraPacket(0x02,PCRType,GraBuf[0]);
		//Send message to main dialog
		GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����
	}
*/
}


void CGraDlg::cycleRow24(void)
{
	// initial cycle number
	PCRCycCnt =0;

	// polling PCR mask
	int i=0;
	while(PollingPCRMask)
	{
		if (PCRMask & (0x01 << i))
		{
			PollingPCRMask = FALSE;		// stop polling
			PCRCycCnt = i;				// ��������λ������PCRType(ע�⣺��λ����PCR����Ŵ�0��ʼ)
			PCRMask &= 0xFF << i+1;		// ��ѡPCRλ����
		}
		else
			i++;
	}
	PollingPCRMask = TRUE;		// polling��־��λ

	PCRType = (PCRCycCnt<<4)|(0x07);	// �ֱ�����4��PCR��type

	GraFlag = SENDGRAMSG;
	MakeGraPacket(0x02,PCRType,GraBuf[0]);
	//Send message to main dialog
	GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����
}


//Start Video
void CGraDlg::OnBnClickedBtnDpvedio()
{
	// TODO: Add your control notification handler code here
/*
	if(!g_DeviceDetected) {
		MessageBox("ULS24 Device Not Attached");
		return;
	}

	if(!m_GainMode) {	
		SetGainMode(1);
	}
	else if(m_GainMode == 1) {
		SetGainMode(0);
	}
	else {
		MessageBox("Video in HDR mode not allowed");
		return;
	}

	GraFlag = SENDVIDEOMSG;
	Vedio12_24Flag = 0;		// 12��vedio
	Gra_videoFlag = true;	// video��ʼѭ��
	Gra_pageFlag = TRUE;	// ��ҳ��ʼѭ��

	GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����
*/
//	OnBnClickedButtonCapture();
	SetTimer(1,VEDIOPITCHTIME,NULL);		// 12��video

}


void CGraDlg::OnBnClickedBtnDpvideo24()
{
	// TODO: Add your control notification handler code here
/*
	if(!g_DeviceDetected) {
		MessageBox("ULS24 Device Not Attached");
		return;
	}

	if(!m_GainMode) {	
		SetGainMode(1);
	}
	else if(m_GainMode == 1) {
		SetGainMode(0);
	}
	else {
		MessageBox("Video in HDR mode not allowed");
		return;
	}

	GraFlag = SENDVIDEOMSG;
	Vedio12_24Flag = 1;		// 24��vedio
	Gra_videoFlag = true;	// video��ʼѭ��
	Gra_pageFlag = TRUE;	// ��ҳ��ʼѭ��

	//Send message to main dialog
	GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����
*/
//	OnBnClickedButtonCapture();
	SetTimer(1,VEDIOPITCHTIME,NULL);		// 12��video

}


//Stop Video
void CGraDlg::OnBnClickedBtnStopvideo()
{
	// TODO: Add your control notification handler code here
/*
	if(!g_DeviceDetected) {
		MessageBox("ULS24 Device Not Attached");
		return;
	}
	
	GraFlag = SENDVIDEOMSG;
	Gra_videoFlag = false;
	memset(TxData,0,sizeof(TxData));
	memset(GraBuf,0,sizeof(GraBuf));
	GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����
	
/*
//---------------------------------------------------------------
// ԭvideo���2014-10-10��Ϊ�û�ҳ�ķ�����ʾvideo���������
//---------------------------------------------------------------
	GraFlag = SENDGRAMSG;

	TxData[0] = 0xaa;		//preamble code
	TxData[1] = 0x02;		//command
	TxData[2] = 0x0C;		//data length
	TxData[3] = 0x03;		//data type, date edit first byte
	TxData[4] = 0x01;		//real data
	TxData[5] = 0x00;		//Ԥ��λ
	TxData[6] = 0x00;
	TxData[7] = 0x00;
	TxData[8] = 0x00;
	TxData[9] = 0x00;
	TxData[10] = 0x00;
	TxData[11] = 0x00;
	TxData[12] = 0x00;
	TxData[13] = 0x00;
	TxData[14] = 0x00;
	TxData[15] = TxData[1]+TxData[2]+TxData[3]+TxData[4]+TxData[5]+TxData[6]+TxData[7]+TxData[8]+TxData[9]
	+TxData[10]+TxData[11]+TxData[12]+TxData[13]+TxData[14];		//check sum
	if (TxData[15]==0x17)
		TxData[15]=0x18;
	else
		TxData[15]=TxData[15];
	TxData[16] = 0x17;		//back code
	TxData[17] = 0x17;		//back code


	//Send message to main dialog
	GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����
*/

	KillTimer(1);		// stop video
}


void CGraDlg::OnBnClickedBtnClear()
{
	// TODO: Add your control notification handler code here
	//reset picture data
	for(int ii=0;ii<4;ii++)
		for(int ij=0;ij<4;ij++)
			m_yData[ii][ij].clear();

	m_PixelData.Empty();
	SetDlgItemText(IDC_EDIT_RecData,m_PixelData);
}


void CGraDlg::OnBnClickedBtnAdcconvert()
{
	// TODO: Add your control notification handler code here
	char ch[2] = {'a','b'};
	TRACE("%s",ch);
}


void CGraDlg::OnBnClickedBtnClearadc()
{
	// TODO: Add your control notification handler code here
	//reset picture data
	for(int ii=0;ii<4;ii++)
		for(int ij=0;ij<4;ij++)
			m_yData[ii][ij].clear();
	m_ADCRecdata.Empty();
	SetDlgItemText(IDC_EDIT_ADCDATA,m_ADCRecdata);
}


void CGraDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	CString str;
	// �������Ի���
	CFileDialog saveDlg(FALSE,".txt",
		NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"�ı��ļ�(*.txt)|*.txt|""�ĵ��ļ�(*.doc)|*.doc|""����ļ�(*.xls)|*.xls|""All Files(*.*)|*.*||",
		NULL,0,TRUE);

	//	saveDlg.m_ofn.lpstrInitialDir = "c:\\";		// ���Ի���Ĭ��·��Ϊc��

	// ���б��涯��
	if (saveDlg.DoModal() == IDOK)
	{
		ofstream ofs(saveDlg.GetPathName());
		CStatic * pst = (CStatic*)GetDlgItem(IDC_EDIT_RecData);	// ��ȡҪ����༭��ؼ��ڵ�����
		// IDC_EDIT_FILE�Ǳ༭��ؼ����
		pst->GetWindowTextA(str);
		ofs << str;
	}
}


void CGraDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here

	CString str;
	// �������Ի���
	CFileDialog saveDlg(FALSE,".txt",
		NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"Text(*.txt)|*.txt|""Word doc(*.doc)|*.doc|""All Files(*.*)|*.*||",
		NULL,0,TRUE);

	//	saveDlg.m_ofn.lpstrInitialDir = "c:\\";		// ���Ի���Ĭ��·��Ϊc��

	// ���б��涯��
	if (saveDlg.DoModal() == IDOK)
	{
		ofstream ofs(saveDlg.GetPathName());
		CStatic * pst = (CStatic*)GetDlgItem(IDC_EDIT_ADCDATA);	// ��ȡҪ����༭��ؼ��ڵ�����
		// IDC_EDIT_FILE�Ǳ༭��ؼ����
		pst->GetWindowTextA(str);
		ofs << str;
	}
}


void CGraDlg::OnBnClickedRadioAdcdata()
{
	// TODO: Add your control notification handler code here
}


void CGraDlg::SetGainMode(int gain)
{
	if (gain == gain_mode) return;

/*	GraFlag = SENDGRAMSG;

	if (!gain)
	{
		TxData[0] = 0xaa;		//preamble code
		TxData[1] = 0x01;		//command
		TxData[2] = 0x02;		//data length
		TxData[3] = 0x07;		//data type, date edit first byte
		TxData[4] = 0x00;	//real data, date edit second byte
		//0x01 means send vedio data
		//0x00 means stop vedio data
		TxData[5] = TxData[1]+TxData[2]+TxData[3]+TxData[4];		//check sum
		if (TxData[5]==0x17)
			TxData[5]=0x18;
		else
			TxData[5]=TxData[5];
		TxData[6] = 0x17;		//back code
		TxData[7] = 0x17;		//back code

		//Call maindlg message
//		GraCalMainMsg();
		gain_mode = 0;
	}
	else	
	{	
		TxData[0] = 0xaa;		//preamble code
		TxData[1] = 0x01;		//command
		TxData[2] = 0x02;		//data length
		TxData[3] = 0x07;		//data type, date edit first byte
		TxData[4] = 0x01;	//real data, date edit second byte
		TxData[5] = TxData[1]+TxData[2]+TxData[3]+TxData[4];		//check sum
		if (TxData[5]==0x17)
			TxData[5]=0x18;
		else
			TxData[5]=TxData[5];
		TxData[6] = 0x17;		//back code
		TxData[7] = 0x17;		//back code

		//Call maindlg message
//			GraCalMainMsg();
		gain_mode = 1;
	}

	GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����
*/



#ifdef DARK_MANAGE

	// Zhimin Ding added: set V20 for each sensor individually

	if(g_pTrimDlg && g_pTrimReader) {
		g_pTrimDlg->SetGainMode(gain, 1);
		if(!gain) g_pTrimDlg->SetV20(g_pTrimReader->Node[0].auto_v20[1], 1); 
		else g_pTrimDlg->SetV20(g_pTrimReader->Node[0].auto_v20[0], 1); 

		g_pTrimDlg->SetGainMode(gain, 2);
		if(!gain) g_pTrimDlg->SetV20(g_pTrimReader->Node[1].auto_v20[1], 2); 
		else g_pTrimDlg->SetV20(g_pTrimReader->Node[1].auto_v20[0], 2); 

		g_pTrimDlg->SetGainMode(gain, 3);
		if(!gain) g_pTrimDlg->SetV20(g_pTrimReader->Node[2].auto_v20[1], 3); 
		else g_pTrimDlg->SetV20(g_pTrimReader->Node[2].auto_v20[0], 3); 

		g_pTrimDlg->SetGainMode(gain, 4);
		if(!gain) g_pTrimDlg->SetV20(g_pTrimReader->Node[3].auto_v20[1], 4); 
		else g_pTrimDlg->SetV20(g_pTrimReader->Node[3].auto_v20[0], 4); 
	}

#endif

}


void CGraDlg::DisplayHDR(void)
{	
	CDC *pDC;		//����Ŀ��DCָ��
	pDC=GetDlgItem(IDC_Bmp)->GetDC();	

	CRect   rect;
	CBrush brush[12][12];	
	int i, j;
	int gray_level;
	int pdata[12][12];

	for (i=0; i<12; i++)
	{
		for(j=0; j<12; j++)
		{
			if(pdata_1[i][j] < 400) pdata[i][j] = pdata_0[i][j];
			else pdata[i][j] = (pdata_1[i][j]-100)*8 + 100;
		}
	}

	for (i=0; i<12; i++)
	{
		for(j=0; j<12; j++)
		{
			gray_level = pdata[i][j]/contrast;
			if(gray_level > 255) gray_level = 255;
			else if(gray_level < 0) gray_level = 0;
			brush[i][j].CreateSolidBrush(RGB(gray_level, gray_level, gray_level));		
		}
	}

	for(i=0; i<12; i++) {
			for(j=0; j<12; j++)		// l�����к�
			{
				rect.SetRect(pixelsize12*j,pixelsize12*i,pixelsize12*(j+1),pixelsize12*(i+1));
				pDC->Rectangle(rect);
				pDC->FillRect(&rect,&brush[i][j]);
			}
	}

	CString sRownum;
	char fstrbuf[10];
	CString sADCData;	// �����ʾ��ADC�����ַ�?	

	for (i=0; i<12; i++)
	{
		sRownum.Format(" %d",i);
		for(j=0; j<12; j++)
		{
			itoa (pdata[i][j],fstrbuf,10);		//�����ת���ַ�����ʾ
			sADCData += fstrbuf;
			sADCData += " ";
		}
		sADCData += sRownum;

		if(m_PixOut) {
			m_ADCRecdata += (sADCData+"\r\n");
			SetDlgItemText(IDC_EDIT_ADCDATA,m_ADCRecdata);		//ÿ�����ݽ��ӻس�
			//���������ڱ༭�����һ����ʾ
			//�༭��ֱ�������׶�
			POINT pt;
			GetDlgItem(IDC_EDIT_ADCDATA)->GetScrollRange(SB_VERT,(LPINT)&pt.x,(LPINT)&pt.y);
			pt.x=0;
			GetDlgItem(IDC_EDIT_ADCDATA)->SendMessage(EM_LINESCROLL,pt.x,pt.y);
		}
		sADCData = "";
	}
}


void CGraDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	if(m_GainMode <= 1) return; // only works for HDR mode.

	int pos;

/*	CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);  

    // Check which slider sent the notification  
    if (pSlider == &c_Slider1)  
    {  
    }
    else if (pSlider == &c_Slider2)  
    {  
    }  
*/
    // Check what happened  
    switch(nSBCode)
    {
    case TB_LINEUP:  
    case TB_LINEDOWN:  
    case TB_PAGEUP:  
    case TB_PAGEDOWN:  
    case TB_THUMBPOSITION: 
		pos = nPos;
		contrast = 116 - pos;
		if(!m_FrameSize)
			DisplayHDR();
		else
			DisplayHDR24();
		break;
    case TB_TOP:  
    case TB_BOTTOM:  
    case TB_THUMBTRACK:  
    case TB_ENDTRACK:  
    default:  
        break;  
    }

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CGraDlg::DisplayHDR24(void)
{
	CDC *pDC;		//����Ŀ��DCָ��
	pDC=GetDlgItem(IDC_Bmp)->GetDC();	

	CRect   rect;
	CBrush brush[24][24];	
	int i, j;
	int gray_level;
	int pdata[24][24];

	for (i=0; i<24; i++)
	{
		for(j=0; j<24; j++)
		{
			if(pdata_1[i][j] < 400) pdata[i][j] = pdata_0[i][j];
			else pdata[i][j] = (pdata_1[i][j]-100)*8 + 100;
		}
	}

	for (i=0; i<24; i++)
	{
		for(j=0; j<24; j++)
		{
			gray_level = pdata[i][j]/contrast;
			if(gray_level > 255) gray_level = 255;
			else if(gray_level < 0) gray_level = 0;
			brush[i][j].CreateSolidBrush(RGB(gray_level, gray_level, gray_level));		
		}
	}

	for(i=0; i<24; i++) {
			for(j=0; j<24; j++)		// l�����к�
			{
				rect.SetRect(pixelsize24*j,pixelsize24*i,pixelsize24*(j+1),pixelsize24*(i+1));
				pDC->Rectangle(rect);
				pDC->FillRect(&rect,&brush[i][j]);
			}
	}

	CString sRownum;
	char fstrbuf[10];
	CString sADCData;	// �����ʾ��ADC�����ַ�?	

	for (i=0; i<24; i++)
	{
		sRownum.Format(" %d",i);
		for(j=0; j<24; j++)
		{
			itoa (pdata[i][j],fstrbuf,10);		//�����ת���ַ�����ʾ
			sADCData += fstrbuf;
			sADCData += " ";
		}
		sADCData += sRownum;

		if(m_PixOut) {
			m_ADCRecdata += (sADCData+"\r\n");
			SetDlgItemText(IDC_EDIT_ADCDATA,m_ADCRecdata);		//ÿ�����ݽ��ӻس�
			//���������ڱ༭�����һ����ʾ
			//�༭��ֱ�������׶�
			POINT pt;
			GetDlgItem(IDC_EDIT_ADCDATA)->GetScrollRange(SB_VERT,(LPINT)&pt.x,(LPINT)&pt.y);
			pt.x=0;
			GetDlgItem(IDC_EDIT_ADCDATA)->SendMessage(EM_LINESCROLL,pt.x,pt.y);
		}
		sADCData = "";
	}
}


void CGraDlg::OnClickedRadiolowgain()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}


void CGraDlg::OnRadiohighgain()
{
	// TODO: Add your command handler code here
	UpdateData(true);
}


void CGraDlg::OnRadiohdr()
{
	// TODO: Add your command handler code here
	UpdateData(true);
}


void CGraDlg::OnClickedRadio12()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	GraFlag = SENDGRAMSG;

	TxData[0] = 0xaa;		//preamble code
	TxData[1] = 0x01;		//command  TXC
	TxData[2] = 0x05;		//data length
	TxData[3] = 0x25;		//data type
	TxData[4] = 0x00;		
	TxData[5] = 0x00;	
	TxData[6] = 0x00;       
	TxData[7] = 0x00;	
	for (int i=1; i<8; i++)
		TxData[8] += TxData[i];
	if (TxData[8]==0x17)
		TxData[8]=0x18;
	else
		TxData[8]=TxData[8];
	TxData[9]=0x17;
	TxData[10]=0x17;

	//Send message to main dialog
	GraCalMainMsg();	//�������Ի�������Ϣ����		
}


void CGraDlg::OnRadio24()
{	
	// TODO: Add your command handler code here
	UpdateData(true);

	GraFlag = SENDGRAMSG;

	TxData[0] = 0xaa;		//preamble code
	TxData[1] = 0x01;		//command  TXC
	TxData[2] = 0x05;		//data length
	TxData[3] = 0x25;		//data type
	TxData[4] = 0x01;		
	TxData[5] = 0x00;	
	TxData[6] = 0x00;       
	TxData[7] = 0x00;	
	for (int i=1; i<8; i++)
		TxData[8] += TxData[i];
	if (TxData[8]==0x17)
		TxData[8]=0x18;
	else
		TxData[8]=TxData[8];
	TxData[9]=0x17;
	TxData[10]=0x17;

	//Send message to main dialog
	GraCalMainMsg();	//�������Ի�������Ϣ����
		
}


void CGraDlg::OnBnClickedButtonCapture()
{
	// TODO: Add your control notification handler code here
	if (!g_DeviceDetected) {
		MessageBox("PDx16 Device Not Attached");
		return;
	}
	else if (ReceiveTemCycNum > 0)
	{
		AfxMessageBox("No manual capture when temp cycle in progress!");
		return;
	}
	CaptureFrame();
}

void CGraDlg::CaptureFrame()
{
	// ��ȡPCR mask��Ϣ
	GraFlag = GETPCRMASK;
	GraCalMainMsg();

	if (PCRMask == 0)
	{
		AfxMessageBox("Please check the PCR mask");
		return;
	}
	else
	{
		// initial cycle flag
		PCRCycCnt = 0;

		// �ж��ǲ��ǰ���һ�λ�ͼ��ť��capture��
		if (DrawPageStart)
		{
			bBmp.DeleteObject();
			bgDC.SelectObject(oldBMP);
			bgDC.DeleteDC();
		}
		DrawPageStart = TRUE;

		// initial background DC 
		dispalyDC = GetDlgItem(IDC_Bmp) -> GetDC();
		bgDC.CreateCompatibleDC(dispalyDC);

		bBmp.CreateCompatibleBitmap(dispalyDC,240,240);
		oldBMP = bgDC.SelectObject(&bBmp);
		bgDC.SetBkColor(RGB(0,0,0));

		//-----------------------------------------------------
		// �����༭�򱣴���ļ�
		//-----------------------------------------------------

#ifdef SAVE_DATA_FILE

		CString sRecDataFileName,sADCDataFileName;
		sRecDataFileName.Empty();
		sADCDataFileName.Empty();

		// ����ʱʱ�䴴���ļ���
		CString sTime_Gra;					// system string format
		sTime_Gra.Empty();
		sysTime_Gra = CTime::GetCurrentTime();
		sTime_Gra.Format("%4d-%.2d-%.2d_%.2d%.2d%.2d",
			sysTime_Gra.GetYear(),sysTime_Gra.GetMonth(),sysTime_Gra.GetDay(),
			sysTime_Gra.GetHour(),sysTime_Gra.GetMinute(),sysTime_Gra.GetSecond());
		sRecDataFileName = "DisplayData_" + sTime_Gra + ".txt";
		sADCDataFileName = "ADCData_" + sTime_Gra + ".txt";

		if (m_recdataFile.m_hFile != CFile::hFileNull)
			m_recdataFile.Close();

		if (m_adcdataFile.m_hFile != CFile::hFileNull)
			m_adcdataFile.Close();

		m_recdataFile.Open(sRecDataFileName,CFile::modeCreate|CFile::modeWrite);
		m_adcdataFile.Open(sADCDataFileName,CFile::modeCreate|CFile::modeWrite);

#endif

		if (!m_GainMode) {
			SetGainMode(1);
		}
		else {
			SetGainMode(0);
		}

		// ������ʾָ��
		if(!m_FrameSize) {
			typePixelSize = 12;
			if(m_GainMode <= 1) {
				CaptureFrame2();
			}
//			else {
//				OnBnClickedBtnDppage12();	//!!!!!û���õ����ɵ�ѭ������ģʽ
//			}
		}
		else {
			typePixelSize = 24;
			if(m_GainMode <= 1) {
				CaptureFrame2();			// Zhimin mod
			}
//			else {
//				OnBnClickedBtnDppage24();	//!!!!!û���õ����ɵ�ѭ������ģʽ
//			}
			if(g_pTrimDlg) g_pTrimDlg->ResetTxBin();
		}
	}
}


void CGraDlg::CaptureFrame2(void)
{
	int i = 0;

	// initial cycle number
	PCRCycCnt = 0;

	// polling PCR mask
	while (PollingPCRMask)
	{
		if (PCRMask & (0x01 << i))
		{
			PollingPCRMask = FALSE;		// stop polling
			PCRCycCnt = i;				// ��������λ������PCRType(ע�⣺��λ����PCR����Ŵ�0��ʼ)
			PCRMask &= 0xFF << i + 1;		// ��ѡPCRλ����
		}
		else
			i++;
	}

	PollingPCRMask = TRUE;		// polling��־��λ

	if (!m_FrameSize) {
		PCRType = (PCRCycCnt << 4) | (0x02);	// �ֱ�����4��PCR��type
	}
	else {
		PCRType = (PCRCycCnt << 4) | (0x08);	// �ֱ�����4��PCR��type
	}

	GraFlag = SENDPAGEMSG;
	Gra_pageFlag = TRUE;	// ��ʼѭ��

	MakeGraPacket(0x02,PCRType,0xff);
	
	//Send message to main dialog
	GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����

	while (Gra_pageFlag)	// ��ǰҳ��δ����	
	{
		//���ݴ���buffer����
		memset(RxData, 0, sizeof(RxData));

		// ��ȡ��һ��
		GraFlag = READGRAMSG;
		GraCalMainMsg();
	}

	while (PCRMask != 0)	// ������ѡPCR�ѻ���
	{
		RxData[0] = 0xbb;
		//���ݴ���buffer����
		memset(RxData, 0, sizeof(RxData));

		cyclePages();	// ������һ����ҳָ��

		while (Gra_pageFlag)	// ��ǰҳ��δ����	
		{
			//���ݴ���buffer����
			memset(RxData, 0, sizeof(RxData));

			// ��ȡ��һ��
			GraFlag = READGRAMSG;
			GraCalMainMsg();
		}

	}

	PCRNum = 0;
	OnPaint_Flag = DRAWPAGE;
	Invalidate();

	Gra_videoFlag = true;		// videoģʽʱ����ʼ������һ��

	if (PollingGraMode)		// polling mode
	{
		// update PCRMask status
		GraFlag = GETPCRMASK;
		GraCalMainMsg();

		ReceiveTemCycNum++;
		GraFlag = STARTTEMHID;
		GraCalMainMsg();
	}
}


void CGraDlg::cyclePages(void)
{
	// initial cycle number
	PCRCycCnt =0;

	// polling PCR mask
	int i=0;
	while(PollingPCRMask)
	{
		if (PCRMask & (0x01 << i))
		{
			PollingPCRMask = FALSE;		// stop polling
			PCRCycCnt = i;				// ��������λ������PCRType(ע�⣺��λ����PCR����Ŵ�0��ʼ)
			PCRMask &= 0xFF << i+1;		// ��ѡPCRλ����
		}
		else
			i++;
	}
	PollingPCRMask = TRUE;		// polling��־��λ

	if (!m_FrameSize) {
		PCRType = (PCRCycCnt << 4) | (0x02);	// �ֱ�����4��PCR��type
	}
	else {
		PCRType = (PCRCycCnt << 4) | (0x08);	// �ֱ�����4��PCR��type
	}

	GraFlag = SENDPAGEMSG;
	Gra_pageFlag = TRUE;	// ��ʼѭ��

	MakeGraPacket(0x02,PCRType,0xff);

	//Send message to main dialog
	GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����
}

/*
void CGraDlg::CaptureFrame24(void)
{
	// initial cycle number
	PCRCycCnt =0;

	// polling PCR mask
	int i=0;
	while(PollingPCRMask)
	{
		if (PCRMask & (0x01 << i))
		{
			PollingPCRMask = FALSE;		// stop polling
			PCRCycCnt = i;				// ��������λ������PCRType(ע�⣺��λ����PCR����Ŵ�0��ʼ)
			PCRMask &= 0xFF << i+1;		// ��ѡPCRλ����
		}
		else
			i++;
	}
	PollingPCRMask = TRUE;		// polling��־��λ

	if(!m_GainMode) {	
		SetGainMode(1);
	}
	else {
		SetGainMode(0);
	}

	if(g_pTrimDlg) g_pTrimDlg->ResetTxBin();

	PCRType = (PCRCycCnt<<4)|(0x08);	// �ֱ�����4��PCR��type

	GraFlag = SENDPAGEMSG;
	Gra_pageFlag = TRUE;	// ��ʼѭ��

	MakeGraPacket(0x02,PCRType,0xff);

	//Send message to main dialog
	GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����	
}

void CGraDlg::cyclePage24(void)
{
	// initial cycle number
	PCRCycCnt =0;

	// polling PCR mask
	int i=0;
	while(PollingPCRMask)
	{
		if (PCRMask & (0x01 << i))
		{
			PollingPCRMask = FALSE;		// stop polling
			PCRCycCnt = i;				// ��������λ������PCRType(ע�⣺��λ����PCR����Ŵ�0��ʼ)
			PCRMask &= 0xFF << i+1;		// ��ѡPCRλ����
		}
		else
			i++;
	}
	PollingPCRMask = TRUE;		// polling��־��λ

	PCRType = (PCRCycCnt<<4)|(0x08);	// �ֱ�����4��PCR��type

	GraFlag = SENDPAGEMSG;
	Gra_pageFlag = TRUE;	// ��ʼѭ��

	MakeGraPacket(0x02,PCRType,0xff);

	//Send message to main dialog
	GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����
}
*/

void CGraDlg::OnBnClickedBtnDppage12()		//!!!!!û���õ����ɵ�ѭ������ģʽ
{
	// TODO: Add your control notification handler code here

	for(PCRCycCnt=0; PCRCycCnt<4; PCRCycCnt++)	// 4��PCR���η���
	{
		PCRType = (PCRCycCnt<<4)|(0x02);	// �ֱ�����4��PCR��type

		SetGainMode(0);
		hdr_phase = 0;

		GraFlag = SENDPAGEMSG;
		Gra_pageFlag = TRUE;	// ��ʼѭ��

		MakeGraPacket(0x02,PCRType,0xff);

		//Send message to main dialog
		GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����

		//===========
		SetGainMode(1);
		hdr_phase = 1;

		GraFlag = SENDPAGEMSG;
		Gra_pageFlag = TRUE;	// ��ʼѭ��

		MakeGraPacket(0x02,PCRType,0xff);

		//Send message to main dialog
		GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����

		DisplayHDR();
	}

}


void CGraDlg::OnBnClickedBtnDppage24()		//!!!!!û���õ����ɵ�ѭ������ģʽ
{
	// TODO: Add your control notification handler code here

	for(PCRCycCnt=0; PCRCycCnt<4; PCRCycCnt++)	// 4��PCR���η���
	{
		PCRType = (PCRCycCnt<<4)|(0x08);	// �ֱ�����4��PCR��type

		SetGainMode(0);
		hdr_phase = 0;

		GraFlag = SENDPAGEMSG;
		Gra_pageFlag = TRUE;	// ��ʼѭ��

		MakeGraPacket(0x02,PCRType,0xff);

		//Send message to main dialog
		GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����

		//===========
		SetGainMode(1);
		hdr_phase = 1;

		GraFlag = SENDPAGEMSG;
		Gra_pageFlag = TRUE;	// ��ʼѭ��

		MakeGraPacket(0x02,PCRType,0xff);

		//Send message to main dialog
		GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����

		//		DisplayHDR24();
	}
}


void CGraDlg::OnClickedCheckPixout()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}


void CGraDlg::OnClickedCheckOrigout()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}


BOOL CGraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	OnPaint_Flag = 0;
//	((CButton*)GetDlgItem(IDC_LED1_EN))->SetCheck(true);
//	((CButton*)GetDlgItem(IDC_LED2_EN))->SetCheck(true);
//	((CButton*)GetDlgItem(IDC_LED3_EN))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_LED_INDV_EN))->SetCheck(true);
	regDispatch();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CGraDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here

	switch (OnPaint_Flag)
	{
	case DRAWPAGE:
		{
			dispalyDC -> BitBlt(0,0,240,240,&bgDC,0,0,SRCCOPY);
			break;
		}
	case DRAWROW:
		{
			RowDC -> BitBlt(0,0,240,240,&bgDC_Row,0,0,SRCCOPY);
			break;
		}
	default:
		break;
	}

	// Do not call CDialogEx::OnPaint() for painting messages
}


void CGraDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	switch(nIDEvent)
	{
	case 1:		// ����video����͡����ݶ�ȡ
		{
			if (Gra_videoFlag)
			{
				Gra_videoFlag = false;		// videoģʽ��ȡ���ݼ����������timer����������
				CaptureFrame();
			}
			break;
		}
	case 2:			// graphic HID polling
		{
//			if ( timerCtrFlag & PollingGraTimerFlag &(!temReadTimerFlag))
			if ( timerCtrFlag & PollingGraTimerFlag )
			{
				GraFlag = SENDGRAMSG;

				TxData[0] = 0xaa;		//preamble code
				TxData[1] = 0x15;		//command  TXC
				TxData[2] = 0x05;		//data length
				TxData[3] = 0x01;		//data type
				TxData[4] = 0x00;		
				TxData[5] = 0x00;	
				TxData[6] = 0x00;       
				TxData[7] = 0x00;	
				for (int i=1; i<8; i++)
					TxData[8] += TxData[i];
				if (TxData[8]==0x17)
					TxData[8]=0x18;
				else
					TxData[8]=TxData[8];
				TxData[9]=0x17;
				TxData[10]=0x17;

				//Send message to main dialog
				GraCalMainMsg();	//�������Ի�������Ϣ����
			}
			break;
		}
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CGraDlg::OnClickedGradlgCheckAutosnap()
{
	// TODO: Add your control notification handler code here

	if(!g_DeviceDetected) {
		MessageBox("PDx16 Device Not Attached");
		return;
	}

	CButton *pCheckbox = (CButton*)GetDlgItem(IDC_GraDlg_Check_AutoSnap);
/*
	if(pCheckbox->GetCheck()) {
		PollingGraMode = TRUE;
		PollingGraTimerFlag = TRUE;
		SetTimer(2,PGRATIMERVALUE,NULL);		// start polling
	}
	else {
		PollingGraMode = FALSE;
		PollingGraTimerFlag = FALSE;
		KillTimer(2);		// stop polling
	}
*/
	TxData[0] = 0xaa;		//preamble code
	TxData[1] = 0x01;		//command
	TxData[2] = 0x02;		//data length
	TxData[3] = 0x28;		//data type, date edit first byte
	TxData[4] = (pCheckbox->GetCheck())? 1:0;		//real data, date edit second byte

	TxData[5] = TxData[1]+TxData[2]+TxData[3]+TxData[4];		//check sum
	if (TxData[5]==0x17)
		TxData[5]=0x18;
	else
		TxData[5]=TxData[5];
	TxData[6] = 0x17;		//back code
	TxData[7] = 0x17;		//back code
	GraFlag = SENDGRAMSG;

	//Send message to main dialog
	GraCalMainMsg();
}


void CGraDlg::OnBnClickedLedIndvEn()
{
	// TODO: Add your control notification handler code here
	if(((CButton*)GetDlgItem(IDC_LED_INDV_EN))->GetCheck()==BST_CHECKED)
	{
	   ((CButton*)GetDlgItem(IDC_LED2_EN))->SetCheck(false);
	   ((CButton*)GetDlgItem(IDC_LED3_EN))->SetCheck(false);
	   ((CButton*)GetDlgItem(IDC_LED4_EN))->SetCheck(false);

	   ((CButton*)GetDlgItem(IDC_LED2_EN))->EnableWindow(true);
	   ((CButton*)GetDlgItem(IDC_LED3_EN))->EnableWindow(true);
	   ((CButton*)GetDlgItem(IDC_LED4_EN))->EnableWindow(true);
	}
	else
	{   
	   ((CButton*)GetDlgItem(IDC_LED2_EN))->EnableWindow(false);
	   ((CButton*)GetDlgItem(IDC_LED3_EN))->EnableWindow(false);
	   ((CButton*)GetDlgItem(IDC_LED4_EN))->EnableWindow(false);
	}
}


void CGraDlg::OnBnClickedLedSwitch()
{
		if(!g_DeviceDetected) {
			MessageBox("PDx16 Device Not Attached");
			return;
		}

		TxData[0] = 0xaa;		//preamble code
		TxData[1] = 0x01;		//command
		TxData[2] = 0x02;		//data length
		TxData[3] = 0x23;		//data type, date edit first byte

		if(((CButton*)GetDlgItem(IDC_LED_INDV_EN))->GetCheck()==BST_UNCHECKED)
		{
			
			TxData[4] = (((CButton*)GetDlgItem(IDC_LED1_EN))->GetCheck()==BST_CHECKED)? 1:0;		//real data, date edit second byte
		}
		else
		{
			TxData[4] = 0x80;
			if((((CButton*)GetDlgItem(IDC_LED1_EN))->GetCheck()==BST_CHECKED))
				TxData[4] |= 1;
			if((((CButton*)GetDlgItem(IDC_LED2_EN))->GetCheck()==BST_CHECKED))
				TxData[4] |= 2;
			if((((CButton*)GetDlgItem(IDC_LED3_EN))->GetCheck()==BST_CHECKED))
				TxData[4] |= 4;
			if((((CButton*)GetDlgItem(IDC_LED4_EN))->GetCheck()==BST_CHECKED))
				TxData[4] |= 8;
		}
		TxData[5] = TxData[1]+TxData[2]+TxData[3]+TxData[4];		//check sum
		if (TxData[5]==0x17)
			TxData[5]=0x18;
		else
			TxData[5]=TxData[5];
		TxData[6] = 0x17;		//back code
		TxData[7] = 0x17;		//back code

		GraFlag = SENDGRAMSG;    
		//Send message to main dialog
		GraCalMainMsg();		//�������Ի��򴮿ڷ�����Ϣ����
}

void CGraDlg::OnBnClickedBtnCopy()
{
	// TODO: Add your control notification handler code here

	CEdit *myEdit;

	myEdit = (CEdit*)GetDlgItem(IDC_EDIT_ADCDATA);
	myEdit->SetSel(0, -1, FALSE);
	myEdit->Copy();
}



void CGraDlg::OnBnClickedPcrcurve()
{
//	CCurveShow dlgCurve;
//	if(dlgCurve.DoModal()==IDOK)
//	{
		
//	}
	
	if (!pCVS) {
		pCVS = new CCurveShow(this);

		pCVS->Create(IDD_CURVESHOW, this);
		pCVS->ShowWindow(SW_SHOW);
	}
	else {
		pCVS->SetForegroundWindow();
	}
}


void CGraDlg::regDispatch()
{
	CString strValue;
	CString path;
	TCHAR CurrentDirectory[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,CurrentDirectory);
	path = CurrentDirectory;
	path += "\\Trim\\dataposition.ini";
	std::regex pattern("[A-Za-z0-9+]+");
	std::smatch result;
	for(int i=0;i<4;i++)
	{
		CString strMatch;
		strMatch.Format("CHIP%d",i+1);
		GetPrivateProfileString("DATA POSITION",strMatch,NULL,strValue.GetBuffer(254),254,path);
		
		std::string str(strValue.GetBuffer());
		std::string::const_iterator it=str.begin();//ȡͷ 
		std::string::const_iterator end=str.end();//ȡβ
		if(std::regex_search(str,result,pattern)) 
		{
			while (std::regex_search(it,end,result,pattern))
			{
				m_PositionChip[i].push_back(result[0]);//����͵õ�����1
				it=result[0].second;  
			}
		}
	}
}
