// FileInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Reader.h"
#include "FileInfoDlg.h"


// CFileInfoDlg dialog

IMPLEMENT_DYNAMIC(CFileInfoDlg, CDialog)

CFileInfoDlg::CFileInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileInfoDlg::IDD, pParent)
	, m_strFileName(_T("\0"))
	, m_strFilePathName(_T("\0"))
	, m_strFilePreview(_T("\0"))
{

}

CFileInfoDlg::~CFileInfoDlg()
{
}

void CFileInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_CBString(pDX, IDC_EDIT_READER_FILEINFO_NAME, m_strFileName);
}


BEGIN_MESSAGE_MAP(CFileInfoDlg, CDialog)
END_MESSAGE_MAP()


// CFileInfoDlg message handlers

BOOL CFileInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	GetDlgItem(IDC_EDIT_READER_FILEINFO_NAME)->SetWindowTextW(m_strFileName);
	GetDlgItem(IDC_EDIT_READER_FILEINFO_PATH)->SetWindowTextW(m_strFilePathName);
	GetDlgItem(IDC_EDIT_READER_FILEINFO_PREVIEWER)->SetWindowTextW(m_strFilePreview);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
