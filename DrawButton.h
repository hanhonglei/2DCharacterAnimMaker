////////////////////////////////////////////////////////// 
#ifndef __DRAWBUTTON_H__
#define __DRAWBUTTON_H__

#include "extgl.h"												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "Texture.h"											// Header File Containing Our Texture Structure ( NEW )
#include "GlobalVariables.h"
#include "ButtonRoutine.h"

void DrawExit();										// 绘制退出按钮
void DrawSaveANM();										// 绘制保存anm文件按钮	
void DrawSaveMove();									// 绘制保存不动点按钮	
void DrawReturn();										// 绘制返回按钮
void DrawSelect_SaveWay();								// 绘制保存所选择路径按钮
void DrawZoomIn();										// 绘制放大按钮
void DrawZoomOut();										// 绘制缩小按钮
void DrawTest();										// 绘制测试动画按钮
void DrawPlay_Stop();									// 绘制开始或者停止按路径测试动画按钮
void DrawChangeBackground();							// 绘制更换背景按钮
#endif