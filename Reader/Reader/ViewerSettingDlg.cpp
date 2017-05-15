// ViewerSettings.cpp : implementation file
//

#include "stdafx.h"
#include "Reader.h"
#include "ViewerSettingDlg.h"
#include "CustomMessages.h"

int CALLBACK EnumFontFamProc(LPENUMLOGFONT lpelf,LPNEWTEXTMETRIC lpntm,DWORD nFontType,LPARAM lparam)   
{ //枚举函数  
	CComboBox* pCombo=(CComboBox*)lparam;
	pCombo->AddString(lpelf->elfLogFont.lfFaceName);
	return 1;
}

// CViewerSettingDlg dialog

IMPLEMENT_DYNAMIC(CViewerSettingDlg, CDialog)

CViewerSettingDlg::CViewerSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewerSettingDlg::IDD, pParent)
{
	m_pParent=pParent;
}

CViewerSettingDlg::~CViewerSettingDlg()
{
}

void CViewerSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FONT_NAME, m_comboFontName);
	DDX_Control(pDX, IDC_COMBO_FONT_SIZE, m_comboFontSize);
	DDX_Control(pDX, IDC_COMBO_OFFSET, m_comboOffset);
}


BEGIN_MESSAGE_MAP(CViewerSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CViewerSettingDlg::OnBnClickedOk)
	ON_WM_SIZE()
	ON_STN_CLICKED(IDC_STATIC_COLOR, &CViewerSettingDlg::OnStnClickedStaticColor)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_FONT_NAME, &CViewerSettingDlg::OnCbnSelchangeComboFontName)
	ON_CBN_SELCHANGE(IDC_COMBO_FONT_SIZE, &CViewerSettingDlg::OnCbnSelchangeComboFontSize)
	ON_CBN_SELCHANGE(IDC_COMBO_OFFSET, &CViewerSettingDlg::OnCbnSelchangeComboOffset)
	ON_BN_CLICKED(IDC_RADIO_BG_PRUECOLOR, &CViewerSettingDlg::OnBnClickedRadioBgPruecolor)
	ON_BN_CLICKED(IDC_RADIO_BG_PICTURE, &CViewerSettingDlg::OnBnClickedRadioBgPicture)
	ON_STN_CLICKED(IDC_STATIC_BG_PURECOLOR, &CViewerSettingDlg::OnStnClickedStaticPureColor)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_PICTURE, &CViewerSettingDlg::OnBnClickedButtonSelectPicture)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_STATIC_PRESET_BG_PIC1,IDC_STATIC_PRESET_BG_PIC10,OnBnClickedStaticPresetBgPics)
	ON_STN_CLICKED(IDC_STATIC_VIEWER_BORDER, &CViewerSettingDlg::OnStnClickedStaticViewerBorder)
	ON_STN_CLICKED(IDC_STATIC_VIEWER_DIA_DFT, &CViewerSettingDlg::OnStnClickedStaticViewerDiaDft)
	ON_STN_CLICKED(IDC_STATIC_VIEWER_DIA_CUR, &CViewerSettingDlg::OnStnClickedStaticViewerDiaCur)
	ON_STN_CLICKED(IDC_STATIC_VIEWER_THEME1, &CViewerSettingDlg::OnStnClickedStaticViewerTheme1)
	ON_STN_CLICKED(IDC_STATIC_VIEWER_THEME2, &CViewerSettingDlg::OnStnClickedStaticViewerTheme2)
	ON_STN_CLICKED(IDC_STATIC_VIEWER_THEME3, &CViewerSettingDlg::OnStnClickedStaticViewerTheme3)
END_MESSAGE_MAP()


// CViewerSettingDlg message handlers
  
void CViewerSettingDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//m_pParent->PostMessage(UM_VIEWER_INIT_EDIT_SETTINGS);
	OnOK();
}

BOOL CViewerSettingDlg::OnInitDialog()
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
	
	GetFontNameFromSystem(m_comboFontName);//字体名组合框
	for(int i=m_comboFontName.GetCount()-1;i>=0;i--)
	{
		CString str;
		m_comboFontName.GetLBText(i,str);
		if(str==m_sttg.m_strFaceName){
			m_comboFontName.SetCurSel(i);
			break;
		}
	}
	
	CString str;
	for(int i=8;i<=24;i++)
	{
		str.Format(_T("%d"),i);
		m_comboFontSize.AddString(str);//文字大小组合框 
	}
	m_comboFontSize.SetCurSel(m_sttg.m_lWordHeight/20-8);
	
	//---------------
	m_brushTextColor.CreateSolidBrush(RGB(128,0,128));//文字颜色
	//((CButton*)GetDlgItem(IDC_CHECK_PITCH))->SetCheck(m_nPitch);//复选框----->暂不支持增加文字间距
	for(int i=0;i<21;i++)
	{
		str.Format(_T("%d"),i);
		m_comboOffset.AddString(str);//行距组合框
	}
	m_comboOffset.SetCurSel(m_sttg.m_lLineOffset/20);
	
	((CEdit*)GetDlgItem(IDC_EDIT_PICTURE_PATH))->SetWindowTextW(m_sttg.m_strPicturePath);//路径显示文本框
	if(m_sttg.m_nFlagOfBg==3)
		((CEdit*)GetDlgItem(IDC_EDIT_PICTURE_PATH))->SetWindowTextW(g_strNameOfBmps[m_sttg.m_nIDOfBmp-g_nIDOfBMPS[0]]);
	if(m_sttg.m_nFlagOfBg==1)
	{
		((CButton*)GetDlgItem(IDC_RADIO_BG_PRUECOLOR))->SetCheck(1);//单选按钮
		GetDlgItem(IDC_BUTTON_SELECT_PICTURE)->EnableWindow(FALSE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_BG_PICTURE))->SetCheck(1);
		GetDlgItem(IDC_STATIC_BG_PURECOLOR)->EnableWindow(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CViewerSettingDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	static int nFlag=0;
	if(!nFlag){
		//PostMessage(UM_SETTINGDLG_ENUM_FANT_FAMILIES);
		nFlag=1;
	}
	// TODO: Add your message handler code here
}

void CViewerSettingDlg::GetFontNameFromSystem(CComboBox& combo)
{
	LOGFONT lf;
	lf.lfCharSet=DEFAULT_CHARSET;
	memset(lf.lfFaceName,0,32);
	CPaintDC dc(this);
	::EnumFontFamiliesEx(dc.m_hDC,&lf,(FONTENUMPROC)EnumFontFamProc,(LPARAM)&combo,0);
	for(int i=0;i<combo.GetCount();i++)
	{
		CString strPre;
		CString strNext;
		combo.GetLBText(i,strPre);
		wchar_t* buffer=strPre.GetBuffer();
		if(buffer[0]==64){
			combo.DeleteString(i);//删除没效果，怪哉
			continue;
		}
		for(int j=i+1;j<combo.GetCount();)
		{
			combo.GetLBText(j,strNext);
			if(strNext==strPre)
				combo.DeleteString(j);//删除这个后下一个自动跟上，如果j增加的话会跳过这个家伙
			else break;//j++;//由于从系统获取字体时，字体名已经是排好序的，所以相同名字的项是扎堆的
			
		}
			
	}
}

HBRUSH CViewerSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_COLOR)
	{
		m_brushTextColor.DeleteObject();//更改静态文本框背景色
		m_brushTextColor.CreateSolidBrush(m_sttg.m_clrText);
		unsigned int r=GetRValue(m_sttg.m_clrText);
		unsigned int g=GetGValue(m_sttg.m_clrText);
		unsigned int b=GetBValue(m_sttg.m_clrText);
		b=~b,g=~g,r=~r;
		pDC->SetTextColor(RGB(r,g,b));//将文字颜色设置为相反的颜色
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)m_brushTextColor;
	}
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_BG_PURECOLOR)
	{
		m_brushBgColor.DeleteObject();//更改静态文本框背景色
		m_brushBgColor.CreateSolidBrush(m_sttg.m_clrBg);
		unsigned int r=GetRValue(m_sttg.m_clrBg);
		unsigned int g=GetGValue(m_sttg.m_clrBg);
		unsigned int b=GetBValue(m_sttg.m_clrBg);
		b=~b,g=~g,r=~r;
		pDC->SetTextColor(RGB(r,g,b));//将文字颜色设置为相反的颜色
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)m_brushBgColor;
	}
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_VIEWER_BORDER)
	{
		m_brushBgColor.DeleteObject();
		m_brushBgColor.CreateSolidBrush(m_sttg.m_clrBorder);
		return (HBRUSH)m_brushBgColor;
	}
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_VIEWER_DIA_DFT)//小方块的当前颜色
	{
		m_brushBgColor.DeleteObject();
		m_brushBgColor.CreateSolidBrush(m_sttg.m_clrDiaDft);
		return (HBRUSH)m_brushBgColor;
	}
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_VIEWER_DIA_CUR)//小方块的默认颜色
	{
		m_brushBgColor.DeleteObject();
		m_brushBgColor.CreateSolidBrush(m_sttg.m_clrDiaCur);
		return (HBRUSH)m_brushBgColor;
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CViewerSettingDlg::OnCbnSelchangeComboFontName()//字体选择
{
	// TODO: Add your control notification handler code here
	m_comboFontName.GetWindowText(m_sttg.m_strFaceName);
	GetParent()->PostMessageW(UM_VIEWER_PREVIEW_SETTING,PREVIWER_FACENAME,(LPARAM)&m_sttg.m_strFaceName);
}
void CViewerSettingDlg::OnCbnSelchangeComboFontSize()//文字大小选择
{
	// TODO: Add your control notification handler code here
	CString str;
	m_comboFontSize.GetWindowText(str);
	int n=0;
	wchar_t* buffer=str.GetBuffer();
	for(int i=0;buffer[i]!=0;i++)
	{
		n*=10;
		n+=buffer[i]-'0';
	}
	GetParent()->PostMessageW(UM_VIEWER_PREVIEW_SETTING,PREVIWER_HEIGHT,(LPARAM)n);
}
void CViewerSettingDlg::OnStnClickedStaticColor()//颜色选择
{
	CColorDialog clrdlg(m_sttg.m_clrText,CC_FULLOPEN,this);
	if(clrdlg.DoModal()==IDOK)
	{
		m_sttg.m_clrText=clrdlg.GetColor();
		GetParent()->PostMessageW(UM_VIEWER_PREVIEW_SETTING,PREVIWER_COLOR,(LPARAM)m_sttg.m_clrText);
		GetDlgItem(IDC_STATIC_COLOR)->RedrawWindow();
	}
}

void CViewerSettingDlg::OnCbnSelchangeComboOffset()
{
	// TODO: Add your control notification handler code here
	CString str;
	m_comboOffset.GetWindowText(str);
	int n=0;
	wchar_t* buffer=str.GetBuffer();
	for(int i=0;buffer[i]!=0;i++)
	{
		n*=10;
		n+=buffer[i]-'0';
	}
	GetParent()->PostMessageW(UM_VIEWER_PREVIEW_SETTING,PREVIWER_OFFSET,(LPARAM)n);
}

void CViewerSettingDlg::OnBnClickedRadioBgPruecolor()
{
	// TODO: Add your control notification handler code here
	if(m_sttg.m_nFlagOfBg==1)
		return;
	m_sttg.m_nFlagOfBg=1;
	GetDlgItem(IDC_BUTTON_SELECT_PICTURE)->EnableWindow(FALSE);//PIC
	GetDlgItem(IDC_STATIC_BG_PURECOLOR)->EnableWindow(TRUE);//CLR
	
	GetParent()->PostMessageW(UM_VIEWER_PREVIEW_SETTING,PREVIWER_BGCOLOR,(LPARAM)m_sttg.m_clrBg);
	GetDlgItem(IDC_STATIC_BG_PURECOLOR)->RedrawWindow();
}

void CViewerSettingDlg::OnBnClickedRadioBgPicture()
{
	// TODO: Add your control notification handler code here
	if(m_sttg.m_nFlagOfBg==2)
		return;
	m_sttg.m_nFlagOfBg=2;
	GetDlgItem(IDC_BUTTON_SELECT_PICTURE)->EnableWindow(TRUE);//PIC
	GetDlgItem(IDC_STATIC_BG_PURECOLOR)->EnableWindow(FALSE);//CLR
	if(m_sttg.m_strPicturePath!=CString(_T("NULL")))
	{
		GetParent()->PostMessageW(UM_VIEWER_PREVIEW_SETTING,PREVIWER_BGPIC,(LPARAM)&m_sttg.m_strPicturePath);
		((CEdit*)GetDlgItem(IDC_EDIT_PICTURE_PATH))->SetWindowTextW(m_sttg.m_strPicturePath);
	}
}

void CViewerSettingDlg::OnStnClickedStaticPureColor()
{
	// TODO: Add your control notification handler code here
	CColorDialog clrdlg(m_sttg.m_clrBg,CC_FULLOPEN,this);
	if(clrdlg.DoModal()==IDOK)
	{
		m_sttg.m_clrBg=clrdlg.GetColor();
		GetParent()->PostMessageW(UM_VIEWER_PREVIEW_SETTING,PREVIWER_BGCOLOR,(LPARAM)m_sttg.m_clrBg);
		GetDlgItem(IDC_STATIC_BG_PURECOLOR)->RedrawWindow();
	}
}

void CViewerSettingDlg::OnBnClickedButtonSelectPicture()
{
	wchar_t szFilter[]=_T("bmp文件(*.bmp)|*.bmp||");//|jpg文件(*.jpg)|*.jpg
	CFileDialog filedlg(true,NULL,NULL,OFN_FILEMUSTEXIST,szFilter,this,0);
	if(filedlg.DoModal()==IDOK)
	{
		m_sttg.m_strPicturePath=filedlg.GetPathName();
		GetParent()->PostMessageW(UM_VIEWER_PREVIEW_SETTING,PREVIWER_BGPIC,(LPARAM)&m_sttg.m_strPicturePath);
		((CEdit*)GetDlgItem(IDC_EDIT_PICTURE_PATH))->SetWindowTextW(m_sttg.m_strPicturePath);
	}
}
void CViewerSettingDlg::OnBnClickedStaticPresetBgPics(UINT nID)
{
	int iIndex=nID-IDC_STATIC_PRESET_BG_PIC1;
	m_sttg.m_nIDOfBmp=g_nIDOfBMPS[iIndex];
	m_sttg.m_nFlagOfBg=3;
	GetParent()->PostMessageW(UM_VIEWER_PREVIEW_SETTING,PREVIWER_BGPIC_PRESET,m_sttg.m_nIDOfBmp);
	((CEdit*)GetDlgItem(IDC_EDIT_PICTURE_PATH))->SetWindowTextW(g_strNameOfBmps[iIndex]);
}
void CViewerSettingDlg::OnStnClickedStaticViewerBorder()
{
	CColorDialog clrdlg(m_sttg.m_clrBorder,CC_FULLOPEN,this);
	if(clrdlg.DoModal()==IDOK)
	{
		m_sttg.m_clrBorder=clrdlg.GetColor();
		GetParent()->PostMessageW(UM_VIEWER_PREVIEW_SETTING,PREVIWER_BORDER,(LPARAM)m_sttg.m_clrBorder);
		GetDlgItem(IDC_STATIC_VIEWER_BORDER)->RedrawWindow();
	}
}

void CViewerSettingDlg::OnStnClickedStaticViewerDiaDft()
{
	CColorDialog clrdlg(m_sttg.m_clrDiaDft,CC_FULLOPEN,this);
	if(clrdlg.DoModal()==IDOK)
	{
		m_sttg.m_clrDiaDft=clrdlg.GetColor();
		GetParent()->PostMessageW(UM_VIEWER_PREVIEW_SETTING,PREVIWER_DIA_DFT,(LPARAM)m_sttg.m_clrDiaDft);
		GetDlgItem(IDC_STATIC_VIEWER_DIA_DFT)->RedrawWindow();
	}
}

void CViewerSettingDlg::OnStnClickedStaticViewerDiaCur()
{
	CColorDialog clrdlg(m_sttg.m_clrDiaCur,CC_FULLOPEN,this);
	if(clrdlg.DoModal()==IDOK)
	{
		m_sttg.m_clrDiaCur=clrdlg.GetColor();
		GetParent()->PostMessageW(UM_VIEWER_PREVIEW_SETTING,PREVIWER_DIA_CUR,(LPARAM)m_sttg.m_clrDiaCur);
		GetDlgItem(IDC_STATIC_VIEWER_DIA_CUR)->RedrawWindow();
	}
}

void CViewerSettingDlg::OnStnClickedStaticViewerTheme1()
{
	GetParent()->PostMessageW(UM_VIEWER_PREVIEW_SETTING,PREVIWER_VIEWER_THEME,0);
}

void CViewerSettingDlg::OnStnClickedStaticViewerTheme2()
{
	GetParent()->PostMessageW(UM_VIEWER_PREVIEW_SETTING,PREVIWER_VIEWER_THEME,1);
}

void CViewerSettingDlg::OnStnClickedStaticViewerTheme3()
{
	GetParent()->PostMessageW(UM_VIEWER_PREVIEW_SETTING,PREVIWER_VIEWER_THEME,2);
	m_sttg=g_PresetViewerSettings[2];
	RedrawWindow();
}
