#pragma once


// CFileInfoDlg dialog

class CFileInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CFileInfoDlg)

public:
	CFileInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFileInfoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_READER_FILEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strFileName;
public:
	CString m_strFilePathName;
public:
	CString m_strFilePreview;
public:
	virtual BOOL OnInitDialog();
};
