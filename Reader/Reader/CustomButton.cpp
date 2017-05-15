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
	ButtonDC.Attach(lpDrawItemStruct->hDC);//得到用于绘制按钮的DC
	rect=lpDrawItemStruct->rcItem;
	UINT state = lpDrawItemStruct->itemState;//获取按钮目前所处的状态，根据不同的状态绘制不同的按钮
	if(state&ODS_SELECTED)//如果按钮已经得到焦点，绘制选中状态下的按钮
	{
		CPen pen(PS_SOLID,1,RGB(0,0,128));
		CPen* pOldPen=ButtonDC.SelectObject(&pen);
		CBrush brush(RGB(128,0,0));
		CBrush* pOldBrush=ButtonDC.SelectObject(&brush);
		CFont font;
		font.CreatePointFont(120,_T("微软雅黑"));
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
		font.CreatePointFont(120,_T("微软雅黑"));
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
