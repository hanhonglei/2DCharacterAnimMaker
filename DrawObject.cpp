/////////////////////////////////////////////////////////////////

#include "DrawObject.h"

// �ڱ����ϻ��ƶ�������������·��ѡ��
void DrawAnimation()
{
	float     tempX[4];								// �ؼ���ĺ�����
	float     tempY[4];								// �ؼ����������
	int i = 0;
	for (i = 0; i < 4; i++)
	{
		tempX[i] = float(g_wayTemp[i][0]);
		tempY[i] = float(g_wayTemp[i][1]);
	}
	Spline  spline(tempX, tempY, 4);				// ����spline����
	spline.Generate();								// ����curve
	POINT	points[1000];							// �洢������ɵ���ɢ������
	int		pointcount = 0;
	spline.GetCurve(points, pointcount);			// ������ɢ��
	int		numpoints = spline.GetCurveCount();		// ��ɢ����Ŀ
	
	static float scale[1000];						// ������ɢ�㴦��ͼ�����
	for (i=0; i<3; i++)
	{
		int num = 0;
		int j = 0;
		while(points[j].x != tempX[i])// �������i���ؼ�������ɢ���е�λ��
			j++;
		while(points[j+num].x != tempX[i+1])
		{
			num++;					// �����ؼ���֮�����ɢ����Ŀ	
		}
		float scalPerPoint = (g_scaleTemp[i+1]-g_scaleTemp[i])/(num+1);
		for (int k=0; k <= num; k++)
		{
			scale[j+k] = g_scaleTemp[i]+scalPerPoint*k;// ���������ɢ��ı���
		}
	}

	bool	direction = true;						// ͼ����ڶ����еĳ����־��Ϊ��Ļ�˵����������ͱ�������ͬ�������෴
	static int point = 0;							// ָ��ԭ���ƶ�����

	if (g_headward == 'r' || g_headward == 'R')				// ��������������������ҵ�
	{
		if (g_wayTemp[0][0] > g_wayTemp[3][0])				// ʵ���˶�������
			direction = false;
	}
	if (g_headward == 'l' || g_headward == 'L')				// ���������������
	{
		if (g_wayTemp[0][0] < g_wayTemp[3][0])				// ʵ���˶�����
			direction = false;
	}
	
	double angle[1000];									// ÿһ����ͨ��������������ĽǶ�
	if (((g_headward == 'r' || g_headward == 'R') && direction)
		|| ((g_headward == 'l' || g_headward == 'L') && !direction))
	{
		for (i = 0; i < numpoints-1; i++)
		{
			angle[i]=atan2(double(points[i+1].y-points[i].y),double(points[i+1].x-points[i].x));
		}
		angle[i] = angle[i-1];
	}
	else
	{
		for (int i = numpoints-2; i >= 0; i--)
		{
			angle[i]=atan2(double(points[i].y-points[i+1].y),double(points[i].x-points[i+1].x));
		}
		angle[numpoints-1] = angle[numpoints-2];
	}
	
	if (first)												// ����ǳ�ʼ״̬��������ԭ���ƶ�����һ���ؼ�����
	{
		g_xtrans = points[0].x;
		g_ytrans = points[0].y;
		point    = 0;
		first = FALSE;
	}
	
	glPushMatrix();											// Push The Matrix
	glColor3f(1.0f, 1.0f, 1.0f);
	glLoadName(6);
	glLoadIdentity();
	glTranslated(g_xtrans, g_ytrans, 0);					// ��ԭ���ƶ�
// 	glRotated(angle[point]*180/3.14159265, 0.0, 0.0, 1.0);
	++ g_frame;												// �����Ƶĵڼ�֡
	if(g_frame > (g_numframe))								// ����ﵽ���֡����ѭ��
		g_frame = 1;
	
 	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_RECTANGLE_NV);
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, texture[0]->texID);			// ����nvidia�Կ������ԣ�non-power of 2)
	int		iWidth  = texture[0]->width/g_col;							// ÿһ֡�Ĵ�С
	int		iHeight = texture[0]->height/g_row;

	glBegin(GL_QUADS);                                     // Draw Our Quad
	if (direction)											// �������������Ǳ�����Ļ�
	{
		if( g_frame%g_col!=0 )
		{
			glTexCoord2i((g_frame%g_col-1)*iWidth,	(g_frame/g_col)*iHeight); 
			glVertex2i(-int(iWidth*scale[point]/2), -int(iHeight*scale[point]/2));
			glTexCoord2i((g_frame%g_col)*iWidth,	(g_frame/g_col)*iHeight);
			glVertex2i(int(iWidth*scale[point]/2), -int(iHeight*scale[point]/2));
			glTexCoord2i((g_frame%g_col)*iWidth,	(g_frame/g_col+1)*iHeight); 
			glVertex2i(int(iWidth*scale[point]/2), int(iHeight*scale[point]/2));
			glTexCoord2i((g_frame%g_col-1)*iWidth,	(g_frame/g_col+1)*iHeight);
			glVertex2i(-int(iWidth*scale[point]/2), int(iHeight*scale[point]/2));
		}
		else
		{
			glTexCoord2i((g_col-1)*iWidth,	(g_frame/g_col-1)*iHeight);
			glVertex2i(-int(iWidth*scale[point]/2),		-int(iHeight*scale[point]/2));
			glTexCoord2i(texture[0]->width,	(g_frame/g_col-1)*iHeight);
			glVertex2i(int(iWidth*scale[point]/2),		-int(iHeight*scale[point]/2));
			glTexCoord2i(texture[0]->width,	(g_frame/g_col)*iHeight);
			glVertex2i(int(iWidth*scale[point]/2),		int(iHeight*scale[point]/2));
			glTexCoord2i((g_col-1)*iWidth,	(g_frame/g_col)*iHeight);
			glVertex2i(-int(iWidth*scale[point]/2),		int(iHeight*scale[point]/2));
		}
	}
	else													// ���ʵ�ʳ����෴
	{
		if( g_frame%g_col!=0)
		{
			glTexCoord2i((g_frame%g_col)*iWidth,	(g_frame/g_col)*iHeight);
			glVertex2i(-int(iWidth*scale[point]/2), -int(iHeight*scale[point]/2));
			glTexCoord2i((g_frame%g_col-1)*iWidth,	(g_frame/g_col)*iHeight); 
			glVertex2i(int(iWidth*scale[point]/2), -int(iHeight*scale[point]/2));
			glTexCoord2i((g_frame%g_col-1)*iWidth,	(g_frame/g_col+1)*iHeight);
			glVertex2i(int(iWidth*scale[point]/2), int(iHeight*scale[point]/2));
			glTexCoord2i((g_frame%g_col)*iWidth,	(g_frame/g_col+1)*iHeight); 
			glVertex2i(-int(iWidth*scale[point]/2), int(iHeight*scale[point]/2));
		}
		else
		{
			glTexCoord2i(texture[0]->width,	(g_frame/g_col-1)*iHeight);
			glVertex2i(-int(iWidth*scale[point]/2),		-int(iHeight*scale[point]/2));
			glTexCoord2i((g_col-1)*iWidth,	(g_frame/g_col-1)*iHeight);
			glVertex2i(int(iWidth*scale[point]/2),		-int(iHeight*scale[point]/2));
			glTexCoord2i((g_col-1)*iWidth,	(g_frame/g_col)*iHeight);
			glVertex2i(int(iWidth*scale[point]/2),		int(iHeight*scale[point]/2));
			glTexCoord2i(texture[0]->width,	(g_frame/g_col)*iHeight);
			glVertex2i(-int(iWidth*scale[point]/2),		int(iHeight*scale[point]/2));
		}

	}
	glEnd();																// Done Drawing The Quad
	glPopMatrix();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_RECTANGLE_NV);										// ��ֹnon-power of two

	if (!g_moveObj)									// ���ѡ���˶������򲻲����ƶ�
	{
		if(direction)								// ͨ������ָ���Ĳ����㣬����֮֡����ƶ�����
		{// ����Ͷ�����������ͬ�������
			g_xtrans = int(points[point].x
				-((g_move[g_frame-1][0]*iWidth)/g_length)*cos(angle[point]));
			g_ytrans = int(points[point].y 
				+ abs((g_move[g_frame-1][0]*iWidth)/g_length)*sin(angle[point]));
			if (g_headward == 'r' || g_headward == 'R')
			{ // ������������ң���ͨ�����ұ����y�����ƫ��
				while (g_xtrans > points[point].x && point < numpoints)
					point++;
				if (g_xtrans > points[numpoints-1].x)
				{
					first = TRUE;
				}
				if (point >= numpoints)
					point = numpoints-1;
			}
			else
			{ // ���������������ͨ�����ұ����y�����ƫ��
				while (g_xtrans < points[point].x && point < numpoints)
					point++;
				if (g_xtrans < points[numpoints].x)
				{
					first = TRUE;
				}
				if (point >= numpoints)
					point = numpoints-1;
			}
		}
		else
		{// ����Ͷ����������෴�������
			g_xtrans = int(points[point].x
				+((g_move[g_frame-1][0]*iWidth)/g_length)*cos(angle[point]));
			g_ytrans = int(points[point].y 
				- abs((g_move[g_frame-1][0]*iWidth)/g_length)*sin(angle[point]));
			if (g_headward == 'r' || g_headward == 'R')
			{ // ������������ң���ͨ�����ұ����y�����ƫ��
				while (g_xtrans < points[point].x && point < numpoints)
					point++;
				if (g_xtrans < points[numpoints-1].x)
				{
					first = TRUE;
				}
				if (point >= numpoints)
					point = numpoints-1;
			}
			else 
			{ // ���������������ͨ�����ұ����y�����ƫ��
				while (g_xtrans > points[point].x && point < numpoints)
					point++;
				if (g_xtrans > points[numpoints-1].x)
				{
					first = TRUE;
				}
				if (point >= numpoints)
					point = numpoints-1;
			}
		}
	}
	Sleep(150);
}
// ���Ʊ���
void DrawBackground(GLsizei width, GLsizei height)				// ���뱳��ͼ��
{
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[16+g_numbackground]->texID);	
	glBegin(GL_QUADS);// ������ͼ��
			glTexCoord2f(0.0f,1.0f);
			glVertex2i(0,  0);
			glTexCoord2f(1.0f,1.0f);
			glVertex2i( width,  0);
			glTexCoord2f(1.0f,0.0f); 
			glVertex2i( width, height);
			glTexCoord2f(0.0f,0.0f); 
			glVertex2i(0, height);
	glEnd();
	glPopMatrix();
}
// ����ѡ��Ĺؼ���
void DrawKeypoint(int num)									// ���Ƶ�num���ؼ���
{
	glPushMatrix();											// Push The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
	glColor3f(1.0f, 1.0f, 1.0f);
	if (g_selcKeypoint[num])								// ���ѡ����һ���ؼ��㣬����ɫ�ı�
	{
		glColor3f(1.0f, 0.0f, 0.0f);
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadName(8+num);											// Assign Object A Name (ID)
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]->texID);
	glBegin(GL_QUADS);											// ������ѡ���Ĺؼ���������ƹؼ���
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(g_wayTemp[num][0]-8, g_wayTemp[num][1]-8);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i(g_wayTemp[num][0]+8, g_wayTemp[num][1]-8);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i(g_wayTemp[num][0]+8, g_wayTemp[num][1]+8);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(g_wayTemp[num][0]-8, g_wayTemp[num][1]+8);
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();											// Pop The Modelview Matrix
}
// �ڹؼ������ñ������ƶ���֡��num��ʾ�ڼ����ؼ��㴦����scale��ʾ���ŵı���
void DrawKeypointPic(int num,float scale)
{
	glPushMatrix();											// Push The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
	bool direction = true;
	if (g_selcKeypointPic[num])								// ���ѡ����һ���ؼ��㣬����ɫ�ı�
	{
		glColor3f(1.0f, 1.0f, 1.0f);
	}
	else
		glColor3f(0.7f, 0.7f, 0.7f);
	if (g_keypoint == 4)
	{
		if ((g_headward == 'R' || g_headward == 'r')
			&& g_wayTemp[0][0] > g_wayTemp[3][0])
		{
			direction = false;
		}
		if ((g_headward == 'L' || g_headward == 'l')
			&& g_wayTemp[0][0] < g_wayTemp[3][0])
		{
			direction = false;
		}
	}
	glLoadName(14+num);											// Assign Object A Name (ID)
	int iWidth = texture[0]->width/g_col;
	int iHeight= texture[0]->height/g_row;
	glEnable(GL_TEXTURE_RECTANGLE_NV);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, texture[0]->texID);
	glBegin(GL_QUADS);											// ������ѡ���Ĺؼ���������ƹؼ���
	if (direction)
	{
		glTexCoord2i(0,			0);								// ��һ�͵������ؼ��㴦������ͼ��
		glVertex2i(int(g_wayTemp[num][0]-iWidth*scale/2), int(g_wayTemp[num][1]-iHeight*scale/2));
		glTexCoord2i(iWidth,	0);
		glVertex2i(int(g_wayTemp[num][0]+iWidth*scale/2), int(g_wayTemp[num][1]-iHeight*scale/2));
		glTexCoord2i(iWidth,	iHeight);
		glVertex2i(int(g_wayTemp[num][0]+iWidth*scale/2), int(g_wayTemp[num][1]+iHeight*scale/2));
		glTexCoord2i(0,			iHeight);
		glVertex2i(int(g_wayTemp[num][0]-iWidth*scale/2), int(g_wayTemp[num][1]+iHeight*scale/2));
	}
	else
	{
		glTexCoord2i(iWidth,	0);
		glVertex2i(int(g_wayTemp[num][0]-iWidth*scale/2), int(g_wayTemp[num][1]-iHeight*scale/2));
		glTexCoord2i(0,			0);								// ��һ�͵������ؼ��㴦������ͼ��
		glVertex2i(int(g_wayTemp[num][0]+iWidth*scale/2), int(g_wayTemp[num][1]-iHeight*scale/2));
		glTexCoord2i(0,			iHeight);
		glVertex2i(int(g_wayTemp[num][0]+iWidth*scale/2), int(g_wayTemp[num][1]+iHeight*scale/2));
		glTexCoord2i(iWidth,	iHeight);
		glVertex2i(int(g_wayTemp[num][0]-iWidth*scale/2), int(g_wayTemp[num][1]+iHeight*scale/2));

	}
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_RECTANGLE_NV);
	glPopMatrix();											// Pop The Modelview Matrix
}
// ���ùؼ������bspline����
void DrawBspline()
{
	float x[4];
	float y[4];
	int i = 0;
	for ( i = 0; i < g_keypoint; i++)
	{
		x[i] = float(g_wayTemp[i][0]);
		y[i] = float(g_wayTemp[i][1]);
	}
	Spline spline(x, y, 4);
	spline.Generate();
	POINT point[1000];
	int pointcount = 0;
	spline.GetCurve(point, pointcount);
	glColor3f(0.6f, 0.6f, 0.6f);

	glBegin(GL_LINE_STRIP);
	for (i = 0; i < spline.GetCurveCount(); i++)
	{
		glVertex2i(point[i].x, point[i].y);
	}
	glEnd();
}

