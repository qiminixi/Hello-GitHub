#pragma once

int GetIntegerFromBuffer(LPCTSTR buffer,int& iStart);//��iPtrָ����λ�ÿ�ʼ��ȡһ������
void GetStringFromBuffer(CString& strDes,LPCTSTR buffer, int& iStart);//��iPtrָ����λ�ÿ�ʼ��ȡһ��"\r\n"��β���ַ���
void GetStringFromBuffer(LPTSTR bufferDes,LPCTSTR buffer, int& iStart);
CString GblGetDirectoryOfExecutableFile();
int GblDrawText(CString& str,CDC* pDC,CRect& rect);//�ھ�������ʾ���֣������ң����ϵ��£�����ضϼ�"..."������ֵ����ʾ�ַ�����Ӣ���㡣
int JudgeFileCode(HANDLE hFileRead);//�����ı�����
void FillRectWithHachures(CDC* pDC,CRect& rect,int nIndex,COLORREF crColor);//����Ӱ��������
void FillRectWithHachuresFake(CDC* pDC,CRect& rect,int nIndex,COLORREF crColor);//����Ӱ��������