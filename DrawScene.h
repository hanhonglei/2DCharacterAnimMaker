
#ifndef __DRAWSCENE_H__
#define __DRAWSCENE_H__

#include <windows.h>											// Header File For Windows
#include "extgl.h"												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "AnmFormat.h"
#include "DrawButton.h"
#include "GlobalVariables.h"
#include "DrawObject.h"
#include "ButtonRoutine.h"
#include "FontRoutine.h"

int DrawGLScene();												// 用来将texture切割为g_row行g_col列分别贴到窗口里
int DrawTestAnim();												// 测试画面
#endif