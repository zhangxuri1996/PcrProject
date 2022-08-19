// excelTestDlg.h : header file
//

#if !defined(AFX_EXCELTESTDLG_H__A2F98AFD_9269_4655_8366_2D712A9522E8__INCLUDED_)
#define AFX_EXCELTESTDLG_H__A2F98AFD_9269_4655_8366_2D712A9522E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "excel.h"
/////////////////////////////////////////////////////////////////////////////
// CExcelTestDlg dialog

class CExcelTestDlg : public CDialog
{
// Construction
public:
	CExcelTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CExcelTestDlg)
	enum { IDD = IDD_EXCELTEST_DIALOG };
	CListBox	m_sheetList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExcelTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CExcelTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnSheet();
	afx_msg void OnBtnRead();
	afx_msg void OnBtnWrite();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXCELTESTDLG_H__A2F98AFD_9269_4655_8366_2D712A9522E8__INCLUDED_)
