#include "SelectObject.h"

// select模式下进行是否选择检验
void Selection(void)
{
	GLuint	buffer[512];										// 设置一个选择缓冲区
	GLint	hits = -1;											// 我们命中的目标数
	// 视口大小. [0]: <x>, [1]: <y>, [2]: <宽>, [3]: <高>
	GLint	viewport[4];

	// 取得视口大小
	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(512, buffer);								// 通知OpenGL使用该数组作为选择缓冲区

	// Puts OpenGL In Selection Mode. Nothing Will Be Drawn.  Object ID's and Extents Are Stored In The Buffer.
	glRenderMode(GL_SELECT);

	glInitNames();												// Initializes The Name Stack
	glPushName(0);												// Push 0 (At Least One Entry) Onto The Stack

	glMatrixMode(GL_PROJECTION);								// Selects The Projection Matrix
	glPushMatrix();												// Push The Projection Matrix
	glLoadIdentity();											// Resets The Matrix

	// This Creates A Matrix That Will Zoom Up To A Small Portion Of The Screen, Where The Mouse Is.
	gluPickMatrix(g_mouse_x, (viewport[3]-g_mouse_y), 1.0, 1.0, viewport);

	// Apply The Perspective Matrix
	glOrtho(0.0f, 1024, 768, 0.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
	glPushMatrix();												// Push The Projection Matrix
	
	if( g_test == false )
	{
		if( g_mouse_ldown == 2*g_numframe )
		{
			DrawSaveMove();											// 绘制保存不动点按钮
		}
		DrawTest();
	}

	DrawExit();

	if( g_test == true )
	{
		if (g_keypoint == 4 && !g_selectWay)			// 如果选择完关键点，则绘制路径
		{
			for (int i = 0; i < 4; i++)
			{
				DrawKeypoint(i);						// 绘制关键点
				if(!g_play)
				{
					if (g_selcKeypointPic[i])			// 如果有一个图像被选种进行缩放，则进行放大或者缩小选择
					{
						DrawZoomIn();
						DrawZoomOut();
					}
					DrawKeypointPic(i,g_scaleTemp[i]);	// 绘制关键点上的图像
				}
			}
		}
		if (g_keypoint == 4 || (g_keypoint == 0 && !g_selectWay))// 绘制关键点按钮和保存关键点按钮应用的是一个name
		{
			DrawAnimation();								// 绘制动画
			DrawSelect_SaveWay();						// 绘制选择关键点按钮或者保存关键点按钮
		}
		if (g_keypoint == 4)							// 绘制开始或者停止测试按钮
		{
			DrawPlay_Stop();
		}
		if (!g_selectWay && g_keypoint == 0)
		{
			DrawChangeBackground();
		}
		DrawReturn();									// 绘制返回按钮
		DrawSaveANM();									// 绘制存储anm文件按钮
	}

	glMatrixMode(GL_PROJECTION);								// Select The Projection Matrix
	glPopMatrix();												// Pop The Projection Matrix
	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
	glPopMatrix();												// Pop The Projection Matrix

	hits = glRenderMode(GL_RENDER);								// Switch To Render Mode, Find Out How Many
																// Objects Were Drawn Where The Mouse Was
	if (hits > 0)												// If There Were More Than 0 Hits
	{
		if (buffer[3] == 7)										// 选择关键点或者选择存储关键点
		{
			if (g_keypoint == 0 && !g_selectWay)	
				g_selectWay	= TRUE;								// 开始选择关键点
			else if (g_keypoint == 4)	
				g_saveWay = TRUE;								// 存储关键点
		}

		for (GLuint i = 0; i < GLuint(g_keypoint); i++)	
		{
			if (buffer[3] == 8+i)								// 选择哪个关键点
			{
				g_selcKeypoint[i] = !g_selcKeypoint[i];
			}
			if (buffer[3] == 14+i)							// 选择那个关键点上的图像调整大小，其中第一个和最后一个点没有图像
			{
				for (int j=0;j<4;j++)
				{
					g_selcKeypointPic[j] = FALSE;			// 将其他的图像设定为不选择状态
				}
				g_selcKeypointPic[i] = TRUE;
			}
		}
		g_saveMove		= (buffer[3] == 1);						// 选择保存不动点按钮
		if (buffer[3] == 2)
		{
			g_test		= TRUE;									// 选择测试动画按钮
		}
		if (buffer[3] == 3)
		{
			g_test		= FALSE;								// 选择返回按钮
		}
		g_exited		= (buffer[3] == 4);						// 选择退出按钮
		g_saveANM		= (buffer[3] == 5);						// 选择保存anm文件按钮
		if (buffer[3] == 6)										// 点击被测试的动画并移动或者松开选择的动画，则移动到鼠标的位置
		{
			g_moveObj	= !g_moveObj;
		}
		if (buffer[3] == 18)									// 选择放大按钮
		{
			for (int i = 0; i < 4; i++)							// 将被选种的图放大
			{
				if (g_selcKeypointPic[i])
				{
					g_scaleTemp[i] += 0.01f;
				}
			}
		}
		if (buffer[3] == 19)									// 选择缩小按钮
		{
			for (int i = 0; i < 4; i++)							// 将被选种的图缩小
			{
				if (g_selcKeypointPic[i])
				{
					g_scaleTemp[i] -= 0.01f;
					if (g_scaleTemp[i] <= 0.0f)
						g_scaleTemp[i] = 0.01f;
				}
			}
		}
		if (buffer[3] == 20)
		{
			g_play = !g_play;
		}
		if (buffer[3] == 21)
		{
			g_numbackground++;					// 如果按下更换背景按钮，则按顺序调入背景
			if (g_numbackground >= MAXBACKGROUNDNUM)	// 如果达到最大的背景数目，则循环到第一个
				g_numbackground = 0;
		}

		PlaySound("Data/click.WAV", NULL, SND_ASYNC);
	}
}
// select模式下进行是否鼠标停留检验
void HoldOn()
{
	GLuint	buffer[512];										// 设置一个选择缓冲区
	GLint	hits = -1;											// 我们命中的目标数
	// 视口大小. [0]: <x>, [1]: <y>, [2]: <宽>, [3]: <高>
	GLint	viewport[4];

	// 取得视口大小
	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(512, buffer);								// 通知OpenGL使用该数组作为选择缓冲区

	// Puts OpenGL In Selection Mode. Nothing Will Be Drawn.  Object ID's and Extents Are Stored In The Buffer.
	glRenderMode(GL_SELECT);

	glInitNames();												// Initializes The Name Stack
	glPushName(0);												// Push 0 (At Least One Entry) Onto The Stack

	glMatrixMode(GL_PROJECTION);								// Selects The Projection Matrix
	glPushMatrix();												// Push The Projection Matrix
	glLoadIdentity();											// Resets The Matrix

	// This Creates A Matrix That Will Zoom Up To A Small Portion Of The Screen, Where The Mouse Is.
	gluPickMatrix(g_mouse_x, (viewport[3]-g_mouse_y), 1.0, 1.0, viewport);

	// Apply The Perspective Matrix
	glOrtho(0.0f,1024, 768, 0.0f,-1.0f,1.0f);					// 用正交投影

	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
	glPushMatrix();												// Push The Projection Matrix
	
	if( g_test == false )
	{
		if (g_mouse_ldown == 2*g_numframe)
		{
			DrawSaveMove();												// Render The Targets To The Selection Buffer
		}

		DrawTest();
	}

	DrawExit();

	if( g_test == true)
	{
		DrawReturn();
		DrawSaveANM();
		if (g_keypoint == 4 || (g_keypoint == 0 && !g_selectWay))
		{
			DrawSelect_SaveWay();
		}
		if (g_keypoint == 4)
		{
			DrawPlay_Stop();
		}
		for (int i = 0; i < 4; i++)
		{
			if (g_selcKeypointPic[i])								// 如果有一个图像被选种进行缩放，则进行放大或者缩小选择
			{
				DrawZoomIn();
				DrawZoomOut();
			}
		}
		if (!g_selectWay && g_keypoint == 0)
			DrawChangeBackground();
	}

	glMatrixMode(GL_PROJECTION);								// Select The Projection Matrix
	glPopMatrix();												// Pop The Projection Matrix
	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
	glPopMatrix();												// Pop The Projection Matrix

	hits=glRenderMode(GL_RENDER);								// Switch To Render Mode, Find Out How Many
	if (hits>0)													// If There Were More Than 0 Hits
	{
		g_hold_save			= (buffer[3] == 1);					// 鼠标停留到保存位移按钮上
		g_hold_test			= (buffer[3] == 2);					// 鼠标停留到测试动画按钮上	
		g_hold_return		= (buffer[3] == 3);					// 鼠标停留到返回按钮上
		g_hold_exited		= (buffer[3] == 4);					// 鼠标停留到退出按钮上
		g_hold_saveANM		= (buffer[3] == 5);					// 鼠标停留到保存anm按钮上
		g_hold_selectWay	= (buffer[3] == 7);					// 鼠标停留到选择路径按钮上
		g_hold_zoomIn		= (buffer[3] == 18);				// 鼠标停留到放大按钮上
		g_hold_zoomOut		= (buffer[3] == 19);				// 鼠标停留到缩小按钮上
		g_hold_play_stop	= (buffer[3] == 20);				// 鼠标停留到开始或者停止沿路径测试按钮上
		g_hold_changback    = (buffer[3] == 21);
	}
	else
	{
		g_hold_saveANM 
			= g_hold_return 
			= g_hold_test 
			= g_hold_save 
			= g_hold_exited 
			= g_hold_selectWay
			= g_hold_zoomIn
			= g_hold_zoomOut
			= g_hold_play_stop
			= g_hold_changback
			= FALSE;
	}
}
