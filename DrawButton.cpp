////////////////////////////////////////////////////////////////
#include "DrawButton.h"
// ���Ʊ��治���㰴ť
void DrawSaveMove()
{
	glPushMatrix();											// Push The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
	if (g_hold_save)
	{
		glColor3f(1.0f,1.0f,1.0f);
	}
	else
		glColor3f(0.5f,0.5f,0.5f);
	glLoadName(1);											// Assign Object A Name (ID)
	glTranslatef(0,0,-1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, texture[4]->texID);
	if (g_saveMove)									// If Object Has Been Hit
	{
		CountMove(g_location, g_framecoord);
		g_saveMove = FALSE;
	}
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(884, 708);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i(1024, 708);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i(1024, 768);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(884, 768);
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();											// Pop The Modelview Matrix
}
// ���Ʋ��Զ�����ť
void DrawTest()
{
	glPushMatrix();											// Push The Modelview Matrix
	glLoadIdentity();										// Reset The Modelview Matrix
	if (g_hold_test)
	{
		glColor3f(1.0f,1.0f,1.0f);
	}
	else
	{
		glColor3f(0.5f,0.5f,0.5f);
	}
	glLoadName(2);											// Assign Object A Name (ID)
	glTranslatef(0, 0, -1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]->texID);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(884, 648);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i(1024, 648);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i(1024, 708);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(884, 708);
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();											// Pop The Modelview Matrix
}
// ���Ʒ��ذ�ť
void DrawReturn()
{
	glPushMatrix();											// Push The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
	if (g_hold_return)
	{
		glColor3f(1.0f,1.0f,1.0f);
	}
	else
		glColor3f(0.5f,0.5f,0.5f);
	glLoadName(3);											// Assign Object A Name (ID)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[6]->texID);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(884, 20);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i(1024, 20);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i(1024, 80);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(884, 80);
	glEnd();
	glDisable(GL_BLEND);

	glPopMatrix();											// Pop The Modelview Matrix
}// �����˳���ť
void DrawExit()
{
	glPushMatrix();											// Push The Modelview Matrix
	if (g_hold_exited)
	{
		glColor3f(1.0f,1.0f,1.0f);
	}
	else
	{
		glColor3f(0.5f,0.5f,0.5f);
	}
	glLoadIdentity();										// Reset The Modelview Matrix
	glLoadName(4);											// Assign Object A Name (ID)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[7]->texID);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(884, 80);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i(1024, 80);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i(1024, 140);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(884, 140);
	glEnd();
	glDisable(GL_BLEND);

	glPopMatrix();											// Pop The Modelview Matrix
}
// ���Ʊ���anm�ļ���ť
void DrawSaveANM()
{
	glPushMatrix();											// Push The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
	if (g_hold_saveANM)
	{
		glColor3f(1.0f,1.0f,1.0f);
	}
	else
		glColor3f(0.5f,0.5f,0.5f);
	glLoadName(5);											// Assign Object A Name (ID)
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, texture[8]->texID);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(884, 140);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i(1024, 140);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i(1024, 200);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(884, 200);
	glEnd();
	if (g_saveANM)
	{
		SaveANM(*texture[0], g_row, g_col);
		g_saveANM = FALSE;									// ��������Ժ��ô洢��־Ϊfalse
	}
	glDisable(GL_BLEND);

	glPopMatrix();											// Pop The Modelview Matrix
}
// ����ѡ��·����ť
void DrawSelect_SaveWay()
{
	glPushMatrix();											// Push The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
	if (g_hold_selectWay)
	{
		glColor3f(1.0f,1.0f,1.0f);
	}
	else
		glColor3f(0.5f,0.5f,0.5f);
	glLoadName(7);											// Assign Object A Name (ID)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	if (g_keypoint == 0 && !g_selectWay)
	{
		glBindTexture(GL_TEXTURE_2D, texture[9]->texID);
	}
	else if (g_keypoint == 4)
		glBindTexture(GL_TEXTURE_2D, texture[11]->texID);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(884, 200);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i(1024, 200);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i(1024, 260);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(884, 260);
	glEnd();
	glDisable(GL_BLEND);

	glPopMatrix();											// Pop The Modelview Matrix
}
// ���ƷŴ�ͼ��ť������nmu��ʾ�ڼ���ͼ��ѡ�н�������
void DrawZoomIn()
{
	glPushMatrix();											// Push The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
	if (g_hold_zoomIn)
	{
		glColor3f(1.0f,1.0f,1.0f);
	}
	else
		glColor3f(0.5f,0.5f,0.5f);
	glLoadName(18);											// Assign Object A Name (ID)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[12]->texID);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(884, 260);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i(954, 260);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i(950, 320);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(884, 320);
	glEnd();
	glDisable(GL_BLEND);

	glPopMatrix();											// Pop The Modelview Matrix
}
// ������Сͼ��ť������nmu��ʾ�ڼ���ͼ��ѡ�н�������
void DrawZoomOut()
{
	glPushMatrix();											// Push The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
	if (g_hold_zoomOut)
	{
		glColor3f(1.0f,1.0f,1.0f);
	}
	else
		glColor3f(0.5f,0.5f,0.5f);
	glLoadName(19);											// Assign Object A Name (ID)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[13]->texID);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(954, 260);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i(1024, 260);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i(1024, 320);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(954, 320);
	glEnd();
	glDisable(GL_BLEND);

	glPopMatrix();											// Pop The Modelview Matrix
}
// �����Ƿ���·�����Զ�����ť��play����stop��
void DrawPlay_Stop()
{
	glPushMatrix();											// Push The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
	if (g_hold_play_stop)									// ����ƶ�����ť�ϱ�
	{
		glColor3f(1.0f,1.0f,1.0f);
	}
	else
		glColor3f(0.5f,0.5f,0.5f);
	glLoadName(20);											// Assign Object A Name (ID)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	if (!g_play)											// �����ֹͣ���Զ�����·��Ч����
	{
		glBindTexture(GL_TEXTURE_2D, texture[14]->texID);	// ���ƿ�ʼ���԰�ť
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture[15]->texID);	// �������ֹͣ���԰�ť
	}
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(884, 320);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i(1024, 320);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i(1024, 380);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(884, 380);
	glEnd();
	
	glDisable(GL_BLEND);
	glPopMatrix();											// Pop The Modelview Matrix
}
// ���Ƹ���������ť
void DrawChangeBackground()
{
	glPushMatrix();											// Push The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
	if (g_hold_changback)									// ����ƶ�����ť�ϱ�
	{
		glColor3f(1.0f,1.0f,1.0f);
	}
	else
		glColor3f(0.5f,0.5f,0.5f);
	glLoadName(21);											// Assign Object A Name (ID)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[10]->texID);		// �������ֹͣ���԰�ť
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(884, 380);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i(1024, 380);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i(1024, 440);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(884, 440);
	glEnd();
	
	glDisable(GL_BLEND);
	glPopMatrix();											// Pop The Modelview Matrix
}// loadnameΪ21