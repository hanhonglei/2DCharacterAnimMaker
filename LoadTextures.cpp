#include "LoadTextures.h"

void CountRowCol(int num, int height, int width, int &row, int &col)
{																// ͨ�����������ݼ���������������������������������
	int temp = int(sqrt(double(num)));							// ֻ��3�����
	if (num == temp*temp)										// ���÷ֽ�Ϊ���Ŀ��������
		row = col = temp;
	
	else if (num == temp*(temp+1) || num < temp*(temp+1))		// ����һ�ֱȽϺõ����
	{
 		if (height > width)										// ͨ������Ƚϣ������ӽ�������
		{
			row = temp;
			col = temp+1;
		}
		else
		{
			row = temp+1;
			col = temp;
		}
	}
	
	else														// ����˷ѿռ����
		row = col = temp+1;
}

void SmoothBound(Texture tex, C_Image *image)					// ƽ��gifͼ��ı߽磨δʹ�ã�
{
	bool flag = TRUE;
	double grayTrans = 0.3*image->Palette[image->Transparent].r
		+ 0.11*image->Palette[image->Transparent].g
		+ 0.59*image->Palette[image->Transparent].b;
	for (unsigned int i = 0; i < tex.width*tex.height*4; i+=4)
	{
		double gray = 0.3*tex.imageData [i]+0.11*tex.imageData[i+1]+0.59*tex.imageData[i+2];
		if((gray-grayTrans)<10 && (gray-grayTrans)>-10)
		{
			tex.imageData[i]=255;
			tex.imageData[i+1]=0;
			tex.imageData[i+2]=0;
			tex.imageData[i+3]=0;
		}
	}
}

void Gif_TexInit(char *file)									// ��gifͼ���ʼ��Ϊtexture
{
	C_ImageSet	imageset;
	imageset.LoadGIF( file );									// ����loadgif������֡�����ݵõ�

	texture[0]->bpp			= 32;								// һ����32λ��
	g_numframe				= imageset.nImages;
	texture[0]->height		= imageset.img[0]->Height;
	texture[0]->width		= imageset.img[0]->Width;

	int i = 0;
	
	for (i = 1; i < g_numframe; i++)		// ȡ����gif֡����Ƚϴ����Ϊtexture����Ļ���
	{
		if(GLubyte(imageset.img[i]->Height) > (texture[0]->height))
			texture[0]->height = imageset.img[i]->Height;
		if(GLubyte(imageset.img[i]->Width)  > (texture[0]->width))
			texture[0]->width  = imageset.img[i]->Width;
	}
	
	CountRowCol(g_numframe,texture[0]->height,texture[0]->width,g_row,g_col);	// ��ʼ��texture����������
	
	texture[0]->height		= texture[0]->height*g_row;			// ����ĸ�
	texture[0]->width		= texture[0]->width*g_col;			// ����Ŀ�
	texture[0]->type			= GL_RGBA;
	texture[0]->imageData	= (GLubyte *)malloc(sizeof(GLubyte)*texture[0]->width*texture[0]->height*4);// �����������ɫ�ռ�
	
	for (i = 0; (unsigned int)i < texture[0]->width*texture[0]->height*4; i++)						// ��ʼ��texture[0]��imageData
	{
		if ((i+1)%4==0)
			texture[0]->imageData[i] = 0;
		else
			texture[0]->imageData[i] = 255;
	}
	
	int current_imgdata;										// texture�ĵڼ�������
	for (int frame = 0; frame < g_numframe; frame++)			// �ֱ�ÿ��gif֡��rgb��䵽texture����
	{
		int n = 0;												// �к�
		int left_up=(frame/g_col)*texture[0]->width*(texture[0]->height/g_row)*4  // ÿһ֡���Ͻ���imageData�ı��
			+(frame%g_col)*(texture[0]->width/g_col)*4;
		current_imgdata = left_up;
		for( int pixel_gif = 0
			; pixel_gif < (imageset.img[frame]->BytesPerRow)*(imageset.img[frame]->Height)
			; pixel_gif++ )	// ѭ����������gif֡		
		{
			int exact_imgdata = current_imgdata + imageset.img[frame]->yPos*texture[0]->width*4
				+ imageset.img[frame]->xPos*4;// ����ƫ�ƺ��imageDataֵ

			if (pixel_gif<n*imageset.img[frame]->BytesPerRow+imageset.img[frame]->Width)		
			{																// ������ALIGN�Ĵ��ڣ����ԣ���pixel_gifͻ��width�Ժ���ĩ�Ͳ����
				if (imageset.img[frame]->Raster[pixel_gif]==imageset.img[frame]->Transparent)
				{
					texture[0]->imageData[exact_imgdata]		= 255;
					texture[0]->imageData[exact_imgdata+1]	= 255;	
					texture[0]->imageData[exact_imgdata+2]	= 255;
					texture[0]->imageData[exact_imgdata+3]	= 0;
				}
				else
				{
					texture[0]->imageData[exact_imgdata]	
						=imageset.img[frame]->Palette[imageset.img[frame]->Raster[pixel_gif]].r;
					texture[0]->imageData[exact_imgdata+1]	
						=imageset.img[frame]->Palette[imageset.img[frame]->Raster[pixel_gif]].g;
					texture[0]->imageData[exact_imgdata+2]	
						=imageset.img[frame]->Palette[imageset.img[frame]->Raster[pixel_gif]].b;
						texture[0]->imageData[exact_imgdata+3] = 255;
				}
			}

			if ((pixel_gif+1)%imageset.img[frame]->BytesPerRow==0)					// �����һ�л���
			{
				n++;										// gifͼ��������imageData����
				current_imgdata = left_up+(n*texture[0]->width)*4;
			}
			else
				current_imgdata += 4;						// �ƶ�һ������
		}
	}
//	SmoothBound(texture[0],imageset.img[0]);
}

int LoadGLTextures(char *file)									// ��������ͼ��file����ͼ����
{
	int Status=FALSE;											// Status Indicator
	Gif_TexInit( file );

	glEnable(GL_TEXTURE_RECTANGLE_NV);
	glGenTextures(1, &texture[0]->texID);				// Create The Texture ( CHANGE )
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, texture[0]->texID);
	glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, texture[0]->bpp / 8, texture[0]->width, texture[0]->height
	 	, 0, texture[0]->type, GL_UNSIGNED_BYTE, texture[0]->imageData);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glEnable(GL_TEXTURE_2D);
	if (   LoadTGA(texture[1], "Data/mouse1.tga")			// ѡ��������ı�־
		&& LoadTGA(texture[2], "Data/title.tga")			// ������
		&& LoadTGA(texture[3], "Data/Font.tga")			// ����
		&& LoadTGA(texture[4], "Data/save.tga")			// �洢������
		&& LoadTGA(texture[5], "Data/test.tga")			// ���Զ�����ť
		&& LoadTGA(texture[6], "Data/return.tga")			// ����ѡ�񲻶��㻭��
		&& LoadTGA(texture[7], "Data/exit.tga")			// �˳���ť
		&& LoadTGA(texture[8], "Data/save_anm.tga")		// �洢anm��ť
		&& LoadTGA(texture[9], "Data/way.tga")				// ��ʼѡ���Ӧ�ڱ�����·����ť
		&& LoadTGA(texture[10],"Data/changeback.tga")		// ����������ť
		&& LoadTGA(texture[11],"Data/save_way.tga")		// ���浱ǰѡ���·����ť
		&& LoadTGA(texture[12],"Data/zoom+.tga")			// �Ŵ�ǰѡ��ͼ��ť
		&& LoadTGA(texture[13],"Data/zoom-.tga")			// ��С��ǰѡ��ͼ��ť
		&& LoadTGA(texture[14],"Data/play.tga")			// ��ʼ��·���˶�
		&& LoadTGA(texture[15],"Data/stop.tga")			// ��ͣ
		&& LoadTGA(texture[16],"Data/scene0.tga")			// ����ͼ��
		&& LoadTGA(texture[17],"Data/scene1.tga"))
	{
		Status=TRUE;											// Set The Status To TRUE
		for(int temp=1; temp<NUMBER; temp++)
		{		
			glGenTextures(1, &texture[temp]->texID);				// Create The Texture ( CHANGE )
			glBindTexture(GL_TEXTURE_2D, texture[temp]->texID);
			glTexImage2D(GL_TEXTURE_2D, 0, texture[temp]->bpp / 8, texture[temp]->width, texture[temp]->height
				, 0, texture[temp]->type, GL_UNSIGNED_BYTE, texture[temp]->imageData);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			if (texture[temp]->imageData)						// If Texture Image Exists ( CHANGE )
			{
				free(texture[temp]->imageData);					// Free The Texture Image Memory ( CHANGE )
				texture[temp]->imageData = NULL;
			}
		}
	}
	strcpy(g_backgroundname[0], "scene0");
	strcpy(g_backgroundname[1], "scene1");

	return Status;												// Return The Status
}
