// TextViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Reader.h"
#include "TextViewerDlg.h"
#include "GlobalFunctions.h"
#include "CustomMessages.h"
#include "ViewerSettingDlg.h"
#include "BookmarkDlg.h"

// CTextViewerDlg dialog

CViewerSetting CTextViewerDlg::m_sttgDefault=CViewerSetting(_T("宋体"),10*20,0,RGB(0,0,0),0,
	1,RGB(158,158,158),_T("NULL"),IDB_BITMAP_BG1,RGB(50,50,50),RGB(111,111,111),RGB(65,65,65));
CViewerSetting CTextViewerDlg::m_sttgCur=CTextViewerDlg::m_sttgDefault;
CViewerSetting CTextViewerDlg::m_sttgPre=CTextViewerDlg::m_sttgDefault;
CRect CTextViewerDlg::m_rectViewerWindow=CRect(245,245,500,500);

IMPLEMENT_DYNAMIC(CTextViewerDlg, CDialog)

CTextViewerDlg::CTextViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextViewerDlg::IDD, pParent),m_edit(NULL)
{
	
}
CTextViewerDlg::CTextViewerDlg(CWnd* pParent,CBlock* pBlock,int iBlock,CRect rectWindow)
:m_strText('\0',FILE_BLOCK_SIZE+FILE_BLOCK_REMAIN_SIZE),m_strTextA('\0',FILE_BLOCK_SIZE+FILE_BLOCK_REMAIN_SIZE)
,m_edit(&pBlock->m_nReadPos)
{
	m_pParent=pParent;
	m_pBlock=pBlock;
	m_rectWindow=rectWindow;
	m_iBlock=iBlock;
	
	m_nFlagCancelEditSel=0;
	m_bmpBg.m_hObject=NULL;
	m_bTurnToLastPos=0;
}
CTextViewerDlg::~CTextViewerDlg()
{
}
void CTextViewerDlg::InitViewer(CWnd* pParent,CBlock* pBlock,int iBlock)
{
	m_pParent=pParent;
	m_pBlock=pBlock;
	m_iBlock=iBlock;
	
	m_nFlagCancelEditSel=0;
	if(m_bmpBg.m_hObject!=0)
		m_bmpBg.DeleteObject();
	m_bmpBg.m_hObject=NULL;
	m_bTurnToLastPos=0;
	
	if(m_pParent!=NULL)
	{
		if(ReadTextFromDisk())
			m_edit.SetWindowTextW(m_strText);
		else
			m_pParent->PostMessageW(UM_VIEWERCLOSE,(WPARAM)this,(LPARAM)m_iBlock);
		SetWindowText(m_pBlock->m_strFileName);
		m_edit.m_pReadPos=&pBlock->m_nReadPos;
	}
}
void CTextViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTextViewerDlg, CDialog)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &CTextViewerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTextViewerDlg::OnBnClickedCancel)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_MESSAGE(UM_VIEWER_SETTINGS,OnViewerSettings)
	ON_MESSAGE(UM_VIEWER_PREVIEW_SETTING,OnPreviewSetting)
	ON_MESSAGE(UM_VIEWER_UPDATE_PROGRESS,OnUpdateProgress)
	ON_MESSAGE(UM_VIEWER_BOOKMARK_ADD,OnAddBookmark)
	ON_MESSAGE(UM_VIEWER_BOOKMARK_MANAGE,OnManageBookmark)
	ON_MESSAGE(UM_VIEWER_JUMPTO_BOOKMARK,OnJumpToBookmark)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_PRE, &CTextViewerDlg::OnBnClickedButtonLoadPre)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_NEXT, &CTextViewerDlg::OnBnClickedButtonLoadNext)
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// CTextViewerDlg message handlers

int CTextViewerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  Add your specialized creation code here

	return 0;
}
BOOL CTextViewerDlg::Create()
{
	return CDialog::Create(CTextViewerDlg::IDD,GetDesktopWindow());
}
void CTextViewerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CTextViewerDlg::OnBnClickedCancel()
{
	//记录窗口位置
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	m_rectViewerWindow=wp.rcNormalPosition;
	
	int iPreviewPos=m_edit.GetFirstVisibleLine();
	iPreviewPos=m_edit.LineIndex(iPreviewPos);
	MakePreviewText(m_pBlock->m_strPreview,iPreviewPos);

	m_pParent->PostMessageW(UM_VIEWERCLOSE,(WPARAM)this,(LPARAM)m_iBlock);
	
	//------------------------------------
	OnCancel();
}

BOOL CTextViewerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

BOOL CTextViewerDlg::OnInitDialog()
{
	//MoveWindow(m_rectViewerWindow);
	CDialog::OnInitDialog();
	// TODO:  Add extra initialization here
	
	
	CRect rectClient;
	GetClientRect(rectClient);
	m_edit.CreateEx(WS_EX_TRANSPARENT,WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN|ES_AUTOVSCROLL|WS_VSCROLL,rectClient,this,12345);
	if(m_pParent!=NULL)
	{
		if(ReadTextFromDisk())
			m_edit.SetWindowTextW(m_strText);
		else
			m_pParent->PostMessageW(UM_VIEWERCLOSE,(WPARAM)this,(LPARAM)m_iBlock);
		SetWindowText(m_pBlock->m_strFileName);
	}
	else
		m_edit.m_pReadPos=NULL;
	m_edit.SetFont(m_sttgCur);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CTextViewerDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	CRect rectClient;
	GetClientRect(rectClient);
	
	CDC MemDC; //首先定义一个显示设备对象
	CBitmap MemBitmap;//定义一个位图对象
	MemDC.CreateCompatibleDC(NULL);//创建内存设备
	MemBitmap.CreateCompatibleBitmap(&dc,rectClient.Width(),rectClient.Height());//创建位图
	CBitmap *pOldBitmap=MemDC.SelectObject(&MemBitmap);
	
	CBrush brushBg;
	MemDC.SelectStockObject(NULL_PEN);
	if(m_sttgCur.m_nFlagOfBg==1)//纯色背景
		brushBg.CreateSolidBrush(m_sttgCur.m_clrBg);
	else if(m_sttgCur.m_nFlagOfBg==2)//位图背景
	{
		if(m_bmpBg.m_hObject!=NULL)//用于第一次载入。
			m_bmpBg.DeleteObject();
		HBITMAP hBitmap=(HBITMAP)::LoadImage(NULL,m_sttgCur.m_strPicturePath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		m_bmpBg.Attach(hBitmap);
		brushBg.CreatePatternBrush(&m_bmpBg);
	}
	else if(m_sttgCur.m_nFlagOfBg==3)
	{
		if(m_bmpBg.m_hObject!=NULL)//用于第一次载入。
			m_bmpBg.DeleteObject();
		m_bmpBg.LoadBitmapW(m_sttgCur.m_nIDOfBmp);
		brushBg.CreatePatternBrush(&m_bmpBg);
	}
	MemDC.FillRect(rectClient,&brushBg);
	
	//分块情况
	m_nDiaWidth=DIA_HEIGHT;//如果块的数量过多，将会减小宽度以容纳所有块
	CPen pen(PS_SOLID,1,m_sttgCur.m_clrBorder);
	CPen* pOldPen=MemDC.SelectObject(&pen);
	MemDC.SelectStockObject(NULL_BRUSH);
	CRect rect(0,0,rectClient.Width()-BTN_WIDTH*2-PG_WIDTH,DIA_HEIGHT+DIA_MARGIN*2);
	MemDC.Rectangle(rect);//块所在的区域
	
	if((m_nDiaWidth+DIA_GAP)*m_nFileBlocks>rectClient.Width()-DIA_MARGIN*2-BTN_WIDTH*2-PG_WIDTH)
		m_nDiaWidth=(rectClient.Width()-DIA_MARGIN*2-DIA_GAP*m_nFileBlocks-BTN_WIDTH*2-PG_WIDTH)/m_nFileBlocks;
	CRect rectDia(DIA_MARGIN,DIA_MARGIN,m_nDiaWidth,DIA_HEIGHT);
	CBrush brushDft(m_sttgCur.m_clrDiaDft),brushCur(m_sttgCur.m_clrDiaCur);
	//MemDC.SetBkMode(TRANSPARENT);
	//MemDC.SetTextColor(RGB(255,255,255));
	for(int i=0;i<m_nFileBlocks;i++)
	{
		CString str;
		str.Format(_T("%d"),i);
		if(m_pBlock->m_iFileBlock!=i)
			MemDC.FillRect(rectDia,&brushDft);
		else
			MemDC.FillRect(rectDia,&brushCur);
		//MemDC.TextOutW(rectDia.left+3,rectDia.top,str);
		rectDia.left+=m_nDiaWidth+DIA_GAP;
		rectDia.right+=m_nDiaWidth+DIA_GAP;	
	}
	//----------------------------------
	
	//进度显示
	CFont font;
	font.CreatePointFont(90,_T("微软雅黑"));
	CFont* pOldFont=MemDC.SelectObject(&font);
	CBrush brushRate(m_sttgCur.m_clrDiaDft);//使用clrDft作为背景
	CBrush* pOldBrush=MemDC.SelectObject(&brushRate);
	CPoint ptPG(rect.Width(),0);
	MemDC.Rectangle(CRect(ptPG,CSize(PG_WIDTH/2,DIA_HEIGHT+DIA_MARGIN*2)));
	float f=GetRateOfProgress();
	CString str;
	str.Format(_T("%2.1f%%"),f*100);
	if(f>=1)
		str.Format(_T("%2.0f%%"),f*100);
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.SetTextColor(m_sttgCur.m_clrDiaCur);//使用clrCur作为文字颜色
	MemDC.TextOutW(ptPG.x+3,ptPG.y+3,str);
	
	ptPG.x+=PG_WIDTH/2;
	MemDC.Rectangle(CRect(ptPG,CSize(PG_WIDTH/2,DIA_HEIGHT+DIA_MARGIN*2)));
	str.Format(_T("%d/%d"),m_pBlock->m_iFileBlock+1,m_nFileBlocks);
	//MemDC.DrawText(str,CRect(ptPG,CSize(PG_WIDTH/2,DIA_HEIGHT+DIA_MARGIN*2)),DT_CENTER|DT_VCENTER);
	MemDC.TextOutW(ptPG.x+3,ptPG.y+3,str);
	//-----------------------------------
	
	dc.BitBlt(0,0,rectClient.Width(),rectClient.Height(),&MemDC,0,0,SRCCOPY);
	
	MemBitmap.DeleteObject();
	MemDC.SelectObject(pOldBitmap);
	MemDC.SelectObject(pOldFont);
	MemDC.SelectObject(pOldPen);
	MemDC.SelectObject(pOldBrush);
	MemDC.DeleteDC();
	
}
bool CTextViewerDlg::ReadTextFromDisk()
{
	HANDLE hFileRead=CreateFile(m_pBlock->m_strFilePath,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,0);
	if(hFileRead==INVALID_HANDLE_VALUE)
	{
		CString str;
		str.Format(_T("无法打开文件：%s"),m_pBlock->m_strFilePath);
		MessageBox(str);
		return false;
	}
	
	
	m_dwFileSize=GetFileSize(hFileRead,NULL);
	if(m_pBlock->m_iFileBlock*FILE_BLOCK_SIZE>m_dwFileSize){
		MessageBox(_T("读取的位置超出了文件边界"));
		return false;
	}
	m_nFileBlocks=m_dwFileSize/FILE_BLOCK_SIZE+1;
	
	//使用字符数相同的字符数组，这是因为如果读取的是全英文的ANSI文件，转换后W字符串的长度和A的相同。
	m_nFileTypeCode=JudgeFileCode(hFileRead);
	wchar_t* buffer=m_strText.GetBuffer();
	memset(buffer,0,m_strText.GetLength()*2);
	DWORD dwBytesRead;
	if(m_nFileTypeCode==1)//ANSI
	{
		char* bufferA=m_strTextA.GetBuffer();
		memset(bufferA,0,FILE_BLOCK_SIZE+FILE_BLOCK_REMAIN_SIZE);//全部清零
		
		//读取文件
		if(m_pBlock->m_iFileBlock==0){
			ReadFile(hFileRead,bufferA,FILE_BLOCK_SIZE,&dwBytesRead,NULL);//一个元素装一个字节
		}
		else{
			SetFilePointer(hFileRead,FILE_BLOCK_SIZE*m_pBlock->m_iFileBlock-FILE_BLOCK_REMAIN_SIZE,0,FILE_BEGIN);//减FILE_BLOCK_REMAIN_SIZE保留上一个块的末尾
			ReadFile(hFileRead,bufferA,FILE_BLOCK_SIZE+FILE_BLOCK_REMAIN_SIZE,&dwBytesRead,NULL);//一个元素装一个字节
		}
		//----------------
		int n=0;
		for(unsigned int i=0;i<dwBytesRead;i++){
			if(bufferA[i]<-1) n++;//判断开头的中文字符是否完整
			else break;
		}
		if(n%2==1)
			bufferA[0]=' ';//将开头的半个字叉掉
		n=0;
		for(unsigned int i=dwBytesRead-1;i>=0;i--){
			if(bufferA[i]<-1) n++;//判断中文字符
			else break;
		}
		if(n%2==1) 
			bufferA[dwBytesRead-1]='\0';//如果字符数为奇数，那么一定是最后一个汉字被分割了，剔除掉
		MultiByteToWideChar(CP_ACP,0,bufferA,-1,buffer,FILE_BLOCK_SIZE+FILE_BLOCK_REMAIN_SIZE);
	}
	else if(m_nFileTypeCode==2)//Unicode
	{
		SetFilePointer(hFileRead,2,NULL,FILE_BEGIN);//跳过编码
		
		if(m_pBlock->m_iFileBlock==0){
			ReadFile(hFileRead,buffer,FILE_BLOCK_SIZE*2,&dwBytesRead,NULL);//一个元素装两个字节
		}
		else{
			SetFilePointer(hFileRead,(FILE_BLOCK_SIZE*m_pBlock->m_iFileBlock-FILE_BLOCK_REMAIN_SIZE)*2,0,FILE_BEGIN);//减FILE_BLOCK_REMAIN_SIZE保留上一个块的末尾
			ReadFile(hFileRead,buffer,(FILE_BLOCK_SIZE+FILE_BLOCK_REMAIN_SIZE)*2,&dwBytesRead,NULL);//一个元素装两个字节
		}
		m_nFileBlocks=m_dwFileSize/FILE_BLOCK_SIZE/2+1;//Uicode一个块多装一倍(中文)
	}
	else if(m_nFileTypeCode==3)//UTF-8
	{
		SetFilePointer(hFileRead,3,NULL,FILE_BEGIN);//跳过编码
		
		char* bufferA=m_strTextA.GetBuffer();
		memset(bufferA,0,FILE_BLOCK_SIZE+FILE_BLOCK_REMAIN_SIZE);//全部清零
		
		//读取文件
		if(m_pBlock->m_iFileBlock==0){
			ReadFile(hFileRead,bufferA,FILE_BLOCK_SIZE,&dwBytesRead,NULL);//一个元素装一个字节
		}
		else{
			SetFilePointer(hFileRead,FILE_BLOCK_SIZE*m_pBlock->m_iFileBlock-FILE_BLOCK_REMAIN_SIZE,0,FILE_BEGIN);//减FILE_BLOCK_REMAIN_SIZE保留上一个块的末尾
			ReadFile(hFileRead,bufferA,FILE_BLOCK_SIZE+FILE_BLOCK_REMAIN_SIZE,&dwBytesRead,NULL);//一个元素装一个字节
		}
		
		int i=0;
		//去掉前面的截断
		for(i;;i++)//判断最高位为1用(x&0x80)结果大于0就是,判断第二高位为0用(x&0x40)结果为0就是
		{
			if(bufferA[i]>=0 && bufferA[i]<=0x7F)//为ASCII字符的话结束
				break;
			if( (bufferA[i]&0x80) && !(bufferA[i]&0x40) )//前两位为10的话，继续
			{	bufferA[i]=' '; continue;}
			else//前两位不是10，它也不是ASCII
				break;
		}
		//------------------------------------
		
		//去掉后面的截断
		i=dwBytesRead-1;
		for(i;i>=0;i--)//判断最高位为1用(x&0x80)结果大于0就是,判断第二高位为0用(x&0x40)结果为0就是
		{
			if(bufferA[i]>=0 && bufferA[i]<=0x7F)//为ASCII字符的话结束
				break;
			if( (bufferA[i]&0x80) && !(bufferA[i]&0x40) )//前两位为10的话，继续
				continue;
			else
				break;//前两位不是10，它也不是ASCII
		}
		bufferA[i]='\0';//管他那么多，砍掉
		//--------------------------------------
		
		MultiByteToWideChar(CP_UTF8,0,bufferA,-1,buffer,FILE_BLOCK_SIZE+FILE_BLOCK_REMAIN_SIZE);
	}
	else if(m_nFileTypeCode==4)//Unicode
	{
		SetFilePointer(hFileRead,2,NULL,FILE_BEGIN);//跳过编码
		
		if(m_pBlock->m_iFileBlock==0){
			ReadFile(hFileRead,buffer,FILE_BLOCK_SIZE*2,&dwBytesRead,NULL);//一个元素装两个字节
		}
		else{
			SetFilePointer(hFileRead,(FILE_BLOCK_SIZE*m_pBlock->m_iFileBlock-FILE_BLOCK_REMAIN_SIZE)*2,0,FILE_BEGIN);//减FILE_BLOCK_REMAIN_SIZE保留上一个块的末尾
			ReadFile(hFileRead,buffer,(FILE_BLOCK_SIZE+FILE_BLOCK_REMAIN_SIZE)*2,&dwBytesRead,NULL);//一个元素装两个字节
		}
		for(unsigned int i=0;i<dwBytesRead/2;i++)
		{
			buffer[i]=((buffer[i]>>8)& 0x00FF) | ((buffer[i]<<8)& 0xFF00);
		}
		m_nFileBlocks=m_dwFileSize/FILE_BLOCK_SIZE/2+1;//Uicode一个块多装一倍(中文)
	}
	else if(m_nFileTypeCode==0)//
	{
		MessageBox(_T("无法识别的文本编码"));
	}
	
	
	return true;
	CloseHandle(hFileRead);
}
void CTextViewerDlg::MakePreviewText(CString& strPreview,int iPreviewPos)
{
	//向前读取一半，包括iPreviewPos
	int i=BLOCK_PREVIEW_SIZE/2-1;
	wchar_t* bufferPreview=strPreview.GetBuffer();
	memset(bufferPreview,0,BLOCK_PREVIEW_SIZE*2);
	wchar_t* bufferText=m_strText.GetBuffer();
	int j=0;
	for(i;i>=0;i--)
	{
		if(iPreviewPos-i<0){//超过数组头部
			continue;
		}
		bufferPreview[j]=bufferText[iPreviewPos-i];
		j++;
	}
	//---------------------------------------
	//向后读取一半，不包括iPreviewPos
	int k=BLOCK_PREVIEW_SIZE/2-1-j;
	for(i=1;i<=BLOCK_PREVIEW_SIZE/2+k;i++)
	{
		if(bufferText[iPreviewPos+i]==0)
			break;
		bufferPreview[j]=bufferText[iPreviewPos+i];
		j++;
	}
	//---------------------------------
	//宽字符不存在将汉子拆开的问题
	strPreview.Replace('\r',' ');//删除，没价值而且影响读档
	strPreview.Replace('\n',' ');//删除，没价值而且影响读档
}
void CTextViewerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if(!m_nFlagCancelEditSel){
		m_edit.SetSel(-1,0);
		m_nFlagCancelEditSel=1;
	}
	CRect rectClient;
	GetClientRect(rectClient);
	CRect rectEditWnd(rectClient);
	rectEditWnd.top+=DIA_HEIGHT+DIA_MARGIN*2;
	//rectEditWnd.left+=300;
	//rectEditWnd.right-=300;
	//rectEditWnd.bottom-=200;
	m_edit.MoveWindow(rectEditWnd);
	
	m_edit.SendMessage(WM_VSCROLL,SB_TOP,0);//先滚动到最前，有时初始化时会自己滚动到某个地方，窗口大小改变也要随时定位
	m_edit.LineScroll(m_edit.LineFromChar(m_pBlock->m_nReadPos));//MoveWindow已经生效了，比较奇怪。
	
	int w=rectClient.Width();
	GetDlgItem(IDC_BUTTON_LOAD_PRE)->MoveWindow(w-BTN_WIDTH*2,0,BTN_WIDTH,DIA_HEIGHT+DIA_MARGIN*2);
	GetDlgItem(IDC_BUTTON_LOAD_NEXT)->MoveWindow(w-BTN_WIDTH,0,BTN_WIDTH,DIA_HEIGHT+DIA_MARGIN*2);
	Invalidate();//强制重绘
	
}

void CTextViewerDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
	//MessageBox(_T("Viewer:GotIt"));
	/*int n=m_edit.GetFirstVisibleLine();
	int m=m_edit.LineIndex(n);
	TRACE(_T("FirstVisibleLine-->%d\n"),n);
	wchar_t c[3];
	c[2]=m_strText.GetAt(m);
	c[1]=m_strText.GetAt(m-2);
	c[0]=m_strText.GetAt(m-1);
	TRACE(_T("%c\n"),c);*/
	
}

LRESULT CTextViewerDlg::OnViewerSettings(WPARAM wParam,LPARAM lParam)
{
	CViewerSettingDlg dlg(this);
	
	//dlg.m_strFaceName=m_sttgCur.m_strFaceName;
	//dlg.m_nHeight=m_sttgCur.m_lWordHeight;
	//dlg.m_nOffset=m_sttgCur.m_lLineOffset;
	//dlg.m_nPitch=m_sttgCur.m_nPitch;
	//dlg.m_clrText=m_sttgCur.m_clrText;
	///////////////
	//dlg.m_nFlagOfBg=m_sttgCur.m_nFlagOfBg;
	//dlg.m_clrBg=m_sttgCur.m_clrBg;
	//dlg.m_strPicturePath=m_sttgCur.m_strPicturePath;
	//dlg.m_nIDOfBmp=m_sttgCur.m_nIDOfBmp;
	//////
	//dlg.m_clrBorder=m_sttgCur.m_clrBorder;
	//dlg.m_clrDiaDft=m_sttgCur.m_clrDiaDft;
	//dlg.m_clrDiaCur=m_sttgCur.m_clrDiaCur;
	dlg.m_sttg=m_sttgCur;
	m_sttgPre=m_sttgCur;//备份
	
	if(dlg.DoModal()==IDCANCEL)
	{
		m_sttgCur=m_sttgPre;
		m_edit.SetFont(m_sttgCur);
		Invalidate();//如果是背景的话，要重绘取消
	}
	//PreviewSetting();
	return 0L;
}
LRESULT CTextViewerDlg::OnPreviewSetting(WPARAM wParam,LPARAM lParam)
{
	if(wParam==PREVIWER_FACENAME)//字体
	{
		CString* pstr=(CString*)lParam;
		if(m_sttgCur.m_strFaceName==*pstr)
			return 0L;
		CHARFORMAT cf;
		cf.dwMask=CFM_FACE;
		lstrcpy(cf.szFaceName ,pstr->GetBuffer());
		cf.dwEffects=0;
		m_edit.SetDefaultCharFormat(cf);
		m_sttgCur.m_strFaceName=*pstr;
		return 0L;
	}
	if(wParam==PREVIWER_HEIGHT)//字高，即设置大小
	{
		long lHeight=(long)lParam*20;
		if(m_sttgCur.m_lWordHeight==lHeight)
			return 0L;
		CHARFORMAT cf;
		cf.dwMask=CFM_SIZE;
		cf.dwEffects=0;
		cf.yHeight=lHeight;
		m_edit.SetDefaultCharFormat(cf);
		m_sttgCur.m_lWordHeight=lHeight;
		return 0L;
	}
	if(wParam==PREVIWER_COLOR)//文字颜色
	{
		COLORREF clr=(COLORREF)lParam;
		if(m_sttgCur.m_clrText==clr)
			return 0L;
		CHARFORMAT cf;
		cf.dwMask=CFM_COLOR;
		cf.dwEffects=0;
		cf.crTextColor=clr;
		m_edit.SetDefaultCharFormat(cf);
		m_sttgCur.m_clrText=clr;
		return 0L;
	}
	if(wParam==PREVIWER_OFFSET)//行距
	{
		int nOffset=(int)lParam*20;
		if(m_sttgCur.m_lLineOffset==nOffset)
			return 0L;
		CHARFORMAT cf;
		cf.dwMask=CFM_OFFSET;
		cf.dwEffects=0;
		cf.yOffset=nOffset;
		m_edit.SetDefaultCharFormat(cf);
		m_sttgCur.m_lLineOffset=nOffset;
		return 0L;
	}
	if(wParam==PREVIWER_BGCOLOR)//背景_纯色
	{//如果是直接切换至纯色背景的话，值和现存的相等
		COLORREF clr=(COLORREF)lParam;
		m_sttgCur.m_clrBg=clr;
		m_sttgCur.m_nFlagOfBg=1;
		Invalidate();
		return 0L;
	}
	if(wParam==PREVIWER_BGPIC)//背景_自选图片
	{//如果是直接切换至纯色背景的话，值和现存的相等
		CString* pstr=(CString*)lParam;
		m_sttgCur.m_strPicturePath=*pstr;
		m_sttgCur.m_nFlagOfBg=2;
		Invalidate();
		return 0L;
	}
	if(wParam==PREVIWER_BGPIC_PRESET)//背景_预置图片
	{
		m_sttgCur.m_nIDOfBmp=(UINT)lParam;
		m_sttgCur.m_nFlagOfBg=3;
		Invalidate();
		return 0L;
	}
	if(wParam==PREVIWER_BORDER)//状态栏边框颜色
	{
		m_sttgCur.m_clrBorder=(COLORREF)lParam;
		Invalidate();
		return 0L;
	}
	if(wParam==PREVIWER_DIA_DFT)//小方块默认颜色
	{
		m_sttgCur.m_clrDiaDft=(COLORREF)lParam;
		Invalidate();
		return 0L;
	}
	if(wParam==PREVIWER_DIA_CUR)//小方块当前颜色
	{
		m_sttgCur.m_clrDiaCur=(COLORREF)lParam;
		Invalidate();
		return 0L;
	}
	if(wParam==PREVIWER_VIEWER_THEME)//预设背景
	{
		m_sttgCur=g_PresetViewerSettings[lParam];
		m_edit.SetFont(m_sttgCur);
		Invalidate();
		return 0L;
	}
	return 0L;
}


LRESULT CTextViewerDlg::OnUpdateProgress(WPARAM wParam, LPARAM lParam)
{
	//CPaintDC dc(this);
	CDC* pDC=GetDC();
	//进度显示
	CRect rectClient;
	GetClientRect(rectClient);
	CPoint ptPG(rectClient.Width()-BTN_WIDTH*2-PG_WIDTH,0);
	
	CFont font;
	font.CreatePointFont(90,_T("微软雅黑"));
	CFont* pOldFont=pDC->SelectObject(&font);
	pDC->SelectStockObject(NULL_BRUSH);
	CPen pen(PS_SOLID,1,m_sttgCur.m_clrBorder);
	CPen* pOldPen=pDC->SelectObject(&pen);
	CBrush brushRate(m_sttgCur.m_clrDiaDft);//使用clrDft作为背景
	CBrush* pOldBrush=pDC->SelectObject(&brushRate);
	pDC->SetTextColor(m_sttgCur.m_clrDiaCur);
	pDC->SetBkMode(TRANSPARENT);
	
	pDC->Rectangle(CRect(ptPG,CSize(PG_WIDTH/2,DIA_HEIGHT+DIA_MARGIN*2)));
	
	float f=GetRateOfProgress();
	CString str;
	str.Format(_T("%2.1f%%"),f*100);
	if(f>=1)
		str.Format(_T("%2.0f%%"),f*100);
	pDC->TextOutW(ptPG.x+3,ptPG.y+3,str);
	
	pDC->SetBkMode(TRANSPARENT);
	ptPG.x+=PG_WIDTH/2;
	pDC->Rectangle(CRect(ptPG,CSize(PG_WIDTH/2,DIA_HEIGHT+DIA_MARGIN*2)));
	str.Format(_T("%d/%d"),m_pBlock->m_iFileBlock+1,m_nFileBlocks);
	//MemDC.DrawText(str,CRect(ptPG,CSize(PG_WIDTH/2,DIA_HEIGHT+DIA_MARGIN*2)),DT_CENTER|DT_VCENTER);
	pDC->TextOutW(ptPG.x+3,ptPG.y+3,str);
	
	//Invalidate();
	
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	ReleaseDC(pDC);
	return 0L;
}
LRESULT CTextViewerDlg::OnAddBookmark(WPARAM wParam, LPARAM lParam)
{
	long s,e;
	m_edit.GetSel(s,e);
	
	CBookmarkDlg dlg(this,&m_pBlock->m_bookmark,TRUE);
	dlg.m_bookmarkAdd.m_iFileBlock=m_pBlock->m_iFileBlock;
	dlg.m_bookmarkAdd.m_nReadPos=s;
	MakePreviewText(dlg.m_bookmarkAdd.m_strPreview,s);
	dlg.DoModal();
	return 0L;
}
LRESULT CTextViewerDlg::OnManageBookmark(WPARAM wParam, LPARAM lParam)
{
	CBookmarkDlg dlg(this,&m_pBlock->m_bookmark,FALSE);
	dlg.DoModal();
	return 0L;
}
LRESULT CTextViewerDlg::OnJumpToBookmark(WPARAM wParam, LPARAM lParam)
{
	int n=m_pBlock->m_iFileBlock;
	int m=m_pBlock->m_nReadPos;
	m_pBlock->m_iFileBlock=m_pBlock->m_bookmark.GetAt(wParam).m_iFileBlock;
	m_pBlock->m_nReadPos=m_pBlock->m_bookmark.GetAt(wParam).m_nReadPos;
	if(n!=wParam)
	{
		if(!ReadTextFromDisk()){
			MessageBox(_T("文件读取失败！"));
			m_pBlock->m_iFileBlock=n;
			m_pBlock->m_nReadPos=m;
			return 0L;
		}
	}
	m_edit.SetWindowTextW(m_strText);
	m_edit.SendMessage(WM_VSCROLL,SB_TOP,0);
	m_edit.LineScroll(m_edit.LineFromChar(m_pBlock->m_nReadPos));
	m_edit.SetSel(m_pBlock->m_nReadPos,m_pBlock->m_nReadPos+10);
	Invalidate();
	return 0L;
}
float CTextViewerDlg::GetRateOfProgress()
{
	CRect rect;
	GetClientRect(rect);
	int i=(rect.Height()-DIA_HEIGHT-DIA_MARGIN*2)/(m_sttgCur.m_lWordHeight/12+m_sttgCur.m_lLineOffset/12);
	i=m_edit.LineFromChar(m_pBlock->m_nReadPos)+i-1;
	if(i>=m_edit.GetLineCount()-1)
		i=m_strText.GetLength()-1;
	else 
	i=m_edit.LineIndex(i);
	int bytes=0;
	wchar_t* buffer=m_strText.GetBuffer();
	if(m_nFileTypeCode==1)
	{
		for(i;i>=0;i--)
		{
			if(buffer[i]<=255&&buffer[i]>=0)
				bytes+=1;
			else bytes+=2;
		}
	}
	else if(m_nFileTypeCode==2)
		bytes=i*2;
	else if(m_nFileTypeCode==3)
		bytes=i*2;
	else if(m_nFileTypeCode==4)
		bytes=i*2;
	float f=(float)(m_pBlock->m_iFileBlock*FILE_BLOCK_SIZE+bytes);
	f=f/m_dwFileSize;
	if(f>1)
		f=1;
	return f;
}
void CTextViewerDlg::OnBnClickedButtonLoadPre()
{
	// TODO: Add your control notification handler code here
	if(m_pBlock->m_iFileBlock==0)
		return;
	m_pBlock->m_iFileBlock--;
	if(!ReadTextFromDisk()){
		MessageBox(_T("文件读取失败！"));
		m_pBlock->m_iFileBlock++;
		return;
	}
	m_pBlock->m_nReadPos=0;
	m_edit.SetWindowTextW(m_strText);
	m_edit.SendMessage(WM_VSCROLL,SB_BOTTOM,0);
	Invalidate();
}

void CTextViewerDlg::OnBnClickedButtonLoadNext()
{
	// TODO: Add your control notification handler code here
	if((m_pBlock->m_iFileBlock+1)*FILE_BLOCK_SIZE>=m_dwFileSize)
		return;
	m_pBlock->m_iFileBlock++;
	if(!ReadTextFromDisk()){
		MessageBox(_T("文件读取失败！"));
		m_pBlock->m_iFileBlock--;
		return;
	}
	m_pBlock->m_nReadPos=0;
	m_edit.SetWindowTextW(m_strText);
	m_edit.SendMessage(WM_VSCROLL,SB_TOP,0);
	Invalidate();
}

void CTextViewerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(point.y>DIA_HEIGHT+DIA_MARGIN)
		return;
	int i=(point.x-DIA_MARGIN)/(m_nDiaWidth+DIA_GAP);
	TRACE(_T("In-->%d,(%d,%d)\n"),i,point.x,point.y);
	if(i>=m_nFileBlocks)
		return;
	int n=m_pBlock->m_iFileBlock;
	m_pBlock->m_iFileBlock=i;
	if(!ReadTextFromDisk()){
		MessageBox(_T("文件读取失败！"));
		m_pBlock->m_iFileBlock=n;
		return;
	}
	m_pBlock->m_nReadPos=0;
	m_edit.SetWindowTextW(m_strText);
	m_edit.SendMessage(WM_VSCROLL,SB_TOP,0);
	
	Invalidate();
	//CDialog::OnLButtonDown(nFlags, point);
}

void CTextViewerDlg::OnDestroy()
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
}

void CTextViewerDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDCtl==IDC_BUTTON_LOAD_NEXT)
		m_btnLoadNext.OnDrawItem(nIDCtl,lpDrawItemStruct);
	if(nIDCtl==IDC_BUTTON_LOAD_PRE)
		m_btnLoadPre.OnDrawItem(nIDCtl,lpDrawItemStruct);
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
