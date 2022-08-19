// FuncMenuDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PCRProject.h"
#include "FuncMenuDlg.h"
#include "afxdialogex.h"

// FuncMenuDlg dialog

IMPLEMENT_DYNAMIC(FuncMenuDlg, CDialogEx)
char mFilePath[];
FuncMenuDlg::FuncMenuDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(FuncMenuDlg::IDD, pParent)
	//, m_ExpName(_T(""))
	//, m_ExpName2(_T(""))
{
}

FuncMenuDlg::~FuncMenuDlg()
{
}


void FuncMenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT1, m_IntTime);
	//  DDV_MinMaxInt(pDX, (int)m_IntTime, 1, 20000);
	//DDX_Text(pDX, IDC_EDIT1, m_ExpName);
	//DDX_Text(pDX, IDC_EDIT2, m_ExpName2);
}


BEGIN_MESSAGE_MAP(FuncMenuDlg, CDialogEx)
	ON_BN_CLICKED(ID_CHIP_SELECTION, &FuncMenuDlg::OnBnClickedChipSelection)
	ON_BN_CLICKED(IDABORT, &FuncMenuDlg::OnBnClickedAbort)
END_MESSAGE_MAP()




void FuncMenuDlg::OnBnClickedChipSelection()
{
	// TODO: 在此添加控件通知处理程序代码
}


void FuncMenuDlg::OnBnClickedAbort()
{
	// TODO: 在此添加控件通知处理程序代码
}
