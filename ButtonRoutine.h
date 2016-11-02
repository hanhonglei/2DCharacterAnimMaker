/////////////////////////////////////////////////////////////////

#ifndef __BUTTONROUTINE_H__
#define __BUTTONROUTINE_H__

#include "extgl.h"												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "Texture.h"											// Header File Containing Our Texture Structure ( NEW )
#include "GlobalVariables.h"
#include "AnmFormat.h"

bool SaveANM(Texture, const long, const long);				// ����anm�ļ�			
void CountMove(const int [100][2], const int [100][2]);
void SaveWay();												// ����·��

#endif