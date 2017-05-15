#pragma once

struct CViewerSetting//CEdit control settings
{
public:
	CString m_strFaceName;//字体名
	long m_lWordHeight;//字体高度，由于是等宽字，所以就是大小
	long m_lLineOffset;//在行中的偏移量，用做行距
	COLORREF m_clrText;//文字颜色
	int m_nPitch;//间距，只能为1或者0-----占不支持，保留它，必须为0
	//
	int m_nFlagOfBg;//是使用纯色还是图片作为背景,1为纯色，2为自选图片,3为预置图片
	COLORREF m_clrBg;
	CString m_strPicturePath;
	UINT m_nIDOfBmp;
	
	//状态栏
	COLORREF m_clrBorder;//边框，
	COLORREF m_clrDiaDft;//进度条背景色也使用它
	COLORREF m_clrDiaCur;//进度数字也使用它,省事的话就和clrBorder相同
	//
public:
	CViewerSetting(CString strFaceName,int nHeight,int nOffset,COLORREF clrText,int nPitch,
		int nFlagOfBg,COLORREF clrBg,CString strPicturePath,UINT nIDOfBmp,COLORREF clrBorder,COLORREF clrDiaDft,COLORREF clrDiaCur)
	{
		m_strFaceName=strFaceName;
		m_lWordHeight=nHeight;
		m_lLineOffset=nOffset;
		m_clrText=clrText;
		m_nPitch=nPitch;
		
		m_nFlagOfBg=nFlagOfBg;
		m_clrBg=clrBg;
		m_strPicturePath=strPicturePath;
		m_nIDOfBmp=nIDOfBmp;
		
		m_clrBorder=clrBorder;
		m_clrDiaDft=clrDiaDft;
		m_clrDiaCur=clrDiaCur;
	}
	CViewerSetting()
	{
		//
		/*m_strFaceName=_T("宋体");
		m_lWordHeight=10*20;
		m_lLineOffset=0;
		m_clrText=0;
		m_nPitch=0;
		
		m_nFlagOfBg=1;
		m_clrBg=RGB(201,193,186);
		m_strPicturePath=_T("NULL");
		m_nIDOfBmp=IDB_BITMAP_BG1;*/
	}
};
struct CBlockSetting
{
	//配置信息
	COLORREF m_clrBlockBg;//块的背景颜色
	COLORREF m_clrBlockTitleBg;//标题背景颜色色
	COLORREF m_clrBlockTitle;//标题文字颜色
	COLORREF m_clrBlockText;//预览文字颜色
	
	COLORREF m_clrMouseOver;//鼠标经过时的边框颜色
	COLORREF m_clrMouseClick;//文件打开时的边框颜色
	//--------------------------
public:
	CBlockSetting(COLORREF clrBg,COLORREF clrTitleBg,COLORREF clrTitle,COLORREF clrText,
		COLORREF clrMouseOver,COLORREF clrMouseClick)
	{
		m_clrBlockBg=clrBg;
		m_clrBlockTitleBg=clrTitleBg;
		m_clrBlockTitle=clrTitle;
		m_clrBlockText=clrText;
		
		m_clrMouseOver=clrMouseOver;
		m_clrMouseClick=clrMouseClick;
	}
	CBlockSetting()
	{
	}
};
struct CReaderSetting
{
	COLORREF m_clrInterchange;//交换位置--->自定义设置
	COLORREF m_clrMoveLocation;//移动位置--->自定义设置
	int m_nFlagOfBg;//背景类型,1纯色，2磁盘图片，3预置图片
	COLORREF m_clrBg;//背景颜色
	CString m_strPicturePath;//背景图片路径
	UINT m_nIDOfBmp;//背景图片ID
public:
	CReaderSetting(COLORREF clrInterchange,COLORREF clrMoveLocation,int nFlagOfBg,
		COLORREF clrBg,CString strPicturePath,UINT nIDOfBmp)
	{
		m_clrInterchange=clrInterchange;
		m_clrMoveLocation=clrMoveLocation;
		m_nFlagOfBg=nFlagOfBg;
		m_clrBg=clrBg;
		m_strPicturePath=strPicturePath;
		m_nIDOfBmp=nIDOfBmp;
	}
	CReaderSetting()
	{
	}
};
extern int g_nIDOfBMPS[10];
extern CString g_strNameOfBmps[10];
extern CBlockSetting g_PresetBlockSettings[];
extern CReaderSetting g_PresetReaderSettings[];
extern CViewerSetting g_PresetViewerSettings[];