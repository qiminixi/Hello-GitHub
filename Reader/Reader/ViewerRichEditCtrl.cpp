// ViewerRichEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Reader.h"
#include "ViewerRichEditCtrl.h"
#include "CustomMessages.h"


// CViewerRichEditCtrl

IMPLEMENT_DYNAMIC(CViewerRichEditCtrl, CRichEditCtrl)

CViewerRichEditCtrl::CViewerRichEditCtrl(int* pReadPos)
{
	m_pReadPos=pReadPos;
	m_bIsFirstScroll=1;
}

CViewerRichEditCtrl::~CViewerRichEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CViewerRichEditCtrl, CRichEditCtrl)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_EDIT_COPY, &CViewerRichEditCtrl::OnEditCopy)
	ON_COMMAND(ID_RICHEDIT_VIEWER_SETTING, &CViewerRichEditCtrl::OnRicheditViewerSetting)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_RICHEDIT_BOOKMARK_MANAGE, &CViewerRichEditCtrl::OnRicheditManageBookmark)
	ON_COMMAND(ID_RICHEDIT_BOOKMARK_ADD, &CViewerRichEditCtrl::OnRicheditAddBookmark)
END_MESSAGE_MAP()

// CViewerRichEditCtrl message handlers
void CViewerRichEditCtrl::SetFont(CViewerSetting& sttg)
{
	CHARFORMAT cf;
	cf.dwMask=CFM_FACE|CFM_BOLD|CFM_OFFSET|CFM_SIZE|CFM_COLOR;//字体、去除加粗，行距，大小
	lstrcpy(cf.szFaceName,sttg.m_strFaceName.GetBuffer());
	cf.dwEffects=0;
	cf.yHeight=sttg.m_lWordHeight;
	cf.yOffset=sttg.m_lLineOffset;
	cf.crTextColor=sttg.m_clrText;
	cf.bPitchAndFamily=sttg.m_nPitch|FF_DONTCARE;
	SetDefaultCharFormat(cf);
}
void CViewerRichEditCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//构造自己的快捷菜单
	::GetCursorPos(&point);
	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenuW(MF_STRING,ID_EDIT_COPY,_T("复制"));
	menu.AppendMenuW(MF_STRING,ID_RICHEDIT_VIEWER_SETTING,_T("设置"));
	menu.AppendMenuW(MF_STRING,ID_RICHEDIT_BOOKMARK_ADD,_T("添加到书签"));
	menu.AppendMenuW(MF_STRING,ID_RICHEDIT_BOOKMARK_MANAGE,_T("查看书签"));
	menu.TrackPopupMenu(TPM_RIGHTBUTTON,point.x,point.y,this);
	menu.DestroyMenu();
	return;
	//CRichEditCtrl::OnRButtonDown(nFlags, point);
}

void CViewerRichEditCtrl::OnEditCopy()
{
	Copy();
}
void CViewerRichEditCtrl::OnRicheditViewerSetting()
{
	// TODO: Add your command handler code here
	GetParent()->PostMessage(UM_VIEWER_SETTINGS);
}
void CViewerRichEditCtrl::PreviewSetting()
{
	CHARFORMAT cf;
	cf.dwMask=CFM_COLOR;
	//cf.dwMask=CFM_FACE|CFM_BOLD|CFM_OFFSET|CFM_SIZE|CFM_COLOR;//字体、去除加粗，行距，大小
	//wcsstr(cf.szFaceName ,sttg.m_strFaceName.GetBuffer());
	cf.dwEffects=0;
	//cf.yHeight=sttg.m_lWordHeight;
	//cf.yOffset=sttg.m_lLineOffset;
	cf.crTextColor=RGB(128,0,0);
	//cf.bPitchAndFamily=sttg.m_nPitch|FF_DONTCARE;
	//SetWordCharFormat(cf);
	SetDefaultCharFormat(cf);
	
	TRACE("RichEdit-->PreviewSettings Worked\n");
}

void CViewerRichEditCtrl::OnSize(UINT nType, int cx, int cy)
{
	CRichEditCtrl::OnSize(nType, cx, cy);
}

void CViewerRichEditCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CRichEditCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	if(nSBCode==SB_TOP||nSBCode==SB_BOTTOM)
		return;
	int nLine=GetFirstVisibleLine();
	*m_pReadPos=this->LineIndex(nLine);
	GetParent()->PostMessageW(UM_VIEWER_UPDATE_PROGRESS);
	//TRACE(_T("MouseTrak:nLine-->%d,nPos-->%d\n"),nLine,*m_pReadPos);

}

void CViewerRichEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	CRichEditCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	if(nChar==33||nChar==34||nChar==38||nChar==40)//33--PageUp,34--PageDown,38--LineUp,40--LineDown
	{
		int nLine=GetFirstVisibleLine();
		*m_pReadPos=this->LineIndex(nLine);
		GetParent()->PostMessageW(UM_VIEWER_UPDATE_PROGRESS);
	}
}

BOOL CViewerRichEditCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	int nLine=GetFirstVisibleLine();
	*m_pReadPos=this->LineIndex(nLine);
	GetParent()->PostMessageW(UM_VIEWER_UPDATE_PROGRESS);
	return CRichEditCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CViewerRichEditCtrl::OnRicheditAddBookmark()
{
	GetParent()->PostMessage(UM_VIEWER_BOOKMARK_ADD);
}

void CViewerRichEditCtrl::OnRicheditManageBookmark()
{
	GetParent()->PostMessage(UM_VIEWER_BOOKMARK_MANAGE);
}
