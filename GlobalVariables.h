
#ifndef __GLOBALVARIABLES_H__
#define __GLOBALVARIABLES_H__

#include <windows.h>
#include "Texture.h"
// 全程变量
extern const int MAXBACKGROUNDNUM;									// 背景的数目
extern const int NUMBER;											// texture贴图的总数目

extern HDC			hDC;									// Private GDI Device Context
extern HGLRC		hRC;									// Permanent Rendering Context
extern HWND		hWnd;										// Holds Our Window Handle
extern HINSTANCE	hInstance;											// Holds The Instance Of The Application
	
extern bool	g_hold_exited	;								// 以下是鼠标是否移动到图标上面
extern bool	g_hold_test		;
extern bool	g_hold_saveMove ;
extern bool	g_hold_save		;
extern bool	g_hold_return	;
extern bool	g_hold_saveANM	;
extern bool	g_hold_selectWay;
extern bool	g_hold_zoomIn	;
extern bool	g_hold_zoomOut	;
extern bool g_hold_play_stop;
extern bool g_hold_changback;
extern bool	g_exited		;								// 是否退出
extern bool	g_test			;								// 绘制测试画面
extern bool	g_saveMove		;								// 是否保存所选择的路径
extern bool	g_saveANM		;								// 是否保存anm文件
extern bool	g_moveObj		;								// 是否点击选择了动画
extern bool	g_selectWay		;								// 是否选择动画移动的路径
extern bool	active			;								// Window Active Flag Set To TRUE By Default
extern bool	fullscreen		;								// Fullscreen Flag Set To Fullscreen Mode By Default
extern bool	first			;								// 是否第一次运行
extern bool	g_saveWay		;								// 是否选择路径
extern bool g_play			;								// 是否进行沿路径动画测试
extern bool	g_selcKeypoint[4];								// 选择哪个关键点
extern bool	g_selcKeypointPic[4];							// 选择关键路径上的图片调整其比例
extern bool	keys[256];										// Array Used For The Keyboard Routine
extern int	g_mouse_x, g_mouse_y;							// 鼠标位置
extern int	g_mouse_ldown;									// 鼠标左键按下的次数
extern int	g_row;											// 分成方格的行数
extern int	g_col;											// 列数
extern int	g_numframe;										// gif帧数
extern int	g_frame;										// 所贴的当前桢
extern int	g_length; 										// 每一桢的长度
extern int	g_xtrans, g_ytrans;								// 在测试动画时，动画的位置
extern int	g_keypoint;										// 选择路径时的关键点（最多是4个）
extern int	g_numway[50];									// 一共选择的路径数目
extern int  g_numbackground;								// 使用的当前背景，利用更换背景按钮改变
extern int	g_location[100][2];								// 相对不动的点的坐标
extern int	g_framecoord[100][2];							// 每一桢的左下角的坐标
extern int	g_move[100][2];									// 最后的结果
extern int	g_wayTemp[4][2];								// 存储4个关键点，然后利用b-spline函数得到路径曲线
extern int	g_way[50][20][4][2];							//  用来记录所有的路径点
extern char	g_headward;										// 记录动画的朝向
extern float g_scaleTemp[4];								// 利用用户交互确定在各个关键点的动画大小比例
extern float g_scale[50][20][4];							// 最后用来记录在关键点处动画大小的比例
extern GLuint	base;										// Base Display List For The Font
extern Texture *texture[];									// 存储打开的.tga格式图
extern char g_backgroundname[50][7];						// 存储打开的背景名字
#endif
