#pragma once
#include "BlockManager.h"
#include "MenuChangedEdit.h"
#include "ViewerRichEditCtrl.h"
#include "CustomButton.h"

// CTextViewerDlg dialog
#define DIA_MARGIN 3//�հ�
#define DIA_GAP 5//���
#define DIA_HEIGHT 20//�߶�
#define BTN_WIDTH 0//��ť���
#define PG_WIDTH 80//����

class CTextViewerDlg : public CDialog
{
public:
	void InitViewer(CWnd* pParent,CBlock* pBlock,int iBlock);//����������Viewer
	bool ReadTextFromDisk();
	void MakePreviewText(CString& strPreview,int iPreviewPos);
	float GetRateOfProgress();
public:
	CBlock* m_pBlock;
	CWnd* m_pParent;
	CRect m_rectWindow;
	int m_iBlock;
	int m_nFileBlocks;//�ļ��������
	
	CViewerRichEditCtrl m_edit;
	CString m_strText;
	CStringA m_strTextA;
	long m_dwFileSize;
	
	static CViewerSetting m_sttgDefault;//����
	static CViewerSetting m_sttgCur;//����
	static CViewerSetting m_sttgPre;//����
	static CRect m_rectViewerWindow;//Viewer�Ĵ���λ�úʹ�С
	
	int m_nFlagCancelEditSel;//ȡ���༭���ʼ״̬��ȫѡ��0Ϊδ��ʼ����1Ϊ��ʼ���ˡ�
	int m_bTurnToLastPos;//��ʶ�Ƿ�������ϴ��Ķ���λ�á�
	int m_nFileTypeCode;//�ļ����ͽ�������
	
	CBitmap m_bmpBg;
	int m_nDiaWidth;//С�����С,��Ҫָ��ȡ�
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
	afx_msg LRESULT OnViewerSettings(WPARAM wParam,LPARAM lParam);//��Ԥ���Ի���
	afx_msg LRESULT OnPreviewSetting(WPARAM wParam,LPARAM lParam);//Ԥ������
	afx_msg LRESULT OnUpdateProgress(WPARAM wParam,LPARAM lParam);//���½�����ʾ
	afx_msg LRESULT OnAddBookmark(WPARAM wParam,LPARAM lParam);//���½�����ʾ
	afx_msg LRESULT OnManageBookmark(WPARAM wParam,LPARAM lParam);//���½�����ʾ
	afx_msg LRESULT OnJumpToBookmark(WPARAM wParam,LPARAM lParam);//��ת����ǩ
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