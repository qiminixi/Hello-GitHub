// MenuChangedEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Reader.h"
#include "MenuChangedEdit.h"


// CMenuChangedEdit

IMPLEMENT_DYNAMIC(CMenuChangedEdit, CEdit)

CMenuChangedEdit::CMenuChangedEdit()
{

}

CMenuChangedEdit::~CMenuChangedEdit()
{
}


BEGIN_MESSAGE_MAP(CMenuChangedEdit, CEdit)
	ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
//	ON_WM_COPYDATA()
ON_COMMAND(ID_EDIT_COPY, &CMenuChangedEdit::OnEditCopy)
END_MESSAGE_MAP()



// CMenuChangedEdit message handlers


void CMenuChangedEdit::OnEditCopy()
{
	Copy();
}

BOOL CMenuChangedEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	//if(pMsg->message==WM_CONTEXTMENU)
	//{
	//	MessageBox(_T("GotIt"));
	//	
	//	CWnd* pWnd=this->GetParent();//(CWnd*)m_hWndOwner;
	//	pWnd->PostMessageW(WM_CONTEXTMENU);
	//	return TRUE;
	//}
	//if(pMsg->message==WM_RBUTTONDOWN)
	//{
	//	MessageBox(_T("GotIt"));
	//}
	return CEdit::PreTranslateMessage(pMsg);
}

void CMenuChangedEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CEdit::OnLButtonDown(nFlags, point);
}

void CMenuChangedEdit::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
}
#define ID_VIEWER_SETTINGS 10
void CMenuChangedEdit::OnRButtonDown(UINT nFlags, CPoint point)
{
	//构造自己的快捷菜单
	::GetCursorPos(&point);
	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenuW(MF_STRING,ID_EDIT_COPY,_T("复制"));
	menu.AppendMenuW(MF_STRING,ID_VIEWER_SETTINGS,_T("设置"));
	menu.TrackPopupMenu(TPM_RIGHTBUTTON,point.x,point.y,this);
	menu.DestroyMenu();
	return;
	//CEdit::OnRButtonDown(nFlags, point);
}



void CMenuChangedEdit::SetFont()
{
	//CFont font;
	font.CreatePointFont(120,_T("楷体"));
	CEdit::SetFont(&font,1);
}