////////////////////////////////////////////////////////////////

#pragma comment(lib, "Glu32.lib")

#include <windows.h>
#include "GLRoutine.h"
#include "GlobalVariables.h"
#include "SelectObject.h"
#include "DrawScene.h"

LRESULT CALLBACK WndProc(	HWND	hWnd,						// Handle For This Window
							UINT	uMsg,						// Message For This Window
							WPARAM	wParam,						// Additional Message Information
							LPARAM	lParam)						// Additional Message Information
{
	switch (uMsg)												// Check For Windows Messages
	{
		case WM_ACTIVATE:										// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))								// Check Minimization State
			{
				active=TRUE;									// Program Is Active
			}
			else
			{
				active=FALSE;									// Program Is No Longer Active
			}

			return 0;											// Return To The Message Loop
		}

		case WM_SYSCOMMAND:										// Intercept System Commands
		{
			switch (wParam)										// Check System Calls
			{
				case SC_SCREENSAVE:								// Screensaver Trying To Start?
				case SC_MONITORPOWER:							// Monitor Trying To Enter Powersave?
				return 0;										// Prevent From Happening
			}
			break;												// Exit
		}

		case WM_CLOSE:											// Did We Receive A Close Message?
		{
			PostQuitMessage(0);									// Send A Quit Message
			return 0;											// Jump Back
		}

		case WM_KEYDOWN:										// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;								// If So, Mark It As TRUE
			return 0;											// Jump Back
		}

		case WM_KEYUP:											// Has A Key Been Released?
		{
			keys[wParam] = FALSE;								// If So, Mark It As FALSE
			return 0;											// Jump Back
		}

		case WM_SIZE:											// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));		// LoWord=Width, HiWord=Height
			return 0;											// Jump Back
		}

		case WM_LBUTTONDOWN:
		{
			g_mouse_x = LOWORD(lParam);							// ��갴���ǵ�����																																	
			g_mouse_y = HIWORD(lParam);
			if (g_mouse_ldown <= 2*g_numframe-1					// ����갴������ѡ����ʱ������갴��λ�úϷ�������¼���λ��
				&& (((g_mouse_ldown == 0 || g_mouse_ldown == 2*g_numframe-1) 
				&& (g_mouse_x>g_framecoord[0][0]&&g_mouse_x<g_framecoord[0][0]+g_length)
				&& (g_mouse_y>g_framecoord[0][1]-g_length&&g_mouse_y<g_framecoord[0][1]))
				|| ((g_mouse_x>g_framecoord[(g_mouse_ldown+1)/2][0]&&g_mouse_x<g_framecoord[(g_mouse_ldown+1)/2][0]+g_length)
				&& (g_mouse_y>g_framecoord[(g_mouse_ldown+1)/2][1]-g_length&&g_mouse_y<g_framecoord[(g_mouse_ldown+1)/2][1])))
				&& !g_test)
			{
				g_location[g_mouse_ldown][0] = g_mouse_x;
				g_location[g_mouse_ldown++][1] = g_mouse_y;
				if (g_mouse_ldown == 2*g_numframe)					// ���ѡ��
				{
					PlaySound("Data/tada.wav",NULL,SND_ASYNC);
				}
			}

			else if (g_mouse_x <= 824 && g_mouse_ldown <= 2*g_numframe-1 && !g_test)  // �����λ�ò��Ϸ��Ǹ�����ʾ
			{
				PlaySound("Data/ir_begin.wav",NULL,SND_ASYNC);
			}

			if (g_selectWay && g_test)							// ѡ��ؼ���									// ��ѡ��·��ģʽ�£���갴����ѡ��һ���ؼ���
			{
				if(g_keypoint < 4)
				{
					g_wayTemp[g_keypoint][0] = g_mouse_x;
					g_wayTemp[g_keypoint][1] = g_mouse_y;
					g_keypoint++;
				}
			}
			Selection();
			if (g_keypoint == 4 && g_test)
			{
				g_selectWay = FALSE;							// ѡ��ؼ������
			}
			return 0;
		}
		case WM_RBUTTONDOWN:								
			{
				if ( !g_test )	// �Ҽ���������ȡ��ѡ��Ĳ�����
				{
					g_mouse_ldown--;
					if (g_mouse_ldown < 0)
					{
						g_mouse_ldown = 0;
					}
				}
				else if (g_test) // ����Ҽ������ȡ��ѡ��Ĺؼ�·���ϵĹؼ���
				{
					g_keypoint--;
					if (g_keypoint < 0)
					{
						g_keypoint = 0;
						g_selectWay = FALSE;	// ȡ����0ʱ����ѡ��·����־Ϊ��
					}
					if (g_keypoint !=0)
					{
						g_selectWay = TRUE;
					}
//					g_selectWay = TRUE;
				}
				return 0;
			}

		case WM_LBUTTONUP:
			{
				if(g_test && g_keypoint == 4 && !g_selectWay)
				{
					for (int i=0; i<4; i++)
					{
						g_selcKeypoint[i] = FALSE;
					}
//					g_moveObj = FALSE;
				}
				if (g_play && g_test)
					g_moveObj = FALSE;
				return	0;
			}
			
		case WM_MOUSEMOVE:
		{
	        g_mouse_x = LOWORD(lParam);          
			g_mouse_y = HIWORD(lParam);
			if (g_moveObj)	// ��ѡ�����˶��еĶ���ʱ���򶯻���������ƶ�
			{
				g_xtrans = g_mouse_x;
				g_ytrans = g_mouse_y;
			}
			for (int i = 0; i < g_keypoint; i++)
			{
				if (g_selcKeypoint[i])// ��ѡ���˹ؼ���ʱ
				{
					g_wayTemp[i][0] = g_mouse_x;
					g_wayTemp[i][1] = g_mouse_y;
				}
			}
			HoldOn();				// �鿴ʱ���ƶ����˰�ť��
			return 0;
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,						// Instance
					HINSTANCE	hPrevInstance,					// Previous Instance
					LPSTR		lpCmdLine,						// Command Line Parameters
					int			nCmdShow)						// Window Show State
{
    PSZ pargs = lpCmdLine ;										//�ȷ���������
	char	 your[100];
	int		i = 0;
	
	while (isspace (*pargs))							// ����пո�������
		pargs++;
    
	while (!isspace (*pargs))
	{
		your[i] = *pargs++;
		i++;
	}
	your[i]='\0';										// ���⶯���ļ�
    
	while (isspace(*pargs)) pargs++;
	i = 0;

	while (isspace(*pargs)) pargs++;			    	// �����ĳ���
	g_headward = *pargs;
	
	
	MSG		msg;												// Windows Message Structure
	BOOL	done = FALSE;											// Bool Variable To Exit Loop
	
	// Create Our OpenGL Window
	if (!CreateGLWindow(your,"���Զ���Ч�������  ��esc�����˳�",1024,768,16,fullscreen))
	{
		return 0;												// Quit If Window Was Not Created
	}
	
	while(!done)												// Loop That Runs While done=FALSE
	{
		if (g_exited)											// ������¡��˳�������ֹ
		{
			done = TRUE;
		}
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))				// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)							// Have We Received A Quit Message?
			{
				done=TRUE;										// If So done=TRUE
			}
			else												// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);							// Translate The Message
				DispatchMessage(&msg);							// Dispatch The Message
			}
		}
		else													// If There Are No Messages
		{
			if (!g_test)										// �������ѡȡ�������㡱ģʽ������Ƹ���֡
			{
				if ((active && !DrawGLScene() )|| keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
				{
					done=TRUE;										// ESC or DrawGLScene Signalled A Quit
				}
				else												// Not Time To Quit, Update Screen
				{	
					SwapBuffers(hDC);								// Swap Buffers (Double Buffering)
				}
			}
			else													// ������¡����Զ�������������Ʋ��Զ���
			{
				if ((active && !DrawTestAnim())|| keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
				{
					done=TRUE;										// ESC or DrawGLScene Signalled A Quit
				}
				else												// Not Time To Quit, Update Screen
				{
					SwapBuffers(hDC);								// Swap Buffers (Double Buffering)
				}

			}
			if (keys[VK_F1])									// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;								// If So Make Key FALSE
				KillGLWindow();									// Kill Our Current Window
				fullscreen=!fullscreen;							// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow(your,"���Զ���Ч�������  ��esc�����˳�",1024,768,16,fullscreen))
				{
					return 0;									// Quit If Window Was Not Created
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();												// Kill The Window
	return (msg.wParam);										// Exit The Program
}

