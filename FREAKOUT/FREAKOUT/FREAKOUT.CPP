#include "FREAKOUT.h"

int WINAPI WinMain(	HINSTANCE	hInstance,
					HINSTANCE	hPrevInstance,
					LPSTR		lpCmdLine,
					int			nCmdShow)
{

	CMyApp App;

	if(!App.InitScene(hInstance, nCmdShow, WINDOW_WIDTH, WINDOW_HEIGHT))
		return 0;

	return App.GameBegin();
}

bool CMyApp::InitScene(HINSTANCE hInstance, int nCmdShow, int nWidth, int nHeight)
{
	
	WNDCLASS wcl;
		
	wcl.style			= CS_HREDRAW | CS_VREDRAW;
	wcl.lpfnWndProc		= (WNDPROC) StaticWndProc;
	wcl.cbClsExtra		= 0L;
	wcl.cbWndExtra		= 0L;
	wcl.hInstance		= GetModuleHandle(NULL);
	wcl.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wcl.hCursor			= LoadCursor(hInstance, IDC_ARROW);
	wcl.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcl.lpszMenuName	= NULL;
	wcl.lpszClassName	= CLASSNAME;
	
	RegisterClass (&wcl);
	
	m_hWnd = CreateWindow(CLASSNAME, APPNAME,
              WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
              0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
              NULL, NULL, hInstance, this);

	if (!m_hWnd) return 0;

	RECT window_rect = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};
	
	AdjustWindowRectEx(&window_rect,
		GetWindowStyle(m_hWnd),
		GetMenu(m_hWnd) != NULL,
		GetWindowExStyle(m_hWnd));

	UINT nWidthScreen = GetSystemMetrics(SM_CXSCREEN);
	UINT nHeightScreen = GetSystemMetrics(SM_CYSCREEN);

	UINT nWidthX = window_rect.right - window_rect.left;
	UINT nWidthY = window_rect.bottom - window_rect.top;

	UINT nPosX =  (nWidthScreen - nWidthX)/2;
	UINT nPosY =  (nHeightScreen - nWidthY)/2;
	
	MoveWindow(m_hWnd,
		nPosX,
        nPosY,
        nWidthX,
        nWidthY,
        FALSE);

	ShowWindow (m_hWnd, nCmdShow);
	UpdateWindow (m_hWnd);
	SetForegroundWindow(m_hWnd);			// Немного высокий приоритет

	Create_BackBuffer();

	return true;
}

LRESULT CALLBACK CMyApp::StaticWndProc(	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//WndProc статическая функция, глобальная потому что WinAPI
	//требует что бы функция окна была глобальной
	//поэтому мы в этой статической функции вызываем функцию класса
	//которая являеться действительно функцией окна и функцией класса
	//другими словами мы действуем в обход правилам WinAPI

	//если GetWindowLong() вызывается без предварительно SetWindowLong()
	//то возвращает память класса
	
	if (uMsg == WM_CREATE)
		SetWindowLong( hWnd, GWL_USERDATA, (LONG)(LONG_PTR)((CREATESTRUCT FAR *)lParam)->lpCreateParams);
		

	CMyApp *Destination = (CMyApp*)(LONG_PTR)GetWindowLong( hWnd, GWL_USERDATA );

	if (Destination)
		return Destination->WndProc( hWnd, uMsg, wParam, lParam );
		
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

LRESULT CMyApp::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	//проверяем сообщения
	switch (uMsg)
	{
		//мы получили сообщение закрыть приложение?
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			break;
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int CMyApp::GameBegin()
{
	MSG		Msg;

	while(true)
	{
		if (PeekMessage(&Msg,NULL,0,0,PM_REMOVE))
		{
			if (Msg.message == WM_QUIT) break;

				TranslateMessage( &Msg );
				DispatchMessage ( &Msg );
		}
		if ( GetKeyState( VK_ESCAPE ) & 0xFF00 ) break;

			RenderScene();
		
	}

	Destroy();

	return 0;
	
}

void CMyApp::LimitFPS( float fLimitFPS)
{
	float fTimeElapsed = 0.0f;
	float TimeScale;
	__int64 CurrentTime;
	__int64 PerfFreq;
	static __int64 LastTime = 0;

	QueryPerformanceFrequency((LARGE_INTEGER *)&PerfFreq);
	TimeScale = 1.0f / PerfFreq;

	if(fLimitFPS > 0.0f)
	{
		while ( fTimeElapsed < (1.0f / fLimitFPS))
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);
			// Вычисляем прошедшее время в секундах
	       fTimeElapsed = (CurrentTime - LastTime) * TimeScale;
		}
	}
	LastTime = CurrentTime;
}

int CMyApp::GetInput()
{
	m_Linput = 0;
 
	//нажата клавиша вперед
	if(GetAsyncKeyState(VK_UP)& 0xFF00) 
        m_Linput |= IN_FORWARD;
	//нажата клавиша назад
	if(GetAsyncKeyState(VK_DOWN)& 0xFF00)
		m_Linput |= IN_BACK;
	//нажата клавиша влево
    if(GetAsyncKeyState(VK_LEFT)& 0xFF00)
        m_Linput |= IN_LEFT;
	//нажата клавиша вправо
    if(GetAsyncKeyState(VK_RIGHT)& 0xFF00)
        m_Linput |= IN_RIGHT;

	/*

	//skip debounce input

	int j;

	static int oldInputDB = 0;
 
    j = m_Linput^(m_Linput&oldInputDB);
    oldInputDB = m_Linput;
    m_Linput &= j;

	*/

	return m_Linput;
 
}


void CMyApp::Init_Blocks()
{
	//инициализируем поле блоков вверху экрана
	//блок видим если значение массива 1
	//блок не видим если в него попал мяч значение массива 0
	for (int Row=0; Row < NUM_BLOCK_ROWS; Row++)
	{
		for (int Col=0; Col < NUM_BLOCK_COLUMNS; Col++)
		{
			 m_Blocks[Row][Col] = 1;
		}
	}

}

void CMyApp::Clear_BackBuffer()
{
	//очищаем экран черным цветом
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
	
	FillRect( m_hBackBuffer, &m_Rc, hBrush );

	DeleteObject(hBrush);
}

void CMyApp::Draw_Blocks(void)
{
	//эта функция выводит вверху экрана секцию с блоками
	//секция с блоками выводится на экран Row major (по рядам)
	
	//используется отслеживать текущую позицию блока
	int x1 = BLOCK_ORIGIN_X;
	int y1 = BLOCK_ORIGIN_Y; 

	//рисуем все блоки у которых значение в массиве m_Blocks 1
	for (int Row=0; Row < NUM_BLOCK_ROWS; Row++)
    {    
		//сбрасываем начальную позицию колонки
		x1 = BLOCK_ORIGIN_X;

		//выводим на экран строку из m_Blocks
		for (int Col=0; Col < NUM_BLOCK_COLUMNS; Col++)
        {
        //выводим на экран следующий блок если его значение в массиве не 0
        if (m_Blocks[Row][Col]!=0)
            {

				//функция WinAPI вывода прямоугольника на экран
				HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 127));
				HBRUSH hOldBrush = (HBRUSH)SelectObject(m_hBackBuffer, hBrush);

				Rectangle( m_hBackBuffer, x1, y1, x1 + BLOCK_WIDTH, y1 + BLOCK_HEIGHT);
	
				SelectObject(m_hBackBuffer, hOldBrush);
				DeleteObject(hBrush);
            }

        //увеличиваем позицию колонки
        x1+=BLOCK_X_GAP;
        }

    //увеличиваем позицию строки
    y1+=BLOCK_Y_GAP;

    }
}

void CMyApp::Process_Ball(void)
{

	//текущая позиция отрисовки
	int x1 = BLOCK_ORIGIN_X;
    int y1 = BLOCK_ORIGIN_Y; 

	//центр мяча
	int BallCx = m_BallX + (BALL_SIZE / 2);
	int BallCy = m_BallY + (BALL_SIZE / 2);

	//тестируем попадание мяча в клюшку
	if (m_BallY > (WINDOW_HEIGHT/2) && m_BallDy > 0)
	{
		//определяем передний край мяча
		int x = m_BallX + (BALL_SIZE/2);
		int y = m_BallY + (BALL_SIZE/2);

		//тестируем на попадание в клюшку
		if ((x >= m_PaddleX && x <= m_PaddleX+PADDLE_WIDTH) &&
			(y >= m_PaddleY && y <= m_PaddleY+PADDLE_HEIGHT))
		{
			//отражение мяча по Y
			m_BallDy=-m_BallDy;

			//поскольку мяч столкнулся с клюшкой мяч отскакивает
			m_BallY+=m_BallDy;

			//добавляем к смещению мяча небольшое значение
			//в зависимости от движения клюшки влево, вправо
			if(m_Linput & IN_RIGHT)
				m_BallDx-=(rand()%3);
			else
			if(m_Linput & IN_LEFT)
				m_BallDx+=(rand()%3);
			else
				m_BallDx+=(-1+rand()%3);
       
			//если в массиве не осталось блоков
			//мяч поразил все блоки
			//можем перейти на следующий уровень
			if (m_BlocksHit >= (NUM_BLOCK_ROWS*NUM_BLOCK_COLUMNS))
			{
				m_Level++;
			}

			return; 

		}

	}

	//тестируем попадание мяча в каждый блок
	//для тестирования используем bounding box
	//каждого блока
	for (int Row=0; Row < NUM_BLOCK_ROWS; Row++)
    {    
		//сбрасываем позицию колонки
		x1 = BLOCK_ORIGIN_X;

    
		//проверяем эту строку блоков
		for (int Col=0; Col < NUM_BLOCK_COLUMNS; Col++)
        {
			//если в массиве блок не 0 проверяем на попадание мяча в этот блок
			if (m_Blocks[Row][Col]!=0)
			{
			
				//тестируем мяч на попадание в bounding box
				//этого блока (ограничивающий прямоугольник блока)
				if ((BallCx > x1) && (BallCx < x1+BLOCK_WIDTH) &&     
					(BallCy > y1) && (BallCy < y1+BLOCK_HEIGHT))
				{
					//если мяч попал в блок- удаляем блок
					m_Blocks[Row][Col] = 0; 

					//увеличиваем счетчик блоков что бы перейти
					//на следующий уровень
					m_BlocksHit++;

					//отражаем движение мячаа в обратную сторону
					m_BallDy=-m_BallDy;
               
					//немного случайного смещения для непредсказуемости игры
					m_BallDx+=(-1+rand()%3);

					//добавляем значение score
					m_Score += 1;
			   
               
					return;

				}
			}

	        //увеличиваем позицию колонки
		    x1+=BLOCK_X_GAP;
        }

		//увеличиваем позицию строки
		y1+=BLOCK_Y_GAP;

    }
}

void CMyApp::RenderScene()
{
	static bool BeginGame = true;

	if(BeginGame)
	{
		BeginGame = false;

		srand((unsigned)time( NULL ));

		m_PaddleX = 0; m_PaddleY = 0; // tracks position of paddle
		m_BallX   = 0; m_BallY   = 0; // tracks position of ball
		m_BallDx  = 0; m_BallDy  = 0; // velocity of ball
		m_BlocksHit = 0;             // tracks number of m_Blocks hit

		// set the paddle position here to the middle bottom
	    m_PaddleX = PADDLE_START_X;
		m_PaddleY = PADDLE_START_Y;

		// set ball position and velocity
		m_BallX = 8 + rand() % (WINDOW_WIDTH-16);
		m_BallY = BALL_START_Y;
		m_BallDx = -4 + rand()%(8+1);
		m_BallDy = 6 + rand()%2;

		// reset block counter
		m_BlocksHit = 0;

		m_Score = 0;

	    // initialize the m_Blocks
	    Init_Blocks();
	}

	LimitFPS(30);

	Clear_BackBuffer();

	m_Linput = GetInput();

	if(m_Linput & IN_LEFT)
	{
		//двигаем клюшку вправо
       m_PaddleX-=8;
 
       //клюшка не должны выйти за границы экрана
       if (m_PaddleX < 0)
          m_PaddleX = 0;
	}
	
	else if(m_Linput & IN_RIGHT)
	{
		//двигаем клюшку влево
       m_PaddleX+=8;
 
       //клюшка не должна выйти за границы экрана
       if (m_PaddleX > (WINDOW_WIDTH-PADDLE_WIDTH))
          m_PaddleX = WINDOW_WIDTH-PADDLE_WIDTH;
	}


    //рисуем секцию блоков вверху экрана
    Draw_Blocks();

    //перемещаем мяч для текущего кадра
    m_BallX+=m_BallDx;
    m_BallY+=m_BallDy;
    
	//держим мяч внутри экрана, если мяч попадет в край экрана
	//отражаем в противоположном направлении движение мяча
    if (m_BallX > (WINDOW_WIDTH - BALL_SIZE) || m_BallX < 0) 
	{
		//отражение движения мяча по оси X
		m_BallDx=-m_BallDx;

		//обновляем позицию мяча
		m_BallX+=m_BallDx;
	}

	//отражение движения мяча по оси Y
    if (m_BallY < 0) 
	{
		//отражение движения мяча по оси Y
		m_BallDy=-m_BallDy;

		//обновляем позицию мяча
		m_BallY+=m_BallDy;
	}
	else if (m_BallY > (WINDOW_HEIGHT - BALL_SIZE))
	{
		//отражение движения мяча по оси Y
		m_BallDy=-m_BallDy;

		//обновляем позицию мяча
		m_BallY+=m_BallDy;

		//снимаем очки с игрока если пропущен мяч
		m_Score -= 10;

		if(m_Score < 0)
		{
			m_Score = 0;
		}
      }
    
	//проверяем что бы скорость движения мяча не была слишком большой
    if (m_BallDx > 8)
	{
		m_BallDx = 8;
	}
    else if (m_BallDx < -8)
	{
		m_BallDx = -8;    
	}

    //тестируем попадание мяча в клюшку, и в секцию блоков
    Process_Ball();

	//рисуем клюшку
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(m_hBackBuffer, hBrush);

	Rectangle( m_hBackBuffer, m_PaddleX, m_PaddleY, 
                   m_PaddleX+PADDLE_WIDTH, 
                   m_PaddleY+PADDLE_HEIGHT );

	SelectObject(m_hBackBuffer, hOldBrush);
	DeleteObject(hBrush);

	//рисуем мяч
	hBrush = CreateSolidBrush(RGB(0, 255, 0));
	hOldBrush = (HBRUSH)SelectObject(m_hBackBuffer, hBrush);

	Ellipse( m_hBackBuffer, m_BallX, m_BallY, m_BallX+BALL_SIZE, m_BallY+BALL_SIZE );

	SelectObject(m_hBackBuffer, hOldBrush);
	DeleteObject(hBrush);

	//печатаем score в заголовке окна
	char buff[256];
	sprintf_s(buff, 256, "FREAKOUT by Andre' LaMothe (с) 1999 Remastered - SCORE %d", m_Score);
	SetWindowText(m_hWnd, buff);

	Present_BackBuffer();

}

void CMyApp::Destroy()
{
	
	Delete_BackBuffer();

	if ( m_hWnd ) DestroyWindow( m_hWnd );
    m_hWnd          = NULL;

	UnregisterClass(CLASSNAME, m_hInstance);
	

}

void CMyApp::Create_BackBuffer()
{

	m_hDC = ::GetDC(m_hWnd);
	GetClientRect(m_hWnd, &m_Rc);

	m_hBackBuffer = ::CreateCompatibleDC(m_hDC);
	m_hBmpBackBuffer = CreateCompatibleBitmap(m_hDC, m_Rc.right, m_Rc.bottom);
	m_hOldBmp = (HBITMAP)::SelectObject(m_hBackBuffer, m_hBmpBackBuffer);

}

void CMyApp::Present_BackBuffer()
{
	BitBlt(m_hDC, 0, 0, m_Rc.right, m_Rc.bottom, m_hBackBuffer, 0, 0, SRCCOPY);
}

void CMyApp::Delete_BackBuffer()
{
	::SelectObject(m_hBackBuffer, m_hOldBmp);
	::DeleteObject(m_hBmpBackBuffer);
	::DeleteDC(m_hBackBuffer);
	::ReleaseDC(m_hWnd, m_hDC);

}

