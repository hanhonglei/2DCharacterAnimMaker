/////////////////////////////////////////////////////////////////
#ifndef __LOADTEXTURES_H__
#define __LOADTEXTURES_H__

#include <windows.h>											// Header File For Windows
#include "extgl.h"												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <math.h>
#include "Texture.h"											// Header File Containing Our Texture Structure ( NEW )
#include "GlobalVariables.h"
#include "winimage.h"

bool LoadTGA(Texture *, char *);								// Function Prototype For LoadTGA ( NEW )

void CountRowCol(int, int, int, int &, int &);				// ͨ�����������gif֡����ȷ���������Ĵ�С��������
void SmoothBound(Texture, C_Image *);						// ƽ��gifͼ��ı߽磨δʹ�ã�
void Gif_TexInit(char *);									// ��gifͼ���ʼ��Ϊtexture
int LoadGLTextures(char *);									// ��������ͼ��file����ͼ����

#endif
