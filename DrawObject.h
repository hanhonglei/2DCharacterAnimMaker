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

void DrawBackground(GLsizei, GLsizei);							// 利用给定大小绘制背景
void DrawKeypoint(int);											// 绘制第n个关键点
void DrawKeypointPic(int, float);								// 绘制关键点上的动画预览
void DrawAnimation();											// 绘制动画动作
void DrawBspline();												// 利用关键点绘制之间的bspline线

#endif		