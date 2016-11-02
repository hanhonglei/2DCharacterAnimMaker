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
// *    �洢����ͼ�����                                       *
// ****************************************************************************
class C_Image {
public:
	// ����ĳ�Ա:
	int		Width, Height;			// ֡�Ŀ�͸�
	int		BPP;					// һ�����ص��ֽ���	
	unsigned char	* Raster;				// ��ɫ����
	COLOR	* Palette;				// ��ɫ�б�
	int		BytesPerRow;			// ÿһ�еĳ���
	int		Transparent;			// ͸����ɫ������-1��ʾû��͸��ɫ��
	// �����ĸ��ӳ�Ա:
	int		xPos, yPos;				// ����ƶ�
	int		Delay;					// һ֮֡����ӳ٣���λ��1/1000��.
	int		Transparency;			// ����͸��.
	// ����ͼ���豸�ӿ�ϸ��:
	BITMAPINFO * pbmi;				// λͼ��Ϣ�ṹ

	// ���캯������������:
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

	// �������=������
	C_Image& operator= (C_Image& rhs);

	// ͼ�γ�ʼ����������ɫ���к�������
	void Init (int iWidth, int iHeight, int iBPP);

	inline unsigned char& Pixel (const int& x, const int& y)
	{
		return Raster[y*BytesPerRow+x];
	}
};

// ****************************************************************************
// * C_ImageSet                                                               *
// *    �洢ͼ�����õ���                                     *
// ****************************************************************************
class C_ImageSet 
{
public:
	int		FrameWidth, FrameHeight;	// ͼ�����õĴ�С.
	int		nLoops;						// ��·�� (0 = ����)

	C_Image	** img;						// ͼ������.
	int		nImages;					// ͼ�ε���Ŀ (������С)

	void AddImage (C_Image*);			// ��ͼ�������������ͼ��
	
	// ���캯������������:
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

	// �ļ���ʽ:
	int LoadGIF (char* szFile);
	int SaveGIF (char* szFile) {return 0;};	// û��ʵ��
};
#endif
