
/////////////////////////////////////////////////////////////////
#ifndef __ANMFORMAT_H__
#define __ANMFORMAT_H__

typedef struct	 header 
{
	char		cToken[4];				// 标记，识别anm文件，格式为：anm\0
	char		cHeadward;				// 指明动画的原始朝向
	int			iBackgroundNum;			// 背景数目
	long		iNumFrame;				// 帧数
	long		iRow;					// 行数
	long		iColumn;				// 列数		
	long		iWidth;					// 宽
	long		iHeight;				// 高
	float		fMotion[256][2];		// 帧之间的移动距离，其中fMotion[frame][0]
										// 表示x方向的，fMotion[frame][1]表示y方向的
} ANMheader;


typedef struct
{
	ANMheader	anm_header;				// 文件头
	GLubyte		*imageData;				// 动画的纹理rgba信息
	char		*backgroundName;		// 背景名称
	int			numWay;					// 对应于背景的路径数目
	int			***keypoints;			// 对应于背景的各个路径的关键点坐标，每一组关键点数目是4个
	float		**scale;				// 对应于背景的各个路径的关键点上的图像比例
} ANMFile;

#endif