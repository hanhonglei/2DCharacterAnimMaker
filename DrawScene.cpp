#include "DrawScene.h"

int DrawGLScene()												// 用来将texture切割为g_row行g_col列分别贴到窗口里
{
	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear The Screen And The Depth Buffer
	glLoadIdentity();											// Reset The Modelview Matrix

	glTranslatef(0.0f, 0.0f, -1.0f);
	int		gridw = 824/g_col;									// 把屏幕分成g_col*g_row个格子，计算每个格子的长宽
	int		gridh = 768/g_row;
	g_length = int(gridw);										// length为长或高中较小的
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

	for (spin1 = 1; spin1 <= g_numframe; spin1++)			// 将图像绘制到屏幕上
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
		
		if( spin1%g_col!=0 )								// 绘制除了最后一列以外的所有列
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
		else														// 绘制最后一列图像
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
	{																	// 绘制左下角的数字
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
	glBindTexture(GL_TEXTURE_2D, texture[2]->texID);			// 绘制标题栏
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
	glBegin(GL_LINES);										// 开始绘制格子框（四周）
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
	for (temp = 0; temp < (g_row-1); temp++)			// 绘制格子框（行）
	{
		glBegin(GL_LINES);
		glVertex2i(0, (temp+1)*gridh);
		glVertex2i(824, (temp+1)*gridh);
		glEnd();
	}
	for (temp = 0; temp < (g_col-1); temp++)				// 绘制格子框（列）
	{
		glBegin(GL_LINES);
		glVertex2i((temp+1)*gridw, 0);
		glVertex2i((temp+1)*gridw, 768);
		glEnd();
	}

	if ( g_mouse_ldown < 2*g_numframe && g_mouse_ldown != 0 )		// 绘制连线和选中点
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
	else if (g_mouse_ldown == 2*g_numframe)				// 选择不动点完成
	{
		DrawSaveMove();										// 绘制是否保存图标

		glPushMatrix();
		glTranslatef(0, 0, 1);

		glColor3f(1.0f, 1.0f, 1.0f);					// 颜色变为白色
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D,texture[1]->texID); // 绘制所选择的不动点
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
		
		for (j = 0; j+1 < g_numframe*2; j+=2)			// 继续绘制连线
		{
			glBegin(GL_LINES);
				glVertex2i(g_location[j][0], g_location[j][1]);
				glVertex2i(g_location[j+1][0], g_location[j+1][1]);
			glEnd();
		}
		glPopMatrix();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindTexture(GL_TEXTURE_2D, texture[3]->texID); // 打印完成提示字符

		glColor3f(0.0f, 0.0f, 1.0f);
		glPrint(0, 0, 1, "Ok! You have finished the animation's movement!");
		glDisable(GL_BLEND);

	}
		
	DrawTest();										// 绘制是否进行测试图标
	DrawExit();										// 绘制退出按钮

	glPopMatrix();
	return TRUE;												// Keep Going
}
// 测试动画效果的入口函数
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
		DrawAnimation();											// 绘制gif图像的各帧到屏幕上
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
				if (g_selcKeypointPic[i])				// 如果任意一个图像被旋踵，则出现缩放按钮
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
	if (!g_selectWay && g_keypoint == 0)										// 如果选择完毕路径则可以更换背景
	{
		DrawChangeBackground();
	}
	if (g_saveWay)											// 如果选择了存储路径按钮则把当前所选择的关键点坐标保存
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
