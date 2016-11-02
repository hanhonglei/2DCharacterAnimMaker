/////////////////////////////////////////////////////////////////

#ifndef _SELECTOBJECT_H__
#define _SELECTOBJECT_H__

#include <windows.h>											// Header File For Windows
#include "extgl.h"												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "DrawButton.h"
#include "Texture.h"											// Header File Containing Our Texture Structure ( NEW )
#include "GlobalVariables.h"
#include "DrawObject.h"

void Selection();												// 鼠标按下所进行的选择
void HoldOn();													// 鼠标移动到上面进行的选择

#endif