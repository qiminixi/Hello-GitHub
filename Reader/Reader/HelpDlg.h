#pragma once
#include "HtmlCtrl.h"

// CHelpDlg dialog

class CHelpDlg : public CDialog
{
public:
	CHtmlCtrl m_html;
private:
	DECLARE_DYNAMIC(CHelpDlg)

public:
	CHelpDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHelpDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_READER_HELP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
};
