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

void CountRowCol(int, int, int, int &, int &);				// 通过所计算出的gif帧数来确定最后纹理的大小和行列数
void SmoothBound(Texture, C_Image *);						// 平滑gif图像的边界（未使用）
void Gif_TexInit(char *);									// 将gif图像初始化为texture
int LoadGLTextures(char *);									// 用来载入图象，file传递图象名

#endif
