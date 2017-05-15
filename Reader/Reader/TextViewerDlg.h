#pragma once
#include "BlockManager.h"
#include "MenuChangedEdit.h"
#include "ViewerRichEditCtrl.h"
#include "CustomButton.h"

// CTextViewerDlg dialog
#define DIA_MARGIN 3//空白
#define DIA_GAP 5//间距
#define DIA_HEIGHT 20//高度
#define BTN_WIDTH 0//按钮宽度
#define PG_WIDTH 80//进度

class CTextViewerDlg : public CDialog
{
public:
	void InitViewer(CWnd* pParent,CBlock* pBlock,int iBlock);//回收再利用Viewer
	bool ReadTextFromDisk();
	void MakePreviewText(CString& strPreview,int iPreviewPos);
	float GetRateOfProgress();
public:
	CBlock* m_pBlock;
	CWnd* m_pParent;
	CRect m_rectWindow;
	int m_iBlock;
	int m_nFileBlocks;//文件块的数量
	
	CViewerRichEditCtrl m_edit;
	CString m_strText;
	CStringA m_strTextA;
	long m_dwFileSize;
	
	static CViewerSetting m_sttgDefault;//设置
	static CViewerSetting m_sttgCur;//设置
	static CViewerSetting m_sttgPre;//设置
	static CRect m_rectViewerWindow;//Viewer的窗口位置和大小
	
	int m_nFlagCancelEditSel;//取消编辑框初始状态的全选，0为未初始化，1为初始化了。
	int m_bTurnToLastPos;//标识是否滚动至上次阅读的位置。
	int m_nFileTypeCode;//文件类型进度条用
	
	CBitmap m_bmpBg;
	int m_nDiaWidth;//小方块大小,主要指宽度。
	CCustomButton m_btnLoadNext;
	CCustomButton m_btnLoadPre;
private:
	DECLARE_DYNAMIC(CTextViewerDlg)
public:
	CTextViewerDlg(CWnd* pParent = NULL);// standard constructor
	CTextViewerDlg(CWnd* pParent,CBlock* pBlock,int iBlock,CRect rectWindow);//modeless dialog
	BOOL Create();
	virtual ~CTextViewerDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_VIEWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
public:
	afx_msg void OnPaint();
public:
	afx_msg LRESULT OnViewerSettings(WPARAM wParam,LPARAM lParam);//打开预览对话框
	afx_msg LRESULT OnPreviewSetting(WPARAM wParam,LPARAM lParam);//预览设置
	afx_msg LRESULT OnUpdateProgress(WPARAM wParam,LPARAM lParam);//更新进度显示
	afx_msg LRESULT OnAddBookmark(WPARAM wParam,LPARAM lParam);//更新进度显示
	afx_msg LRESULT OnManageBookmark(WPARAM wParam,LPARAM lParam);//更新进度显示
	afx_msg LRESULT OnJumpToBookmark(WPARAM wParam,LPARAM lParam);//跳转至书签
public:
	afx_msg void OnBnClickedButtonLoadPre();
public:
	afx_msg void OnBnClickedButtonLoadNext();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};