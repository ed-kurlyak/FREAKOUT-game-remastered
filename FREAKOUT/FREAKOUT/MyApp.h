#ifndef _MYAPP_
#define _MYAPP_

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <time.h>

#define APPNAME TEXT("ARKANOID by Kurlyak")
#define CLASSNAME TEXT("Sample")

// The game was originally developed for a screen resolution of 640 by 480.
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//���������� ����� � �������
#define NUM_BLOCK_ROWS          6
//���������� ������� � �������
#define NUM_BLOCK_COLUMNS       8

//������ ������ �����, ������
#define BLOCK_WIDTH             (int)(64 * ((float)800 / 640))
#define BLOCK_HEIGHT            (int)(16 * ((float)800 / 640))

//������ ������ ������ �� x,y (�����, ������)
#define BLOCK_ORIGIN_X          8
#define BLOCK_ORIGIN_Y          8

//������ ������ ����� ������ � ��������� ����� ������� �������
#define BLOCK_X_GAP             (int)(80 * ((float)800 / 640))
#define BLOCK_Y_GAP             (int)(32 * ((float)800 / 640))

//���������� ������ ����� ������ - ������, ������
#define PADDLE_WIDTH            (int)(96 * ((float)800 / 640))
#define PADDLE_HEIGHT           (int)(12 * ((float)800 / 640))
//�������� ������� ������ ����� ������ ����
#define PADDLE_START_X          (WINDOW_WIDTH / 2 - PADDLE_WIDTH / 2)
#define PADDLE_START_Y          (WINDOW_HEIGHT - 32);

//���������� ���
//��������� ������� �� ������
#define BALL_START_Y            (WINDOW_HEIGHT / 2)
//������� ���� � �������� (�������)
#define BALL_SIZE                (int)(16 * ((float)800 / 640))

//����������� ��� ���������� � ����������
#define IN_FORWARD  (1 << 0)
#define IN_BACK     (1 << 1)
#define IN_LEFT     (1 << 2)
#define IN_RIGHT    (1 << 3)

class CMyApp
{
public:
	bool InitScene(HINSTANCE	hInstance, int nCmdShow, int nWidth, int nHeight);
	int GameBegin();


private:
	void Create_BackBuffer();
	void Clear_BackBuffer();
	void Present_BackBuffer();
	void Delete_BackBuffer();

	void Destroy();

	void Init_Blocks();
	void Draw_Blocks();
	void Process_Ball();

	void LimitFPS( float fLimitFPS);
	void RenderScene();

	int GetInput();	

	static LRESULT CALLBACK StaticWndProc(	HWND m_hWnd, UINT	uMsg, WPARAM	wParam,	LPARAM	lParam);
	LRESULT WndProc(	HWND m_hWnd, UINT	uMsg, WPARAM	wParam,	LPARAM	lParam);

	HWND m_hWnd;
	HINSTANCE m_hInstance;

	HDC m_hBackBuffer;
	HBITMAP m_hBmpBackBuffer;
	HBITMAP m_hOldBmp;
	HDC m_hDC;
	RECT m_Rc;

	//� ���������� ������������ ���� � ����������
	int m_Linput;

	//����������� ������� ������ �� x,y
	int m_PaddleX;
	int m_PaddleY;

	//����������� ������� ���� �� x,y
	int m_BallX;
	int m_BallY;
	
	//�������� �������� ���� (����������) �� x,y
	int m_BallDx;
	int m_BallDy;

	int m_Score;               //score ��������� ����
	int m_Level;               //������� �������
	int m_BlocksHit;             //����������� ����� ������ � ������� ����� ���

	//������, ������ ������� ���� 1 ���� 0
	//������ ���������� ����� ����� ������,
	//� � ����� ����� ���� ��������� ����
	//���� ��� ����� � ���� - � ������� ���� ���� 0
	unsigned char m_Blocks[NUM_BLOCK_ROWS][NUM_BLOCK_COLUMNS];

};

#endif