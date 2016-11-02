#include "GLRoutine.h"

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)				// Resize And Initialize The GL Window
{
	if (height==0)												// Prevent A Divide By Zero By
	{
		height=1;												// Making Height Equal One
	}

	glViewport(0, 0, width, height);							// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);								// Select The Projection Matrix
	glLoadIdentity();											// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	glOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f);					// Create Ortho 640x480 View (0,0 At Top Left)
	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
}

int InitGL(char *file)											// file��������ͼ����
{
	if (!LoadGLTextures(file))									// Jump To Texture Loading Routine ( NEW )
	{
		return FALSE;											// If Texture Didn't Load Return FALSE
	}
	glEnable(GL_TEXTURE_2D);									// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);									// Enable Smooth Shading
	glClearColor(0.5f, 0.5f, 1.0f, 0.5f);						// Black Background
	glClearDepth(1.0f);											// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);						// Set Line Antialiasing
	BuildFont();
	
	return TRUE;												// Initialization Went OK
}

GLvoid KillGLWindow(GLvoid)										// Properly Kill The Window
{
	PlaySound("Data/exit.wav", NULL,SND_ASYNC);
	if (fullscreen)												// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL, 0);							// If So Switch Back To The Desktop
		ShowCursor(TRUE);										// Show Mouse Pointer
	}

	if (hRC)													// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))							// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))								// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;												// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))							// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC = NULL;												// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))							// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;												// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))					// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;											// Set hInstance To NULL
	}
	KillFont();
	if (texture[0]->imageData)
	{
		free(texture[0]->imageData);
		texture[0]->imageData = NULL;
	}
	for (int i = 0; i < NUMBER; i++)
	{
		delete texture[i];
		texture[i] = NULL;
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:
 *  file            - ����winmain��������ͼ�������     					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char *file,char* title, int width, int height, int bits, bool fullscreenflag)
{
	// ��texture���г�ʼ�� [11/23/2012 Han]
	//texture = new *Texture[NUMBER];
	for (int i = 0; i < NUMBER; i++)
	{
		texture[i] = new Texture;
	}
	//for (int i = 0; i < NUMBER; i++)
	//{
	//	texture[i] = new Texture;
	//}

	GLuint		PixelFormat;									// Holds The Results After Searching For A Match
	WNDCLASS	wc;												// Windows Class Structure
	DWORD		dwExStyle;										// Window Extended Style
	DWORD		dwStyle;										// Window Style
	RECT		WindowRect;										// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;									// Set Left Value To 0
	WindowRect.right=(long)width;								// Set Right Value To Requested Width
	WindowRect.top=(long)0;										// Set Top Value To 0
	WindowRect.bottom=(long)height;								// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;
	// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
//	fullscreen=false;											//��Զ�Ǵ��ڷ�ʽ��ʾ
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;								// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(TRUE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();											// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=							// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),							// Size Of This Pixel Format Descriptor
		1,														// Version Number
		PFD_DRAW_TO_WINDOW |									// Format Must Support Window
		PFD_SUPPORT_OPENGL |									// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,										// Must Support Double Buffering
		PFD_TYPE_RGBA,											// Request An RGBA Format
		bits,													// Select Our Color Depth
		0, 0, 0, 0, 0, 0,										// Color Bits Ignored
		0,														// No Alpha Buffer
		0,														// Shift Bit Ignored
		0,														// No Accumulation Buffer
		0, 0, 0, 0,												// Accumulation Bits Ignored
		16,														// 16Bit Z-Buffer (Depth Buffer)  
		0,														// No Stencil Buffer
		0,														// No Auxiliary Buffer
		PFD_MAIN_PLANE,											// Main Drawing Layer
		0,														// Reserved
		0, 0, 0													// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))										// Did We Get A Device Context?
	{
		KillGLWindow();											// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))				// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();											// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))					// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();											// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))							// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();											// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))								// Try To Activate The Rendering Context
	{
		KillGLWindow();											// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);									// Show The Window
	SetForegroundWindow(hWnd);									// Slightly Higher Priority
	SetFocus(hWnd);												// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);								// Set Up Our Perspective GL Screen

	if (!InitGL(file))												// Initialize Our Newly Created GL Window
	{
		KillGLWindow();											// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	return TRUE;												// Success
}