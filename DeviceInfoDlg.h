#pragma once


// DeviceInfoDlg dialog

class DeviceInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DeviceInfoDlg)

public:
	DeviceInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~DeviceInfoDlg();

// Dialog Data
	enum { IDD = IDD_DEVICE_INFO_DLG};
	CComboBox	m_combo_well;
	CComboBox	m_combo_ch;
	CEdit	m_edit_well;
	CEdit	m_edit_ch;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboWell();
	afx_msg void OnSelchangeComboCh();
	DECLARE_MESSAGE_MAP()
public:
	CString m_NWELLS;
	CString m_NCHANNELS;
};
