#pragma once
// FuncMenuDlg dialog
class FuncMenuDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FuncMenuDlg)

public:
	FuncMenuDlg(CWnd* pParent = NULL);
	virtual ~FuncMenuDlg();

// Dialog Data
	enum { IDD = IDD_FUNC_MENU_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChipSelection();
	afx_msg void OnBnClickedAbort();
};
