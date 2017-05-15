#include "stdafx.h"
#include "GlobalFunctions.h"

CString GblGetDirectoryOfExecutableFile()
{
	CString str('\0',255);
	::GetCurrentDirectory(255,str.GetBuffer());
	::GetModuleFileName(NULL,str.GetBuffer(),255);
	int i=0,j=0;
	while(j!=-1)
	{
		i=j;
		j=str.Find('\\',j+1);
	}
	i++;//�������һ��'\'
	for(;str.GetAt(i)!='\0';i++) str.SetAt(i,'\0');
	str.Remove('\0');
	return str;
}
int GetIntegerFromBuffer(LPCTSTR buffer, int& iStart)
{
	int nData=0;
	while((buffer[iStart]>='0'&&buffer[iStart]<='9')&&buffer[iStart]!='\0')
	{
		nData*=10;
		nData+=buffer[iStart]-'0';
		iStart++;
	}
	return nData;
}
void GetStringFromBuffer(CString& strDes,LPCTSTR buffer, int& iStart)
{
	while((buffer[iStart]!='\r'||buffer[iStart+1]!='\n')&&buffer[iStart]!='\0')
	{//��Ӷ�'\0'���жϽ�����һ��û�лس�������
		strDes+=buffer[iStart];
		iStart++;
	}
}
void GetStringFromBuffer(LPTSTR bufferDes,LPCTSTR buffer, int& iStart)
{
	int i=0;
	while((buffer[iStart]!='\r'||buffer[iStart+1]!='\n')&&buffer[iStart]!='\0')
	{//��Ӷ�'\0'���жϽ�����һ��û�лس�������
		bufferDes[i]=buffer[iStart];
		i++;
		iStart++;
	}
}
int GblDrawText(CString& str,CDC* pDC,CRect& rect)
{
	CString strWord=_T("a");
	CSize szWord=pDC->GetTextExtent(strWord);//һ�����ֵĿ��=2��Ӣ���ַ��Ŀ��-1���߶���ͬ
	int nE=0,nC=0;
	CPoint ptTopLeft=rect.TopLeft();
	int nRectRight=rect.right;//�ұ߽�
	bool bIsNewLine=true;

	for(int i=0;i<=str.GetLength();i++)
	{
		bool bIsE;
		wchar_t c[2]={'\0'};
		c[0]=str.GetAt(i);
		if(c[0]<=254&&c[0]>=1&&c[0]!='\r'&&c[0]!='\n')//�ж��ַ�����
			bIsE=true;
		else if(c[0]=='\r'||c[0]=='\n'||c[0]=='\0')
			continue;
		else bIsE=false;
		if( (bIsE&&(ptTopLeft.x+szWord.cx>nRectRight)) || (!bIsE&&(ptTopLeft.x+szWord.cx+szWord.cx>nRectRight)) )
		{
			if(ptTopLeft.y+szWord.cy+szWord.cy>rect.bottom)//����֮ǰ�жϣ�����֮���жϵĻ����ûص���һ��λ�á�
				break;//�����һ���Ѿ��㶨��
			ptTopLeft.y+=szWord.cy;//����
			ptTopLeft.x=rect.left;
			bIsNewLine=true;
		}
		if(ptTopLeft.y+szWord.cy+szWord.cy>rect.bottom&&bIsNewLine)//��һ�е��±߳����˾���Ļ�
			nRectRight-=szWord.cx*3;//�ճ�"..."��λ��
		
		pDC->TextOutW(ptTopLeft.x,ptTopLeft.y,c,1);
		if(bIsE)
			ptTopLeft.x+=szWord.cx;//׼����һ���ַ�
		else
			ptTopLeft.x+=szWord.cx+szWord.cx;//׼����һ���ַ�
		bIsNewLine=false;
	}
	for(int i=0;i<6;i++)//��ʾʡ�Ժ�
	{
		if(ptTopLeft.x+szWord.cx/2<rect.right)
			pDC->TextOutW(ptTopLeft.x,ptTopLeft.y,_T("."),1);//����'.'ռһ��Ӣ���ַ�,
		else if(ptTopLeft.y+szWord.cy<rect.bottom)//����
		{
			ptTopLeft.x=rect.left;
			ptTopLeft.y+=szWord.cy;
			pDC->TextOutW(ptTopLeft.x,ptTopLeft.y,_T("."),1);//����'.'ռһ��Ӣ���ַ�,
		}
		else//���ж����о�û�취��
			break;
		ptTopLeft.x+=szWord.cx/2;
	}
	return 1;//�����ַ����ݲ�����
}
//�ж��ļ��ı�������
//����ֵ��0Ϊ�޷��жϣ�1ΪANSCI��2ΪUnicode,3ΪUTF-8,4ΪUnicode big endian
int JudgeFileCode(HANDLE hFileRead)
{
	unsigned char bufferA[33];
	DWORD dwBytesRead;
	ReadFile(hFileRead,bufferA,32,&dwBytesRead,NULL);
	SetFilePointer(hFileRead,0,NULL,FILE_BEGIN);
	bufferA[32]='\0';
	if(bufferA[0]==255&&bufferA[1]==254)//FFFEΪUnicode
		return 2;
	if(bufferA[0]==239&&bufferA[1]==187&&bufferA[2]==191)//EF(239)BB(187)BF(191)ΪUTF_8
		return 3;
	if(bufferA[0]==254&&bufferA[1]==255)//FEFFΪUnicode big endian
		return 4;
	return 1;//������ֻ֧��Unicode��ANSI��
	//return 0;
}
void FillRectWithHachuresFake(CDC* pDC,CRect& rect,int nIndex,COLORREF crColor )
{
	int nGap;
	int l=rect.left;
	int t=rect.top;
	int r=rect.right;
	int b=rect.bottom;
	CPen pen(PS_SOLID,1,crColor);
	CPen* pOldPen=pDC->SelectObject(&pen);
	//for(int i=l;i<=r;i+=nGap)// *\\\*
	//{
	//	pDC->MoveTo(i,t);
	//	pDC->LineTo(r,t+r-i);
	//}
	//for(int i=
	/*for(int i=t;i<=b;i+=nGap)
	{
		pDC->MoveTo(l,i);
		pDC->LineTo(
	}*/
	nGap=(r-l)/12;
	int m=(r-l)%12;
	for(int i=l;i<r;i+=nGap)//����
	{
		pDC->MoveTo(i,t);
		pDC->LineTo(i,b);
		int k=i;
		if(m>1){
			i++;
			m--;
		}
	}
	for(int i=t;i<=b;i+=nGap)
	{
		pDC->MoveTo(l,i);
		pDC->LineTo(r,i);
	}
	pDC->SelectObject(pOldPen);
}
void FillRectWithHachures(CDC* pDC,CRect& rect,int nIndex,COLORREF crColor )
{
	int nGap=8;
	int l=rect.left;
	int t=rect.top;
	int r=rect.right;
	int b=rect.bottom;
	int k=0;
	CPen pen(PS_SOLID,1,crColor);
	CPen* pOldPen=pDC->SelectObject(&pen);
	//************\\\**************
	if(r-l<=b-t)//�������κ�������
	{
		for(int i=l;i<=r;i+=nGap)//����Ĳ��֣�������
		{
			pDC->MoveTo(i,t);
			pDC->LineTo(r,t+r-i);
		}
		for(int i=t+r-l;i<=b;i+=nGap)//�м�Ĳ��֣����ϵ���
		{
			pDC->MoveTo(r,i);
			pDC->LineTo(l,i-(r-l));
		}
		k=(b-t-(r-l))%nGap;
		if(k!=0) k=nGap-k;
		for(int i=b-(r-l)+k;i<=b;i+=nGap)//����Ĳ��֣����ϵ��£������ϴε�����
		{
			pDC->MoveTo(l,i);
			pDC->LineTo(l+(b-i),b);
		}
	}
	else//��������
	{
		for(int i=t;i<=b;i+=nGap)//��ߵĲ��֣����ϵ���
		{
			pDC->MoveTo(l,i);
			pDC->LineTo(l+b-i,b);
		}
		for(int i=l+(b-t);i<=r;i+=nGap)//�м䲿�֣�������
		{
			pDC->MoveTo(i,b);
			pDC->LineTo(i-(b-t),t);
		}
		k=(r-l-(b-t))%nGap;
		if(k!=0) k=nGap-k;
		for(int i=r-(b-t)+k;i<=r;i+=nGap)//�м䲿�֣�������,�����ϴε�����
		{
			pDC->MoveTo(i,t);
			pDC->LineTo(r,t+r-i);
		}
	}
	//-------------------------------------------------------------------------
	
	//************\\\**************
	if(r-l<=b-t)//�������κ�������
	{
		for(int i=r;i>=l;i-=nGap)//����Ĳ��֣����ҵ���
		{
			pDC->MoveTo(i,t);
			pDC->LineTo(l,t+i-l);
		}
		for(int i=t+r-l;i<=b;i+=nGap)//�м�Ĳ��֣����ϵ���
		{
			pDC->MoveTo(l,i);
			pDC->LineTo(r,i-(r-l));
		}
		k=(b-t-(r-l))%nGap;
		if(k!=0) k=nGap-k;
		for(int i=b-(r-l)+k;i<=b;i+=nGap)//����Ĳ��֣����ϵ��£������ϴε�����
		{
			pDC->MoveTo(r,i);
			pDC->LineTo(r-(b-i),b);
		}
	}
	else//��������
	{
		for(int i=b;i>=t;i-=nGap)//��ߵĲ��֣����ϵ���
		{
			pDC->MoveTo(l,i);
			pDC->LineTo(l+i-t,t);
		}
		for(int i=l+(b-t);i<=r;i+=nGap)//�м䲿�֣�������
		{
			pDC->MoveTo(i,t);
			pDC->LineTo(i-(b-t),b);
		}
		k=(r-l-(b-t))%nGap;
		if(k!=0) k=nGap-k;
		for(int i=r-(b-t)+k;i<=r;i+=nGap)//�м䲿�֣�������,�����ϴε�����
		{
			pDC->MoveTo(i,b);
			pDC->LineTo(r,b-(r-i));
		}
	}
	//-------------------------------------------------------------------------
	pDC->SelectObject(pOldPen);
}