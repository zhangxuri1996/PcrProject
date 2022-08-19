// IntTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PCRProject.h"
#include "IntTimeDlg.h"
#include "afxdialogex.h"

// IntTimeDlg dialog

IMPLEMENT_DYNAMIC(IntTimeDlg, CDialogEx)
char mFilePath[];
IntTimeDlg::IntTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IntTimeDlg::IDD, pParent)
	, m_ExpName(_T(""))
	, m_ExpName2(_T(""))
{
}

IntTimeDlg::~IntTimeDlg()
{
}


void IntTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT1, m_IntTime);
	//  DDV_MinMaxInt(pDX, (int)m_IntTime, 1, 20000);
	DDX_Text(pDX, IDC_EDIT1, m_ExpName);
	DDX_Text(pDX, IDC_EDIT2, m_ExpName2);
}


BEGIN_MESSAGE_MAP(IntTimeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &IntTimeDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON13, &IntTimeDlg::OnBnClickedButton13)
END_MESSAGE_MAP()


// IntTimeDlg message handlers


//void IntTimeDlg::OnBnClickedButtonNew()
//{
//	// TODO: Add your control notification handler code here
//	if (m_ExpName.GetLength() > 0)
//		OnOK();
//	else
//		MessageBox("Please enter an experiment name");
//}


void IntTimeDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

	CString sFilePath;
	CFileDialog FileOpenDialog(TRUE);

	FileOpenDialog.m_ofn.Flags = OFN_ENABLEHOOK | OFN_EXPLORER;

	CString strFilter;
	strFilter = _T("INI");
	strFilter += (TCHAR)'\0';
	strFilter += _T("*.ini");
	strFilter += (TCHAR)'\0';

	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';

	FileOpenDialog.m_ofn.lpstrFilter = strFilter;
	FileOpenDialog.m_ofn.lpstrTitle = "Download Filename...";

	CString sFileName;
	sFileName.Empty();

	BOOL bResult = FileOpenDialog.DoModal() == IDOK ? TRUE : FALSE;
	if (bResult)
	{
		sFilePath = FileOpenDialog.GetPathName();
		SetDlgItemText(IDC_EDIT2, sFilePath);
	}
}


void IntTimeDlg::OnBnClickedButton13()
{
	// TODO: Add your control notification handler code here
	CString sFilePath;
	CFileDialog FileOpenDialog(TRUE);

	FileOpenDialog.m_ofn.Flags = OFN_ENABLEHOOK | OFN_EXPLORER;

	CString strFilter;
	strFilter = _T("DAT");
	strFilter += (TCHAR)'\0';
	strFilter += _T("*.dat");
	strFilter += (TCHAR)'\0';

	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';

	FileOpenDialog.m_ofn.lpstrFilter = strFilter;
	FileOpenDialog.m_ofn.lpstrTitle = "Download Filename...";

	CString sFileName;
	sFileName.Empty();

	BOOL bResult = FileOpenDialog.DoModal() == IDOK ? TRUE : FALSE;
	if (bResult)
	{
		sFilePath = FileOpenDialog.GetPathName();
		SetDlgItemText(IDC_EDIT1, sFilePath);
	}

}
