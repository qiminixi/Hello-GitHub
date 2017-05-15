// CustomButton.cpp : implementation file
//

#include "stdafx.h"
#include "Reader.h"
#include "CustomButton.h"


// CCustomButton

IMPLEMENT_DYNAMIC(CCustomButton, CButton)

CCustomButton::CCustomButton()
{

}

CCustomButton::~CCustomButton()
{
}


BEGIN_MESSAGE_MAP(CCustomButton, CButton)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()



// CCustomButton message handlers



void CCustomButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
}

void CCustomButton::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC ButtonDC;
	CBitmap bitmapTrans;
	CRect rect;
	ButtonDC.Attach(lpDrawItemStruct->hDC);//�õ����ڻ��ư�ť��DC
	rect=lpDrawItemStruct->rcItem;
	UINT state = lpDrawItemStruct->itemState;//��ȡ��ťĿǰ������״̬�����ݲ�ͬ��״̬���Ʋ�ͬ�İ�ť
	if(state&ODS_SELECTED)//�����ť�Ѿ��õ����㣬����ѡ��״̬�µİ�ť
	{
		CPen pen(PS_SOLID,1,RGB(0,0,128));
		CPen* pOldPen=ButtonDC.SelectObject(&pen);
		CBrush brush(RGB(128,0,0));
		CBrush* pOldBrush=ButtonDC.SelectObject(&brush);
		CFont font;
		font.CreatePointFont(120,_T("΢���ź�"));
		CFont* pOldFont=ButtonDC.SelectObject(&font);
		ButtonDC.Rectangle(rect);
		ButtonDC.SetBkMode(TRANSPARENT);
		if(nIDCtl==IDC_BUTTON_LOAD_NEXT)
			ButtonDC.TextOutW(0,0,_T(">>"));
		if(nIDCtl==IDC_BUTTON_LOAD_PRE)
			ButtonDC.TextOutW(0,0,_T("<<"));
		ButtonDC.SelectObject(pOldPen);
		ButtonDC.SelectObject(pOldBrush);
		ButtonDC.SelectObject(pOldFont);
	}
	else
	{
		CPen pen(PS_SOLID,1,RGB(0,0,128));
		CPen* pOldPen=ButtonDC.SelectObject(&pen);
		CBrush brush(RGB(128,0,128));
		CBrush* pOldBrush=ButtonDC.SelectObject(&brush);
		CFont font;
		font.CreatePointFont(120,_T("΢���ź�"));
		CFont* pOldFont=ButtonDC.SelectObject(&font);
		ButtonDC.Rectangle(rect);
		ButtonDC.SetBkMode(TRANSPARENT);
		if(nIDCtl==IDC_BUTTON_LOAD_NEXT)
			ButtonDC.TextOutW(0,0,_T(">>"));
		if(nIDCtl==IDC_BUTTON_LOAD_PRE)
			ButtonDC.TextOutW(0,0,_T("<<"));
		ButtonDC.SelectObject(pOldPen);
		ButtonDC.SelectObject(pOldBrush);
		ButtonDC.SelectObject(pOldFont);
	}
	//CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CCustomButton::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0,BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}
