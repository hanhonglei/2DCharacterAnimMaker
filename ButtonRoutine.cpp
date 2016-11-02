/////////////////////////////////////////////////////////////////

#include "ButtonRoutine.h"

// 利用anmformat.h里面给定的格式存储anm文件
bool SaveANM(Texture TextureFile, const long row, const long column)
{
	FILE	*ANMfile = fopen("anmtest.anm", "wb");				// 新建一个anm文件
	ANMheader anmheader;
	strcpy(anmheader.cToken, "anm");							// 填充文件头
	anmheader.cHeadward		= g_headward;						// 图形的朝向
	anmheader.iBackgroundNum= MAXBACKGROUNDNUM;					// 背景的数目
	anmheader.iNumFrame		= (long)g_numframe ;				// 帧数
	anmheader.iRow			= (long)row;						// 行数
	anmheader.iColumn		= (long)column;						// 列数
	anmheader.iWidth		= long(TextureFile.width);			// 纹理宽度	
	anmheader.iHeight		= long(TextureFile.height);			// 纹理高度	
	int i = 0;
	for (i=0; i<g_numframe; i++)
	{
		anmheader.fMotion[i][0] = float(g_move[i][0])/float(g_length);// 着地点的相对移动比例
		anmheader.fMotion[i][1] = float(g_move[i][0])/float(g_length);
	}															// 把文件头写入
	if(!fwrite(&anmheader, sizeof(anmheader), 1, ANMfile))
		return FALSE;
																// 把图像数据文件写入
	if(!fwrite(TextureFile.imageData, 
		sizeof(GLubyte)*4*(TextureFile.width)*(TextureFile.height), 1, ANMfile))
		return FALSE;
	for(i = 0; i < anmheader.iBackgroundNum; i++)				// 将各个背景图像的路径数据等写入文件
	{
		if(!fwrite(g_backgroundname[i], sizeof(char)*7, 1,ANMfile))// 背景名称，6个字节
			return FALSE;
		if(!fwrite(&g_numway[i], sizeof(int), 1, ANMfile))		// 此背景下的路径数目
			return FALSE;
		for(int j=0; j < g_numway[i]; j++)
		{
			if(!fwrite(g_way[i][j], sizeof(int)*8, 1, ANMfile))	// 路径的关键点
				return FALSE;
			if(!fwrite(g_scale[i][j], sizeof(float)*4, 1, ANMfile))// 关键点处的图像比例
				return FALSE;
		}
	}
	
// debug 
//char sTemp[100];
//char cTemp;
//int	 iTemp;
//int  iTemp1;
//fseek(ANMfile, 0L, SEEK_SET);
//fscanf(ANMfile, "%c", &sTemp[0]);
//fscanf(ANMfile, "%c", &sTemp[1]);
//fscanf(ANMfile, "%c", &sTemp[2]);
//fscanf(ANMfile, "%c", &sTemp[3]);
//fscanf(ANMfile, "%c", &cTemp);
//fscanf(ANMfile, "%i", &iTemp);
//fscanf(ANMfile, "%l", &iTemp1);
//fscanf(ANMfile, "%l", &anmheader.iRow);
//fscanf(ANMfile, "%l", &anmheader.iColumn);
//fscanf(ANMfile, "%l", &anmheader.iWidth);
//fscanf(ANMfile, "%l", &anmheader.iHeight);
//for (i = 0; i < 256; i++)
//{
//	fscanf(ANMfile, "%l", &anmheader.fMotion[i][0]);
//	fscanf(ANMfile, "%l", &anmheader.fMotion[i][1]);
//}
//fread(TextureFile.imageData,
//	sizeof(GLubyte)*4*(TextureFile.width)*(TextureFile.height), 1, ANMfile);
//fscanf(ANMfile, "%s", g_backgroundname[49]);
//fscanf(ANMfile, "%d", &g_numway[48]);
//fscanf(ANMfile, "%d", &g_numway[49]);
//
//fclose(ANMfile);
//end debug

	fclose(ANMfile);
	return TRUE;
}

// 计算动作的协调。第一个参数表示所选取的不动点坐标，第二个表示每一帧左下角坐标
void CountMove(const int locate[100][2], const int coord[100][2])
{
	FILE	*fp = fopen("move.txt", "w");
	int		 temp[100][2];		// 相对每一帧左下角的移动坐标
	temp[0][0] = locate[0][0] - coord[0][0];
	temp[0][1] = locate[0][1] - coord[0][1];
	int i = 1;
	int j = 1;
	for (j = 1; j < g_numframe; j++)
	{
		temp[i][0] = locate[i][0] - coord[j][0];
		temp[i][1] = locate[i][1] - coord[j][1];
		i++;
		temp[i][0] = locate[i][0] - coord[j][0];
		temp[i][1] = locate[i][1] - coord[j][1];
		i++;
	}

	temp[i][0] = locate[i][0] - coord[0][0];
	temp[i][1] = locate[i][1] - coord[0][1];

	j = 0;
	for ( i = 0; i < g_numframe; i++, j+=2)
	{
		g_move[i][0] = temp[j+1][0] - temp[j][0];		// 计算移动的像素值，是相对于g_length的
		g_move[i][1] = temp[j+1][1] - temp[j][1];
	}
	
	for (j = 0; j < g_numframe; j++)					 // 移动路径的浮点数表示
	{
		fprintf(fp, "%f   %f\n",g_move[j][0]/float(g_length),g_move[j][1]/float(g_length));
	}
	fclose(fp);

	fp = fopen("moveint.txt", "w");
	for (j = 0; j < g_numframe; j++)					// 移动路径的整型表示（像素级）
	{
		fprintf(fp, "%d   %d\n", g_move[j][0], g_move[j][1]);
	}
	fclose(fp);
}
// 存储所选择的路径函数
void SaveWay()
{
	int i = 0;
	for (i = 0; i < g_keypoint; i++)
	{
		g_way[g_numbackground][(g_numway[g_numbackground])][i][0] = g_wayTemp[i][0];
		g_way[g_numbackground][(g_numway[g_numbackground])][i][1] = g_wayTemp[i][0];
		g_scale[g_numbackground][(g_numway[g_numbackground])][i] = g_scaleTemp[i];
	}
	g_numway[g_numbackground]++;
	for (i = 0;i < 4; i++)									// 将所有的临时变量置初值
	{
		g_selcKeypointPic[i]	= FALSE;
		g_scaleTemp[i]		= 1.0f;	
		g_selcKeypoint[i] = FALSE;
		g_keypoint = 0;
	}
}