#pragma once
#include "afxwin.h"

#define BM_END -2 //尾部
#define BM_BEGIN -1//头部
#define BLOCK_PREVIEW_SIZE 128//预览字数(Unicode)

struct CBookmark
{
	int m_iFileBlock;
	int m_nReadPos;
	CString m_strName;
	CString m_strPreview;
	
public:
	CBookmark():m_strPreview('\0',BLOCK_PREVIEW_SIZE)
	{
	}
};

struct CBlock
{
	CString m_strFileName;
	CString m_strFilePath;
	CString m_strPreview;
	int m_iFileBlock;//文件块索引，从0开始算
	int m_nReadPos;//在块中字符的索引，从0开始算
	CArray<CBookmark,CBookmark> m_bookmark;
	
	
	CBitmap m_bmpThumb;//缩略图
	BOOL m_bThumbInForce;//缩略图是否有效
	CBlock* m_pNext;
	
public:
	CBlock():m_strPreview('\0',BLOCK_PREVIEW_SIZE)
	{
		m_bThumbInForce=0;
		/*for(int i=0;i<4;i++)
		{
			CBookmark bookmark;
			CString str;
			str.Format(_T("%d"),i+1111111);
			bookmark.m_iFileBlock=i;
			bookmark.m_nReadPos=i;
			bookmark.m_strName=str;
			lstrcpy(bookmark.m_strPreview.GetBuffer(),str.GetBuffer());
			
			m_bookmark.Add(bookmark);
		}*/
	}
};
class CBlockManager
{
public:
	
public:
	CBlock m_blockLH;
public:
	int m_nListLength;
	//在窗口的显示属性
	int m_nMargin;
	int m_nPreviewMargin;
	CBrush m_brushSelected;
	
	CSize m_szBlock;//一个缩略图占据的大小，它减去m_nMargin就是缩略图的大小。
	
	//配置信息
	COLORREF m_clrBlockBg;
	COLORREF m_clrBlockTitleBg;
	COLORREF m_clrBlockTitle;
	COLORREF m_clrBlockText;
	//
	COLORREF m_clrMouseOver;
	COLORREF m_clrMouseClick;
	//--------------------------
public:
	CBlockManager(int nM,CSize szBlock);
	~CBlockManager(void);
public:
	bool LoadFilesInfo();//读取文本文件信息
	bool StoreFilesInfo();//存储文本文件信息
	void GetAFile(CString& strFilePath);//获取文件信息(只是建立块，不是打开)
	void CreateCompatibleBitmap(CBlock* pBlock,CDC* pDeviceDC);
	void DrawItem(CBlock* pBlock,CDC* pDC,CPoint ptTopLeft);
	void OnMouseOver(CBlock* pBlock,CDC* pDC,CPoint ptTopLeft);
	void OnMouseClick(CBlock* pBlock,CDC* pDC,CPoint ptTopLeft);
	CBlock* InsertBlock(int nAddPos=-1);//nAddPos指明添加的位置
	void InterchangeBlocks(int iBlcokA,int iBlockB);
	void MoveBlock(int iOrgBlcok,int iDesBlock);
	CBlock* GetBlcok(int iIndex);//获取索引为i的块
	bool DelBlock(int iBlock);//nDelPos指明删除位置
	bool IsFileExisted(CString strFilePathName);
	
};
