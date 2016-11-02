/////////////////////////////////////////////////////////////////
#ifndef __FONTROUTINE_H__
#define __FONTROUTINE_H__

#include "extgl.h"												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "Texture.h"											// Header File Containing Our Texture Structure ( NEW )
#include "GlobalVariables.h"

GLvoid BuildFont(GLvoid);											// Build Our Font Display List
GLvoid KillFont(GLvoid);											// Delete The Font From Memory
GLvoid glPrint(GLint x, GLint y, int set, const char *fmt, ...);	// Where The Printing Happens

#endif