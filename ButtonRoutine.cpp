/////////////////////////////////////////////////////////////////

#include "ButtonRoutine.h"

// ����anmformat.h��������ĸ�ʽ�洢anm�ļ�
bool SaveANM(Texture TextureFile, const long row, const long column)
{
	FILE	*ANMfile = fopen("anmtest.anm", "wb");				// �½�һ��anm�ļ�
	ANMheader anmheader;
	strcpy(anmheader.cToken, "anm");							// ����ļ�ͷ
	anmheader.cHeadward		= g_headward;						// ͼ�εĳ���
	anmheader.iBackgroundNum= MAXBACKGROUNDNUM;					// ��������Ŀ
	anmheader.iNumFrame		= (long)g_numframe ;				// ֡��
	anmheader.iRow			= (long)row;						// ����
	anmheader.iColumn		= (long)column;						// ����
	anmheader.iWidth		= long(TextureFile.width);			// ������	
	anmheader.iHeight		= long(TextureFile.height);			// ����߶�	
	int i = 0;
	for (i=0; i<g_numframe; i++)
	{
		anmheader.fMotion[i][0] = float(g_move[i][0])/float(g_length);// �ŵص������ƶ�����
		anmheader.fMotion[i][1] = float(g_move[i][0])/float(g_length);
	}															// ���ļ�ͷд��
	if(!fwrite(&anmheader, sizeof(anmheader), 1, ANMfile))
		return FALSE;
																// ��ͼ�������ļ�д��
	if(!fwrite(TextureFile.imageData, 
		sizeof(GLubyte)*4*(TextureFile.width)*(TextureFile.height), 1, ANMfile))
		return FALSE;
	for(i = 0; i < anmheader.iBackgroundNum; i++)				// ����������ͼ���·�����ݵ�д���ļ�
	{
		if(!fwrite(g_backgroundname[i], sizeof(char)*7, 1,ANMfile))// �������ƣ�6���ֽ�
			return FALSE;
		if(!fwrite(&g_numway[i], sizeof(int), 1, ANMfile))		// �˱����µ�·����Ŀ
			return FALSE;
		for(int j=0; j < g_numway[i]; j++)
		{
			if(!fwrite(g_way[i][j], sizeof(int)*8, 1, ANMfile))	// ·���Ĺؼ���
				return FALSE;
			if(!fwrite(g_scale[i][j], sizeof(float)*4, 1, ANMfile))// �ؼ��㴦��ͼ�����
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

// ���㶯����Э������һ��������ʾ��ѡȡ�Ĳ��������꣬�ڶ�����ʾÿһ֡���½�����
void CountMove(const int locate[100][2], const int coord[100][2])
{
	FILE	*fp = fopen("move.txt", "w");
	int		 temp[100][2];		// ���ÿһ֡���½ǵ��ƶ�����
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
		g_move[i][0] = temp[j+1][0] - temp[j][0];		// �����ƶ�������ֵ���������g_length��
		g_move[i][1] = temp[j+1][1] - temp[j][1];
	}
	
	for (j = 0; j < g_numframe; j++)					 // �ƶ�·���ĸ�������ʾ
	{
		fprintf(fp, "%f   %f\n",g_move[j][0]/float(g_length),g_move[j][1]/float(g_length));
	}
	fclose(fp);

	fp = fopen("moveint.txt", "w");
	for (j = 0; j < g_numframe; j++)					// �ƶ�·�������ͱ�ʾ�����ؼ���
	{
		fprintf(fp, "%d   %d\n", g_move[j][0], g_move[j][1]);
	}
	fclose(fp);
}
// �洢��ѡ���·������
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
	for (i = 0;i < 4; i++)									// �����е���ʱ�����ó�ֵ
	{
		g_selcKeypointPic[i]	= FALSE;
		g_scaleTemp[i]		= 1.0f;	
		g_selcKeypoint[i] = FALSE;
		g_keypoint = 0;
	}
}