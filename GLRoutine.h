#ifndef __GLROUTINE_H__
#define __GLROUTINE_H__

#include <windows.h>
#include "extgl.h"												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <windows.h>
#include "LoadTextures.h"
#include "FontRoutine.h"

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);			// Declaration For WndProc

GLvoid ReSizeGLScene(GLsizei, GLsizei);				// Resize And Initialize The GL Window
int InitGL(char *);									// file用来传递图象名
GLvoid KillGLWindow(GLvoid);										// Properly Kill The Window
BOOL CreateGLWindow(char *,char* , int , int , int , bool);

#endif