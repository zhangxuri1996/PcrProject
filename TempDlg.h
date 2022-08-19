#pragma once

// CTempDlg dialog

class CTempDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTempDlg)

public:
	CTempDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTempDlg();

	// Dialog Data
	enum { IDD = IDD_TEMPCTRL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnTempStart();
	afx_msg void OnBnClickedBtnTempStop();
	afx_msg void OnBnClickedBtnSaveData();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnTempCycPollProcess(WPARAM wParam, LPARAM lParam);
	void TempCalMainMsg();

	CString m_CHA1;
	CString m_CHA2;
	CString m_CHA3;
	CString m_CHA4;
	CString m_CHA5;
	CString m_CHA6;
	CString m_CHA7;
	CString m_CHA8;
	CString m_CHB1;
	CString m_CHB2;
	CString m_CHB3;
	CString m_CHB4;
	CString m_CHB5;
	CString m_CHB6;
	CString m_CHB7;
	CString m_CHB8;

};
