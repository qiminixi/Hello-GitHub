#include "StdAfx.h"
#include "BlockManager.h"
#include "GlobalFunctions.h"
#include "CustomMessages.h"

CBlockManager::CBlockManager(int nM,CSize szBlock):m_szBlock(szBlock)
{
	m_nListLength=0;
	m_blockLH.m_pNext=NULL;
	m_nMargin=nM;
	m_nPreviewMargin=6;
	
	m_clrBlockBg=RGB(119,115,116);
	m_clrBlockTitleBg=RGB(94,83,88);
	m_clrBlockTitle=RGB(182,175,177);
	m_clrBlockText=RGB(0,0,0);
	m_clrMouseOver=RGB(93,93,93);
	m_clrMouseClick=RGB(172,172,172);
}

CBlockManager::~CBlockManager(void)
{
	//-------------------------------------------------
	//释放CBlock对象
	CBlock* pBlockPre=&m_blockLH;
	CBlock* pBlockNext=pBlockPre->m_pNext;
	while(pBlockNext=NULL)
	{
		//尚待添加	
	}
	//--------------------
}
void CBlockManager::CreateCompatibleBitmap(CBlock *pBlock, CDC *pDeviceDC)
{
	pBlock->m_bmpThumb.DeleteObject();
	pBlock->m_bmpThumb.CreateCompatibleBitmap(pDeviceDC,m_szBlock.cx-m_nMargin,m_szBlock.cy-m_nMargin);
	
}
void CBlockManager::DrawItem(CBlock* pBlock,CDC* pDC,CPoint ptTopLeft)
{
	if(pBlock==NULL) return;
	pDC->SelectStockObject(NULL_BRUSH);
	//pDC->Rectangle(CRect(ptTopLeft,m_szBlock));//
	CPoint ptInsideTopLeft=ptTopLeft+CPoint(m_nMargin/2,m_nMargin/2);
	
	CDC memDC;
	memDC.CreateCompatibleDC(NULL);
	CBitmap* pOldBmp=memDC.SelectObject(&pBlock->m_bmpThumb);
	if(!pBlock->m_bThumbInForce)
	{
		CBrush brushPreview(m_clrBlockBg);
		CBrush* pOldBrush=memDC.SelectObject(&brushPreview);
		int nL,nT,nR,nB;
		CPoint ptInsideTopLeft(0,0);
		nL=ptInsideTopLeft.x;/**/nT=ptInsideTopLeft.y;/**/nR=nL+m_szBlock.cx-m_nMargin+1;/**/nB=nT+m_szBlock.cy-m_nMargin+1;
		CRect rect=CRect(nL,nT,nR,nB);//预览总区域
		memDC.SelectStockObject(NULL_PEN);
		memDC.Rectangle(rect);//预览区域

		CFont font,*pOldFont;
		memDC.SetBkMode(TRANSPARENT);
		font.CreatePointFont(110,_T("黑体"),pDC);
		pOldFont=memDC.SelectObject(&font);
		CSize szString=memDC.GetTextExtent(pBlock->m_strFileName);
		nL=rect.left+m_nPreviewMargin;/**/nT=rect.top+m_nPreviewMargin;/**/nR=rect.right-m_nPreviewMargin;/**/nB=nT+szString.cy+2;//+2免得上下粘到一起
		rect=CRect(nL,nT,nR,nB);//标题区域
		CBrush brushTitle(m_clrBlockTitleBg);
		memDC.SelectObject(&brushTitle);
		memDC.Rectangle(rect);
		memDC.SetTextColor(m_clrBlockTitle);
		memDC.DrawText(pBlock->m_strFileName,rect,DT_WORD_ELLIPSIS);//DT_WORDBREAK和DT_WORD_ELLIPSIS一起的话不截断

		font.DeleteObject();
		font.CreatePointFont(80,_T("Arial"),pDC);
		pOldFont=memDC.SelectObject(&font);
		/*nL=nL*/nT=nB;/**/nR=rect.right;/**/nB=ptInsideTopLeft.y+m_szBlock.cy-m_nMargin-m_nPreviewMargin;
		rect=CRect(nL,nT,nR,nB);//预览文字区域
		//memDC.Rectangle(rect);
		memDC.SetTextColor(m_clrBlockText);
		GblDrawText(pBlock->m_strPreview,&memDC,rect);

		font.DeleteObject();
		memDC.SelectStockObject(BLACK_PEN);
		memDC.SelectObject(pOldFont);
		memDC.SelectObject(pOldBrush);
		pBlock->m_bThumbInForce=1;
		//TRACE(_T("CreateBitmap\n"));
	}
	pDC->BitBlt(ptInsideTopLeft.x,ptInsideTopLeft.y,m_szBlock.cx-m_nMargin,m_szBlock.cy-m_nMargin,&memDC,0,0,SRCCOPY);
	memDC.SelectObject(pOldBmp);
}
void CBlockManager::OnMouseOver(CBlock* pBlock,CDC *pDC,CPoint ptTopLeft)
{
	CRect rectBlock(ptTopLeft,m_szBlock+CSize(1,1));
	CBrush brush(m_clrMouseOver),*pOldBrush;
	pOldBrush=pDC->SelectObject(&brush);
	pDC->SelectStockObject(NULL_PEN);
	pDC->Rectangle(rectBlock);
	pDC->SelectObject(pOldBrush);
	
	DrawItem(pBlock,pDC,ptTopLeft);
}

void CBlockManager::OnMouseClick(CBlock* pBlock,CDC* pDC,CPoint ptTopLeft)
{
	CRect rectBlock(ptTopLeft,m_szBlock+CSize(1,1));
	CBrush brushOver(m_clrMouseClick),*pOldBrush;
	pOldBrush=pDC->SelectObject(&brushOver);
	pDC->SelectStockObject(NULL_PEN);
	pDC->Rectangle(rectBlock);
	pDC->SelectObject(pOldBrush);
	//pDC->SelectStockObject(BLACK_PEN);
	//FillRectWithHachures(pDC,rectBlock,0,m_clrMouseClick);
	
	DrawItem(pBlock,pDC,ptTopLeft);
}
CBlock* CBlockManager::InsertBlock(int nAddPos)
{//添加位置为0~N-1，原位置的数据后移。-1为插入到最后。
	if(nAddPos==m_nListLength&&m_nListLength!=0)//这个少不了的
		return NULL;//只有列表长度为空的时候才允许插入位置值和长度相等，不遵守对调用的规定,
	if(nAddPos==-1)//转化为正统的插入，就都可以用循环解决了
		nAddPos=m_nListLength;
	if(nAddPos>=0&&nAddPos<=m_nListLength)//定位到前驱节点，不是指定位置的节点。
	{
		int i=0;
		CBlock* pBlock=&m_blockLH;
		CBlock* pNewBlock=NULL;
		while(pBlock->m_pNext!=NULL)
		{
			if(i==nAddPos) break;
			i++;
			pBlock=pBlock->m_pNext;
		}
		//插入位置为0且表为空，插入位置找到、插入位置为末尾(即没找到)的前驱节点都是pBlock
		pNewBlock=new CBlock;
		pNewBlock->m_pNext=pBlock->m_pNext;
		pBlock->m_pNext=pNewBlock;
		m_nListLength++;
		return pNewBlock;
	}
	return NULL;
}
bool CBlockManager::DelBlock(int iBlock)
{
	if(m_nListLength==0||iBlock<0||iBlock>=m_nListLength)
		return false;
	CBlock* pPreBlock=&m_blockLH;
	int i=0;
	while(pPreBlock->m_pNext!=NULL)
	{
		if(i==iBlock)
			break;
		pPreBlock=pPreBlock->m_pNext;
		i++;
	}
	
	CBlock* pTemp=pPreBlock->m_pNext;
	pPreBlock->m_pNext=pTemp->m_pNext;
	delete pTemp;
	
	return true;
}
void CBlockManager::InterchangeBlocks(int iBlockA,int iBlockB)
{
	if(iBlockA==iBlockB)
		return;
	if(iBlockB>=m_nListLength||iBlockA<0)
		return;
	if(iBlockA>iBlockB)//把小的放到A上
	{
		iBlockA=iBlockA+iBlockB;
		iBlockB=iBlockA-iBlockB;
		iBlockA=iBlockA-iBlockB;
	}
	
	CBlock* pBlockA=&m_blockLH;
	int i=0;
	while(pBlockA->m_pNext!=NULL)
	{
		if(i==iBlockA)
			break;
		pBlockA=pBlockA->m_pNext;
		i++;
	}
	if(iBlockA-iBlockB==-1)//相邻的两个块直接交换
	{
		CBlock* pTempA=pBlockA->m_pNext;
		pBlockA->m_pNext=pTempA->m_pNext;//A提出来
		pBlockA=pBlockA->m_pNext;//pBlockA指向B
		pTempA->m_pNext=pBlockA->m_pNext;//A的next设为B的next
		pBlockA->m_pNext=pTempA;//B的next接上A
		return;
	}
	i=0;
	CBlock* pBlockB=&m_blockLH;
	while(pBlockB->m_pNext!=NULL)
	{
		if(i==iBlockB)
			break;
		pBlockB=pBlockB->m_pNext;
		i++;
	}
	CBlock* pTempA=pBlockA->m_pNext;
	CBlock* pTempB=pBlockB->m_pNext;
	pBlockA->m_pNext=pTempA->m_pNext;//A提出来
	pBlockB->m_pNext=pTempB->m_pNext;//B提出来
	pTempA->m_pNext=pBlockB->m_pNext;
	pBlockB->m_pNext=pTempA;//A插到B位置
	pTempB->m_pNext=pBlockA->m_pNext;
	pBlockA->m_pNext=pTempB;//B插到A位置
	
	
}
void CBlockManager::MoveBlock(int iOrgBlock,int iDesBlock)
{
	if(iDesBlock==iOrgBlock)
		return;
	if(iDesBlock>=m_nListLength||iOrgBlock<0)
		return;
	int i=0;
	CBlock* pOrg=&m_blockLH;
	while(pOrg->m_pNext!=NULL)
	{
		if(i==iOrgBlock)
			break;
		pOrg=pOrg->m_pNext;
		i++;
	}
	i=0;
	CBlock* pDes=&m_blockLH;
	while(pDes->m_pNext!=NULL)
	{
		if(i==iDesBlock)
			break;
		pDes=pDes->m_pNext;
		i++;
	}
	CBlock* pTemp=pOrg->m_pNext;
	pOrg->m_pNext=pTemp->m_pNext;//将源块提出来
	if(pTemp==pDes)//i移到i+1的处理
		pDes=pOrg->m_pNext;
	pTemp->m_pNext=pDes->m_pNext;//将源块的next接上
	pDes->m_pNext=pTemp;//将目标位置的next接上
}

CBlock* CBlockManager::GetBlcok(int iIndex)
{
	if(iIndex>=m_nListLength||iIndex<=-1)
		return NULL;
	CBlock* pBlock=m_blockLH.m_pNext;
	int i=0;
	while(pBlock!=NULL)
	{
		if(i==iIndex)
			return pBlock;
		i++;
		pBlock=pBlock->m_pNext;
	}
	return NULL;
}
bool CBlockManager::LoadFilesInfo()
{
	CString strFilePath=GblGetDirectoryOfExecutableFile()+_T("文件信息.data");
	HANDLE hFileRead=CreateFile(strFilePath,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,NULL,NULL);
	if(hFileRead==INVALID_HANDLE_VALUE )
	{
		TRACE(_T("-----------------------导入文件信息时文件打开失败\n"));
		return false;
	}
	UINT nFileSize=GetFileSize(hFileRead,NULL);
	DWORD dwNumOfBytesRead=0;
	int nLength=0;
	while(nFileSize!=0)//为0的话文件就读完了
	{
		CBlock* pBlock=InsertBlock();
		
		ReadFile(hFileRead,(wchar_t*)&nLength,4,&dwNumOfBytesRead,NULL);
		pBlock->m_strFileName.GetBufferSetLength(nLength);
		ReadFile(hFileRead,pBlock->m_strFileName.GetBuffer(),nLength*2,&dwNumOfBytesRead,NULL);//文件名
		nFileSize-=(4+dwNumOfBytesRead);
		
		ReadFile(hFileRead,(wchar_t*)&nLength,4,&dwNumOfBytesRead,NULL);
		pBlock->m_strFilePath.GetBufferSetLength(nLength);
		ReadFile(hFileRead,pBlock->m_strFilePath.GetBuffer(),nLength*2,&dwNumOfBytesRead,NULL);//文件路径名
		nFileSize-=(4+dwNumOfBytesRead);
		
		ReadFile(hFileRead,(wchar_t*)&pBlock->m_iFileBlock,4,&dwNumOfBytesRead,NULL);//块id
		nFileSize-=4;
		ReadFile(hFileRead,(wchar_t*)&pBlock->m_nReadPos,4,&dwNumOfBytesRead,NULL);//字符位置
		nFileSize-=4;
		ReadFile(hFileRead,pBlock->m_strPreview.GetBuffer(),BLOCK_PREVIEW_SIZE*2,&dwNumOfBytesRead,NULL);//预览(固定128字符)
		nFileSize-=dwNumOfBytesRead;
		
		//导入书签
		int n=0;
		ReadFile(hFileRead,(wchar_t*)&n,4,&dwNumOfBytesRead,NULL);
		nFileSize-=4;
		pBlock->m_bookmark.SetSize(n);
		for(int i=0;i<n;i++)
		{
			CBookmark& bmk=pBlock->m_bookmark.GetAt(i);
			ReadFile(hFileRead,(wchar_t*)&bmk.m_iFileBlock,4,&dwNumOfBytesRead,NULL);//块id
			ReadFile(hFileRead,(wchar_t*)&bmk.m_nReadPos,4,&dwNumOfBytesRead,NULL);//字符位置
			nFileSize-=8;
			
			ReadFile(hFileRead,(wchar_t*)&nLength,4,&dwNumOfBytesRead,NULL);
			bmk.m_strName.GetBufferSetLength(nLength);
			ReadFile(hFileRead,bmk.m_strName.GetBuffer(),nLength*2,&dwNumOfBytesRead,NULL);//书签名
			nFileSize-=(4+dwNumOfBytesRead);
			
			ASSERT(pBlock->m_strPreview.GetLength()==BLOCK_PREVIEW_SIZE);
			ReadFile(hFileRead,bmk.m_strPreview.GetBuffer(),BLOCK_PREVIEW_SIZE*2,&dwNumOfBytesRead,NULL);//预览(固定128字符)
			nFileSize-=dwNumOfBytesRead;
		}
		//-----------------------------------------------
	}
	CloseHandle(hFileRead);
	TRACE("Load %d Files From Disk.\n",m_nListLength);
	return true;
}

bool CBlockManager::StoreFilesInfo()
{
	CString strFilePath=GblGetDirectoryOfExecutableFile()+_T("文件信息.data");
	HANDLE hFileWrite=CreateFile(strFilePath,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,NULL,NULL);
	if(hFileWrite==INVALID_HANDLE_VALUE )
	{
		MessageBox(NULL,_T("未能存储打开的文件信息"),_T("提示"),MB_OK);
		return false;
	}
	DWORD dwNumOfBytesWritten=0;
	CBlock* pBlock=m_blockLH.m_pNext;
	int nLength=0;
	while(pBlock!=NULL)
	{
		nLength=pBlock->m_strFileName.GetLength();
		WriteFile(hFileWrite,(wchar_t*)&nLength,4,&dwNumOfBytesWritten,NULL);
		WriteFile(hFileWrite,pBlock->m_strFileName.GetBuffer(),nLength*2,&dwNumOfBytesWritten,NULL);//文件名
		
		nLength=pBlock->m_strFilePath.GetLength();
		WriteFile(hFileWrite,(wchar_t*)&nLength,4,&dwNumOfBytesWritten,NULL);
		WriteFile(hFileWrite,pBlock->m_strFilePath.GetBuffer(),nLength*2,&dwNumOfBytesWritten,NULL);//文件路径名
		
		WriteFile(hFileWrite,(wchar_t*)&pBlock->m_iFileBlock,4,&dwNumOfBytesWritten,NULL);//块id
		WriteFile(hFileWrite,(wchar_t*)&pBlock->m_nReadPos,4,&dwNumOfBytesWritten,NULL);//字符位置
		ASSERT(pBlock->m_strPreview.GetLength()==BLOCK_PREVIEW_SIZE);
		WriteFile(hFileWrite,pBlock->m_strPreview.GetBuffer(),BLOCK_PREVIEW_SIZE*2,&dwNumOfBytesWritten,NULL);//预览(固定128字符)
		
		//存储书签
		int n=(int)pBlock->m_bookmark.GetCount();
		WriteFile(hFileWrite,(wchar_t*)&n,4,&dwNumOfBytesWritten,NULL);
		for(int i=0;i<n;i++)
		{
			CBookmark& bmk=pBlock->m_bookmark.GetAt(i);
			WriteFile(hFileWrite,(wchar_t*)&bmk.m_iFileBlock,4,&dwNumOfBytesWritten,NULL);//块id
			WriteFile(hFileWrite,(wchar_t*)&bmk.m_nReadPos,4,&dwNumOfBytesWritten,NULL);//字符位置
			
			nLength=bmk.m_strName.GetLength();
			WriteFile(hFileWrite,(wchar_t*)&nLength,4,&dwNumOfBytesWritten,NULL);
			WriteFile(hFileWrite,bmk.m_strName.GetBuffer(),nLength*2,&dwNumOfBytesWritten,NULL);//书签名
			
			ASSERT(bmk.m_strPreview.GetLength()==BLOCK_PREVIEW_SIZE);
			WriteFile(hFileWrite,bmk.m_strPreview.GetBuffer(),BLOCK_PREVIEW_SIZE*2,&dwNumOfBytesWritten,NULL);//预览(固定128字符)
		}
		//-----------------------------------------------
		
		pBlock=pBlock->m_pNext;
		
	}
	CloseHandle(hFileWrite);
	return true;
}

void CBlockManager::GetAFile(CString& strFilePath)
{
	HANDLE hFileRead=CreateFileW(strFilePath.GetBuffer(),GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING, NULL, NULL);
	if(hFileRead==INVALID_HANDLE_VALUE )
	{
		MessageBox(NULL,_T("无法打开文件"),_T("Reader"),IDOK);
		return;
	}
	int nFileTypeCode=JudgeFileCode(hFileRead);//判断文本编码
	if(nFileTypeCode==0)
	{
		MessageBox(NULL,_T("无法识别的文本编码"),_T("Reader"),IDOK);
		CloseHandle(hFileRead);
		return;
	}
	//获取文件名
	CString strFileName;//单纯的文件名
	int i=0,j=0;
	while(j!=-1)
	{
		i=j;
		j=strFilePath.Find('\\',j+1);
	}
	i++;
	while(strFilePath.GetAt(i)!='\0'){strFileName+=strFilePath.GetAt(i);i++;}
	//--------------------------------------------------
	CBlock* pBlock=NULL;
	pBlock=InsertBlock();
	pBlock->m_strFileName=strFileName;
	pBlock->m_strFilePath=strFilePath;
	pBlock->m_iFileBlock=0;
	pBlock->m_nReadPos=0;
	//用'\0'填充字符串，读完就自动有'\0'了
	DWORD dwBytesRead;
	if(nFileTypeCode==1)//Read ANSI File
	{
		CStringA strTextA('\0',BLOCK_PREVIEW_SIZE*2);//自带了一个'\0',没有记入字符串长度
		ReadFile(hFileRead,strTextA.GetBuffer(),BLOCK_PREVIEW_SIZE*2,&dwBytesRead,NULL);
		char* bufferA=strTextA.GetBuffer();
		int n=0;
		for(dwBytesRead=0;bufferA[dwBytesRead]!=0;dwBytesRead++)
			if(bufferA[dwBytesRead]<-1) n++;//判断中文字符
		if(n%2==1) 
			bufferA[dwBytesRead-1]='\0';//如果字符数为奇数，那么一定是最后一个汉字被分割了，剔除掉
		
		memset(pBlock->m_strPreview.GetBuffer(),0,BLOCK_PREVIEW_SIZE*2);//用'\0'填充它
		MultiByteToWideChar(CP_ACP,0,bufferA,-1,pBlock->m_strPreview.GetBuffer(),BLOCK_PREVIEW_SIZE);
	}
	else if(nFileTypeCode==2)//Read Unicode File
	{
		memset(pBlock->m_strPreview.GetBuffer(),0,BLOCK_PREVIEW_SIZE*2);//用'\0'填充它
		SetFilePointer(hFileRead,2,NULL,FILE_BEGIN);//跳过文本编码标识
		ReadFile(hFileRead,pBlock->m_strPreview.GetBuffer(),BLOCK_PREVIEW_SIZE*2,&dwBytesRead,NULL);
	}
	else if(nFileTypeCode==3)//Read UTF-8 File
	{
		SetFilePointer(hFileRead,3,NULL,FILE_BEGIN);
		
		CStringA strTextA('\0',BLOCK_PREVIEW_SIZE*2);
		char* bufferA=strTextA.GetBuffer();
		ReadFile(hFileRead,bufferA,BLOCK_PREVIEW_SIZE*2,&dwBytesRead,NULL);
		
		int i=BLOCK_PREVIEW_SIZE*2-1;
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
		
		memset(pBlock->m_strPreview.GetBuffer(),0,BLOCK_PREVIEW_SIZE*2);//用'\0'填充它
		MultiByteToWideChar(CP_UTF8,0,bufferA,-1,pBlock->m_strPreview.GetBuffer(),BLOCK_PREVIEW_SIZE);
	}
	else if(nFileTypeCode==4)//Read UnicodeBigEndian File
	{
		memset(pBlock->m_strPreview.GetBuffer(),0,BLOCK_PREVIEW_SIZE*2);//用'\0'填充它
		SetFilePointer(hFileRead,2,NULL,FILE_BEGIN);//跳过文本编码标识
		ReadFile(hFileRead,pBlock->m_strPreview.GetBuffer(),BLOCK_PREVIEW_SIZE*2,&dwBytesRead,NULL);
		
		for(unsigned int i=0;i<dwBytesRead/2;i++)
		{
			wchar_t* buffer=pBlock->m_strPreview.GetBuffer();
			buffer[i]=((buffer[i]>>8)& 0x00FF) | ((buffer[i]<<8)& 0xFF00);
		}
	}
	//pBlock->m_strPreview.Replace('\0',' ');//读ANSI文件和小文件的话会产生多余的'\0'，删除掉
	pBlock->m_strPreview.Replace('\r',' ');//删除，没价值而且影响读档
	pBlock->m_strPreview.Replace('\n',' ');//删除，没价值而且影响读档
	//TRACE(_T("%d\n"),pBlock->m_strPreview.GetLength());
	CloseHandle(hFileRead);	
}
bool CBlockManager::IsFileExisted(CString strFilePathName)
{
	CBlock* pBlock=m_blockLH.m_pNext;
	while(pBlock!=NULL)
	{
		if(pBlock->m_strFilePath==strFilePathName)
			return true;
		pBlock=pBlock->m_pNext;
	}
	return false;
}