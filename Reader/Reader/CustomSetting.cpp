#include "stdafx.h"
#include "Reader.h"
#include "ViewerSetting.h"

int g_nIDOfBMPS[10]={
	IDB_BITMAP_BG1,
	IDB_BITMAP_BG2,
	IDB_BITMAP_BG3,
	IDB_BITMAP_BG4,
	IDB_BITMAP_BG5,
	IDB_BITMAP_BG6,
	IDB_BITMAP_BG7,
	IDB_BITMAP_BG8,
	IDB_BITMAP_BG9,
	IDB_BITMAP_BG10};

CString g_strNameOfBmps[10]={
	_T("预置图片---[1]纸1"),
	_T("预置图片---[2]纸2"),
	_T("预置图片---[3]纸3"),
	_T("预置图片---[4]蓝色"),
	_T("预置图片---[5]绿色"),
	_T("预置图片---[6]热茶"),
	_T("预置图片---[7]落叶"),
	_T("预置图片---[8]紫色"),
	_T("预置图片---[9]木纹1"),
	_T("预置图片---[10]木纹2")};

CBlockSetting g_PresetBlockSettings[]={//块
	CBlockSetting(//灰色
	RGB(119,115,116),	//背景色
	RGB(94,83,88),		//标题背景色	
	RGB(182,175,177),	//标题颜色
	RGB(0,0,0),			//文字颜色
	RGB(93,93,93),		//鼠标经过
	RGB(172,172,172)	//鼠标单击
	),
	CBlockSetting(//蓝黑色
	RGB(19,6,72),		//背景色
	RGB(38,20,82),		//标题背景色
	RGB(105,44,250),	//标题颜色
	RGB(99,72,223),		//文字颜色
	RGB(82,38,208),		//鼠标经过
	RGB(107,83,247)		//鼠标单击
	),
	CBlockSetting(//草纸
	RGB(81,54,30),		//背景色
	RGB(101,73,31),		//标题背景色
	RGB(0,0,0),			//标题颜色
	RGB(0,0,0),			//文字颜色
	RGB(138,83,53),		//鼠标经过
	RGB(188,121,54)		//鼠标单击
	)
};
	
CReaderSetting g_PresetReaderSettings[]={//主窗口
	CReaderSetting(//灰色
	RGB(186,180,184),	//位置交换
	RGB(80,80,80),		//位置移动
	1,					//背景类型标识
	RGB(128,128,128),	//背景颜色
	_T("NULL"),			//背景图片路径
	IDB_BITMAP_BG1		//预置图片ID
	),
	CReaderSetting(//蓝黑色
	RGB(73,48,173),		//位置交换
	RGB(73,48,173),		//位置移动
	1,					//背景类型标识
	RGB(0,0,64),		//背景颜色
	_T("NULL"),			//背景图片路径
	IDB_BITMAP_BG1		//预置图片ID
	),
	CReaderSetting(//草纸
	RGB(149,100,70),	//位置交换
	RGB(149,100,70),	//位置移动
	3,					//背景类型标识
	RGB(255,255,255),	//背景颜色
	_T("NULL"),			//背景图片路径
	IDB_BITMAP_BG2)		//预置图片ID
};

CViewerSetting g_PresetViewerSettings[]={//阅读窗口//为了省事边框颜色=clrDiaCur
	CViewerSetting(//灰色
	_T("宋体"),			//字体
	10*20,				//大小
	0,					//行距
	RGB(0,0,0),			//文字颜色
	0,					//文字间距(保持为0)
	1,					//背景类型标识
	RGB(158,158,158),	//背景颜色
	_T("NULL"),			//背景图片路径
	IDB_BITMAP_BG1,		//预置图片ID
	RGB(50,50,50),		//状态栏边框颜色
	RGB(111,111,111),	//小方块默认颜色
	RGB(65,65,65)		//小方块当前颜色
	),
	CViewerSetting(//蓝黑色
	_T("宋体"),			//字体
	10*20,				//大小
	0,					//行距
	RGB(99,72,223),		//文字颜色
	0,					//文字间距(保持为0)
	1,					//背景类型标识
	RGB(19,6,72),		//背景颜色
	_T("NULL"),			//背景图片路径
	IDB_BITMAP_BG1,		//预置图片ID
	RGB(82,38,208),		//状态栏边框颜色
	RGB(107,83,247),	//小方块默认颜色
	RGB(82,38,208)		//小方块当前颜色
	),
	CViewerSetting(//草纸
	_T("宋体"),			//字体
	10*20,				//大小
	0,					//行距
	RGB(0,0,0),			//文字颜色
	0,					//文字间距(保持为0)
	3,					//背景类型标识
	RGB(255,255,255),	//背景颜色
	_T("NULL"),			//背景图片路径
	IDB_BITMAP_BG2,	//预置图片ID
	RGB(138,83,53),		//状态栏边框颜色
	RGB(176,114,57),	//小方块默认颜色
	RGB(138,83,53)		//小方块当前颜色
	)
};