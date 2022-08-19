// TheTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PCRProject.h"
#include "TheTestDlg.h"
#include "afxdialogex.h"
#include  <map>
using namespace std;

// CTheTestDlg dialog

IMPLEMENT_DYNAMIC(CTheTestDlg, CDialogEx)

CTheTestDlg::CTheTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTheTestDlg::IDD, pParent)
{

}

CTheTestDlg::~CTheTestDlg()
{
}

void CTheTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_NWELLS);
	DDX_Text(pDX, IDC_EDIT2, m_NCHANNELS);
	DDX_Text(pDX, IDC_EDIT3, m_WELLFORMAT);
	DDX_Text(pDX, IDC_EDIT8, m_CHANNELFORMAT);
	//DDX_Text(pDX, IDC_EDIT4, m_MODEL);
	DDX_Text(pDX, IDC_EDIT5, m_VERSION);
	DDX_Text(pDX, IDC_EDIT6, m_SN1);
	DDX_Text(pDX, IDC_EDIT7, m_SN2);
	DDX_Text(pDX, IDC_PRODUCE_YEAR, m_YEAR);
	DDX_Text(pDX, IDC_PRODUCE_WEEK, m_WEEK);
	DDX_Text(pDX, IDC_FACTORY_NUM, m_FACTORYNUM);
	DDX_Text(pDX, IDC_DEVICE_NUM, m_DEVICENUM);
	DDX_Text(pDX, IDC_STYLE_NUM, m_STYLENUM);
	DDX_Control(pDX, IDC_FACTORY_NUM, m_edit_factory);
	DDX_Control(pDX, IDC_STYLE_NUM, m_edit_style);
	DDX_Control(pDX, IDC_EDIT1, m_edit_well);
	DDX_Control(pDX, IDC_EDIT2, m_edit_ch);
	DDX_Control(pDX, IDC_EDIT6, m_edit_company);
	DDX_Control(pDX, IDC_EDIT3, m_edit_well_format);
	DDX_Control(pDX, IDC_EDIT8, m_edit_channel_format);
	DDX_Control(pDX, IDC_EDIT5, m_edit_version);
	DDX_Control(pDX, IDC_PRODUCE_YEAR, m_edit_year);
	DDX_Control(pDX, IDC_PRODUCE_WEEK, m_edit_week);

	DDX_Control(pDX, IDC_COMBO_FACTORY, m_combo_factory);
	DDX_Control(pDX, IDC_COMBO_STYLE, m_combo_style);
	DDX_Control(pDX, IDC_COMBO_WELL1, m_combo_well);
	DDX_Control(pDX, IDC_COMBO_CH1, m_combo_ch);
	DDX_Control(pDX, IDC_COMBO_COMPANY, m_combo_company);
	DDX_Control(pDX, IDC_COMBO_WELL_FORMAT, m_combo_well_format);
	DDX_Control(pDX, IDC_COMBO_CHANNEL_FORMAT, m_combo_channel_format);
	DDX_Control(pDX, IDC_COMBO_VERSION, m_combo_version);
	DDX_Control(pDX, IDC_COMBO_YEAR, m_combo_year);
	DDX_Control(pDX, IDC_COMBO_WEEK, m_combo_week);

	DDV_MinMaxFloat(pDX, m_DEVICENUM, 0, 9999);
}



BEGIN_MESSAGE_MAP(CTheTestDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_FACTORY, OnSelchangeComboFactory)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, OnSelchangeComboStyle)
	ON_CBN_SELCHANGE(IDC_COMBO_WELL1, OnSelchangeComboWell)
	ON_CBN_SELCHANGE(IDC_COMBO_CH1, OnSelchangeComboCh)
	ON_CBN_SELCHANGE(IDC_COMBO_COMPANY, OnSelchangeComboCompany)
	ON_CBN_SELCHANGE(IDC_COMBO_WELL_FORMAT, OnSelchangeComboWellFormat)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL_FORMAT, OnSelchangeComboChannelFormat)
	ON_CBN_SELCHANGE(IDC_COMBO_VERSION, OnSelchangeComboVerison)
	ON_CBN_SELCHANGE(IDC_COMBO_YEAR, OnSelchangeComboYear)
	ON_CBN_SELCHANGE(IDC_COMBO_WEEK, OnSelchangeComboWeek)
	ON_BN_CLICKED(IDC_CHECK_SINGLE_TEMP, &CTheTestDlg::OnBnClickedCheckSingleTemp)
END_MESSAGE_MAP()


BOOL CTheTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnInitDeviceFactory();
	OnInitDeviceStyle();
	OnInitDeviceWell();
	OnInitDeviceCh();
	OnInitDeviceCompany();
	OnInitDeviceWellFormat();
	OnInitDeviceChannelFormat();
	OnInitDeviceVersion();
	OnInitDeviceYear();
	OnInitDeviceWeek();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTheTestDlg::OnSelchangeComboFactory()
{
	// TODO: Add your control notification handler code here
	CString factory;
	m_combo_factory.GetLBText(m_combo_factory.GetCurSel(), factory);

	m_edit_factory.SetWindowText(factory);
	return;
}

void CTheTestDlg::OnSelchangeComboStyle()
{
	// TODO: Add your control notification handler code here
	CString style;
	m_combo_style.GetLBText(m_combo_style.GetCurSel(), style);

	m_edit_style.SetWindowText(style);
	return;
}

void CTheTestDlg::OnSelchangeComboWell()
{
	// TODO: Add your control notification handler code here
	CString well;
	m_combo_well.GetLBText(m_combo_well.GetCurSel(), well);

	m_edit_well.SetWindowText(well);
	return;
}

void CTheTestDlg::OnSelchangeComboCh()
{
	// TODO: Add your control notification handler code here
	CString ch;
	m_combo_ch.GetLBText(m_combo_ch.GetCurSel(), ch);

	m_edit_ch.SetWindowText(ch);
	return;
}

void CTheTestDlg::OnSelchangeComboCompany()
{
	// TODO: Add your control notification handler code here
	CString company;
	m_combo_company.GetLBText(m_combo_company.GetCurSel(), company);

	m_edit_company.SetWindowText(company);
	return;
}

void CTheTestDlg::OnSelchangeComboWellFormat()
{
	// TODO: Add your control notification handler code here
	CString wellFormat;
	m_combo_well_format.GetLBText(m_combo_well_format.GetCurSel(), wellFormat);

	m_edit_well_format.SetWindowText(wellFormat);
	return;
}

void CTheTestDlg::OnSelchangeComboChannelFormat()
{
	// TODO: Add your control notification handler code here
	CString channelFormat;
//	m_combo_channel_format.GetLBText(m_combo_channel_format.GetCurSel(), channelFormat);
	//int strLength = m_CHANNELFORMAT.GetLength() + 1;
	//char ch[2];
	//memset(ch, 0, sizeof(bool) * 2); //将数组的垃圾内容清空.
	//strncpy(ch, m_CHANNELFORMAT, strLength);

	//channelFormat.Format("%d", m_Map[ch]);
	//m_edit_channel_format.SetWindowText(channelFormat);
	CString wellFormat;
	m_combo_channel_format.GetLBText(m_combo_channel_format.GetCurSel(), channelFormat);

	m_edit_channel_format.SetWindowText(channelFormat);
	return;
}

void CTheTestDlg::OnSelchangeComboVerison()
{
	// TODO: Add your control notification handler code here
	CString verison;
	m_combo_version.GetLBText(m_combo_version.GetCurSel(), verison);

	m_edit_version.SetWindowText(verison);
	return;
}

void CTheTestDlg::OnSelchangeComboYear()
{
	// TODO: Add your control notification handler code here
	CString year;
	m_combo_year.GetLBText(m_combo_year.GetCurSel(), year);

	m_edit_year.SetWindowText(year);
	return;
}

void CTheTestDlg::OnSelchangeComboWeek()
{
	// TODO: Add your control notification handler code here
	CString selectedItem;
	m_combo_week.GetLBText(m_combo_week.GetCurSel(), selectedItem);

	m_edit_week.SetWindowText(selectedItem);
	return;
}

void CTheTestDlg::OnInitDeviceFactory() {
	m_combo_factory.AddString("00");
	m_combo_factory.AddString("01");
	m_combo_factory.AddString("02");
	m_combo_factory.AddString("03");
	m_combo_factory.AddString("04");
	m_combo_factory.AddString("05");

	map<string, int> Map;

	Map.insert(map<string, int>::value_type("00", 0));
	Map.insert(map<string, int>::value_type("01", 1));
	Map.insert(map<string, int>::value_type("02", 2));
	Map.insert(map<string, int>::value_type("03", 3));
	Map.insert(map<string, int>::value_type("04", 4));
	Map.insert(map<string, int>::value_type("05", 5));
	int strLength = m_FACTORYNUM.GetLength() + 1;
	char ch[2];
	memset(ch, 0, sizeof(bool) * 2); //将数组的垃圾内容清空.
	strncpy(ch, m_FACTORYNUM, strLength);

	switch (Map[ch]) {
	case 0:
		m_combo_factory.SetCurSel(0);
		break;
	case 1:
		m_combo_factory.SetCurSel(1);
		break;
	case 2:
		m_combo_factory.SetCurSel(2);
		break;
	case 3:
		m_combo_factory.SetCurSel(3);
		break;
	case 4:
		m_combo_factory.SetCurSel(4);
		break;
	case 5:
		m_combo_factory.SetCurSel(5);
		break;
	}
}

void CTheTestDlg::OnInitDeviceStyle() {
	m_combo_style.AddString("0");
	m_combo_style.AddString("1");
	m_combo_style.AddString("2");
	m_combo_style.AddString("3");
	m_combo_style.AddString("4");
	m_combo_style.AddString("5");

	map<string, int> Map;
	Map.insert(map<string, int>::value_type("0", 0));
	Map.insert(map<string, int>::value_type("1", 1));
	Map.insert(map<string, int>::value_type("2", 2));
	Map.insert(map<string, int>::value_type("3", 3));
	Map.insert(map<string, int>::value_type("4", 4));
	Map.insert(map<string, int>::value_type("5", 5));
	int strLength = m_STYLENUM.GetLength() + 1;
	char ch[1];
	memset(ch, 0, sizeof(bool) * 1); //将数组的垃圾内容清空.
	strncpy(ch, m_STYLENUM, strLength);

	switch (Map[ch]) {
	case 0:
		m_combo_style.SetCurSel(0);
		break;
	case 1:
		m_combo_style.SetCurSel(1);
		break;
	case 2:
		m_combo_style.SetCurSel(2);
		break;
	case 3:
		m_combo_style.SetCurSel(3);
		break;
	case 4:
		m_combo_style.SetCurSel(4);
		break;
	case 5:
		m_combo_style.SetCurSel(5);
		break;
	}
}

void CTheTestDlg::OnInitDeviceWell() {
	m_combo_well.AddString("04");
	m_combo_well.AddString("08");
	m_combo_well.AddString("16");

	map<string, int> Map;
	Map.insert(map<string, int>::value_type("04", 0));
	Map.insert(map<string, int>::value_type("08", 1));
	Map.insert(map<string, int>::value_type("16", 2));
	int strLength = m_NWELLS.GetLength() + 1;
	char ch[2];
	memset(ch, 0, sizeof(bool) * 2); //将数组的垃圾内容清空.
	strncpy(ch, m_NWELLS, strLength);

	switch (Map[ch]) {
	case 0:
		m_combo_well.SetCurSel(0);
		break;
	case 1:
		m_combo_well.SetCurSel(1);
		break;
	case 2:
		m_combo_well.SetCurSel(2);
		break;
	}
}

void CTheTestDlg::OnInitDeviceCh() {
	m_combo_ch.AddString("1");
	m_combo_ch.AddString("2");
	m_combo_ch.AddString("4");

	map<string, int> Map;
	Map.insert(map<string, int>::value_type("1", 0));
	Map.insert(map<string, int>::value_type("2", 1));
	Map.insert(map<string, int>::value_type("4", 2));
	int strLength = m_NCHANNELS.GetLength() + 1;
	char ch[2];
	memset(ch, 0, sizeof(bool) * 2); //将数组的垃圾内容清空.
	strncpy(ch, m_NCHANNELS, strLength);

	switch (Map[ch]) {
	case 0:
		m_combo_ch.SetCurSel(0);
		break;
	case 1:
		m_combo_ch.SetCurSel(1);
		break;
	case 2:
		m_combo_ch.SetCurSel(2);
		break;
	}
}

void CTheTestDlg::OnInitDeviceCompany() {
	m_combo_company.AddString("0");
	m_combo_company.AddString("1");
	m_combo_company.AddString("2");
	m_combo_company.AddString("3");
	m_combo_company.AddString("4");
	m_combo_company.AddString("5");
	m_combo_company.AddString("6");
	m_combo_company.AddString("7");

	map<string, int> Map;
	Map.insert(map<string, int>::value_type("0", 0));
	Map.insert(map<string, int>::value_type("1", 1));
	Map.insert(map<string, int>::value_type("2", 2));
	Map.insert(map<string, int>::value_type("3", 3));
	Map.insert(map<string, int>::value_type("4", 4));
	Map.insert(map<string, int>::value_type("5", 5));
	Map.insert(map<string, int>::value_type("6", 6));
	Map.insert(map<string, int>::value_type("7", 7));

	int strLength = m_SN1.GetLength() + 1;
	char ch[1];
	memset(ch, 0, sizeof(bool) * 1); //将数组的垃圾内容清空.
	strncpy(ch, m_SN1, strLength);

	switch (Map[ch]) {
	case 0:
		m_combo_company.SetCurSel(0);
		break;
	case 1:
		m_combo_company.SetCurSel(1);
		break;
	case 2:
		m_combo_company.SetCurSel(2);
		break;
	case 3:
		m_combo_company.SetCurSel(3);
		break;
	case 4:
		m_combo_company.SetCurSel(4);
		break;
	case 5:
		m_combo_company.SetCurSel(5);
		break;
	case 6:
		m_combo_company.SetCurSel(6);
		break;
	case 7:
		m_combo_company.SetCurSel(7);
		break;
	}
}


void CTheTestDlg::OnInitDeviceWellFormat() {
	m_combo_well_format.AddString("1");
	m_combo_well_format.AddString("2");

	map<string, int> Map;
	Map.insert(map<string, int>::value_type("1", 0));
	Map.insert(map<string, int>::value_type("2", 1));
	int strLength = m_WELLFORMAT.GetLength() + 1;
	char ch[2];
	memset(ch, 0, sizeof(bool) * 2); //将数组的垃圾内容清空.
	strncpy(ch, m_WELLFORMAT, strLength);

	m_combo_well_format.SetCurSel(Map[ch]);
}

void CTheTestDlg::OnInitDeviceChannelFormat() {
	m_combo_channel_format.AddString("00");
	m_combo_channel_format.AddString("01");
	m_combo_channel_format.AddString("02");
	m_combo_channel_format.AddString("03");
	m_combo_channel_format.AddString("04");
	m_combo_channel_format.AddString("05");
	m_combo_channel_format.AddString("06");
	m_combo_channel_format.AddString("07");
	m_combo_channel_format.AddString("08");
	m_combo_channel_format.AddString("09");
	m_combo_channel_format.AddString("10");
	m_combo_channel_format.AddString("11");
	m_combo_channel_format.AddString("12");
	m_combo_channel_format.AddString("13");
	m_combo_channel_format.AddString("14");

	map<string, int> Map;
	Map.insert(map<string, int>::value_type("0", 0));
	Map.insert(map<string, int>::value_type("1", 1));
	Map.insert(map<string, int>::value_type("2", 2));
	Map.insert(map<string, int>::value_type("3", 3));
	Map.insert(map<string, int>::value_type("4", 4));
	Map.insert(map<string, int>::value_type("5", 5));
	Map.insert(map<string, int>::value_type("6", 6));
	Map.insert(map<string, int>::value_type("7", 7));
	Map.insert(map<string, int>::value_type("8", 8));
	Map.insert(map<string, int>::value_type("9", 9));
	Map.insert(map<string, int>::value_type("10", 10));
	Map.insert(map<string, int>::value_type("11", 11));
	Map.insert(map<string, int>::value_type("12", 12));
	Map.insert(map<string, int>::value_type("13", 13));
	Map.insert(map<string, int>::value_type("14", 14));

	int strLength = m_CHANNELFORMAT.GetLength() + 1;
	char ch[2];
	memset(ch, 0, sizeof(bool) * 2); //将数组的垃圾内容清空.
	strncpy(ch, m_CHANNELFORMAT, strLength);

	m_combo_channel_format.SetCurSel(Map[ch]);

}

//void CTheTestDlg::OnInitDeviceChannelFormat() {
//	m_combo_channel_format.AddString("FAM");
//	m_combo_channel_format.AddString("HEX");
//	m_combo_channel_format.AddString("ROX");
//	map<string, int> Map;
//	Map.insert(map<string, int>::value_type("0", 0));
//	Map.insert(map<string, int>::value_type("1", 1));
//	Map.insert(map<string, int>::value_type("2", 2));
//
//	int strLength = m_CHANNELFORMAT.GetLength() + 1;
//	char ch[2];
//	memset(ch, 0, sizeof(bool) * 2); //将数组的垃圾内容清空.
//	strncpy(ch, m_CHANNELFORMAT, strLength);
//
//	m_combo_channel_format.SetCurSel(Map[ch]);
//
//}

void CTheTestDlg::OnInitDeviceVersion() {
	m_combo_version.AddString("1");
	m_combo_version.AddString("2");
	m_combo_version.AddString("3");
	m_combo_version.AddString("4");
	m_combo_version.AddString("5");
	m_combo_version.AddString("6");
	m_combo_version.AddString("7");
	m_combo_version.AddString("8");
	m_combo_version.AddString("9");

	map<string, int> Map;
	Map.insert(map<string, int>::value_type("1", 0));
	Map.insert(map<string, int>::value_type("2", 1));
	Map.insert(map<string, int>::value_type("3", 2));
	Map.insert(map<string, int>::value_type("4", 3));
	Map.insert(map<string, int>::value_type("5", 4));
	Map.insert(map<string, int>::value_type("6", 5));
	Map.insert(map<string, int>::value_type("7", 6));
	Map.insert(map<string, int>::value_type("8", 7));
	Map.insert(map<string, int>::value_type("9", 8));
	int strLength = m_VERSION.GetLength() + 1;
	char ch[2];
	memset(ch, 0, sizeof(bool) * 2); //将数组的垃圾内容清空.
	strncpy(ch, m_VERSION, strLength);

	switch (Map[ch]) {
	case 0:
		m_combo_version.SetCurSel(0);
		break;
	case 1:
		m_combo_version.SetCurSel(1);
		break;
	case 2:
		m_combo_version.SetCurSel(2);
		break;
	case 3:
		m_combo_version.SetCurSel(3);
		break;
	case 4:
		m_combo_version.SetCurSel(4);
		break;
	case 5:
		m_combo_version.SetCurSel(5);
		break;
	case 6:
		m_combo_version.SetCurSel(6);
		break;
	case 7:
		m_combo_version.SetCurSel(7);
		break;
	case 8:
		m_combo_version.SetCurSel(8);
		break;
	}
}

void CTheTestDlg::OnInitDeviceYear() {
	m_combo_year.AddString("15");
	m_combo_year.AddString("16");
	m_combo_year.AddString("17");
	m_combo_year.AddString("18");
	m_combo_year.AddString("19");
	m_combo_year.AddString("20");
	m_combo_year.AddString("21");
	m_combo_year.AddString("22");
	m_combo_year.AddString("23");
	m_combo_year.AddString("24");
	m_combo_year.AddString("25");
	m_combo_year.AddString("26");
	m_combo_year.AddString("27");
	m_combo_year.AddString("28");
	m_combo_year.AddString("29");
	m_combo_year.AddString("30");
	map<string, int> Map;
	Map.insert(map<string, int>::value_type("15", 0));
	Map.insert(map<string, int>::value_type("16", 1));
	Map.insert(map<string, int>::value_type("17", 2));
	Map.insert(map<string, int>::value_type("18", 3));
	Map.insert(map<string, int>::value_type("19", 4));
	Map.insert(map<string, int>::value_type("20", 5));
	Map.insert(map<string, int>::value_type("21", 6));
	Map.insert(map<string, int>::value_type("22", 7));
	Map.insert(map<string, int>::value_type("23", 8));
	Map.insert(map<string, int>::value_type("24", 9));
	Map.insert(map<string, int>::value_type("25", 10));
	Map.insert(map<string, int>::value_type("26", 11));
	Map.insert(map<string, int>::value_type("27", 12));
	Map.insert(map<string, int>::value_type("28", 13));
	Map.insert(map<string, int>::value_type("29", 14));
	Map.insert(map<string, int>::value_type("30", 15));
	int strLength = m_YEAR.GetLength() + 1;
	char ch[2];
	memset(ch, 0, sizeof(bool) * 2); //将数组的垃圾内容清空.
	strncpy(ch, m_YEAR, strLength);

	m_combo_year.SetCurSel(Map[ch]);

}

void CTheTestDlg::OnInitDeviceWeek() {

	int strLength = m_WEEK.GetLength() + 1;
	char ch[2];
	memset(ch, 0, sizeof(bool) * 2);
	strncpy(ch, m_WEEK, strLength);

	map<string, int> Map;
	for (int i = 0; i < 54; i++) {
		CString item;
		item.Format("%02d", i);
		m_combo_week.AddString(item);
		Map.insert(map<string, int>::value_type(item, i));
	}
	m_combo_week.SetCurSel(Map[ch]);
}
// CTheTestDlg message handlers


void CTheTestDlg::OnBnClickedCheckSingleTemp()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton *pButton;
	pButton = (CButton*)GetDlgItem(IDC_CHECK_SINGLE_TEMP); //ID为CheckBox的ID
	m_CHECKTEMP = pButton->GetCheck();//获得checkbox的点击状态，值只有0 1 2三种状态，点击后为非0值
}
