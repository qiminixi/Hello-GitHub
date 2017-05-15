// ReaderDlg.cpp : implementation file
//
#include "stdafx.h"
#include "Reader.h"
#include "ReaderDlg.h"
#include "GlobalFunctions.h"
#include "CustomMessages.h"
#include "ReaderSettingDlg.h"
#include "HelpDlg.h"
#include "FileInfoDlg.h"

CEvent g_eventGoOn;
UINT g_nMessageType=0;//1Ϊ�����ʾ
BOOL g_Focused=FALSE;//�Ƿ��ý���
UINT AFX_CDECL RelayMessage(LPVOID lpVoid)
{
	CWnd* pWnd=(CWnd*)lpVoid;
	while(1)
	{
		TRACE(_T("Thread start\n"));
		//::WaitForSingleObject(g_eventGoOn,INFINITE);
		int i=0;
		CPoint ptPre(-1,-1);
		CPoint ptCur(-1,-1);
		CPoint ptLastTip(-1,-1);//��һ��Tip��λ��
		while(1)
		{
			::GetCursorPos(&ptCur);
			if(ptCur==ptLastTip){
				Sleep(1000);
				//TRACE(_T("It is the same position as last\n"));
				continue;
			}
			if(!g_Focused){
				Sleep(1000);//TRACE(_T("Program lost focus\n"));
				continue;
			}
			if(ptPre==ptCur)
			{
				i++;
				//TRACE(_T("RelayMessage:i=%d\n"),i);
				if(i==TIP_RELAY_TIME)//����3����ȣ���ʾ��ʾ
				{
					i=20;
					pWnd->SendMessage(UM_READER_TIP_BLOCKINFO_SHOW,MAKEWPARAM(ptCur.x,ptCur.y));
					ptLastTip=ptCur;
					for(i;i>=1;i--)
					{
						::GetCursorPos(&ptCur);
						if(ptCur!=ptLastTip)
							break;
						Sleep(1000);
					}
					pWnd->SendMessage(UM_READER_TIP_BLOCKINFO_CLOSE);//�ر�TIP
				}
			}
			else//һ������ȣ�������
				i=0;
			ptPre=ptCur;
			Sleep(1000);
		}
	}
	return 0;
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CReaderDlg dialog
CReaderDlg::CReaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReaderDlg::IDD, pParent),m_strTextA('\0',1024),m_strTextW('\0',1024),m_rectViewerWindow(100,300,700,500),
	m_blockManager(10,CSize(135,170))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nMargin=20;
	m_nCol=m_nRow=0;
	m_iPage=0;
	m_nPreBlock=-1;
	m_nIDCursor=-1;
	
	
	//Reader��Ĭ������
	m_sttgReaderDefault.m_clrInterchange=RGB(186,180,184);
	m_sttgReaderDefault.m_clrMoveLocation=RGB(80,80,80);
	m_sttgReaderDefault.m_nFlagOfBg=1;
	m_sttgReaderDefault.m_clrBg=RGB(128,128,128);
	m_sttgReaderDefault.m_strPicturePath=_T("NULL");
	m_sttgReaderDefault.m_nIDOfBmp=IDB_BITMAP_BG1;
	m_sttgReaderCur=m_sttgReaderDefault;
	//-----------------------
	//�϶���
	m_bLButtonDown=0;
	m_iDesBlock=-1;
	m_nMoved=0;
	m_clrInterchange=m_sttgReaderDefault.m_clrInterchange;
	m_clrMoveLocation=m_sttgReaderDefault.m_clrMoveLocation;
	m_bShiftKeyDown=FALSE;
	//---------------------
	//ͣ����ʾ��ʾ
	m_bShowTip=FALSE;
	m_iTipBlock=-1;
	//-----------------------
	//��������
	m_nFlagOfBg=m_sttgReaderCur.m_nFlagOfBg;
	m_clrBg=m_sttgReaderCur.m_clrBg;
	m_strPicturePath=m_sttgReaderCur.m_strPicturePath;
	m_bmpBg.m_hObject=NULL;
	//---------------------
	//������,��BlockManager�Դ���������ΪĬ������
	m_sttgBlockDefault.m_clrBlockBg=m_blockManager.m_clrBlockBg;
	m_sttgBlockDefault.m_clrBlockTitleBg=m_blockManager.m_clrBlockTitleBg;
	m_sttgBlockDefault.m_clrBlockTitle=m_blockManager.m_clrBlockTitle;
	m_sttgBlockDefault.m_clrBlockText=m_blockManager.m_clrBlockText;
	m_sttgBlockDefault.m_clrMouseOver=m_blockManager.m_clrMouseOver;
	m_sttgBlockDefault.m_clrMouseClick=m_blockManager.m_clrMouseClick;
	m_sttgBlockCur=m_sttgBlockDefault;
	//---------------------
	m_pBlockShowFileInfo=NULL;
	m_iBlockMenu=-1;
}

void CReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CReaderDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CReaderDlg::OnBnClickedOk)
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_FILEOPEN, &CReaderDlg::OnFileOpen)
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_NCMOUSEMOVE()
	ON_MESSAGE(UM_VIEWERCLOSE,OnViewerClose)
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_READER_SETTING, &CReaderDlg::OnReaderSetting)
	ON_MESSAGE(UM_READER_PREVIEW_SETTING,OnPreviewSetting)
	ON_MESSAGE(UM_READER_TIP_BLOCKINFO_SHOW,OnShowBlockInfo)
	ON_MESSAGE(UM_READER_TIP_BLOCKINFO_CLOSE,OnCloseBlockInfo)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_READER_DELETE_BLOCK, &CReaderDlg::OnReaderDeleteBlock)
	ON_COMMAND(ID_READER_ABOUT, &CReaderDlg::OnReaderAbout)
	ON_COMMAND(ID_READER_HELP, &CReaderDlg::OnReaderHelp)
	ON_COMMAND(ID_READER_SHOW_FILEINFO, &CReaderDlg::OnReaderShowFileinfo)
	ON_WM_VSCROLL()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CReaderDlg message handlers

BOOL CReaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	// TODO: Add extra initialization here
	if(LoadProgramConfigFromFile())
	{
		MoveWindow(m_rectWindow);
		m_blockManager.m_clrBlockBg=m_sttgBlockCur.m_clrBlockBg;
		m_blockManager.m_clrBlockTitleBg=m_sttgBlockCur.m_clrBlockTitleBg;
		m_blockManager.m_clrBlockTitle=m_sttgBlockCur.m_clrBlockTitle;
		m_blockManager.m_clrBlockText=m_sttgBlockCur.m_clrBlockText;
		m_blockManager.m_clrMouseOver=m_sttgBlockCur.m_clrMouseOver;
		m_blockManager.m_clrMouseClick=m_sttgBlockCur.m_clrMouseClick;
	}
	
	AfxBeginThread(RelayMessage,this);
	
	m_blockManager.LoadFilesInfo();
	if(m_MarkSelBlocks.GetSize()<m_blockManager.m_nListLength)
		IncrecseTheTwoArrays();
	
	CTextViewerDlg* pViewer=new CTextViewerDlg(NULL,NULL,-1,CRect(0,0,0,0));//����Viewer
	pViewer->Create();
	pViewer->ShowWindow(SW_HIDE);
	m_ViewerManager.AddHead(pViewer);
	pViewer=new CTextViewerDlg(NULL,NULL,-1,CRect(0,0,0,0));//����Viewer
	pViewer->Create();
	pViewer->ShowWindow(SW_HIDE);
	m_ViewerManager.AddHead(pViewer);
	
	//��������ʼ��
	SCROLLINFO vinfo;  
	vinfo.cbSize=sizeof(vinfo);  
	vinfo.fMask=SIF_ALL;  
	vinfo.nPage=50;//����������ĳ��̣�ͨ�������¹�ϵ���䳤��/���������ȣ���������ͷ��=nPage/(nMax+2),����nPageȡֵ-1ʱ���������᲻���ˡ�  
	vinfo.nMax=400;//���������ܹ��������ֵ  
	vinfo.nMin=0;//���������ܹ�������Сֵ  
	vinfo.nTrackPos=0;  
	SetScrollInfo(SB_VERT,&vinfo);//��ʹ��������һ������ʹ�ô������Ҳ������ʾ������ 
	//-----------------------------------
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CReaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CReaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		/*CPaintDC dc(this);
		dc.DrawTextW(CString(_T("dsfffffffffffffffffffeaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"))
			,CRect(0,0,70,100),DT_MODIFYSTRING | DT_END_ELLIPSIS |DT_EDITCONTROL| DT_WORDBREAK); 
		return;*/
		//CDialog::OnPaint();
	}
	
	CPaintDC dc(this);
	CRect rectClient;
	GetClientRect(rectClient);
	
	//-------------------------------------------------
	//�����ڴ�ͼ��
	CDC MemDC; //���ȶ���һ����ʾ�豸����
	CBitmap MemBitmap;//����һ��λͼ����
	MemDC.CreateCompatibleDC(NULL);//�����ڴ��豸
	MemBitmap.CreateCompatibleBitmap(&dc,rectClient.Width(),rectClient.Height());//����λͼ
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
	//�ڴ�DC�л�������ʾͼ��
	//----------------------
	
	CBrush brushBg;
	if(m_sttgReaderCur.m_nFlagOfBg==1)//��ɫ����
		brushBg.CreateSolidBrush(m_sttgReaderCur.m_clrBg);
	else if(m_sttgReaderCur.m_nFlagOfBg==2)//λͼ����
	{
		if(m_bmpBg.m_hObject!=NULL)//���ڵ�һ�����롣
			m_bmpBg.DeleteObject();
		HBITMAP hBitmap=(HBITMAP)::LoadImage(NULL,m_sttgReaderCur.m_strPicturePath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		m_bmpBg.Attach(hBitmap);
		brushBg.CreatePatternBrush(&m_bmpBg);
	}
	else
	{
		if(m_bmpBg.m_hObject!=NULL)//���ڵ�һ�����롣
			m_bmpBg.DeleteObject();
		m_bmpBg.LoadBitmapW(m_sttgReaderCur.m_nIDOfBmp);
		brushBg.CreatePatternBrush(&m_bmpBg);
	}
	MemDC.FillRect(rectClient,&brushBg);
	
	//--------------------------------------------------
	
	//TRACE(_T("R=%d,C=%d\n"),m_nRow,m_nCol);
	for(int i=0;i<m_nRow;i++)
		for(int j=0;j<m_nCol;j++)
		{
			CPoint ptTopLeft;
			ptTopLeft.x=m_nMargin+(m_blockManager.m_szBlock.cx+m_nGapH)*j;//ֻ��һ��ʱ�����GapH
			ptTopLeft.y=m_nMargin+(m_blockManager.m_szBlock.cy+m_nGapV)*i;//ֻ��һ��ʱ�����GapV
			m_blockManager.GetBlcok(-1);
			int iBlock=(i*m_nCol+j)+m_nRow*m_nCol*m_iPage;
			CBlock* pBlock=m_blockManager.GetBlcok(iBlock);
			if(pBlock!=NULL)
			{//�����ѡ��״̬ά�����������Ԥѡ��״̬ά����
				if(!pBlock->m_bThumbInForce)
					m_blockManager.CreateCompatibleBitmap(pBlock,&dc);
				if(m_nPreBlock==iBlock)
					m_blockManager.OnMouseOver(pBlock,&MemDC,ptTopLeft);
				else if(m_MarkSelBlocks[iBlock]==1)
					m_blockManager.OnMouseClick(pBlock,&MemDC,ptTopLeft);
				else
					m_blockManager.DrawItem(pBlock,&MemDC,ptTopLeft);
				pBlock=NULL;
			}
		}
	//����϶�λ�ý�����ʾ������
	if(m_nMoved>=2&&!m_bShiftKeyDown&&m_iDesBlock!=m_iMovedBlock&&m_iDesBlock>=0&&m_iDesBlock<m_blockManager.m_nListLength)
	{
		int r=(m_iDesBlock-m_nRow*m_nCol*m_iPage)/m_nCol;//����к�
		int c=(m_iDesBlock-m_nRow*m_nCol*m_iPage)-r*m_nCol;//����к�
		CPoint ptDesBlock;
		int n=m_blockManager.m_nMargin;
		ptDesBlock.x=m_nMargin+(m_blockManager.m_szBlock.cx+m_nGapH)*c+n/2;
		ptDesBlock.y=m_nMargin+(m_blockManager.m_szBlock.cy+m_nGapV)*r+n/2;
		//CBrush brush(HS_DIAGCROSS,RGB(128,0,128));
		//MemDC.SelectStockObject(NULL_BRUSH);
		//MemDC.SetBkMode(TRANSPARENT);
		//MemDC.FillRect(CRect(ptDesBlock,m_blockManager.m_szBlock-CSize(n,n)),&brush);
		FillRectWithHachuresFake(&MemDC,CRect(ptDesBlock,m_blockManager.m_szBlock-CSize(n,n)),0,m_sttgReaderCur.m_clrInterchange);
	}
	//---------------------------------------
	
	//����϶�λ���ƶ���ʾ��
	if(m_nMoved>=2&&m_bShiftKeyDown&&m_iDesBlock!=m_iMovedBlock&&m_iDesBlock>=0&&m_iDesBlock<m_blockManager.m_nListLength)
	{
		int r=(m_iDesBlock-m_nRow*m_nCol*m_iPage)/m_nCol;//����к�
		int c=(m_iDesBlock-m_nRow*m_nCol*m_iPage)-r*m_nCol;//����к�
		CPoint ptDesBlock;
		int n=m_blockManager.m_nMargin;
		ptDesBlock.x=m_nMargin+(m_blockManager.m_szBlock.cx+m_nGapH)*c+n/2;
		ptDesBlock.y=m_nMargin+(m_blockManager.m_szBlock.cy+m_nGapV)*r+n/2;
		CPen pen(PS_SOLID,4,m_sttgReaderCur.m_clrMoveLocation);
		CPen* pOldPen=MemDC.SelectObject(&pen);
		MemDC.MoveTo(ptDesBlock.x-m_nGapH/2,ptDesBlock.y);
		MemDC.LineTo(ptDesBlock.x-m_nGapH/2,ptDesBlock.y+m_blockManager.m_szBlock.cy-m_blockManager.m_nMargin);
		//FillRectWithHachures(&MemDC,CRect(ptDesBlock,m_blockManager.m_szBlock-CSize(n,n)),0,m_clrMoveLocation);
	}
	//-----------------------------------------
	
	//��ʾ������ƶ��Ŀ�
	if(m_nMoved>=2&&m_bLButtonDown!=0)
	{
		int n=m_blockManager.m_nMargin+2;
		m_blockManager.DrawItem(m_blockManager.GetBlcok(m_iMovedBlock),&MemDC,m_ptMousePos-CPoint(n,n));
	}
	//------------------------------------
	
	//���ͣ����ʾ
	if(m_bShowTip)
	{
		MemDC.SelectStockObject(BLACK_PEN);
		MemDC.SelectStockObject(WHITE_BRUSH);
		CRect rect(m_ptMousePosTip,CSize(200,100));
		MemDC.Rectangle(rect);
		CFont font;
		font.CreatePointFont(90,_T("����"));
		CFont* pOldFont=MemDC.SelectObject(&font);
		rect.left+=3;rect.right-=3;rect.top+=3;rect.bottom-=3;
		MemDC.DrawText(m_pBlockToShow->m_strPreview,rect,DT_EDITCONTROL|DT_END_ELLIPSIS|DT_WORDBREAK);
		MemDC.SelectObject(pOldFont);
	}
	//-------------------------------------
	dc.BitBlt(0,0,rectClient.Width(),rectClient.Height(),&MemDC,0,0,SRCCOPY);
	MemBitmap.DeleteObject();
	MemDC.SelectObject(pOldBit);
	MemDC.DeleteDC();
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CReaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CReaderDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

BOOL CReaderDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message==WM_KEYDOWN)
	{
		if (pMsg->wParam==VK_ESCAPE)
			return FALSE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CReaderDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return true;
	return CDialog::OnEraseBkgnd(pDC);
}

void CReaderDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CDialog::OnContextMenu(pWnd,point);
}
void CReaderDlg::OnFileOpen()
{
	//ʹ��CFileDialog��ȡ�ļ�·��
	CString strFilePath('\0',255);
	wchar_t szFilter[]=_T("�����ļ�(*.*)|*.*|�ı��ļ�(*.txt)|*.txt||");
	//::GetCurrentDirectory(strFilePathW.GetLength(),strFilePathW.GetBuffer());
	CFileDialog fileDlg(true,_T("txt"),_T("*.txt"),OFN_ALLOWMULTISELECT,szFilter,NULL,NULL);
	if(fileDlg.DoModal()==IDCANCEL){
		return;
	}
	POSITION pos=fileDlg.GetStartPosition();
	while(pos!=NULL)
	{
		strFilePath=fileDlg.GetNextPathName(pos);
		if(!m_blockManager.IsFileExisted(strFilePath))
		{
			m_blockManager.GetAFile(strFilePath);
			if(m_MarkSelBlocks.GetSize()<m_blockManager.m_nListLength)
				IncrecseTheTwoArrays();
		}
		else 
		{
			CString strMessage;
			strMessage.Format(_T("�ļ� <%s> �Ѵ��ڣ�Ҫ�ظ������"),strFilePath.GetBuffer());
			if(MessageBox(strMessage.GetBuffer(),_T("��ʾ"),MB_YESNO)==IDYES)
			{
				m_blockManager.GetAFile(strFilePath);
				if(m_MarkSelBlocks.GetSize()<m_blockManager.m_nListLength)
					IncrecseTheTwoArrays();
			}
		}
	//-----------------------------
	}
	Invalidate();
}

void CReaderDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	//--------------------------------------------------
	//���������
	m_nGapH=m_nGapV=2;
	m_nCol=(cx-m_nMargin*2)/m_blockManager.m_szBlock.cx;
	m_nRow=(cy-m_nMargin*2)/m_blockManager.m_szBlock.cy;
	if(m_nRow>2) m_nGapV=(cy-m_nRow*m_blockManager.m_szBlock.cy-m_nMargin*2)/(m_nRow-1);//2�������Gap���ѿ�
	if(m_nCol>2) m_nGapH=(cx-m_nCol*m_blockManager.m_szBlock.cx-m_nMargin*2)/(m_nCol-1);//2�������Gap���ѿ�
	if(m_nRow==0) m_nRow=1;//��Ȳ���ʱǿ����ʾ
	if(m_nCol==0) m_nCol=1;//�߶Ȳ���ʱǿ����ʾ
	m_nPage=m_blockManager.m_nListLength/(m_nRow*m_nCol)+1;
	if(m_blockManager.m_nListLength%(m_nRow*m_nCol)==0 && m_blockManager.m_nListLength!=0)
		m_nPage-=1;
	//TRACE(_T("cx=%3d,cy=%3d  R=%d,C=%d P=%d\n"),cx,cy,m_nRow,m_nCol,m_nPage);
	//-------------------
	
	//���������Ը���
	SCROLLINFO vinfo;  
	vinfo.cbSize=sizeof(vinfo);
	vinfo.fMask=SIF_PAGE;//SIF_ALL
	if(m_nPage==1)
		vinfo.nPage=-1;//����������ĳ��̣�ͨ�������¹�ϵ���䳤��/���������ȣ���������ͷ��=nPage/(nMax+2),����nPageȡֵ-1ʱ���������᲻���ˡ�
	else
		vinfo.nPage=400/m_nPage;
	SetScrollInfo(SB_VERT,&vinfo);//��ʹ��������һ������ʹ�ô������Ҳ������ʾ������ 
	//------------------------------
	
	Invalidate();
}
void CReaderDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CSize& szBlock=m_blockManager.m_szBlock;
	int i=(point.y-m_nMargin)/(szBlock.cy+m_nGapV);
	int j=(point.x-m_nMargin)/(szBlock.cx+m_nGapH);
	int iBlock=i*m_nCol+j+m_nRow*m_nCol*m_iPage;
	CPoint ptTopLeft;
	ptTopLeft.x=m_nMargin+(szBlock.cx+m_nGapH)*j;
	ptTopLeft.y=m_nMargin+(szBlock.cy+m_nGapV)*i;
	
	//����Ϸ�
	if(m_bLButtonDown&&m_nMoved>=1)
	{
		m_ptMousePos=point;
		Invalidate();
		m_nIDCursor=0;
		m_iDesBlock=iBlock;
		m_nMoved=2;
		//TRACE(_T("Move: %d,%d,%d\n"),m_bMoved,m_bLButtonDown,m_iMovedBlock);
		return;
	}
	if(m_bLButtonDown)
		m_nMoved++;
	//------------------
	
	if((point.x-ptTopLeft.x>=0&&point.x-ptTopLeft.x<=szBlock.cx)
		&&(point.y-ptTopLeft.y>=0&&point.y-ptTopLeft.y<=szBlock.cy)
		&&i<=m_nRow-1&&j<=m_nCol-1/*ȥ����Ե����Ч��*/)
	{
		CBlock* pBlock;
		pBlock=m_blockManager.GetBlcok(iBlock);
		if(pBlock!=NULL&&(m_nPreBlock!=iBlock))//���������ԭ���Ŀ��оͲ������»���
		{
			if(m_MarkSelBlocks[iBlock]==0)//����Ѿ���ѡ���Ӧ�ñ���ѡ��״̬,�����ô�ѡ״̬�滻
			{
				CDC* pDC=GetDC();
				CDC memDC;
				CBitmap memBitmap;
				memDC.CreateCompatibleDC(NULL);
				memBitmap.CreateCompatibleBitmap(pDC,szBlock.cx,szBlock.cy);
				CBitmap* pOldBitmap=memDC.SelectObject(&memBitmap);
				m_blockManager.OnMouseOver(pBlock,&memDC,CPoint(0,0));
				pDC->BitBlt(ptTopLeft.x,ptTopLeft.y,szBlock.cx,szBlock.cy,&memDC,0,0,SRCCOPY);
				memDC.SelectObject(pOldBitmap);

				int r=(m_nPreBlock-m_nRow*m_nCol*m_iPage)/m_nCol;//����к�
				int c=(m_nPreBlock-m_nRow*m_nCol*m_iPage)-r*m_nCol;//����к�
				CPoint ptPreBlock;
				ptPreBlock.x=m_nMargin+(szBlock.cx+m_nGapH)*c;ptPreBlock.y=m_nMargin+(szBlock.cy+m_nGapV)*r;
				InvalidateRect(CRect(ptPreBlock,szBlock));//ȡ���ϴεĿ�ѡ״̬[ֱ���л�����һ�����ÿ���]
				m_nPreBlock=iBlock;//����
			}
			else
				;//�����Ѿ�ѡ���˵�״̬��Ӧ�û������
			m_nIDCursor=1;//����ڿ��еĻ����Ĺ��
			//TRACE("Got at available block\n");
		}
		if(m_nPreBlock!=-1&&pBlock==NULL)
		{
			int r=(m_nPreBlock-m_nRow*m_nCol*m_iPage)/m_nCol;//����к�
			int c=(m_nPreBlock-m_nRow*m_nCol*m_iPage)-r*m_nCol;//����к�
			CPoint ptPreBlock;
			ptPreBlock.x=m_nMargin+(szBlock.cx+m_nGapH)*c;ptPreBlock.y=m_nMargin+(szBlock.cy+m_nGapV)*r;
			InvalidateRect(CRect(ptPreBlock,szBlock));//ȡ���ϴεĿ�ѡ״̬[ֱ���л�����һ�������ÿ���]
			m_nPreBlock=-1;
			m_nIDCursor=0;//��겻�ڵĿ��ڵĻ��Ļ�ȥ����Ч����
			//TRACE("Got at unuseable block\n");
		}
	}
	else
	{	
		if(m_nPreBlock!=-1)//ȡ���ϴεĿ�ѡ״̬[ֱ���л�����϶����]
		{
			int r=(m_nPreBlock-m_nRow*m_nCol*m_iPage)/m_nCol;//����к�
			int c=(m_nPreBlock-m_nRow*m_nCol*m_iPage)-r*m_nCol;//����к�
			CPoint ptPreBlock;
			ptPreBlock.x=m_nMargin+(szBlock.cx+m_nGapH)*c;ptPreBlock.y=m_nMargin+(szBlock.cy+m_nGapV)*r;
			InvalidateRect(CRect(ptPreBlock,szBlock));
			m_nPreBlock=-1;
		}
		m_nIDCursor=0;//��겻�ڵĿ��ڵĻ��Ļ�ȥ��Gap��
		//TRACE("Got at gap\n");
	}
	//TRACE("Mouse moved(%d,%d)\n",point.x,point.y);
	CDialog::OnMouseMove(nFlags, point);
}

void CReaderDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CReaderDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CSize& szBlock=m_blockManager.m_szBlock;
	int i=(point.y-m_nMargin)/(szBlock.cy+m_nGapV);
	int j=(point.x-m_nMargin)/(szBlock.cx+m_nGapH);
	int iBlock=i*m_nCol+j+m_nRow*m_nCol*m_iPage;
	CPoint ptTopLeft;
	ptTopLeft.x=m_nMargin+(szBlock.cx+m_nGapH)*j;
	ptTopLeft.y=m_nMargin+(szBlock.cy+m_nGapV)*i;
	if((point.x-ptTopLeft.x>=0&&point.x-ptTopLeft.x<=szBlock.cx)
		&&(point.y-ptTopLeft.y>=0&&point.y-ptTopLeft.y<=szBlock.cy)
		&&i<=m_nRow-1&&j<=m_nCol-1/*ȥ����Ե����Ч��*/)
	{
		CBlock* pBlock;
		pBlock=m_blockManager.GetBlcok(iBlock);
		if(pBlock!=NULL)
		{
			m_bLButtonDown=1;
			m_ptMousePos=point;
			m_iMovedBlock=iBlock;
			//TRACE(_T("Down: %d,%d,%d\n"),m_bMoved,m_bLButtonDown,m_iMovedBlock);
			//TRACE(_T("The %dth block is prepared to move\n"),iBlock);
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}
void CReaderDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//TRACE(_T("Up: %d,%d,%d\n"),m_bMoved,m_bLButtonDown,m_iMovedBlock);
	if(m_bLButtonDown&&m_nMoved>=2){
		m_bLButtonDown=0;
		if(m_bShiftKeyDown)
			m_blockManager.MoveBlock(m_iMovedBlock,m_iDesBlock);
		else
			m_blockManager.InterchangeBlocks(m_iMovedBlock,m_iDesBlock);
		m_iDesBlock=-1;
		m_nMoved=0;
		Invalidate();
		return;
	}
	m_bLButtonDown=0;
	m_nMoved=0;
	m_iDesBlock=-1;
	//Invalidate();
	CSize& szBlock=m_blockManager.m_szBlock;
	int i=(point.y-m_nMargin)/(szBlock.cy+m_nGapV);
	int j=(point.x-m_nMargin)/(szBlock.cx+m_nGapH);
	CPoint ptTopLeft;
	ptTopLeft.x=m_nMargin+(szBlock.cx+m_nGapH)*j;
	ptTopLeft.y=m_nMargin+(szBlock.cy+m_nGapV)*i;
	if((point.x-ptTopLeft.x>=0&&point.x-ptTopLeft.x<=szBlock.cx)
		&&(point.y-ptTopLeft.y>=0&&point.y-ptTopLeft.y<=szBlock.cy)&&i<=m_nRow-1&&j<=m_nCol-1/*ȥ����Ե����Ч��*/)
	{
		int iBlock=i*m_nCol+j+m_nRow*m_nCol*m_iPage;
		CBlock* pBlock;
		pBlock=m_blockManager.GetBlcok(iBlock);
		if(pBlock!=NULL)
		{
			int& nIsSelected=m_MarkSelBlocks.GetAt(iBlock);
			if(nIsSelected==0)//û�б�ѡ�����ѡ��
			{
				CDC* pDC=GetDC();
				CDC memDC;
				CBitmap memBitmap;
				memDC.CreateCompatibleDC(NULL);
				memBitmap.CreateCompatibleBitmap(pDC,szBlock.cx,szBlock.cy);
				CBitmap* pOldBitmap=memDC.SelectObject(&memBitmap);
				m_blockManager.OnMouseClick(pBlock,&memDC,CPoint(0,0));
				pDC->BitBlt(ptTopLeft.x,ptTopLeft.y,szBlock.cx,szBlock.cy,&memDC,0,0,SRCCOPY);
				memDC.SelectObject(pOldBitmap);
				
				CTextViewerDlg* pViewer=NewViewer();//����Viewer
				pViewer->InitViewer(this,pBlock,iBlock);
				pViewer->ShowWindow(SW_SHOW);
				m_ViewerPtrOfBlocks[iBlock]=pViewer;
				
				nIsSelected=1;
				m_nPreBlock=-1;//���ｫԤѡ״̬ȡ���ˡ�����ʹ�õ�����Ӱˢ�ӣ�û��Ԥѡ״̬��֧�֣�������ߺ��������Ƴ�Ԥѡ״̬��
				//���ػ����С����Ҫ�����ʹ����Ӱˢ�ӵĻ����ö��⻭��Ԥѡ״̬�ı���!!!!!!!!
				TRACE(_T("The %dth block have been selected\n"),iBlock);
			}
			else//ѡ�����
			{//�����
				CTextViewerDlg* pViewer=m_ViewerPtrOfBlocks[iBlock];
				if(pViewer!=NULL){
					if(pViewer->IsIconic()) m_ViewerPtrOfBlocks[iBlock]->ShowWindow(SW_RESTORE);
					else m_ViewerPtrOfBlocks[iBlock]->ShowWindow(SW_SHOWNA);
				}
				
			}
		}
		//else TRACE(_T("Arrived at a unuseable block\n"));
	}
	CDialog::OnLButtonUp(nFlags, point);
}
void CReaderDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	
	CSize& szBlock=m_blockManager.m_szBlock;
	int i=(point.y-m_nMargin)/(szBlock.cy+m_nGapV);
	int j=(point.x-m_nMargin)/(szBlock.cx+m_nGapH);
	int iBlock=i*m_nCol+j+m_nRow*m_nCol*m_iPage;
	//TRACE(_T("%d\n"),iBlock);
	CPoint ptTopLeft;
	ptTopLeft.x=m_nMargin+(szBlock.cx+m_nGapH)*j;
	ptTopLeft.y=m_nMargin+(szBlock.cy+m_nGapV)*i;
	if((point.x-ptTopLeft.x>=0&&point.x-ptTopLeft.x<=szBlock.cx)
		&&(point.y-ptTopLeft.y>=0&&point.y-ptTopLeft.y<=szBlock.cy)
		&&i<=m_nRow-1&&j<=m_nCol-1/*ȥ����Ե����Ч��*/)
	{
		CBlock* pBlock;
		pBlock=m_blockManager.GetBlcok(iBlock);
		if(pBlock!=NULL)
		{
			m_nIDCursor=0;
			CMenu menu;
			menu.CreatePopupMenu();
			menu.AppendMenu(MF_STRING,ID_READER_SHOW_FILEINFO,_T("�ļ���Ϣ"));
			menu.AppendMenu(MF_STRING,ID_READER_DELETE_BLOCK,_T("ɾ���ļ�"));
			menu.TrackPopupMenu(TPM_RIGHTBUTTON,ptCursor.x,ptCursor.y,this);
			menu.DestroyMenu();
			m_pBlockShowFileInfo=pBlock;
			m_iBlockMenu=iBlock;
			return;
		}
	}
	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenuW(MF_STRING,ID_FILEOPEN,_T("����ļ�"));
	menu.AppendMenu(MF_STRING,ID_READER_SETTING,_T("����������"));
	menu.AppendMenu(MF_STRING,ID_READER_HELP,_T("����"));
	menu.AppendMenu(MF_STRING,ID_READER_ABOUT,_T("����"));
	menu.TrackPopupMenu(TPM_RIGHTBUTTON,ptCursor.x,ptCursor.y,this);
	menu.DestroyMenu();
	CDialog::OnRButtonUp(nFlags, point);
}

void CReaderDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	m_rectWindow=wp.rcNormalPosition;
	StoreProgramConfigToFile();
	m_blockManager.StoreFilesInfo();
	CDialog::OnClose();
}

bool CReaderDlg::LoadProgramConfigFromFile()
{
	//CString strFilePath=GblGetDirectoryOfExecutableFile()+_T("ProgramConfig.txt");
	CString strFilePath=GblGetDirectoryOfExecutableFile()+_T("��������.data");
	HANDLE hFileRead=CreateFile(strFilePath,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,NULL,NULL);
	if(hFileRead==INVALID_HANDLE_VALUE )
	{
		TRACE(_T("δ�ܵ������������Ϣ"));
		return false;
	}
	//long lFileSize=GetFileSize(hFileRead,NULL);
	//wchar_t* buffer=NULL;
	//DWORD dwNumOfBytesToRead;
	DWORD dwNumOfBytesRead;
	int nLength=0;
	//����������
	ReadFile(hFileRead,(wchar_t*)&m_rectWindow,16,&dwNumOfBytesRead,NULL);//����������
	
	ReadFile(hFileRead,(wchar_t*)&m_sttgBlockCur.m_clrBlockBg,4,&dwNumOfBytesRead,NULL);//Ԥ��������ɫ
	ReadFile(hFileRead,(wchar_t*)&m_sttgBlockCur.m_clrBlockTitleBg,4,&dwNumOfBytesRead,NULL);//���ⱳ����ɫ
	ReadFile(hFileRead,(wchar_t*)&m_sttgBlockCur.m_clrBlockTitle,4,&dwNumOfBytesRead,NULL);//����������ɫ
	ReadFile(hFileRead,(wchar_t*)&m_sttgBlockCur.m_clrBlockText,4,&dwNumOfBytesRead,NULL);//Ԥ��������ɫ
	ReadFile(hFileRead,(wchar_t*)&m_sttgBlockCur.m_clrMouseOver,4,&dwNumOfBytesRead,NULL);//��꾭����ɫ
	ReadFile(hFileRead,(wchar_t*)&m_sttgBlockCur.m_clrMouseClick,4,&dwNumOfBytesRead,NULL);//�������ɫ
	
	ReadFile(hFileRead,(wchar_t*)&m_sttgReaderCur.m_clrMoveLocation,4,&dwNumOfBytesRead,NULL);//�ƶ���ʾ��ɫ
	ReadFile(hFileRead,(wchar_t*)&m_sttgReaderCur.m_clrInterchange,4,&dwNumOfBytesRead,NULL);//������ʾ��ɫ
	ReadFile(hFileRead,(wchar_t*)&m_sttgReaderCur.m_nFlagOfBg,4,&dwNumOfBytesRead,NULL);//�����ڱ������ͱ�ʶ
	ReadFile(hFileRead,(wchar_t*)&m_sttgReaderCur.m_clrBg,4,&dwNumOfBytesRead,NULL);//�����ڱ�����ɫ
	ReadFile(hFileRead,(wchar_t*)&nLength,4,&dwNumOfBytesRead,NULL);
	m_sttgReaderCur.m_strPicturePath.GetBufferSetLength(nLength);
	ReadFile(hFileRead,m_sttgReaderCur.m_strPicturePath.GetBuffer(),nLength*2,&dwNumOfBytesRead,NULL);//����ͼƬ·��
	ReadFile(hFileRead,(wchar_t*)&m_sttgReaderCur.m_nIDOfBmp,4,&dwNumOfBytesRead,NULL);//Ԥ��ͼƬID
	//-------------------------------------------------

	//�Ķ���������
	ReadFile(hFileRead,(wchar_t*)&CTextViewerDlg::m_rectViewerWindow,16,&dwNumOfBytesRead,NULL);//�Ķ���������
	
	CViewerSetting& sttg=CTextViewerDlg::m_sttgCur;
	ReadFile(hFileRead,(wchar_t*)&nLength,4,&dwNumOfBytesRead,NULL);
	sttg.m_strFaceName.GetBufferSetLength(nLength);
	ReadFile(hFileRead,sttg.m_strFaceName.GetBuffer(),nLength*2,&dwNumOfBytesRead,NULL);//����
	ReadFile(hFileRead,(wchar_t*)&sttg.m_lWordHeight,4,&dwNumOfBytesRead,NULL);//��С
	ReadFile(hFileRead,(wchar_t*)&sttg.m_lLineOffset,4,&dwNumOfBytesRead,NULL);//�о�
	ReadFile(hFileRead,(wchar_t*)&sttg.m_clrText,4,&dwNumOfBytesRead,NULL);//��ɫ
	ReadFile(hFileRead,(wchar_t*)&sttg.m_nFlagOfBg,4,&dwNumOfBytesRead,NULL);//��������
	ReadFile(hFileRead,(wchar_t*)&sttg.m_clrBg,4,&dwNumOfBytesRead,NULL);//������ɫ
	ReadFile(hFileRead,(wchar_t*)&nLength,4,&dwNumOfBytesRead,NULL);
	sttg.m_strPicturePath.GetBufferSetLength(nLength);
	ReadFile(hFileRead,sttg.m_strPicturePath.GetBuffer(),nLength*2,&dwNumOfBytesRead,NULL);//����ͼƬ·��
	ReadFile(hFileRead,(wchar_t*)&sttg.m_nIDOfBmp,4,&dwNumOfBytesRead,NULL);//Ԥ��ͼƬID
	ReadFile(hFileRead,(wchar_t*)&sttg.m_clrBorder,4,&dwNumOfBytesRead,NULL);//״̬���߿���ɫ
	ReadFile(hFileRead,(wchar_t*)&sttg.m_clrDiaCur,4,&dwNumOfBytesRead,NULL);//С����Ĭ����ɫ
	ReadFile(hFileRead,(wchar_t*)&sttg.m_clrDiaDft,4,&dwNumOfBytesRead,NULL);//С���鵱ǰ��ɫ
	//-------------------------------------------------
	
	CloseHandle(hFileRead);
	return true;
	
}

void CReaderDlg::StoreProgramConfigToFile()
{
	CString strFilePath=GblGetDirectoryOfExecutableFile()+_T("��������.data");
	CFile fileWrite;
	HANDLE hFileWrite=CreateFile(strFilePath,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,NULL,NULL);
	if(hFileWrite==INVALID_HANDLE_VALUE )
	{
		MessageBox(_T("δ�ܴ洢����������Ϣ"));
		return;
	}
	
	//wchar_t* buffer=NULL;
	//DWORD dwNumOfBytesToWrite;
	DWORD dwNumOfBytesWritten;
	int nLength=0;
	//����������
	WriteFile(hFileWrite,(wchar_t*)&m_rectWindow,16,&dwNumOfBytesWritten,NULL);//����������
	
	WriteFile(hFileWrite,(wchar_t*)&m_sttgBlockCur.m_clrBlockBg,4,&dwNumOfBytesWritten,NULL);//Ԥ��������ɫ
	WriteFile(hFileWrite,(wchar_t*)&m_sttgBlockCur.m_clrBlockTitleBg,4,&dwNumOfBytesWritten,NULL);//���ⱳ����ɫ
	WriteFile(hFileWrite,(wchar_t*)&m_sttgBlockCur.m_clrBlockTitle,4,&dwNumOfBytesWritten,NULL);//����������ɫ
	WriteFile(hFileWrite,(wchar_t*)&m_sttgBlockCur.m_clrBlockText,4,&dwNumOfBytesWritten,NULL);//Ԥ��������ɫ
	WriteFile(hFileWrite,(wchar_t*)&m_sttgBlockCur.m_clrMouseOver,4,&dwNumOfBytesWritten,NULL);//��꾭����ɫ
	WriteFile(hFileWrite,(wchar_t*)&m_sttgBlockCur.m_clrMouseClick,4,&dwNumOfBytesWritten,NULL);//�������ɫ
	
	WriteFile(hFileWrite,(wchar_t*)&m_sttgReaderCur.m_clrMoveLocation,4,&dwNumOfBytesWritten,NULL);//�ƶ���ʾ��ɫ
	WriteFile(hFileWrite,(wchar_t*)&m_sttgReaderCur.m_clrInterchange,4,&dwNumOfBytesWritten,NULL);//������ʾ��ɫ
	WriteFile(hFileWrite,(wchar_t*)&m_sttgReaderCur.m_nFlagOfBg,4,&dwNumOfBytesWritten,NULL);//�����ڱ������ͱ�ʶ
	WriteFile(hFileWrite,(wchar_t*)&m_sttgReaderCur.m_clrBg,4,&dwNumOfBytesWritten,NULL);//�����ڱ�����ɫ
	nLength=m_sttgReaderCur.m_strPicturePath.GetLength();
	WriteFile(hFileWrite,(wchar_t*)&nLength,4,&dwNumOfBytesWritten,NULL);
	WriteFile(hFileWrite,m_sttgReaderCur.m_strPicturePath.GetBuffer(),nLength*2,&dwNumOfBytesWritten,NULL);//����ͼƬ·��
	WriteFile(hFileWrite,(wchar_t*)&m_sttgReaderCur.m_nIDOfBmp,4,&dwNumOfBytesWritten,NULL);//Ԥ��ͼƬID
	//-------------------------------------------------

	//�Ķ���������
	WriteFile(hFileWrite,(wchar_t*)&CTextViewerDlg::m_rectViewerWindow,16,&dwNumOfBytesWritten,NULL);//�Ķ���������
	
	CViewerSetting& sttg=CTextViewerDlg::m_sttgCur;
	nLength=sttg.m_strFaceName.GetLength();
	WriteFile(hFileWrite,(wchar_t*)&nLength,4,&dwNumOfBytesWritten,NULL);
	WriteFile(hFileWrite,sttg.m_strFaceName.GetBuffer(),nLength*2,&dwNumOfBytesWritten,NULL);//����
	WriteFile(hFileWrite,(wchar_t*)&sttg.m_lWordHeight,4,&dwNumOfBytesWritten,NULL);//��С
	WriteFile(hFileWrite,(wchar_t*)&sttg.m_lLineOffset,4,&dwNumOfBytesWritten,NULL);//�о�
	WriteFile(hFileWrite,(wchar_t*)&sttg.m_clrText,4,&dwNumOfBytesWritten,NULL);//��ɫ
	WriteFile(hFileWrite,(wchar_t*)&sttg.m_nFlagOfBg,4,&dwNumOfBytesWritten,NULL);//��������
	WriteFile(hFileWrite,(wchar_t*)&sttg.m_clrBg,4,&dwNumOfBytesWritten,NULL);//������ɫ
	nLength=sttg.m_strPicturePath.GetLength();
	WriteFile(hFileWrite,(wchar_t*)&nLength,4,&dwNumOfBytesWritten,NULL);
	WriteFile(hFileWrite,sttg.m_strPicturePath.GetBuffer(),nLength*2,&dwNumOfBytesWritten,NULL);//����ͼƬ·��
	WriteFile(hFileWrite,(wchar_t*)&sttg.m_nIDOfBmp,4,&dwNumOfBytesWritten,NULL);//Ԥ��ͼƬID
	WriteFile(hFileWrite,(wchar_t*)&sttg.m_clrBorder,4,&dwNumOfBytesWritten,NULL);//״̬���߿���ɫ
	WriteFile(hFileWrite,(wchar_t*)&sttg.m_clrDiaCur,4,&dwNumOfBytesWritten,NULL);//С����Ĭ����ɫ
	WriteFile(hFileWrite,(wchar_t*)&sttg.m_clrDiaDft,4,&dwNumOfBytesWritten,NULL);//С���鵱ǰ��ɫ
	//-------------------------------------------------
	
	CloseHandle(hFileWrite);
}


BOOL CReaderDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	switch(m_nIDCursor)
	{
	case 0://�������
		break;
	case 1://���͹��
		SetCursor(::LoadCursor(NULL,IDC_HAND));
		return TRUE;
		break;
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CReaderDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_nIDCursor=0;
	//TRACE(_T("Moved to border\n"));
	CDialog::OnNcMouseMove(nHitTest, point);
}

LRESULT CReaderDlg::OnViewerClose(WPARAM wParam, LPARAM lParam)
{//wParam��ָ��(��ʱ���ڶ����Ѿ�������)��lParam�ǿ�����
	
	DeleteViewer(m_ViewerPtrOfBlocks[lParam]);
	m_ViewerPtrOfBlocks[lParam]=NULL;
	m_MarkSelBlocks[lParam]=0;
	
	//ȡ��ѡ��״̬
	int r=((int)lParam-m_nRow*m_nCol*m_iPage)/m_nCol;//����к�
	int c=((int)lParam-m_nRow*m_nCol*m_iPage)-r*m_nCol;//����к�
	CPoint ptPreBlock;
	ptPreBlock.x=m_nMargin+(m_blockManager.m_szBlock.cx+m_nGapH)*c;
	ptPreBlock.y=m_nMargin+(m_blockManager.m_szBlock.cy+m_nGapV)*r;
	m_blockManager.GetBlcok((int)lParam)->m_bThumbInForce=0;
	InvalidateRect(CRect(ptPreBlock,m_blockManager.m_szBlock));
	TRACE(_T("The %dth block have been deselected.\n"),lParam);
	
	SetFocus();
	//----------------------------------------------------------
	return 0L;
}
void CReaderDlg::IncrecseTheTwoArrays()
{
	ASSERT(m_MarkSelBlocks.GetSize()==m_ViewerPtrOfBlocks.GetSize());
	int nPreLength=(int)m_MarkSelBlocks.GetSize();
	m_MarkSelBlocks.SetSize(m_blockManager.m_nListLength+ARRAY_INCREMENT);
	m_ViewerPtrOfBlocks.SetSize(m_blockManager.m_nListLength+ARRAY_INCREMENT);
	//����memset�����ˣ����ö�����������Ԫ�ؽ��г�ʼ��
}
void CReaderDlg::MoveThumb(CPoint point)
{
	
}
void CReaderDlg::OnReaderSetting()
{
	CReaderSettingDlg dlg(this);
	dlg.m_sttgBlockCur=m_sttgBlockCur;
	dlg.m_sttgBlockDft=m_sttgBlockDefault;
	m_sttgBlockPre=m_sttgBlockCur;
	dlg.m_sttgReaderDft=m_sttgReaderDefault;
	dlg.m_sttgReaderCur=m_sttgReaderCur;
	m_sttgReaderPre=m_sttgReaderCur;
	if(dlg.DoModal()==IDOK)
	{
		return;//����֮ǰ��Cur�����ĸ��ļ���
	}
	else
	{
		m_sttgBlockCur=m_sttgBlockPre;
		m_blockManager.m_clrBlockBg=m_sttgBlockCur.m_clrBlockBg;
		m_blockManager.m_clrBlockTitleBg=m_sttgBlockCur.m_clrBlockTitleBg;
		m_blockManager.m_clrBlockTitle=m_sttgBlockCur.m_clrBlockTitle;
		m_blockManager.m_clrBlockText=m_sttgBlockCur.m_clrBlockText;
		m_blockManager.m_clrMouseOver=m_sttgBlockCur.m_clrMouseOver;
		m_blockManager.m_clrMouseClick=m_sttgBlockCur.m_clrMouseClick;
		
		m_sttgReaderCur=m_sttgReaderPre;
		m_clrInterchange=m_sttgReaderCur.m_clrInterchange;
		m_clrMoveLocation=m_sttgReaderCur.m_clrMoveLocation;
		m_nFlagOfBg=m_sttgReaderCur.m_nFlagOfBg;
		m_clrBg=m_sttgReaderCur.m_clrBg;
		m_strPicturePath=m_sttgReaderCur.m_strPicturePath;
		
		CBlock* pBlock=m_blockManager.m_blockLH.m_pNext;
		while(pBlock!=NULL)
		{
			pBlock->m_bThumbInForce=0;
			pBlock=pBlock->m_pNext;
		}
		Invalidate();
		return;
	}
}

LRESULT CReaderDlg::OnPreviewSetting(WPARAM wParam, LPARAM lParam)
{
	if(wParam==PREVIEW_BLOCK_BGCOLOR)
	{
		m_sttgBlockCur.m_clrBlockBg=(COLORREF)lParam;
		m_blockManager.m_clrBlockBg=(COLORREF)lParam;
		CBlock* pBlock=m_blockManager.m_blockLH.m_pNext;
		while(pBlock!=NULL)
		{
			pBlock->m_bThumbInForce=0;
			pBlock=pBlock->m_pNext;
		}
		Invalidate();
		return 0L;
	}
	if(wParam==PREVIEW_BLOCK_TITLE_BG)
	{
		m_sttgBlockCur.m_clrBlockTitleBg=(COLORREF)lParam;
		m_blockManager.m_clrBlockTitleBg=(COLORREF)lParam;
		CBlock* pBlock=m_blockManager.m_blockLH.m_pNext;
		while(pBlock!=NULL)
		{
			pBlock->m_bThumbInForce=0;
			pBlock=pBlock->m_pNext;
		}
		Invalidate();
		return 0L;
	}
	if(wParam==PREVIEW_BLOCK_TITLE)
	{
		m_sttgBlockCur.m_clrBlockTitle=(COLORREF)lParam;
		m_blockManager.m_clrBlockTitle=(COLORREF)lParam;
		CBlock* pBlock=m_blockManager.m_blockLH.m_pNext;
		while(pBlock!=NULL)
		{
			pBlock->m_bThumbInForce=0;
			pBlock=pBlock->m_pNext;
		}
		Invalidate();
		return 0L;
	}
	if(wParam==PREVIEW_BLOCK_TEXT)
	{
		m_sttgBlockCur.m_clrBlockText=(COLORREF)lParam;
		m_blockManager.m_clrBlockText=(COLORREF)lParam;
		CBlock* pBlock=m_blockManager.m_blockLH.m_pNext;
		while(pBlock!=NULL)
		{
			pBlock->m_bThumbInForce=0;
			pBlock=pBlock->m_pNext;
		}
		Invalidate();
		return 0L;
	}
	if(wParam==PREVIEW_BLOCK_MOUSEOVER)
	{
		//��֧��Ԥ��
		m_sttgBlockCur.m_clrMouseOver=(COLORREF)lParam;
		m_blockManager.m_clrMouseOver=(COLORREF)lParam;
		/*CBlock* pBlock=m_blockManager.m_blockLH.m_pNext;
		while(pBlock!=NULL)
		{
			pBlock->m_bThumbInForce=0;
			pBlock=pBlock->m_pNext;
		}
		Invalidate();*/
		return 0L;
	}
	if(wParam==PREVIEW_BLOCK_MOUSECLICK)
	{
		//��֧��Ԥ��
		m_sttgBlockCur.m_clrMouseClick=(COLORREF)lParam;
		m_blockManager.m_clrMouseClick=(COLORREF)lParam;
		/*CBlock* pBlock=m_blockManager.m_blockLH.m_pNext;
		while(pBlock!=NULL)
		{
			pBlock->m_bThumbInForce=0;
			pBlock=pBlock->m_pNext;
		}
		Invalidate();*/
		return 0L;
	}
	if(wParam==PREVIEW_READER_INTERCHANGE)
	{
		//��֧��Ԥ��
		m_sttgReaderCur.m_clrInterchange=(COLORREF)lParam;
		m_clrInterchange=(COLORREF)lParam;
		/*Invalidate();*/
		return 0L;
	}
	if(wParam==PREVIEW_READER_MOVELOCATION)
	{
		//��֧��Ԥ��
		m_sttgReaderCur.m_clrMoveLocation=(COLORREF)lParam;
		m_clrMoveLocation=(COLORREF)lParam;
		/*Invalidate();*/
		return 0L;
	}
	if(wParam==PREVIEW_READER_BGCOLOR)
	{
		m_sttgReaderCur.m_nFlagOfBg=1;
		m_sttgReaderCur.m_clrBg=(COLORREF)lParam;
		m_nFlagOfBg=1;
		m_clrBg=(COLORREF)lParam;
		Invalidate();
		return 0L;
	}
	if(wParam==PREVIEW_READER_BGPICTURE)
	{
		
		CString* pStr=(CString*)lParam;
		m_sttgReaderCur.m_nFlagOfBg=2;
		m_sttgReaderCur.m_strPicturePath=*pStr;
		m_nFlagOfBg=2;
		m_strPicturePath=m_sttgReaderCur.m_strPicturePath;
		Invalidate();
		return 0L;
	}
	if(wParam==PREVIEW_READER_BGPICTURE_PRESET)
	{
		m_sttgReaderCur.m_nFlagOfBg=3;
		m_sttgReaderCur.m_nIDOfBmp=(UINT)lParam;
		m_nFlagOfBg=3;
		Invalidate();
		return 0L;
	}
	if(wParam==PREVIEW_READER_THEME)//Ԥ������
	{
		m_sttgBlockCur=g_PresetBlockSettings[lParam];
		m_sttgReaderCur=g_PresetReaderSettings[lParam];
		CBlock* pBlock=m_blockManager.m_blockLH.m_pNext;
		m_blockManager.m_clrBlockBg=m_sttgBlockCur.m_clrBlockBg;
		m_blockManager.m_clrBlockTitleBg=m_sttgBlockCur.m_clrBlockTitleBg;
		m_blockManager.m_clrBlockTitle=m_sttgBlockCur.m_clrBlockTitle;
		m_blockManager.m_clrBlockText=m_sttgBlockCur.m_clrBlockText;
		m_blockManager.m_clrMouseOver=m_sttgBlockCur.m_clrMouseOver;
		m_blockManager.m_clrMouseClick=m_sttgBlockCur.m_clrMouseClick;
		while(pBlock!=NULL)
		{
			pBlock->m_bThumbInForce=0;
			pBlock=pBlock->m_pNext;
		}
		Invalidate();
		return 0L;
	}
	return 0L;
}
void CReaderDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if(nChar==VK_SHIFT)
	{
		m_bShiftKeyDown=TRUE;
	}
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CReaderDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if(nChar==VK_SHIFT){
		m_bShiftKeyDown=FALSE;
	}
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CReaderDlg::OnReaderDeleteBlock()
{
	CBlock* pBlock;
	pBlock=m_blockManager.GetBlcok(m_iBlockMenu);
	if(pBlock!=NULL)
	{
		if(MessageBox(_T("ȷ��Ҫɾ����?"),_T("��ʾ"),MB_YESNO)==IDYES)
		{
			m_blockManager.DelBlock(m_iBlockMenu);
			Invalidate();
		}
		return;
	}
}

void CReaderDlg::OnReaderAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CReaderDlg::OnReaderHelp()
{
	CHelpDlg helpdlg;
	helpdlg.DoModal();
}

LRESULT CReaderDlg::OnShowBlockInfo(WPARAM wParam, LPARAM lParam)
{
	CPoint pointScreen((CPoint)wParam);
	CPoint point=pointScreen;
	ScreenToClient(&point);
	
	CSize& szBlock=m_blockManager.m_szBlock;
	int i=(point.y-m_nMargin)/(szBlock.cy+m_nGapV);
	int j=(point.x-m_nMargin)/(szBlock.cx+m_nGapH);
	int iBlock=i*m_nCol+j+m_nRow*m_nCol*m_iPage;
	//TRACE(_T("%d\n"),iBlock);
	CPoint ptTopLeft;
	ptTopLeft.x=m_nMargin+(szBlock.cx+m_nGapH)*j;
	ptTopLeft.y=m_nMargin+(szBlock.cy+m_nGapV)*i;
	if((point.x-ptTopLeft.x>=0&&point.x-ptTopLeft.x<=szBlock.cx)
		&&(point.y-ptTopLeft.y>=0&&point.y-ptTopLeft.y<=szBlock.cy)
		&&i<=m_nRow-1&&j<=m_nCol-1/*ȥ����Ե����Ч��*/)
	{
		CBlock* pBlock;
		pBlock=m_blockManager.GetBlcok(iBlock);
		if(pBlock!=NULL)
		{
			m_bShowTip=TRUE;
			m_ptMousePosTip=point;
			m_pBlockToShow=pBlock;
			Invalidate();//��ʾ��ʾ
			return 0L;
		}
	}
	return 0L;
}
LRESULT CReaderDlg::OnCloseBlockInfo(WPARAM wParam, LPARAM lParam)
{
	m_bShowTip=FALSE;
	Invalidate();//ʹ������Ч
	return 0L;
}
void CReaderDlg::OnReaderShowFileinfo()
{
	if(m_pBlockShowFileInfo==NULL)
		return;
	CFileInfoDlg dlg;
	dlg.m_strFileName=m_pBlockShowFileInfo->m_strFileName;
	dlg.m_strFilePathName=m_pBlockShowFileInfo->m_strFilePath;
	dlg.m_strFilePreview=m_pBlockShowFileInfo->m_strPreview;
	dlg.DoModal();
}


CTextViewerDlg* CReaderDlg::NewViewer()
{
	//ͨ��m_pParent=?NULL���ж�Viewer�Ƿ���ʹ����
	CTextViewerDlg* pViewer=NULL;
	POSITION pos=m_ViewerManager.GetHeadPosition();
	while(pos!=NULL)
	{
		pViewer=m_ViewerManager.GetNext(pos);
		if(pViewer->m_pParent==NULL)
			return pViewer;
	}
	pViewer=new CTextViewerDlg(NULL,NULL,-1,CRect(0,0,0,0));//����Viewer
	pViewer->Create();
	pViewer->ShowWindow(SW_HIDE);
	m_ViewerManager.AddTail(pViewer);
	return pViewer;
}
void CReaderDlg::DeleteViewer(CTextViewerDlg* pViewer)
{
	//�ǵý�m_pParent����ΪNULL
	CTextViewerDlg* pViewerTemp=NULL;
	POSITION pos=m_ViewerManager.GetHeadPosition();
	POSITION posPre=pos;
	int i=0;
	while(pos!=NULL)
	{
		i++;
		posPre=pos;
		pViewerTemp=m_ViewerManager.GetNext(pos);
		if(pViewerTemp==pViewer)
			break;
	}
	if(i>2)
	{
		m_ViewerManager.RemoveAt(posPre);
		pViewerTemp->DestroyWindow();
		delete pViewerTemp;
	}
	else
	{
		pViewerTemp->m_pParent=NULL;
		pViewerTemp->ShowWindow(SW_HIDE);
	}
}
void CReaderDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);

	switch (nSBCode)
	{
	case SB_BOTTOM: //�����������ײ�
		if(m_nPage>1)
			m_iPage=m_nPage-1;
		Invalidate();
		scrollinfo.nPos = scrollinfo.nMax; //�趨������λ��
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); //���»���λ��
		break;
	case SB_TOP: //������������
		m_iPage=0;
		Invalidate();
		scrollinfo.nPos = scrollinfo.nMin;
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		break;
	case SB_LINEUP: //�����ϼ�ͷ
		scrollinfo.nPos -= scrollinfo.nPage;
		if (scrollinfo.nPos<scrollinfo.nMin)
		{
			scrollinfo.nPos = scrollinfo.nMin;
			break;
		}
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		if(m_iPage>0)
			m_iPage-=1;
		Invalidate();
		break;
	case SB_LINEDOWN: //�����¼�ͷ
		scrollinfo.nPos += scrollinfo.nPage;
		if (scrollinfo.nPos>scrollinfo.nMax)
		{
			scrollinfo.nPos = scrollinfo.nMax;
			break;
		}
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		if(m_iPage<=m_nPage-1)
			m_iPage+=1;
		Invalidate();
		break;
	//case SB_PAGEUP: //���������Ϸ��հ�����
	//	scrollinfo.nPos -= 5;
	//	if (scrollinfo.nPos<scrollinfo.nMin)
	//	{
	//		scrollinfo.nPos = scrollinfo.nMin;
	//		break;
	//	}
	//	SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
	//	ScrollWindow(0,10*5);
	//	break;
	//case SB_PAGEDOWN: //���������·��հ�����
	//	scrollinfo.nPos += 5;
	//	if (scrollinfo.nPos>scrollinfo.nMax)
	//	{
	//		scrollinfo.nPos = scrollinfo.nMax;
	//		break;
	//	}
	//	SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
	//	ScrollWindow(0,-10*5);
	//	break;
	case SB_ENDSCROLL: //����뿪���飬���������϶�
		//MessageBox(_T("SB_ENDSCROLL"));
		break;
	case SB_THUMBPOSITION:
		// ScrollWindow(0,(scrollinfo.nPos-nPos)*10);
		// scrollinfo.nPos = nPos;
		// SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		//TRACE(_T("ThumbPosition\n"));
		break;
	case SB_THUMBTRACK: //�϶�����
		scrollinfo.nPos = nPos;
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		if(nPos/scrollinfo.nPage!=m_iPage)
		{
			m_iPage=nPos/scrollinfo.nPage;
			Invalidate();
		}
		//TRACE(_T("ThumbTrack\n"));
		break;
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CReaderDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialog::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
	g_Focused=FALSE;
}

void CReaderDlg::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);

	g_Focused=TRUE;
}
