#pragma once


// CTheTestDlg dialog

class CTheTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTheTestDlg)

public:
	CTheTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTheTestDlg();

// Dialog Data
	enum { IDD = IDD_MYTESTDLG };
	CComboBox	m_combo_factory;
	CComboBox	m_combo_style;
	CComboBox	m_combo_well;
	CComboBox	m_combo_ch;
	CComboBox	m_combo_company;
	CComboBox	m_combo_well_format;
	CComboBox	m_combo_channel_format;
	CComboBox	m_combo_version;
	CComboBox	m_combo_year;
	CComboBox	m_combo_week;
	CEdit	m_edit_factory;
	CEdit	m_edit_style;
	CEdit	m_edit_well;
	CEdit	m_edit_ch;
	CEdit	m_edit_company;
	CEdit	m_edit_well_format;
	CEdit	m_edit_channel_format;
	CEdit	m_edit_version;
	CEdit	m_edit_year;
	CEdit	m_edit_week;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboFactory();
	afx_msg void OnSelchangeComboStyle();
	afx_msg void OnSelchangeComboWell();
	afx_msg void OnSelchangeComboCh();
	afx_msg void OnSelchangeComboCompany();
	afx_msg void OnSelchangeComboWellFormat();
	afx_msg void OnSelchangeComboChannelFormat();
	afx_msg void OnSelchangeComboVerison();
	afx_msg void OnSelchangeComboYear();
	afx_msg void OnSelchangeComboWeek();
	DECLARE_MESSAGE_MAP()
	void OnInitDeviceFactory();
	void OnInitDeviceStyle();
	void OnInitDeviceWell();
	void OnInitDeviceCh();
	void OnInitDeviceCompany();
	void OnInitDeviceWellFormat();
	void OnInitDeviceChannelFormat();
	void OnInitDeviceVersion();
	void OnInitDeviceYear();
	void OnInitDeviceWeek();
public:
//	CString m_ChipID;
	CString m_NWELLS;
	// current ambient temperature
	CString m_NCHANNELS;
	CString m_WELLFORMAT;
	CString m_CHANNELFORMAT;
//	CString m_MODEL;
	CString m_VERSION;
	CString m_SN1;
	int m_SN2;
	CString m_YEAR;
	CString m_WEEK;
	CString m_FACTORYNUM;
	int m_DEVICENUM;
	CString m_STYLENUM;
	int m_CHECKTEMP;
	afx_msg void OnBnClickedCheckSingleTemp();
};
