
#ifndef __GLOBALVARIABLES_H__
#define __GLOBALVARIABLES_H__

#include <windows.h>
#include "Texture.h"
// ȫ�̱���
extern const int MAXBACKGROUNDNUM;									// ��������Ŀ
extern const int NUMBER;											// texture��ͼ������Ŀ

extern HDC			hDC;									// Private GDI Device Context
extern HGLRC		hRC;									// Permanent Rendering Context
extern HWND		hWnd;										// Holds Our Window Handle
extern HINSTANCE	hInstance;											// Holds The Instance Of The Application
	
extern bool	g_hold_exited	;								// ����������Ƿ��ƶ���ͼ������
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
extern bool	g_exited		;								// �Ƿ��˳�
extern bool	g_test			;								// ���Ʋ��Ի���
extern bool	g_saveMove		;								// �Ƿ񱣴���ѡ���·��
extern bool	g_saveANM		;								// �Ƿ񱣴�anm�ļ�
extern bool	g_moveObj		;								// �Ƿ���ѡ���˶���
extern bool	g_selectWay		;								// �Ƿ�ѡ�񶯻��ƶ���·��
extern bool	active			;								// Window Active Flag Set To TRUE By Default
extern bool	fullscreen		;								// Fullscreen Flag Set To Fullscreen Mode By Default
extern bool	first			;								// �Ƿ��һ������
extern bool	g_saveWay		;								// �Ƿ�ѡ��·��
extern bool g_play			;								// �Ƿ������·����������
extern bool	g_selcKeypoint[4];								// ѡ���ĸ��ؼ���
extern bool	g_selcKeypointPic[4];							// ѡ��ؼ�·���ϵ�ͼƬ���������
extern bool	keys[256];										// Array Used For The Keyboard Routine
extern int	g_mouse_x, g_mouse_y;							// ���λ��
extern int	g_mouse_ldown;									// ���������µĴ���
extern int	g_row;											// �ֳɷ��������
extern int	g_col;											// ����
extern int	g_numframe;										// gif֡��
extern int	g_frame;										// �����ĵ�ǰ��
extern int	g_length; 										// ÿһ��ĳ���
extern int	g_xtrans, g_ytrans;								// �ڲ��Զ���ʱ��������λ��
extern int	g_keypoint;										// ѡ��·��ʱ�Ĺؼ��㣨�����4����
extern int	g_numway[50];									// һ��ѡ���·����Ŀ
extern int  g_numbackground;								// ʹ�õĵ�ǰ���������ø���������ť�ı�
extern int	g_location[100][2];								// ��Բ����ĵ������
extern int	g_framecoord[100][2];							// ÿһ������½ǵ�����
extern int	g_move[100][2];									// ���Ľ��
extern int	g_wayTemp[4][2];								// �洢4���ؼ��㣬Ȼ������b-spline�����õ�·������
extern int	g_way[50][20][4][2];							//  ������¼���е�·����
extern char	g_headward;										// ��¼�����ĳ���
extern float g_scaleTemp[4];								// �����û�����ȷ���ڸ����ؼ���Ķ�����С����
extern float g_scale[50][20][4];							// ���������¼�ڹؼ��㴦������С�ı���
extern GLuint	base;										// Base Display List For The Font
extern Texture *texture[];									// �洢�򿪵�.tga��ʽͼ
extern char g_backgroundname[50][7];						// �洢�򿪵ı�������
#endif
