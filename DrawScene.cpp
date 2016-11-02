#include "DrawScene.h"

int DrawGLScene()												// ������texture�и�Ϊg_row��g_col�зֱ�����������
{
	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear The Screen And The Depth Buffer
	glLoadIdentity();											// Reset The Modelview Matrix

	glTranslatef(0.0f, 0.0f, -1.0f);
	int		gridw = 824/g_col;									// ����Ļ�ֳ�g_col*g_row�����ӣ�����ÿ�����ӵĳ���
	int		gridh = 768/g_row;
	g_length = int(gridw);										// lengthΪ������н�С��
	if (gridh < gridw)
	{
		g_length = gridh;
	}

	glPushMatrix();												// Push The Matrix
	glColor3f(0.85f, 0.85f, 0.85f);
	glEnable(GL_TEXTURE_RECTANGLE_NV);
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, texture[0]->texID);	// ( CHANGE )
	glBegin(GL_QUADS);											// Draw Our Quad

	int spin1 = 1;

	for (spin1 = 1; spin1 <= g_numframe; spin1++)			// ��ͼ����Ƶ���Ļ��
	{
		if (((2*spin1-3 == g_mouse_ldown || 2*spin1-2 == g_mouse_ldown) && spin1!=1)
			|| (g_mouse_ldown == 0 && spin1 == 1)
			|| (g_mouse_ldown == 2*g_numframe-1 && spin1 == 1)
			|| (g_mouse_ldown == 2*g_numframe))
		{
			glColor3f(1.0f,1.0f,1.0f);
		}
		else
			glColor3f(0.85f,0.85f,0.85f);
	
		int		iWidth  = texture[0]->width/g_col;
		int		iHeight = texture[0]->height/g_row;
		
		if( spin1%g_col!=0 )								// ���Ƴ������һ�������������
		{
				glTexCoord2i((spin1%g_col-1)*iWidth,		(spin1/g_col)*iHeight); 
				glVertex2i((spin1%g_col-1)*gridw,			(spin1/g_col+1)*gridh-g_length);
				glTexCoord2i((spin1%g_col)*iWidth,			(spin1/g_col)*iHeight);
				glVertex2i((spin1%g_col-1)*gridw+g_length,	(spin1/g_col+1)*gridh-g_length);
				glTexCoord2i((spin1%g_col)*iWidth,			(spin1/g_col+1)*iHeight); 
				glVertex2i((spin1%g_col-1)*gridw+g_length,	(spin1/g_col+1)*gridh);
				glTexCoord2i((spin1%g_col-1)*iWidth,		(spin1/g_col+1)*iHeight);
				glVertex2i((spin1%g_col-1)*gridw,			(spin1/g_col+1)*gridh);
				
				g_framecoord[spin1-1][0] = int((spin1%g_col-1)*gridw);
				g_framecoord[spin1-1][1] = int((spin1/g_col+1)*gridh);
	
		}
		else														// �������һ��ͼ��
		{
				glTexCoord2i((g_col-1)*iWidth,		(spin1/g_col-1)*iHeight);
				glVertex2i((g_col-1)*gridw,			(spin1/g_col)*gridh-g_length);
				glTexCoord2i(texture[0]->width,	(spin1/g_col-1)*iHeight);
				glVertex2i((g_col-1)*gridw+g_length,(spin1/g_col)*gridh-g_length);
				glTexCoord2i(texture[0]->width,	(spin1/g_col)*iHeight);
				glVertex2i((g_col-1)*gridw+g_length,(spin1/g_col)*gridh);
				glTexCoord2i((g_col-1)*iWidth,		(spin1/g_col)*iHeight);
				glVertex2i((g_col-1)*gridw,			(spin1/g_col)*gridh);
				
				g_framecoord[spin1-1][0] = int((g_col-1)*gridw);
				g_framecoord[spin1-1][1] = int((spin1/g_col)*gridh);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_RECTANGLE_NV);
	glEnable(GL_TEXTURE_2D);
	for ( spin1 = 1; spin1 <= g_numframe; spin1++)
	{																	// �������½ǵ�����
		glColor3f(1.0f,0.0f,0.0f);
	
		if( spin1%g_col!=0 )
		{
				
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindTexture(GL_TEXTURE_2D, texture[3]->texID);
			glPrint(int((spin1%g_col-1)*gridw+10), int((spin1/g_col+1)*gridh-35),0,"%d",spin1);
			glDisable(GL_BLEND);
		}
		else
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindTexture(GL_TEXTURE_2D, texture[3]->texID);
			glPrint(int((g_col-1)*gridw+15), int((spin1/g_col)*gridh-35),0,"%d",spin1);
			glDisable(GL_BLEND);

		}	
	}
	glPopMatrix();											// Pop The Matrix
	glEnable(GL_BLEND);	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, texture[2]->texID);			// ���Ʊ�����
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(824.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(884.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(884.0f, 668.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(824.0f, 668.0f);
	glEnd();												// Done Drawing The Quad
	glDisable(GL_BLEND);

	glColor3f(0.5,0.5,0.5);
	glLineWidth(3.0f);
	glBegin(GL_LINES);										// ��ʼ���Ƹ��ӿ����ܣ�
		glVertex2f(0.0f, 0.0f);
		glVertex2f(824.0f, 0.0f);
		glVertex2f(0.0f, 768.0f);
		glVertex2f(824.0f, 768.0f);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, 768.0f);
		glVertex2f(824.0f, 0.0f);
		glVertex2f(824.0f, 768.0f);
	glEnd();

	int temp = 0;
	for (temp = 0; temp < (g_row-1); temp++)			// ���Ƹ��ӿ��У�
	{
		glBegin(GL_LINES);
		glVertex2i(0, (temp+1)*gridh);
		glVertex2i(824, (temp+1)*gridh);
		glEnd();
	}
	for (temp = 0; temp < (g_col-1); temp++)				// ���Ƹ��ӿ��У�
	{
		glBegin(GL_LINES);
		glVertex2i((temp+1)*gridw, 0);
		glVertex2i((temp+1)*gridw, 768);
		glEnd();
	}

	if ( g_mouse_ldown < 2*g_numframe && g_mouse_ldown != 0 )		// �������ߺ�ѡ�е�
	{
		glPushMatrix();
		glTranslatef(0, 0, 1);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D,texture[1]->texID);
		int j = 0;
		for ( j = 0; j < g_mouse_ldown; j++)
		{
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f);
				glVertex2i(g_location[j][0]- 6, g_location[j][1]+ 6);
				glTexCoord2f(1.0f, 1.0f);
				glVertex2i(g_location[j][0]+ 6, g_location[j][1]+ 6);
				glTexCoord2f(1.0f, 0.0f);
				glVertex2i(g_location[j][0]+ 6, g_location[j][1]- 6);
				glTexCoord2f(0.0f, 0.0f);
				glVertex2i(g_location[j][0]- 6, g_location[j][1]- 6);
			glEnd();
		}
		glDisable(GL_BLEND);

		glColor3f(1.0f, 0.5f, 0.5f);
		glLineWidth(2.0f);

		for (j = 0; j+1 < g_mouse_ldown; j+=2)
		{
			glBegin(GL_LINES);
				glVertex2i(g_location[j][0], g_location[j][1]);
				glVertex2i(g_location[j+1][0], g_location[j+1][1]);
			glEnd();
		}
		glPopMatrix();

	}
	else if (g_mouse_ldown == 2*g_numframe)				// ѡ�񲻶������
	{
		DrawSaveMove();										// �����Ƿ񱣴�ͼ��

		glPushMatrix();
		glTranslatef(0, 0, 1);

		glColor3f(1.0f, 1.0f, 1.0f);					// ��ɫ��Ϊ��ɫ
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D,texture[1]->texID); // ������ѡ��Ĳ�����
		glColor3f(0.5f, 0.5f, 0.5f);
		int j = 0;
		for ( j = 0; j < 2*g_numframe; j++)
		{
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f);
				glVertex2i(g_location[j][0]-5, g_location[j][1]+ 5);
				glTexCoord2f(1.0f, 1.0f);
				glVertex2i(g_location[j][0]+5, g_location[j][1]+ 5);
				glTexCoord2f(1.0f, 0.0f);
				glVertex2i(g_location[j][0]+5, g_location[j][1]- 5);
				glTexCoord2f(0.0f, 0.0f);
				glVertex2i(g_location[j][0]-5, g_location[j][1]-5);
			glEnd();
		}
		glDisable(GL_BLEND);
		
		for (j = 0; j+1 < g_numframe*2; j+=2)			// ������������
		{
			glBegin(GL_LINES);
				glVertex2i(g_location[j][0], g_location[j][1]);
				glVertex2i(g_location[j+1][0], g_location[j+1][1]);
			glEnd();
		}
		glPopMatrix();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindTexture(GL_TEXTURE_2D, texture[3]->texID); // ��ӡ�����ʾ�ַ�

		glColor3f(0.0f, 0.0f, 1.0f);
		glPrint(0, 0, 1, "Ok! You have finished the animation's movement!");
		glDisable(GL_BLEND);

	}
		
	DrawTest();										// �����Ƿ���в���ͼ��
	DrawExit();										// �����˳���ť

	glPopMatrix();
	return TRUE;												// Keep Going
}
// ���Զ���Ч������ں���
int DrawTestAnim()
{
	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear The Screen And The Depth Buffer
	glColor3f(1.0f,1.0f,1.0f);
	DrawBackground(1024,768);
	DrawExit();
	DrawReturn();
	DrawSaveANM();
	if (g_play)	
		DrawAnimation();											// ����gifͼ��ĸ�֡����Ļ��
 	if (g_keypoint == 4 
		|| (g_keypoint == 0 && !g_selectWay))
	{
		DrawSelect_SaveWay();
	}
	if(g_keypoint == 4)
	{
		DrawBspline();
		DrawPlay_Stop();
		if (!g_selectWay && !g_play)
		{
			for (int i = 0; i < 4; i++)
			{
				if (g_selcKeypointPic[i])				// �������һ��ͼ�����࣬��������Ű�ť
				{
					DrawZoomIn();
					DrawZoomOut();
				}
			}
		}
	}
	if(g_keypoint != 0)
	{
		for (int i = 0; i < g_keypoint; i++)
		{
			if (!g_play)
			{
				DrawKeypointPic(i, g_scaleTemp[i]);
			}
			DrawKeypoint(i);
		}
	}
	if (!g_selectWay && g_keypoint == 0)										// ���ѡ�����·������Ը�������
	{
		DrawChangeBackground();
	}
	if (g_saveWay)											// ���ѡ���˴洢·����ť��ѵ�ǰ��ѡ��Ĺؼ������걣��
	{
		SaveWay();
		g_saveWay = FALSE;
		g_keypoint = 0;
		g_selectWay = FALSE;
		g_play = FALSE;
	}

	glPopMatrix();
	return TRUE;											// Keep Going
}
