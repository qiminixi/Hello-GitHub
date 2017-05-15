#pragma once
#include "afxwin.h"

#define BM_END -2 //β��
#define BM_BEGIN -1//ͷ��
#define BLOCK_PREVIEW_SIZE 128//Ԥ������(Unicode)

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
	int m_iFileBlock;//�ļ�����������0��ʼ��
	int m_nReadPos;//�ڿ����ַ�����������0��ʼ��
	CArray<CBookmark,CBookmark> m_bookmark;
	
	
	CBitmap m_bmpThumb;//����ͼ
	BOOL m_bThumbInForce;//����ͼ�Ƿ���Ч
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
	//�ڴ��ڵ���ʾ����
	int m_nMargin;
	int m_nPreviewMargin;
	CBrush m_brushSelected;
	
	CSize m_szBlock;//һ������ͼռ�ݵĴ�С������ȥm_nMargin��������ͼ�Ĵ�С��
	
	//������Ϣ
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
	bool LoadFilesInfo();//��ȡ�ı��ļ���Ϣ
	bool StoreFilesInfo();//�洢�ı��ļ���Ϣ
	void GetAFile(CString& strFilePath);//��ȡ�ļ���Ϣ(ֻ�ǽ����飬���Ǵ�)
	void CreateCompatibleBitmap(CBlock* pBlock,CDC* pDeviceDC);
	void DrawItem(CBlock* pBlock,CDC* pDC,CPoint ptTopLeft);
	void OnMouseOver(CBlock* pBlock,CDC* pDC,CPoint ptTopLeft);
	void OnMouseClick(CBlock* pBlock,CDC* pDC,CPoint ptTopLeft);
	CBlock* InsertBlock(int nAddPos=-1);//nAddPosָ����ӵ�λ��
	void InterchangeBlocks(int iBlcokA,int iBlockB);
	void MoveBlock(int iOrgBlcok,int iDesBlock);
	CBlock* GetBlcok(int iIndex);//��ȡ����Ϊi�Ŀ�
	bool DelBlock(int iBlock);//nDelPosָ��ɾ��λ��
	bool IsFileExisted(CString strFilePathName);
	
};
