#pragma once
#include "BlockManager.h"
#include "afxcmn.h"

// CBookmarkDlg dialog

class CBookmarkDlg : public CDialog
{
public:
	void UpdateListCtrl();
public:
	BOOL m_bAdd;
	CArray<CBookmark,CBookmark>* m_pBookmark;
	CBookmark m_bookmarkAdd;
	int m_iSelect;
private:
	DECLARE_DYNAMIC(CBookmarkDlg)

public:
	CBookmarkDlg(CWnd* pParent = NULL,CArray<CBookmark,CBookmark>* pBookmark=NULL,BOOL bAdd=FALSE);   // standard constructor
	virtual ~CBookmarkDlg();
	
// Dialog Data
	enum { IDD = IDD_DIALOG_BOOKMARK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	CListCtrl m_listctrl;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedButtonBookmarkModify();
public:
	afx_msg void OnBnClickedButtonBookmarkDel();
public:
	afx_msg void OnBnClickedButtonBookmarkAdd();
public:
	afx_msg void OnBnClickedButtonBookmarkCancel();
public:
	afx_msg void OnNMClickListctrlBookmarkList(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMDblclkListctrlBookmarkList(NMHDR *pNMHDR, LRESULT *pResult);
};
