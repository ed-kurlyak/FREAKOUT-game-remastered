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

//количество строк с блоками
#define NUM_BLOCK_ROWS          6
//количество колонок с блоками
#define NUM_BLOCK_COLUMNS       8

//ширина одного блока, высота
#define BLOCK_WIDTH             (int)(64 * ((float)800 / 640))
#define BLOCK_HEIGHT            (int)(16 * ((float)800 / 640))

//начало секции блоков по x,y (слева, свехру)
#define BLOCK_ORIGIN_X          8
#define BLOCK_ORIGIN_Y          8

//размер одного блока вместе с проблеами между другими блоками
#define BLOCK_X_GAP             (int)(80 * ((float)800 / 640))
#define BLOCK_Y_GAP             (int)(32 * ((float)800 / 640))

//определяем клюшку внизу экрана - ширина, высота
#define PADDLE_WIDTH            (int)(96 * ((float)800 / 640))
#define PADDLE_HEIGHT           (int)(12 * ((float)800 / 640))
//исходная поизция клюшки после начала игры
#define PADDLE_START_X          (WINDOW_WIDTH / 2 - PADDLE_WIDTH / 2)
#define PADDLE_START_Y          (WINDOW_HEIGHT - 32);

//определяем мяч
//стартовая позиция по высота
#define BALL_START_Y            (WINDOW_HEIGHT / 2)
//размера мяча в пикселях (диаметр)
#define BALL_SIZE                (int)(16 * ((float)800 / 640))

//определения для управления с клавиатуры
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

	//в переменную записывается ввод с клавиатуры
	int m_Linput;

	//отслеживаем позицию клюшки по x,y
	int m_PaddleX;
	int m_PaddleY;

	//отслеживаем позицию мяча по x,y
	int m_BallX;
	int m_BallY;
	
	//скорость движения мяча (приращение) по x,y
	int m_BallDx;
	int m_BallDy;

	int m_Score;               //score результат игры
	int m_Level;               //текущий уровень
	int m_BlocksHit;             //отслеживаем число блоков в которые попал мяч

	//массив, каждый элемент либо 1 либо 0
	//массив определяет какие блоки видимы,
	//а в какие блоки было попадание мяча
	//если мяч попал в блок - в массиве этот блок 0
	unsigned char m_Blocks[NUM_BLOCK_ROWS][NUM_BLOCK_COLUMNS];

};

#endif