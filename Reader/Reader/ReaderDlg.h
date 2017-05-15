// ReaderDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "MenuChangedEdit.h"
#include "BlockManager.h"
#include "TextViewerDlg.h"
#include "ViewerSetting.h"

#define TIP_RELAY_TIME 2//�ӳ�
#define TIP_SHOW_TIME 3//����ʱ��
#define ARRAY_INCREMENT 12

// CReaderDlg dialog
class CReaderDlg : public CDialog
{
public:
	bool LoadProgramConfigFromFile();//�洢����������Ϣ
	void StoreProgramConfigToFile();//��ȡ�ļ�������Ϣ
	void IncrecseTheTwoArrays();//Ϊ�˱���������Ƶ���Ĳ��뵼��Ƶ�����ڴ���䡢���ƺ��ͷţ�������Ƭ
	void MoveThumb(CPoint point);
	CTextViewerDlg* NewViewer();
	void DeleteViewer(CTextViewerDlg* pViewer);
public:
	CString m_strTextW;
	CStringA m_strTextA;
	CMenuChangedEdit m_editText1;

	//�ÿ��Լ�������ô��ʾ�Լ��������ǿ����ʾ���
	//int m_nMinMargin;//
	int m_nMargin;//�����е�����
	int m_nGapH;//ˮƽ�����Ͽ�֮��ļ��
	int m_nGapV;//��ֱ�����Ͽ�֮��ļ�
	int m_nRow;//����
	int m_nCol;//����
	int m_nPage;//ҳ��
	int m_iPage;//��ǰ�ǵڼ�ҳ
	//-------------------------
	//Read����
	int m_nFlagOfBg;//--->�Զ�������
	COLORREF m_clrBg;//--->�Զ�������
	CString m_strPicturePath;//--->�Զ�������
	CBitmap m_bmpBg;//���Ʊ�����
	//-------------------------
	CRect m_rectWindow;//ֻ�Ǵ���������ʾʱ�����(������󻯺���С��)
	CBlockManager m_blockManager;
	int m_nPreBlock;//���ڱ�ʶ�Ƿ��п�ѡ״̬�Ŀ� preselected
	//ע�ⲻ�ܱ���������������ɳ�ȥ����û������Ļ����������ָı�Ļ����ͻ������
	CArray<int,int> m_MarkSelBlocks;//������ʶ��ѡ��Ŀ飬ֵΪ0/1,����һ�������ǵȳ���
	int m_nIDCursor;
	
	CRect m_rectViewerWindow;
	CArray<CTextViewerDlg*,CTextViewerDlg*> m_ViewerPtrOfBlocks;//����һ������ĵȳ���
	CList<CTextViewerDlg*,CTextViewerDlg*> m_ViewerManager;//����Viewer,����פ����������2��

	//����϶�Ч�����ƣ����ڴ��ڻ�ý���ʱ�ᷢ��WM_MOUSEMOVE��Ϣ������Ҫ���ƶ����δ���nMoved
	BOOL m_bLButtonDown;
	CPoint m_ptMousePos;
	int m_iMovedBlock;
	int m_iDesBlock;
	BOOL m_nMoved;//0(δ�ƶ�),1(�ƶ�һ��),2(�ƶ���������)
	BOOL m_bShiftKeyDown;
	COLORREF m_clrInterchange;//����λ��--->�Զ�������
	COLORREF m_clrMoveLocation;//�ƶ�λ��--->�Զ�������
	//-----------------
	//���ͣ����ʾ��ʾ
	int m_bShowTip;
	CPoint m_ptMousePosTip;
	int m_iTipBlock;
	CBlock* m_pBlockToShow;
	//-------------------------------
	
	//�˵�
	CBlock* m_pBlockShowFileInfo;//��ʾ�ļ���Ϣ�Ի�����
	int m_iBlockMenu;//�˵��ã���Ϊ�����˵���Ӧʱ����ȡ�����겻���һ�ʱ������
	//-------------------------------
	
	//�������
	CBlockSetting m_sttgBlockDefault;
	CBlockSetting m_sttgBlockCur;
	CBlockSetting m_sttgBlockPre;
	//----------------
	//reader�Լ�������
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
