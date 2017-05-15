// BookmakDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Reader.h"
#include "BookmarkDlg.h"
#include "CustomMessages.h"

// CBookmarkDlg dialog

IMPLEMENT_DYNAMIC(CBookmarkDlg, CDialog)

CBookmarkDlg::CBookmarkDlg(CWnd* pParent /*=NULL*/,CArray<CBookmark,CBookmark>* pBookmark /*=NULL*/,BOOL bAdd)
	: CDialog(CBookmarkDlg::IDD, pParent)
{ 
	m_pBookmark=pBookmark;
	m_bAdd=bAdd;
	m_iSelect=-1;
}

CBookmarkDlg::~CBookmarkDlg()
{
}

void CBookmarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTCTRL_BOOKMARK_LIST, m_listctrl);
}


BEGIN_MESSAGE_MAP(CBookmarkDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CBookmarkDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBookmarkDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_BOOKMARK_MODIFY, &CBookmarkDlg::OnBnClickedButtonBookmarkModify)
	ON_BN_CLICKED(IDC_BUTTON_BOOKMARK_DEL, &CBookmarkDlg::OnBnClickedButtonBookmarkDel)
	ON_BN_CLICKED(IDC_BUTTON_BOOKMARK_ADD, &CBookmarkDlg::OnBnClickedButtonBookmarkAdd)
	ON_BN_CLICKED(IDC_BUTTON_BOOKMARK_CANCEL, &CBookmarkDlg::OnBnClickedButtonBookmarkCancel)
	ON_NOTIFY(NM_CLICK, IDC_LISTCTRL_BOOKMARK_LIST, &CBookmarkDlg::OnNMClickListctrlBookmarkList)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCTRL_BOOKMARK_LIST, &CBookmarkDlg::OnNMDblclkListctrlBookmarkList)
END_MESSAGE_MAP()


// CBookmarkDlg message handlers

BOOL CBookmarkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//将它移动到父窗口旁边
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	CRect rect=wp.rcNormalPosition;
	int w=rect.Width();
	GetParent()->GetWindowPlacement(&wp);
	rect.left=wp.rcNormalPosition.right;
	rect.right=rect.left+w;
	MoveWindow(rect);
	//-------------------------
	
	
	DWORD dwStyle = m_listctrl.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
    dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_listctrl.SetExtendedStyle(dwStyle); //设置扩展风格
	m_listctrl.InsertColumn(0,_T("序号") );
	m_listctrl.InsertColumn(1,_T("书签名"));
	m_listctrl.InsertColumn(2,_T("书签位置"));
	m_listctrl.SetColumnWidth(0,40);
	m_listctrl.SetColumnWidth(1,100);
	m_listctrl.SetColumnWidth(2,204);
	UpdateListCtrl();
	
	if(m_bAdd)
	{
		int n=(int)m_pBookmark->GetCount();
		CString str;
		str.Format(_T("书签%d"),n+1);
		GetDlgItem(IDC_EDIT_BOOKMARK_NAME)->SetWindowTextW(str);
		((CEdit*)GetDlgItem(IDC_EDIT_BOOKMARK_NAME))->SetSel(0,-1);
		GetDlgItem(IDC_EDIT_BOOKMARK_PREVIEWE)->SetWindowTextW(m_bookmarkAdd.m_strPreview);

		GetDlgItem(IDC_BUTTON_BOOKMARK_MODIFY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_BOOKMARK_DEL)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_BOOKMARK_ADD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_BOOKMARK_CANCEL)->ShowWindow(SW_HIDE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CBookmarkDlg::UpdateListCtrl()
{
	m_listctrl.DeleteAllItems();
	INT_PTR i=m_pBookmark->GetCount();
	for(int j=0;j<i;j++)
	{
		CString str;
		str.Format(_T("%d"),j+1);
		m_listctrl.InsertItem(j,_T(""));
		m_listctrl.SetItemText(j,0,str);
		m_listctrl.SetItemText(j,1,m_pBookmark->GetAt(j).m_strName);
		m_listctrl.SetItemText(j,2,m_pBookmark->GetAt(j).m_strPreview);
	}
}
void CBookmarkDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	
	//OnOK();
}

void CBookmarkDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}


void CBookmarkDlg::OnBnClickedButtonBookmarkModify()
{
	if(m_bAdd)
		return;
	if(m_iSelect==-1){
		MessageBox(_T("请先选择一个书签"));
		return;
	}
	GetDlgItem(IDC_EDIT_BOOKMARK_NAME)->GetWindowTextW(m_pBookmark->GetAt(m_iSelect).m_strName);
	UpdateListCtrl();
	UpdateData(false);
}

void CBookmarkDlg::OnBnClickedButtonBookmarkDel()
{
	if(m_bAdd)
		return;
	if(m_iSelect==-1){
		MessageBox(_T("请先选择一个书签"));
		return;
	}
	m_pBookmark->RemoveAt(m_iSelect);
	GetDlgItem(IDC_EDIT_BOOKMARK_NAME)->SetWindowTextW(_T(""));
	GetDlgItem(IDC_EDIT_BOOKMARK_PREVIEWE)->SetWindowTextW(_T(""));
	UpdateListCtrl();
	UpdateData(false);
	m_iSelect=-1;
}

void CBookmarkDlg::OnBnClickedButtonBookmarkAdd()
{
	GetDlgItem(IDC_EDIT_BOOKMARK_NAME)->GetWindowTextW(m_bookmarkAdd.m_strName);
	GetDlgItem(IDC_BUTTON_BOOKMARK_CANCEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BOOKMARK_ADD)->EnableWindow(FALSE);
	m_pBookmark->Add(m_bookmarkAdd);
	UpdateListCtrl();
	UpdateData(false);
}

void CBookmarkDlg::OnBnClickedButtonBookmarkCancel()
{
	OnCancel();
}

void CBookmarkDlg::OnNMClickListctrlBookmarkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_iSelect=pNMListView->iItem;
	if(m_iSelect!=-1)
	{
		GetDlgItem(IDC_EDIT_BOOKMARK_NAME)->SetWindowTextW(m_pBookmark->GetAt(m_iSelect).m_strName);
		GetDlgItem(IDC_EDIT_BOOKMARK_PREVIEWE)->SetWindowTextW(m_pBookmark->GetAt(m_iSelect).m_strPreview);
	}
	else
	{
		GetDlgItem(IDC_EDIT_BOOKMARK_NAME)->SetWindowTextW(_T(""));
		GetDlgItem(IDC_EDIT_BOOKMARK_PREVIEWE)->SetWindowTextW(_T(""));
	}
}

void CBookmarkDlg::OnNMDblclkListctrlBookmarkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_iSelect=pNMListView->iItem;
	if(m_iSelect!=-1)
	{
		if(MessageBox(_T("确定跳转到该书签标识的位置吗？"),_T("提示"),MB_YESNO)==IDYES)
			GetParent()->PostMessageW(UM_VIEWER_JUMPTO_BOOKMARK,m_iSelect);
	}
}
