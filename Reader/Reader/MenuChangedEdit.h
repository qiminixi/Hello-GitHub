#pragma once

// CMenuChangedEdit

class CMenuChangedEdit : public CEdit
{
	DECLARE_DYNAMIC(CMenuChangedEdit)
public:
	CFont font;
	void SetFont();
public:
	CMenuChangedEdit();
	virtual ~CMenuChangedEdit();
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:
//	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
public:
	afx_msg void OnEditCopy();
};


