#pragma once
#include "afxwin.h"
#include "ViewerSetting.h"

#define PREVIWER_FACENAME			1
#define PREVIWER_HEIGHT				2
#define PREVIWER_OFFSET				3
#define PREVIWER_COLOR				4
#define PREVIWER_PITCH				5
#define PREVIWER_BGCOLOR			6
#define PREVIWER_BGPIC				7
#define PREVIWER_BGPIC_PRESET		8
#define PREVIWER_BORDER				9
#define PREVIWER_DIA_DFT			10
#define PREVIWER_DIA_CUR			11
#define PREVIWER_VIEWER_THEME		12

// CViewerSettingDlg dialog

class CViewerSettingDlg : public CDialog
{
public:
	void GetFontNameFromSystem(CComboBox& combo);
public:
	//CString m_strFaceName;
	//int m_nHeight;
	//int m_nOffset;
	//int m_nPitch;
	//COLORREF m_clrText;//
	//int m_nFlagOfBg;//
	//COLORREF m_clrBg;
	//CString m_strPicturePath;
	//UINT m_nIDOfBmp;
	//
	//COLORREF m_clrBorder;
	//COLORREF m_clrDiaDft;
	//COLORREF m_clrDiaCur;
	CViewerSetting m_sttg;
	
	CBrush m_brushTextColor;
	CBrush m_brushBgColor;
/////////////////////////////////////////////////////////////
private:
	DECLARE_DYNAMIC(CViewerSettingDlg)

public:
	CWnd* m_pParent;
	CViewerSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CViewerSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_VIEWER_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	CComboBox m_comboFontName;
	CComboBox m_comboFontSize;
	CComboBox m_comboOffset;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnStnClickedStaticColor();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnCbnSelchangeComboFontName();
public:
	afx_msg void OnCbnSelchangeComboFontSize();
public:
	afx_msg void OnBnClickedCheckPitch();
public:
	afx_msg void OnCbnSelchangeComboOffset();
public:
	afx_msg void OnBnClickedRadioBgPruecolor();
public:
	afx_msg void OnBnClickedRadioBgPicture();
public:
	afx_msg void OnStnClickedStaticPureColor();
public:
	afx_msg void OnStnClickedStaticBgPic();
public:
	afx_msg void OnBnClickedButtonSelectPicture();
public:
	afx_msg void OnBnClickedStaticPresetBgPics(UINT nID);
public:
	afx_msg void OnStnClickedStaticViewerBorder();
public:
	afx_msg void OnStnClickedStaticViewerDiaDft();
public:
	afx_msg void OnStnClickedStaticViewerDiaCur();
public:
	afx_msg void OnStnClickedStaticViewerTheme1();
public:
	afx_msg void OnStnClickedStaticViewerTheme2();
public:
	afx_msg void OnStnClickedStaticViewerTheme3();
};
