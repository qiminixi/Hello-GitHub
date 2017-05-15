#pragma once
#include "ViewerSetting.h"

// CViewerRichEditCtrl

class CViewerRichEditCtrl : public CRichEditCtrl
{
public:
	void SetFont(CViewerSetting& pEditSetting);
	void SetFont();
	void PreviewSetting();
public:
	int* m_pReadPos;//指向Viewer中的m_nReadPos，方便更新当前阅读位置
	int m_bIsFirstScroll;//第一次滚动的话，不要更新ReadPos
public:
	//CFont font;
private:
	DECLARE_DYNAMIC(CViewerRichEditCtrl)

public:
	CViewerRichEditCtrl(int* pReadPos);
	virtual ~CViewerRichEditCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnEditCopy();
public:
	afx_msg void OnRicheditViewerSetting();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
public:
	afx_msg void OnRicheditManageBookmark();
public:
	afx_msg void OnRicheditAddBookmark();
};


