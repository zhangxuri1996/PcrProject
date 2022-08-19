// excelTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "excelTestDlg.h"
#include "comdef.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExcelTestDlg dialog

CExcelTestDlg::CExcelTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExcelTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExcelTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExcelTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExcelTestDlg)
	DDX_Control(pDX, IDC_LIST_SHEET, m_sheetList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExcelTestDlg, CDialog)
	//{{AFX_MSG_MAP(CExcelTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SHEET, OnBtnSheet)
	ON_BN_CLICKED(IDC_BTN_READ, OnBtnRead)
	ON_BN_CLICKED(IDC_BTN_WRITE, OnBtnWrite)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExcelTestDlg message handlers

BOOL CExcelTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CExcelTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CExcelTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CExcelTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CExcelTestDlg::OnBtnSheet() 
{
	// TODO: Add your control notification handler code here
	_Application app;
	Workbooks books;
	_Workbook book;
	Worksheets sheets;
	_Worksheet sheet;

	COleVariant vtOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR),
		vtTrue((short)TRUE),vtFalse((short)FALSE);

	if(!app.CreateDispatch(_T("Excel.Application"),NULL))  
	{
		AfxMessageBox( _T( "创建Excel服务失败！" ), NULL );
		exit(1);
	}
	app.SetVisible(FALSE);   
	books.AttachDispatch(app.GetWorkbooks(),TRUE);

	LPDISPATCH lpDisp = NULL;
	// 打开文件
	
	lpDisp = books.Open( _T("D:\\test.xlsx"),
		vtOptional,
		COleVariant((long)true),
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional );   //填写自己工程的路径
	// 获得活动的WorkBook(工作簿)
	book.AttachDispatch( lpDisp, TRUE );
	// 获得工作表集
	sheets.AttachDispatch( book.GetSheets(), TRUE );
	// 包含的WorkSheet的个数
	long lgSheetNum = sheets.GetCount();

	// 获得名称列表
	CString strName;
	for ( int i = 1; i <= lgSheetNum; i++ )
	{
		sheet.AttachDispatch( sheets.GetItem( COleVariant( long( i ) ) ), TRUE );
		strName = sheet.GetName();	
		m_sheetList.AddString(strName);
	}
	
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	app.Quit();
}

void CExcelTestDlg::OnBtnRead() 
{
	// TODO: Add your control notification handler code here
	_Application app;
	Workbooks books;
	_Workbook book;
	Worksheets sheets;
	_Worksheet sheet;

	COleVariant vtOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR),
		vtTrue((short)TRUE),vtFalse((short)FALSE);

	if(!app.CreateDispatch(_T("Excel.Application"),NULL))  
	{
		AfxMessageBox( _T( "创建Excel服务失败！" ), NULL );
		exit(1);
	}
	app.SetVisible(FALSE);   
	books.AttachDispatch(app.GetWorkbooks(),TRUE);

	LPDISPATCH lpDisp = NULL;
	// 打开文件
	
	lpDisp = books.Open( _T("D:\\test.xlsx"),
		vtOptional,
		COleVariant((long)true),
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional );   //填写自己工程的路径
	// 获得活动的WorkBook(工作簿)
	book.AttachDispatch( lpDisp, TRUE );
	// 获得工作表集
	sheets.AttachDispatch( book.GetSheets(), TRUE );

	//对工作表集中单个的工作表操作
	sheet.AttachDispatch(sheets.GetItem( COleVariant(_T("sheet1")) ) , TRUE );
	Range range;
	range.AttachDispatch(sheet.GetCells());
	range.AttachDispatch((range.GetItem(COleVariant((long)2),COleVariant((long)3))).pdispVal);

	//获取该range的值
	COleVariant vResult = range.GetValue2();
	CString str;
	if(vResult.vt == VT_BSTR)
	{	
		str = vResult.bstrVal;
	}
	if(vResult.vt == VT_EMPTY)
	{
		str = "";
	}
	CString strMessage = "sheet1中第2行第3列的值为 " + str;
	AfxMessageBox(strMessage,NULL);

	range.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	app.Quit();
}

void CExcelTestDlg::OnBtnWrite() 
{
	// TODO: Add your control notification handler code here
	_Application app;
	Workbooks books;
	_Workbook book;
	Worksheets sheets;
	_Worksheet sheet;

	COleVariant vtOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR),
		vtTrue((short)TRUE),vtFalse((short)FALSE);

	if(!app.CreateDispatch(_T("Excel.Application"),NULL))  
	{
		AfxMessageBox( _T( "创建Excel服务失败！" ), NULL );
		exit(1);
	}
	app.SetVisible(FALSE);   
	books.AttachDispatch(app.GetWorkbooks(),TRUE);

	LPDISPATCH lpDisp = NULL;
	// 打开文件
	
	CString strFileName = "D:\\test.xlsx";
	lpDisp = books.Open( strFileName,
		vtOptional,
		COleVariant((long)false),
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional );   //填写自己工程的路径
	// 获得活动的WorkBook(工作簿)
	book.AttachDispatch( lpDisp, TRUE );
	// 获得工作表集
	sheets.AttachDispatch( book.GetSheets(), TRUE );

	//对工作表集中单个的工作表操作
	sheet.AttachDispatch(sheets.GetItem( COleVariant(_T("sheet1")) ) , TRUE );
	Range range;
	range.AttachDispatch(sheet.GetCells());
	range.SetItem(COleVariant((long)4),COleVariant((long)6),COleVariant(_T("HELLO,EXCEL")));
	
	//保存文件
	book.Save();
	range.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	

	book.ReleaseDispatch();
	books.ReleaseDispatch();
	app.Quit();
}
