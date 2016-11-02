// ****************************************************************************
//
// WINIMAGE.H : Generic classes for raster images (MSWindows specialization)
//
//  Content: Class declarations of:
//  - class C_Image             : Storage class for single images
//  - class C_ImageSet          : Storage class for sets of images
//  - class C_AnimationWindow   : Window Class to display animations
//
//  (Includes declarations of routines to Load and Save BMP files and to load
// GIF files into these classes).
//
//  --------------------------------------------------------------------------
//
// Copyright ?2000, Juan Soulie <jsoulie@cplusplus.com>
//
// Permission to use, copy, modify, distribute and sell this software or any
// part thereof and/or its documentation for any purpose is granted without fee
// provided that the above copyright notice and this permission notice appear
// in all copies.
//
// This software is provided "as is" without express or implied warranty of
// any kind. The author shall have no liability with respect to the
// infringement of copyrights or patents that any modification to the content
// of this file or this file itself may incur.
//
// ****************************************************************************
#ifndef __WINIMAGE_H__
#define __WINIMAGE_H__
#ifdef _WIN32
#pragma pack(1)
#endif

// Windows specific types and constants:
#include <windows.h>
struct COLOR 
{
	unsigned char b,g,r,x;
};	// Windows GDI expects 4bytes per color
#define ALIGN sizeof(int)				// Windows GDI expects all int-aligned

// ****************************************************************************
// * C_Image                                                                  *
// *    存储单个图像的类                                       *
// ****************************************************************************
class C_Image {
public:
	// 常规的成员:
	int		Width, Height;			// 帧的宽和高
	int		BPP;					// 一个像素的字节数	
	unsigned char	* Raster;				// 颜色序列
	COLOR	* Palette;				// 颜色列表
	int		BytesPerRow;			// 每一行的长度
	int		Transparent;			// 透明颜色索引（-1表示没有透明色）
	// 动画的附加成员:
	int		xPos, yPos;				// 相对移动
	int		Delay;					// 一帧之后的延迟，单位是1/1000秒.
	int		Transparency;			// 动画透明.
	// 窗口图形设备接口细节:
	BITMAPINFO * pbmi;				// 位图信息结构

	// 构造函数和析构函数:
	C_Image() 
	{ 
		Raster = 0; 
		Palette = 0; 
		pbmi = 0;
	}
	~C_Image() 
	{ 
		delete[]pbmi;
		delete[]Raster; 
	}

	// 运算符“=”重载
	C_Image& operator= (C_Image& rhs);

	// 图形初始化（分配颜色序列和索引）
	void Init (int iWidth, int iHeight, int iBPP);

	inline unsigned char& Pixel (const int& x, const int& y)
	{
		return Raster[y*BytesPerRow+x];
	}
};

// ****************************************************************************
// * C_ImageSet                                                               *
// *    存储图像设置的类                                     *
// ****************************************************************************
class C_ImageSet 
{
public:
	int		FrameWidth, FrameHeight;	// 图形设置的大小.
	int		nLoops;						// 环路数 (0 = 无限)

	C_Image	** img;						// 图形向量.
	int		nImages;					// 图形的数目 (向量大小)

	void AddImage (C_Image*);			// 在图像向量后面添加图形
	
	// 构造函数和析构函数:
	C_ImageSet() 
	{
		img		= 0;
		nImages	= 0;
		nLoops	= 0;
	}
	~C_ImageSet() 
	{
		for (int n = 0; n < nImages; n++) 
			delete img[n];
		delete[] img;
	}

	// 文件格式:
	int LoadGIF (char* szFile);
	int SaveGIF (char* szFile) {return 0;};	// 没有实现
};
#endif
