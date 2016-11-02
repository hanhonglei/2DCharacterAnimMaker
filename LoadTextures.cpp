#include "LoadTextures.h"

void CountRowCol(int num, int height, int width, int &row, int &col)
{																// 通过给定的数据计算行数和列数，是排列类似与正方形
	int temp = int(sqrt(double(num)));							// 只有3种情况
	if (num == temp*temp)										// 正好分解为它的开方的相乘
		row = col = temp;
	
	else if (num == temp*(temp+1) || num < temp*(temp+1))		// 另外一种比较好的情况
	{
 		if (height > width)										// 通过长宽比较，尽量接近正方形
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
	
	else														// 最坏的浪费空间情况
		row = col = temp+1;
}

void SmoothBound(Texture tex, C_Image *image)					// 平滑gif图像的边界（未使用）
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

void Gif_TexInit(char *file)									// 将gif图像初始化为texture
{
	C_ImageSet	imageset;
	imageset.LoadGIF( file );									// 利用loadgif将各个帧的数据得到

	texture[0]->bpp			= 32;								// 一律用32位的
	g_numframe				= imageset.nImages;
	texture[0]->height		= imageset.img[0]->Height;
	texture[0]->width		= imageset.img[0]->Width;

	int i = 0;
	
	for (i = 1; i < g_numframe; i++)		// 取各个gif帧里面比较大的作为texture里面的基数
	{
		if(GLubyte(imageset.img[i]->Height) > (texture[0]->height))
			texture[0]->height = imageset.img[i]->Height;
		if(GLubyte(imageset.img[i]->Width)  > (texture[0]->width))
			texture[0]->width  = imageset.img[i]->Width;
	}
	
	CountRowCol(g_numframe,texture[0]->height,texture[0]->width,g_row,g_col);	// 初始化texture的行数列数
	
	texture[0]->height		= texture[0]->height*g_row;			// 纹理的高
	texture[0]->width		= texture[0]->width*g_col;			// 纹理的宽
	texture[0]->type			= GL_RGBA;
	texture[0]->imageData	= (GLubyte *)malloc(sizeof(GLubyte)*texture[0]->width*texture[0]->height*4);// 给纹理分配颜色空间
	
	for (i = 0; (unsigned int)i < texture[0]->width*texture[0]->height*4; i++)						// 初始化texture[0]的imageData
	{
		if ((i+1)%4==0)
			texture[0]->imageData[i] = 0;
		else
			texture[0]->imageData[i] = 255;
	}
	
	int current_imgdata;										// texture的第几个像素
	for (int frame = 0; frame < g_numframe; frame++)			// 分别将每个gif帧的rgb填充到texture里面
	{
		int n = 0;												// 行号
		int left_up=(frame/g_col)*texture[0]->width*(texture[0]->height/g_row)*4  // 每一帧左上角在imageData的编号
			+(frame%g_col)*(texture[0]->width/g_col)*4;
		current_imgdata = left_up;
		for( int pixel_gif = 0
			; pixel_gif < (imageset.img[frame]->BytesPerRow)*(imageset.img[frame]->Height)
			; pixel_gif++ )	// 循环处理整个gif帧		
		{
			int exact_imgdata = current_imgdata + imageset.img[frame]->yPos*texture[0]->width*4
				+ imageset.img[frame]->xPos*4;// 经过偏移后的imageData值

			if (pixel_gif<n*imageset.img[frame]->BytesPerRow+imageset.img[frame]->Width)		
			{																// 由于有ALIGN的存在，所以，当pixel_gif突破width以后到行末就不填充
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

			if ((pixel_gif+1)%imageset.img[frame]->BytesPerRow==0)					// 填充完一行换行
			{
				n++;										// gif图像换行引起imageData换行
				current_imgdata = left_up+(n*texture[0]->width)*4;
			}
			else
				current_imgdata += 4;						// 移动一个像素
		}
	}
//	SmoothBound(texture[0],imageset.img[0]);
}

int LoadGLTextures(char *file)									// 用来载入图象，file传递图象名
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
	if (   LoadTGA(texture[1], "Data/mouse1.tga")			// 选定不动点的标志
		&& LoadTGA(texture[2], "Data/title.tga")			// 标题栏
		&& LoadTGA(texture[3], "Data/Font.tga")			// 字体
		&& LoadTGA(texture[4], "Data/save.tga")			// 存储不动点
		&& LoadTGA(texture[5], "Data/test.tga")			// 测试动画按钮
		&& LoadTGA(texture[6], "Data/return.tga")			// 返回选择不动点画面
		&& LoadTGA(texture[7], "Data/exit.tga")			// 退出按钮
		&& LoadTGA(texture[8], "Data/save_anm.tga")		// 存储anm按钮
		&& LoadTGA(texture[9], "Data/way.tga")				// 开始选择对应于背景的路径按钮
		&& LoadTGA(texture[10],"Data/changeback.tga")		// 更换背景按钮
		&& LoadTGA(texture[11],"Data/save_way.tga")		// 保存当前选择的路径按钮
		&& LoadTGA(texture[12],"Data/zoom+.tga")			// 放大当前选择图像按钮
		&& LoadTGA(texture[13],"Data/zoom-.tga")			// 缩小当前选择图像按钮
		&& LoadTGA(texture[14],"Data/play.tga")			// 开始沿路径运动
		&& LoadTGA(texture[15],"Data/stop.tga")			// 暂停
		&& LoadTGA(texture[16],"Data/scene0.tga")			// 背景图像
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
