#include "GlobalVariables.h"

HDC			hDC;												// Private GDI Device Context
HGLRC		hRC;												// Permanent Rendering Context
HWND		hWnd;												// Holds Our Window Handle
HINSTANCE	hInstance;											// Holds The Instance Of The Application

bool g_hold_exited		= FALSE;								// 以下是鼠标是否移动到图标上面
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
bool g_exited			= FALSE;								// 是否退出
bool g_test				= FALSE;								// 绘制测试画面
bool g_saveMove			= FALSE;								// 是否保存所选择的路径
bool g_saveANM			= FALSE;								// 是否保存anm文件
bool g_moveObj			= FALSE;								// 是否点击选择了动画
bool g_selectWay		= FALSE;								// 是否选择动画移动的路径
bool active				= TRUE;									// Window Active Flag Set To TRUE By Default
bool fullscreen			= FALSE;									// Fullscreen Flag Set To Fullscreen Mode By Default
bool first				= TRUE;									// 是否第一次运行
bool g_saveWay			= FALSE;								// 是否选择路径
bool g_play				= FALSE;
bool g_selcKeypointPic[4];										// 选择关键路径上的图片调整其比例
bool keys[256];													// Array Used For The Keyboard Routine
bool g_selcKeypoint[4]	= {FALSE,FALSE,FALSE,FALSE};
int g_mouse_ldown		= 0;									// 鼠标左键按下的次数
int g_frame				= 0;									// 所贴的当前桢
int g_xtrans;
int g_ytrans;													// 在测试动画时，动画的位置
int g_keypoint			= 0;									// 选择路径时的关键点（最多是6个）
int g_numway[50];												// 一共选择的路径数目，其中每一维对应一个背景
int		g_mouse_x, g_mouse_y;									// 鼠标位置
int		g_row;													// 分成方格的行数
int		g_col;													// 列数
int		g_numframe;												// gif帧数
int		g_length; 												// 每一桢的长度
int		g_wayTemp[4][2];										// 临时存储关键点
int		g_numbackground	= 0;									// 使用的当前背景，利用更换背景按钮改变
int		g_location[100][2];										// 相对不动的点的坐标
int		g_framecoord[100][2];									// 每一桢的左下角的坐标
int		g_move[100][2];											// 最后的结果
int		g_way[50][20][4][2];									//  用来记录所有的路径点，第一维表示对应的背景，第二维表示第几条路径
char	g_headward;												// 记录动画的朝向
char	g_backgroundname[50][7];
float	g_scale[50][20][4];										// 最后用来记录在关键点处动画大小的比例，第一维表示对应的背景
float   g_scaleTemp[4]	= {1.0f,1.0f,1.0f,1.0f};				// 利用用户交互确定在各个关键点的动画大小比例
const int MAXBACKGROUNDNUM = 2;									// 背景的数目
const int NUMBER = 16 + MAXBACKGROUNDNUM		;				// texture贴图的总数目
//Texture **texture = NULL;
Texture *texture[NUMBER];

GLuint	base;
