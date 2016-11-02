/////////////////////////////////////////////////////////////////
#ifndef _DRAWOBJECT_H__
#define _DRAWOBJECT_H__

#include "DrawButton.h"
#include "extgl.h"												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "Texture.h"											// Header File Containing Our Texture Structure ( NEW )
#include "GlobalVariables.h"
#include <math.h>
#include "spline.h"
#include <stdlib.h>

void DrawBackground(GLsizei, GLsizei);							// ���ø�����С���Ʊ���
void DrawKeypoint(int);											// ���Ƶ�n���ؼ���
void DrawKeypointPic(int, float);								// ���ƹؼ����ϵĶ���Ԥ��
void DrawAnimation();											// ���ƶ�������
void DrawBspline();												// ���ùؼ������֮���bspline��

#endif		