#pragma once
#include "ViewerSetting.h"
#include "afxwin.h"

#define PREVIEW_BLOCK_BGCOLOR				1
#define PREVIEW_BLOCK_TITLE_BG				2
#define PREVIEW_BLOCK_TITLE					3
#define PREVIEW_BLOCK_TEXT					4
#define PREVIEW_BLOCK_MOUSEOVER				5
#define PREVIEW_BLOCK_MOUSECLICK			6
#define PREVIEW_READER_INTERCHANGE			7
#define PREVIEW_READER_MOVELOCATION			8
#define PREVIEW_READER_BGCOLOR				9
#define PREVIEW_READER_BGPICTURE			10
#define PREVIEW_READER_BGPICTURE_PRESET		11
#define PREVIEW_READER_THEME				12


// CReaderSettingDlg dialog

class CReaderSettingDlg : public CDialog
{
public:
	CBrush m_brushCtlColor;
	CBlockSetting m_sttgBlockCur;
	CBlockSetting m_sttgBlockDft;
	
	CReaderSetting m_sttgReaderDft;
	CReaderSetting m_sttgReaderCur;
	
private:
	DECLARE_DYNAMIC(CReaderSettingDlg)

public:
	CReaderSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReaderSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_READER_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnStnClickedStaticBlockBgColor();
public:
	afx_msg void OnStnClickedStaticBlockTitleBg();
public:
	afx_msg void OnStnClickedStaticBlockTitleColor();
public:
	afx_msg void OnStnClickedStaticBlockTextColor();
public:
	afx_msg void OnStnClickedStaticMouseOver();
public:
	afx_msg void OnStnClickedStaticMouseClick();
public:
	afx_msg void OnStnClickedStaticInterchange();
public:
	afx_msg void OnStnClickedStaticMoveLocation();
public:
	afx_msg void OnBnClickedRadioPureColor();
public:
	afx_msg void OnStnClickedStaticPureColor();
public:
	afx_msg void OnBnClickedButtonSelectPicture();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedRadioPicture();
public:
	afx_msg void OnBnClickedButtonPresetBgPic();
public:
	afx_msg void OnBnClickedStaticPresetBgPics(UINT nID);
public:
	afx_msg void OnStnClickedStaticThemeMain1();
public:
	afx_msg void OnStnClickedStaticThemeMain2();
public:
	afx_msg void OnStnClickedStaticThemeMain3();
};
