#include "GlobalVariables.h"

HDC			hDC;												// Private GDI Device Context
HGLRC		hRC;												// Permanent Rendering Context
HWND		hWnd;												// Holds Our Window Handle
HINSTANCE	hInstance;											// Holds The Instance Of The Application

bool g_hold_exited		= FALSE;								// ����������Ƿ��ƶ���ͼ������
bool g_hold_test		= FALSE;
bool g_hold_saveMove	= FALSE;
bool g_hold_save		= FALSE;
bool g_hold_return		= FALSE;
bool g_hold_saveANM		= FALSE;
bool g_hold_selectWay	= FALSE;
bool g_hold_zoomIn		= FALSE;
bool g_hold_zoomOut		= FALSE;
bool g_hold_play_stop   = FALSE;
bool g_hold_changback   = FALSE;
bool g_exited			= FALSE;								// �Ƿ��˳�
bool g_test				= FALSE;								// ���Ʋ��Ի���
bool g_saveMove			= FALSE;								// �Ƿ񱣴���ѡ���·��
bool g_saveANM			= FALSE;								// �Ƿ񱣴�anm�ļ�
bool g_moveObj			= FALSE;								// �Ƿ���ѡ���˶���
bool g_selectWay		= FALSE;								// �Ƿ�ѡ�񶯻��ƶ���·��
bool active				= TRUE;									// Window Active Flag Set To TRUE By Default
bool fullscreen			= FALSE;									// Fullscreen Flag Set To Fullscreen Mode By Default
bool first				= TRUE;									// �Ƿ��һ������
bool g_saveWay			= FALSE;								// �Ƿ�ѡ��·��
bool g_play				= FALSE;
bool g_selcKeypointPic[4];										// ѡ��ؼ�·���ϵ�ͼƬ���������
bool keys[256];													// Array Used For The Keyboard Routine
bool g_selcKeypoint[4]	= {FALSE,FALSE,FALSE,FALSE};
int g_mouse_ldown		= 0;									// ���������µĴ���
int g_frame				= 0;									// �����ĵ�ǰ��
int g_xtrans;
int g_ytrans;													// �ڲ��Զ���ʱ��������λ��
int g_keypoint			= 0;									// ѡ��·��ʱ�Ĺؼ��㣨�����6����
int g_numway[50];												// һ��ѡ���·����Ŀ������ÿһά��Ӧһ������
int		g_mouse_x, g_mouse_y;									// ���λ��
int		g_row;													// �ֳɷ��������
int		g_col;													// ����
int		g_numframe;												// gif֡��
int		g_length; 												// ÿһ��ĳ���
int		g_wayTemp[4][2];										// ��ʱ�洢�ؼ���
int		g_numbackground	= 0;									// ʹ�õĵ�ǰ���������ø���������ť�ı�
int		g_location[100][2];										// ��Բ����ĵ������
int		g_framecoord[100][2];									// ÿһ������½ǵ�����
int		g_move[100][2];											// ���Ľ��
int		g_way[50][20][4][2];									//  ������¼���е�·���㣬��һά��ʾ��Ӧ�ı������ڶ�ά��ʾ�ڼ���·��
char	g_headward;												// ��¼�����ĳ���
char	g_backgroundname[50][7];
float	g_scale[50][20][4];										// ���������¼�ڹؼ��㴦������С�ı�������һά��ʾ��Ӧ�ı���
float   g_scaleTemp[4]	= {1.0f,1.0f,1.0f,1.0f};				// �����û�����ȷ���ڸ����ؼ���Ķ�����С����
const int MAXBACKGROUNDNUM = 2;									// ��������Ŀ
const int NUMBER = 16 + MAXBACKGROUNDNUM		;				// texture��ͼ������Ŀ
//Texture **texture = NULL;
Texture *texture[NUMBER];

GLuint	base;
