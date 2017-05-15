#pragma once

struct CViewerSetting//CEdit control settings
{
public:
	CString m_strFaceName;//������
	long m_lWordHeight;//����߶ȣ������ǵȿ��֣����Ծ��Ǵ�С
	long m_lLineOffset;//�����е�ƫ�����������о�
	COLORREF m_clrText;//������ɫ
	int m_nPitch;//��ֻ࣬��Ϊ1����0-----ռ��֧�֣�������������Ϊ0
	//
	int m_nFlagOfBg;//��ʹ�ô�ɫ����ͼƬ��Ϊ����,1Ϊ��ɫ��2Ϊ��ѡͼƬ,3ΪԤ��ͼƬ
	COLORREF m_clrBg;
	CString m_strPicturePath;
	UINT m_nIDOfBmp;
	
	//״̬��
	COLORREF m_clrBorder;//�߿�
	COLORREF m_clrDiaDft;//����������ɫҲʹ����
	COLORREF m_clrDiaCur;//��������Ҳʹ����,ʡ�µĻ��ͺ�clrBorder��ͬ
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
		/*m_strFaceName=_T("����");
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
	//������Ϣ
	COLORREF m_clrBlockBg;//��ı�����ɫ
	COLORREF m_clrBlockTitleBg;//���ⱳ����ɫɫ
	COLORREF m_clrBlockTitle;//����������ɫ
	COLORREF m_clrBlockText;//Ԥ��������ɫ
	
	COLORREF m_clrMouseOver;//��꾭��ʱ�ı߿���ɫ
	COLORREF m_clrMouseClick;//�ļ���ʱ�ı߿���ɫ
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
	COLORREF m_clrInterchange;//����λ��--->�Զ�������
	COLORREF m_clrMoveLocation;//�ƶ�λ��--->�Զ�������
	int m_nFlagOfBg;//��������,1��ɫ��2����ͼƬ��3Ԥ��ͼƬ
	COLORREF m_clrBg;//������ɫ
	CString m_strPicturePath;//����ͼƬ·��
	UINT m_nIDOfBmp;//����ͼƬID
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