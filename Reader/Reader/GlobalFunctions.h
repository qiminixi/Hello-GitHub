#pragma once

int GetIntegerFromBuffer(LPCTSTR buffer,int& iStart);//从iPtr指定的位置开始读取一个整数
void GetStringFromBuffer(CString& strDes,LPCTSTR buffer, int& iStart);//从iPtr指定的位置开始读取一个"\r\n"结尾的字符串
void GetStringFromBuffer(LPTSTR bufferDes,LPCTSTR buffer, int& iStart);
CString GblGetDirectoryOfExecutableFile();
int GblDrawText(CString& str,CDC* pDC,CRect& rect);//在矩形中显示文字，从左到右，从上到下，多余截断加"..."。返回值是显示字符数按英文算。
int JudgeFileCode(HANDLE hFileRead);//鉴别文本编码
void FillRectWithHachures(CDC* pDC,CRect& rect,int nIndex,COLORREF crColor);//用阴影线填充矩形
void FillRectWithHachuresFake(CDC* pDC,CRect& rect,int nIndex,COLORREF crColor);//用阴影线填充矩形