// DeviceInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PCRProject.h"
#include "DeviceInfoDlg.h"
#include "afxdialogex.h"


// DeviceInfoDlg dialog

IMPLEMENT_DYNAMIC(DeviceInfoDlg, CDialogEx)

DeviceInfoDlg::DeviceInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(DeviceInfoDlg::IDD, pParent)
	, m_NWELLS(_T("04"))
	, m_NCHANNELS(_T("1"))
{

}

DeviceInfoDlg::~DeviceInfoDlg()
{
}

void DeviceInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_NWELLS);
	DDX_Text(pDX, IDC_EDIT2, m_NCHANNELS);
	DDX_Control(pDX, IDC_EDIT1, m_edit_well);
	DDX_Control(pDX, IDC_EDIT2, m_edit_ch);
	DDX_Control(pDX, IDC_COMBO_WELL, m_combo_well);
	DDX_Control(pDX, IDC_COMBO_CH, m_combo_ch);
}


BEGIN_MESSAGE_MAP(DeviceInfoDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_WELL, OnSelchangeComboWell)
	ON_CBN_SELCHANGE(IDC_COMBO_CH, OnSelchangeComboCh)
END_MESSAGE_MAP()

BOOL DeviceInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_combo_well.AddString("04");
	m_combo_well.AddString("08");
	m_combo_well.AddString("16");
	m_combo_well.SetCurSel(0);

	m_combo_ch.AddString("1");
	m_combo_ch.AddString("2");
	m_combo_ch.AddString("4");
	m_combo_ch.SetCurSel(0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void DeviceInfoDlg::OnSelchangeComboWell()
{
	// TODO: Add your control notification handler code here
	CString well;
	m_combo_well.GetLBText(m_combo_well.GetCurSel(), well);

	m_edit_well.SetWindowText(well);
	return;
}

void DeviceInfoDlg::OnSelchangeComboCh()
{
	// TODO: Add your control notification handler code here
	CString ch;
	m_combo_ch.GetLBText(m_combo_ch.GetCurSel(), ch);

	m_edit_ch.SetWindowText(ch);
	return;
}