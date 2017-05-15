// ReaderSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Reader.h"
#include "ReaderSettingDlg.h"
#include "CustomMessages.h"

// CReaderSettingDlg dialog

IMPLEMENT_DYNAMIC(CReaderSettingDlg, CDialog)

CReaderSettingDlg::CReaderSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReaderSettingDlg::IDD, pParent)
{
	
}

CReaderSettingDlg::~CReaderSettingDlg()
{
}

void CReaderSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CReaderSettingDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_BLOCK_BG_COLOR, &CReaderSettingDlg::OnStnClickedStaticBlockBgColor)
	ON_STN_CLICKED(IDC_STATIC_BLOCK_TITLE_BG_COLOR, &CReaderSettingDlg::OnStnClickedStaticBlockTitleBg)
	ON_STN_CLICKED(IDC_STATIC_BLOCK_TITLE_COLOR, &CReaderSettingDlg::OnStnClickedStaticBlockTitleColor)
	ON_STN_CLICKED(IDC_STATIC_BLOCK_TEXT_COLOR, &CReaderSettingDlg::OnStnClickedStaticBlockTextColor)
	ON_STN_CLICKED(IDC_STATIC_MOUSEOVER, &CReaderSettingDlg::OnStnClickedStaticMouseOver)
	ON_STN_CLICKED(IDC_STATIC_MOUSECLICK, &CReaderSettingDlg::OnStnClickedStaticMouseClick)
	ON_STN_CLICKED(IDC_STATIC_INTERCHANGE, &CReaderSettingDlg::OnStnClickedStaticInterchange)
	ON_STN_CLICKED(IDC_STATIC_MOVELOCATION, &CReaderSettingDlg::OnStnClickedStaticMoveLocation)
	ON_BN_CLICKED(IDC_RADIO_PURECOLOR, &CReaderSettingDlg::OnBnClickedRadioPureColor)
	ON_STN_CLICKED(IDC_STATIC_PURECOLOR, &CReaderSettingDlg::OnStnClickedStaticPureColor)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_PICTURE, &CReaderSettingDlg::OnBnClickedButtonSelectPicture)
	ON_BN_CLICKED(IDC_RADIO_PICTURE, &CReaderSettingDlg::OnBnClickedRadioPicture)
	ON_BN_CLICKED(IDC_BUTTON_PRESET_BG_PIC, &CReaderSettingDlg::OnBnClickedButtonPresetBgPic)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_STATIC_PRESET_BG_PIC1,IDC_STATIC_PRESET_BG_PIC10,OnBnClickedStaticPresetBgPics)
	ON_STN_CLICKED(IDC_STATIC_THEME_MAIN_1, &CReaderSettingDlg::OnStnClickedStaticThemeMain1)
	ON_STN_CLICKED(IDC_STATIC_THEME_MAIN_2, &CReaderSettingDlg::OnStnClickedStaticThemeMain2)
	ON_STN_CLICKED(IDC_STATIC_THEME_MAIN_3, &CReaderSettingDlg::OnStnClickedStaticThemeMain3)
END_MESSAGE_MAP()

BOOL CReaderSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	
	//将它移动到父窗口旁边
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	CRect rect=wp.rcNormalPosition;
	int w=rect.Width();
	GetParent()->GetWindowPlacement(&wp);
	rect.left=wp.rcNormalPosition.right;
	rect.right=rect.left+w;
	MoveWindow(rect);
	//-------------------------
	
	GetDlgItem(IDC_EDIT_READER_PICTURE_PATH)->SetWindowTextW(m_sttgReaderCur.m_strPicturePath);
	if(m_sttgReaderCur.m_nFlagOfBg==3)
		GetDlgItem(IDC_EDIT_READER_PICTURE_PATH)->SetWindowTextW(g_strNameOfBmps[m_sttgReaderCur.m_nIDOfBmp-g_nIDOfBMPS[0]]);
	
	if(m_sttgReaderCur.m_nFlagOfBg==1)
	{
		((CButton*)GetDlgItem(IDC_RADIO_PURECOLOR))->SetCheck(TRUE);
		GetDlgItem(IDC_BUTTON_SELECT_PICTURE)->EnableWindow(FALSE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_PICTURE))->SetCheck(TRUE);
		GetDlgItem(IDC_STATIC_PURECOLOR)->EnableWindow(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CReaderSettingDlg message handlers

HBRUSH CReaderSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_BLOCK_BG_COLOR)//预览背景颜色
	{
		m_brushCtlColor.DeleteObject();
		m_brushCtlColor.CreateSolidBrush(m_sttgBlockCur.m_clrBlockBg);
		return (HBRUSH)m_brushCtlColor;
	}
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_BLOCK_TITLE_BG_COLOR)////预览标题背景颜色
	{
		m_brushCtlColor.DeleteObject();
		m_brushCtlColor.CreateSolidBrush(m_sttgBlockCur.m_clrBlockTitleBg);
		return (HBRUSH)m_brushCtlColor;
	}
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_BLOCK_TITLE_COLOR)////预览标题文字颜色
	{
		m_brushCtlColor.DeleteObject();
		m_brushCtlColor.CreateSolidBrush(m_sttgBlockCur.m_clrBlockTitle);
		return (HBRUSH)m_brushCtlColor;
	}
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_BLOCK_TEXT_COLOR)////预览文字颜色
	{
		m_brushCtlColor.DeleteObject();
		m_brushCtlColor.CreateSolidBrush(m_sttgBlockCur.m_clrBlockText);
		return (HBRUSH)m_brushCtlColor;
	}
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_MOUSEOVER)////鼠标经过边框颜色
	{
		m_brushCtlColor.DeleteObject();
		m_brushCtlColor.CreateSolidBrush(m_sttgBlockCur.m_clrMouseOver);
		return (HBRUSH)m_brushCtlColor;
	}
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_MOUSECLICK)////鼠标选择颜色
	{
		m_brushCtlColor.DeleteObject();
		m_brushCtlColor.CreateSolidBrush(m_sttgBlockCur.m_clrMouseClick);
		return (HBRUSH)m_brushCtlColor;
	}
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_INTERCHANGE)////交换位置提示颜色
	{
		m_brushCtlColor.DeleteObject();
		m_brushCtlColor.CreateSolidBrush(m_sttgReaderCur.m_clrInterchange);
		return (HBRUSH)m_brushCtlColor;
	}
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_MOVELOCATION)////移动位置提示颜色
	{
		m_brushCtlColor.DeleteObject();
		m_brushCtlColor.CreateSolidBrush(m_sttgReaderCur.m_clrMoveLocation);
		return (HBRUSH)m_brushCtlColor;
	}
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_PURECOLOR)////背景颜色
	{
		m_brushCtlColor.DeleteObject();
		m_brushCtlColor.CreateSolidBrush(m_sttgReaderCur.m_clrBg);
		return (HBRUSH)m_brushCtlColor;
	}
	return hbr;
}

void CReaderSettingDlg::OnStnClickedStaticBlockBgColor()
{
	CColorDialog clrdlg(m_sttgBlockCur.m_clrBlockBg,CC_FULLOPEN,this);
	if(clrdlg.DoModal()==IDOK)
	{
		m_sttgBlockCur.m_clrBlockBg=clrdlg.GetColor();
		GetParent()->PostMessageW(UM_READER_PREVIEW_SETTING,PREVIEW_BLOCK_BGCOLOR,(LPARAM)m_sttgBlockCur.m_clrBlockBg);
		GetDlgItem(IDC_STATIC_BLOCK_BG_COLOR)->RedrawWindow();
	}
}

void CReaderSettingDlg::OnStnClickedStaticBlockTitleBg()
{
	CColorDialog clrdlg(m_sttgBlockCur.m_clrBlockTitleBg,CC_FULLOPEN,this);
	if(clrdlg.DoModal()==IDOK)
	{
		m_sttgBlockCur.m_clrBlockTitleBg=clrdlg.GetColor();
		GetParent()->PostMessageW(UM_READER_PREVIEW_SETTING,PREVIEW_BLOCK_TITLE_BG,(LPARAM)m_sttgBlockCur.m_clrBlockTitleBg);
		GetDlgItem(IDC_STATIC_BLOCK_TITLE_BG_COLOR)->RedrawWindow();
	}
}

void CReaderSettingDlg::OnStnClickedStaticBlockTitleColor()
{
	CColorDialog clrdlg(m_sttgBlockCur.m_clrBlockTitle,CC_FULLOPEN,this);
	if(clrdlg.DoModal()==IDOK)
	{
		m_sttgBlockCur.m_clrBlockTitle=clrdlg.GetColor();
		GetParent()->PostMessageW(UM_READER_PREVIEW_SETTING,PREVIEW_BLOCK_TITLE,(LPARAM)m_sttgBlockCur.m_clrBlockTitle);
		GetDlgItem(IDC_STATIC_BLOCK_TITLE_COLOR)->RedrawWindow();
	}
}

void CReaderSettingDlg::OnStnClickedStaticBlockTextColor()
{
	CColorDialog clrdlg(m_sttgBlockCur.m_clrBlockText,CC_FULLOPEN,this);
	if(clrdlg.DoModal()==IDOK)
	{
		m_sttgBlockCur.m_clrBlockText=clrdlg.GetColor();
		GetParent()->PostMessageW(UM_READER_PREVIEW_SETTING,PREVIEW_BLOCK_TEXT,(LPARAM)m_sttgBlockCur.m_clrBlockText);
		GetDlgItem(IDC_STATIC_BLOCK_TEXT_COLOR)->RedrawWindow();
	}
}

void CReaderSettingDlg::OnStnClickedStaticMouseOver()
{
	CColorDialog clrdlg(m_sttgBlockCur.m_clrMouseOver,CC_FULLOPEN,this);
	if(clrdlg.DoModal()==IDOK)
	{
		m_sttgBlockCur.m_clrMouseOver=clrdlg.GetColor();
		GetParent()->PostMessageW(UM_READER_PREVIEW_SETTING,PREVIEW_BLOCK_MOUSEOVER,(LPARAM)m_sttgBlockCur.m_clrMouseOver);
		GetDlgItem(IDC_STATIC_MOUSEOVER)->RedrawWindow();
	}
}

void CReaderSettingDlg::OnStnClickedStaticMouseClick()
{
	CColorDialog clrdlg(m_sttgBlockCur.m_clrMouseClick,CC_FULLOPEN,this);
	if(clrdlg.DoModal()==IDOK)
	{
		m_sttgBlockCur.m_clrMouseClick=clrdlg.GetColor();
		GetParent()->PostMessageW(UM_READER_PREVIEW_SETTING,PREVIEW_BLOCK_MOUSECLICK,(LPARAM)m_sttgBlockCur.m_clrMouseClick);
		GetDlgItem(IDC_STATIC_MOUSECLICK)->RedrawWindow();
	}
}

void CReaderSettingDlg::OnStnClickedStaticInterchange()
{
	CColorDialog clrdlg(m_sttgReaderCur.m_clrInterchange,CC_FULLOPEN,this);
	if(clrdlg.DoModal()==IDOK)
	{
		m_sttgReaderCur.m_clrInterchange=clrdlg.GetColor();
		GetParent()->PostMessageW(UM_READER_PREVIEW_SETTING,PREVIEW_READER_INTERCHANGE,(LPARAM)m_sttgReaderCur.m_clrInterchange);
		GetDlgItem(IDC_STATIC_INTERCHANGE)->RedrawWindow();
	}
}

void CReaderSettingDlg::OnStnClickedStaticMoveLocation()
{
	CColorDialog clrdlg(m_sttgReaderCur.m_clrMoveLocation,CC_FULLOPEN,this);
	if(clrdlg.DoModal()==IDOK)
	{
		m_sttgReaderCur.m_clrMoveLocation=clrdlg.GetColor();
		GetParent()->PostMessageW(UM_READER_PREVIEW_SETTING,PREVIEW_READER_MOVELOCATION,(LPARAM)m_sttgReaderCur.m_clrMoveLocation);
		GetDlgItem(IDC_STATIC_MOVELOCATION)->RedrawWindow();
	}
}

void CReaderSettingDlg::OnBnClickedRadioPureColor()
{
	if(m_sttgReaderCur.m_nFlagOfBg==1)
		return;
	m_sttgReaderCur.m_nFlagOfBg=1;
	GetDlgItem(IDC_STATIC_PURECOLOR)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SELECT_PICTURE)->EnableWindow(FALSE);
	
	GetParent()->PostMessageW(UM_READER_PREVIEW_SETTING,PREVIEW_READER_BGCOLOR,(LPARAM)m_sttgReaderCur.m_clrBg);
	GetDlgItem(IDC_STATIC_PURECOLOR)->RedrawWindow();
}

void CReaderSettingDlg::OnBnClickedRadioPicture()
{
	if(m_sttgReaderCur.m_nFlagOfBg==2)
		return;
	m_sttgReaderCur.m_nFlagOfBg=2;
	GetDlgItem(IDC_STATIC_PURECOLOR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SELECT_PICTURE)->EnableWindow(TRUE);
	
	if(m_sttgReaderCur.m_strPicturePath!=CString(_T("NULL")))
	{
		GetParent()->PostMessageW(UM_READER_PREVIEW_SETTING,PREVIEW_READER_BGPICTURE,(LPARAM)&m_sttgReaderCur.m_strPicturePath);
		((CEdit*)GetDlgItem(IDC_EDIT_READER_PICTURE_PATH))->SetWindowTextW(m_sttgReaderCur.m_strPicturePath);
	}
	if(m_sttgReaderCur.m_nFlagOfBg==3)
	{
		GetParent()->PostMessageW(UM_READER_PREVIEW_SETTING,PREVIEW_READER_BGPICTURE_PRESET,(LPARAM)&m_sttgReaderCur.m_nIDOfBmp);
		GetDlgItem(IDC_EDIT_READER_PICTURE_PATH)->SetWindowTextW(g_strNameOfBmps[m_sttgReaderCur.m_nIDOfBmp-g_nIDOfBMPS[0]]);
	}
	//TRACE(_T("Clicked!\n"));
}

void CReaderSettingDlg::OnStnClickedStaticPureColor()
{
	CColorDialog clrdlg(m_sttgReaderCur.m_clrBg,CC_FULLOPEN,this);
	if(clrdlg.DoModal()==IDOK)
	{
		m_sttgReaderCur.m_clrBg=clrdlg.GetColor();
		GetParent()->PostMessageW(UM_READER_PREVIEW_SETTING,PREVIEW_READER_BGCOLOR,(LPARAM)m_sttgReaderCur.m_clrBg);
		GetDlgItem(IDC_STATIC_PURECOLOR)->RedrawWindow();
	}
}

void CReaderSettingDlg::OnBnClickedButtonSelectPicture()
{
	wchar_t szFilter[]=_T("bmp文件(*.bmp)|*.bmp||");//|jpg文件(*.jpg)|*.jpg
	CFileDialog filedlg(true,NULL,NULL,OFN_FILEMUSTEXIST,szFilter,this,0);
	if(filedlg.DoModal()==IDOK)
	{
		m_sttgReaderCur.m_strPicturePath=filedlg.GetPathName();
		GetParent()->PostMessageW(UM_READER_PREVIEW_SETTING,PREVIEW_READER_BGPICTURE,(LPARAM)&m_sttgReaderCur.m_strPicturePath);
		((CEdit*)GetDlgItem(IDC_EDIT_READER_PICTURE_PATH))->SetWindowTextW(m_sttgReaderCur.m_strPicturePath);
	}
}

void CReaderSettingDlg::OnBnClickedButtonPresetBgPic()
{
	// TODO: Add your control notification handler code here
	
}
void CReaderSettingDlg::OnBnClickedStaticPresetBgPics(UINT nID)
{
	int iIndex=nID-IDC_STATIC_PRESET_BG_PIC1;
	m_sttgReaderCur.m_nIDOfBmp=g_nIDOfBMPS[iIndex];
	m_sttgReaderCur.m_nFlagOfBg=3;
	GetParent()->PostMessageW(UM_READER_PREVIEW_SETTING,PREVIEW_READER_BGPICTURE_PRESET,m_sttgReaderCur.m_nIDOfBmp);
	((CEdit*)GetDlgItem(IDC_EDIT_READER_PICTURE_PATH))->SetWindowTextW(g_strNameOfBmps[iIndex]);
}
void CReaderSettingDlg::OnStnClickedStaticThemeMain1()
{
	GetParent()->PostMessageW(UM_READER_PREVIEW_SETTING,PREVIEW_READER_THEME,0);
}

void CReaderSettingDlg::OnStnClickedStaticThemeMain2()
{
	GetParent()->PostMessageW(UM_READER_PREVIEW_SETTING,PREVIEW_READER_THEME,1);
}

void CReaderSettingDlg::OnStnClickedStaticThemeMain3()
{
	GetParent()->PostMessageW(UM_READER_PREVIEW_SETTING,PREVIEW_READER_THEME,2);
}
