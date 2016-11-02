/////////////////////////////////////////////////////////////////

#include "DrawObject.h"

// 在背景上绘制动画，并最后进行路径选择
void DrawAnimation()
{
	float     tempX[4];								// 关键点的横坐标
	float     tempY[4];								// 关键点的纵坐标
	int i = 0;
	for (i = 0; i < 4; i++)
	{
		tempX[i] = float(g_wayTemp[i][0]);
		tempY[i] = float(g_wayTemp[i][1]);
	}
	Spline  spline(tempX, tempY, 4);				// 创建spline对象
	spline.Generate();								// 生成curve
	POINT	points[1000];							// 存储最后生成的离散点坐标
	int		pointcount = 0;
	spline.GetCurve(points, pointcount);			// 生成离散点
	int		numpoints = spline.GetCurveCount();		// 离散点数目
	
	static float scale[1000];						// 各个离散点处的图像比例
	for (i=0; i<3; i++)
	{
		int num = 0;
		int j = 0;
		while(points[j].x != tempX[i])// 计算出第i个关键点在离散点中的位置
			j++;
		while(points[j+num].x != tempX[i+1])
		{
			num++;					// 两个关键点之间的离散点数目	
		}
		float scalPerPoint = (g_scaleTemp[i+1]-g_scaleTemp[i])/(num+1);
		for (int k=0; k <= num; k++)
		{
			scale[j+k] = g_scaleTemp[i]+scalPerPoint*k;// 计算各个离散点的比例
		}
	}

	bool	direction = true;						// 图像的在动画中的朝向标志，为真的话说明动画朝向和本身朝向相同，否则相反
	static int point = 0;							// 指定原点移动到的

	if (g_headward == 'r' || g_headward == 'R')				// 如果本来动画朝向是向右的
	{
		if (g_wayTemp[0][0] > g_wayTemp[3][0])				// 实际运动是向左
			direction = false;
	}
	if (g_headward == 'l' || g_headward == 'L')				// 如果本来朝向向左
	{
		if (g_wayTemp[0][0] < g_wayTemp[3][0])				// 实际运动向右
			direction = false;
	}
	
	double angle[1000];									// 每一个点通过切线所计算出的角度
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
	
	if (first)												// 如果是初始状态，则将坐标原点移动到第一个关键点上
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
	glTranslated(g_xtrans, g_ytrans, 0);					// 将原点移动
// 	glRotated(angle[point]*180/3.14159265, 0.0, 0.0, 1.0);
	++ g_frame;												// 所绘制的第几帧
	if(g_frame > (g_numframe))								// 如果达到最大帧数则循环
		g_frame = 1;
	
 	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_RECTANGLE_NV);
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, texture[0]->texID);			// 利用nvidia显卡的特性（non-power of 2)
	int		iWidth  = texture[0]->width/g_col;							// 每一帧的大小
	int		iHeight = texture[0]->height/g_row;

	glBegin(GL_QUADS);                                     // Draw Our Quad
	if (direction)											// 动画朝向本来就是本身朝向的话
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
	else													// 如果实际朝向相反
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
	glDisable(GL_TEXTURE_RECTANGLE_NV);										// 禁止non-power of two

	if (!g_moveObj)									// 如果选择了动画，则不产生移动
	{
		if(direction)								// 通过交互指定的不动点，计算帧之间的移动距离
		{// 朝向和动画本身朝向相同的情况下
			g_xtrans = int(points[point].x
				-((g_move[g_frame-1][0]*iWidth)/g_length)*cos(angle[point]));
			g_ytrans = int(points[point].y 
				+ abs((g_move[g_frame-1][0]*iWidth)/g_length)*sin(angle[point]));
			if (g_headward == 'r' || g_headward == 'R')
			{ // 如果朝向本来向右，则通过查找表计算y方向的偏移
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
			{ // 如果朝向本来向左，则通过查找表计算y方向的偏移
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
		{// 朝向和动画本身朝向相反的情况下
			g_xtrans = int(points[point].x
				+((g_move[g_frame-1][0]*iWidth)/g_length)*cos(angle[point]));
			g_ytrans = int(points[point].y 
				- abs((g_move[g_frame-1][0]*iWidth)/g_length)*sin(angle[point]));
			if (g_headward == 'r' || g_headward == 'R')
			{ // 如果朝向本来向右，则通过查找表计算y方向的偏移
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
			{ // 如果朝向本来向左，则通过查找表计算y方向的偏移
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
// 绘制背景
void DrawBackground(GLsizei width, GLsizei height)				// 载入背景图像
{
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[16+g_numbackground]->texID);	
	glBegin(GL_QUADS);// 画背景图像
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
// 绘制选择的关键点
void DrawKeypoint(int num)									// 绘制第num个关键点
{
	glPushMatrix();											// Push The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
	glColor3f(1.0f, 1.0f, 1.0f);
	if (g_selcKeypoint[num])								// 如果选定了一个关键点，则颜色改变
	{
		glColor3f(1.0f, 0.0f, 0.0f);
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadName(8+num);											// Assign Object A Name (ID)
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]->texID);
	glBegin(GL_QUADS);											// 利用所选定的关键点坐标绘制关键点
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
// 在关键处利用比例绘制动画帧，num表示第几个关键点处，而scale表示缩放的比例
void DrawKeypointPic(int num,float scale)
{
	glPushMatrix();											// Push The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
	bool direction = true;
	if (g_selcKeypointPic[num])								// 如果选定了一个关键点，则颜色改变
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
	glBegin(GL_QUADS);											// 利用所选定的关键点坐标绘制关键点
	if (direction)
	{
		glTexCoord2i(0,			0);								// 第一和第六个关键点处不产生图像
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
		glTexCoord2i(0,			0);								// 第一和第六个关键点处不产生图像
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
// 利用关键点绘制bspline曲线
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

