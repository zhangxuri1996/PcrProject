#pragma once
// IntTimeDlg dialog
class IntTimeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(IntTimeDlg)

public:
	IntTimeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~IntTimeDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	float m_IntTime;
//	afx_msg void OnBnClickedButtonNew();
	// New experiment name
	CString m_ExpName;
	afx_msg void OnBnClickedButton2();
	CString m_ExpName2;
	afx_msg void OnBnClickedButton13();
};
