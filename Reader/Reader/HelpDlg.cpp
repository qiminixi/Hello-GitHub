// HelpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Reader.h"
#include "HelpDlg.h"


// CHelpDlg dialog

IMPLEMENT_DYNAMIC(CHelpDlg, CDialog)

CHelpDlg::CHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpDlg::IDD, pParent)
{

}

CHelpDlg::~CHelpDlg()
{
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CHelpDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CHelpDlg message handlers

BOOL CHelpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rectClient;
	GetClientRect(rectClient);
	GetDlgItem(IDC_STATIC_HELP)->MoveWindow(rectClient);
	
	m_html.SetHideContextMenu(TRUE);
    m_html.CreateFromStatic(IDC_STATIC_HELP,this);
	CString sNavigatePath;
	CString strAppName=AfxGetApp()->m_pszAppName;
	sNavigatePath=_T("res://");
	sNavigatePath+=strAppName;
	sNavigatePath+=_T(".exe/");
	sNavigatePath+=_T("135");
	//m_html.Navigate(_T("about:bank"));  //这句话一定要写MAKEINTRESOURCE(IDR_HTML_HELP)
   // m_html.Navigate(MAKEINTRESOURCE(IDR_HTML_HELP)); 
	 m_html.Navigate(sNavigatePath); 
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CHelpDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}
