
/////////////////////////////////////////////////////////////////
#ifndef __ANMFORMAT_H__
#define __ANMFORMAT_H__

typedef struct	 header 
{
	char		cToken[4];				// ��ǣ�ʶ��anm�ļ�����ʽΪ��anm\0
	char		cHeadward;				// ָ��������ԭʼ����
	int			iBackgroundNum;			// ������Ŀ
	long		iNumFrame;				// ֡��
	long		iRow;					// ����
	long		iColumn;				// ����		
	long		iWidth;					// ��
	long		iHeight;				// ��
	float		fMotion[256][2];		// ֮֡����ƶ����룬����fMotion[frame][0]
										// ��ʾx����ģ�fMotion[frame][1]��ʾy�����
} ANMheader;


typedef struct
{
	ANMheader	anm_header;				// �ļ�ͷ
	GLubyte		*imageData;				// ����������rgba��Ϣ
	char		*backgroundName;		// ��������
	int			numWay;					// ��Ӧ�ڱ�����·����Ŀ
	int			***keypoints;			// ��Ӧ�ڱ����ĸ���·���Ĺؼ������꣬ÿһ��ؼ�����Ŀ��4��
	float		**scale;				// ��Ӧ�ڱ����ĸ���·���Ĺؼ����ϵ�ͼ�����
} ANMFile;

#endif