// ReaderDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "MenuChangedEdit.h"
#include "BlockManager.h"
#include "TextViewerDlg.h"
#include "ViewerSetting.h"

#define TIP_RELAY_TIME 2//延迟
#define TIP_SHOW_TIME 3//存在时间
#define ARRAY_INCREMENT 12

// CReaderDlg dialog
class CReaderDlg : public CDialog
{
public:
	bool LoadProgramConfigFromFile();//存储程序配置信息
	void StoreProgramConfigToFile();//读取文件配置信息
	void IncrecseTheTwoArrays();//为了避免数组因频繁的插入导致频繁的内存分配、复制和释放，产生碎片
	void MoveThumb(CPoint point);
	CTextViewerDlg* NewViewer();
	void DeleteViewer(CTextViewerDlg* pViewer);
public:
	CString m_strTextW;
	CStringA m_strTextA;
	CMenuChangedEdit m_editText1;

	//让块自己决定怎么显示自己，下面是块的显示情况
	//int m_nMinMargin;//
	int m_nMargin;//窗口中的留白
	int m_nGapH;//水平方向上块之间的间距
	int m_nGapV;//竖直方向上块之间的间
	int m_nRow;//行数
	int m_nCol;//列数
	int m_nPage;//页数
	int m_iPage;//当前是第几页
	//-------------------------
	//Read背景
	int m_nFlagOfBg;//--->自定义设置
	COLORREF m_clrBg;//--->自定义设置
	CString m_strPicturePath;//--->自定义设置
	CBitmap m_bmpBg;//绘制背景用
	//-------------------------
	CRect m_rectWindow;//只是窗口正常显示时的情况(不是最大化和最小化)
	CBlockManager m_blockManager;
	int m_nPreBlock;//用于标识是否有可选状态的块 preselected
	//注意不能保存块的区域。如果鼠标飞出去导致没有清除的话，而窗口又改变的话，就会出乱子
	CArray<int,int> m_MarkSelBlocks;//用来标识被选择的块，值为0/1,和另一个数组是等长的
	int m_nIDCursor;
	
	CRect m_rectViewerWindow;
	CArray<CTextViewerDlg*,CTextViewerDlg*> m_ViewerPtrOfBlocks;//和另一个数组的等长的
	CList<CTextViewerDlg*,CTextViewerDlg*> m_ViewerManager;//管理Viewer,将常驻数量限制在2个

	//鼠标拖动效果控制，由于窗口获得焦点时会发送WM_MOUSEMOVE消息，所以要求移动两次触发nMoved
	BOOL m_bLButtonDown;
	CPoint m_ptMousePos;
	int m_iMovedBlock;
	int m_iDesBlock;
	BOOL m_nMoved;//0(未移动),1(移动一次),2(移动两次以上)
	BOOL m_bShiftKeyDown;
	COLORREF m_clrInterchange;//交换位置--->自定义设置
	COLORREF m_clrMoveLocation;//移动位置--->自定义设置
	//-----------------
	//鼠标停留显示提示
	int m_bShowTip;
	CPoint m_ptMousePosTip;
	int m_iTipBlock;
	CBlock* m_pBlockToShow;
	//-------------------------------
	
	//菜单
	CBlock* m_pBlockShowFileInfo;//显示文件信息对话框用
	int m_iBlockMenu;//菜单用，因为单击菜单响应时，获取的坐标不是右击时的坐标
	//-------------------------------
	
	//块的设置
	CBlockSetting m_sttgBlockDefault;
	CBlockSetting m_sttgBlockCur;
	CBlockSetting m_sttgBlockPre;
	//----------------
	//reader自己的设置
	CReaderSetting m_sttgReaderDefault;
	CReaderSetting m_sttgReaderCur;
	CReaderSetting m_sttgReaderPre;
	//-----------------
	/*CButton m_btnFile;
	CButton m_buttons[10];
	CButton m_btnNextPage;
	CButton m_btnPrePage;*/
// Construction
public:
	CReaderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_READER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
public:
	afx_msg void OnFileOpen();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnClose();
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
public:
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
public:
	afx_msg LRESULT OnViewerClose(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnShowBlockInfo(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnCloseBlockInfo(WPARAM wParam,LPARAM lParam);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnReaderSetting();
public:
	afx_msg LRESULT OnPreviewSetting(WPARAM wParam,LPARAM lParam);
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnReaderDeleteBlock();
public:
	afx_msg void OnReaderAbout();
public:
	afx_msg void OnReaderHelp();
public:
	afx_msg void OnReaderShowFileinfo();
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};
