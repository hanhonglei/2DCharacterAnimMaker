////////////////////////////////////////////////////////// 
#ifndef __DRAWBUTTON_H__
#define __DRAWBUTTON_H__

#include "extgl.h"												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "Texture.h"											// Header File Containing Our Texture Structure ( NEW )
#include "GlobalVariables.h"
#include "ButtonRoutine.h"

void DrawExit();										// �����˳���ť
void DrawSaveANM();										// ���Ʊ���anm�ļ���ť	
void DrawSaveMove();									// ���Ʊ��治���㰴ť	
void DrawReturn();										// ���Ʒ��ذ�ť
void DrawSelect_SaveWay();								// ���Ʊ�����ѡ��·����ť
void DrawZoomIn();										// ���ƷŴ�ť
void DrawZoomOut();										// ������С��ť
void DrawTest();										// ���Ʋ��Զ�����ť
void DrawPlay_Stop();									// ���ƿ�ʼ����ֹͣ��·�����Զ�����ť
void DrawChangeBackground();							// ���Ƹ���������ť
#endif