#include "SelectObject.h"

// selectģʽ�½����Ƿ�ѡ�����
void Selection(void)
{
	GLuint	buffer[512];										// ����һ��ѡ�񻺳���
	GLint	hits = -1;											// �������е�Ŀ����
	// �ӿڴ�С. [0]: <x>, [1]: <y>, [2]: <��>, [3]: <��>
	GLint	viewport[4];

	// ȡ���ӿڴ�С
	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(512, buffer);								// ֪ͨOpenGLʹ�ø�������Ϊѡ�񻺳���

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
			DrawSaveMove();											// ���Ʊ��治���㰴ť
		}
		DrawTest();
	}

	DrawExit();

	if( g_test == true )
	{
		if (g_keypoint == 4 && !g_selectWay)			// ���ѡ����ؼ��㣬�����·��
		{
			for (int i = 0; i < 4; i++)
			{
				DrawKeypoint(i);						// ���ƹؼ���
				if(!g_play)
				{
					if (g_selcKeypointPic[i])			// �����һ��ͼ��ѡ�ֽ������ţ�����зŴ������Сѡ��
					{
						DrawZoomIn();
						DrawZoomOut();
					}
					DrawKeypointPic(i,g_scaleTemp[i]);	// ���ƹؼ����ϵ�ͼ��
				}
			}
		}
		if (g_keypoint == 4 || (g_keypoint == 0 && !g_selectWay))// ���ƹؼ��㰴ť�ͱ���ؼ��㰴ťӦ�õ���һ��name
		{
			DrawAnimation();								// ���ƶ���
			DrawSelect_SaveWay();						// ����ѡ��ؼ��㰴ť���߱���ؼ��㰴ť
		}
		if (g_keypoint == 4)							// ���ƿ�ʼ����ֹͣ���԰�ť
		{
			DrawPlay_Stop();
		}
		if (!g_selectWay && g_keypoint == 0)
		{
			DrawChangeBackground();
		}
		DrawReturn();									// ���Ʒ��ذ�ť
		DrawSaveANM();									// ���ƴ洢anm�ļ���ť
	}

	glMatrixMode(GL_PROJECTION);								// Select The Projection Matrix
	glPopMatrix();												// Pop The Projection Matrix
	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
	glPopMatrix();												// Pop The Projection Matrix

	hits = glRenderMode(GL_RENDER);								// Switch To Render Mode, Find Out How Many
																// Objects Were Drawn Where The Mouse Was
	if (hits > 0)												// If There Were More Than 0 Hits
	{
		if (buffer[3] == 7)										// ѡ��ؼ������ѡ��洢�ؼ���
		{
			if (g_keypoint == 0 && !g_selectWay)	
				g_selectWay	= TRUE;								// ��ʼѡ��ؼ���
			else if (g_keypoint == 4)	
				g_saveWay = TRUE;								// �洢�ؼ���
		}

		for (GLuint i = 0; i < GLuint(g_keypoint); i++)	
		{
			if (buffer[3] == 8+i)								// ѡ���ĸ��ؼ���
			{
				g_selcKeypoint[i] = !g_selcKeypoint[i];
			}
			if (buffer[3] == 14+i)							// ѡ���Ǹ��ؼ����ϵ�ͼ�������С�����е�һ�������һ����û��ͼ��
			{
				for (int j=0;j<4;j++)
				{
					g_selcKeypointPic[j] = FALSE;			// ��������ͼ���趨Ϊ��ѡ��״̬
				}
				g_selcKeypointPic[i] = TRUE;
			}
		}
		g_saveMove		= (buffer[3] == 1);						// ѡ�񱣴治���㰴ť
		if (buffer[3] == 2)
		{
			g_test		= TRUE;									// ѡ����Զ�����ť
		}
		if (buffer[3] == 3)
		{
			g_test		= FALSE;								// ѡ�񷵻ذ�ť
		}
		g_exited		= (buffer[3] == 4);						// ѡ���˳���ť
		g_saveANM		= (buffer[3] == 5);						// ѡ�񱣴�anm�ļ���ť
		if (buffer[3] == 6)										// ��������ԵĶ������ƶ������ɿ�ѡ��Ķ��������ƶ�������λ��
		{
			g_moveObj	= !g_moveObj;
		}
		if (buffer[3] == 18)									// ѡ��Ŵ�ť
		{
			for (int i = 0; i < 4; i++)							// ����ѡ�ֵ�ͼ�Ŵ�
			{
				if (g_selcKeypointPic[i])
				{
					g_scaleTemp[i] += 0.01f;
				}
			}
		}
		if (buffer[3] == 19)									// ѡ����С��ť
		{
			for (int i = 0; i < 4; i++)							// ����ѡ�ֵ�ͼ��С
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
			g_numbackground++;					// ������¸���������ť����˳����뱳��
			if (g_numbackground >= MAXBACKGROUNDNUM)	// ����ﵽ���ı�����Ŀ����ѭ������һ��
				g_numbackground = 0;
		}

		PlaySound("Data/click.WAV", NULL, SND_ASYNC);
	}
}
// selectģʽ�½����Ƿ����ͣ������
void HoldOn()
{
	GLuint	buffer[512];										// ����һ��ѡ�񻺳���
	GLint	hits = -1;											// �������е�Ŀ����
	// �ӿڴ�С. [0]: <x>, [1]: <y>, [2]: <��>, [3]: <��>
	GLint	viewport[4];

	// ȡ���ӿڴ�С
	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(512, buffer);								// ֪ͨOpenGLʹ�ø�������Ϊѡ�񻺳���

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
	glOrtho(0.0f,1024, 768, 0.0f,-1.0f,1.0f);					// ������ͶӰ

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
			if (g_selcKeypointPic[i])								// �����һ��ͼ��ѡ�ֽ������ţ�����зŴ������Сѡ��
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
		g_hold_save			= (buffer[3] == 1);					// ���ͣ��������λ�ư�ť��
		g_hold_test			= (buffer[3] == 2);					// ���ͣ�������Զ�����ť��	
		g_hold_return		= (buffer[3] == 3);					// ���ͣ�������ذ�ť��
		g_hold_exited		= (buffer[3] == 4);					// ���ͣ�����˳���ť��
		g_hold_saveANM		= (buffer[3] == 5);					// ���ͣ��������anm��ť��
		g_hold_selectWay	= (buffer[3] == 7);					// ���ͣ����ѡ��·����ť��
		g_hold_zoomIn		= (buffer[3] == 18);				// ���ͣ�����Ŵ�ť��
		g_hold_zoomOut		= (buffer[3] == 19);				// ���ͣ������С��ť��
		g_hold_play_stop	= (buffer[3] == 20);				// ���ͣ������ʼ����ֹͣ��·�����԰�ť��
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
