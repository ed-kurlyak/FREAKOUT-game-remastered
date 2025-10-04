# FreakOut (WinAPI Port)

This game is based on the project from the book  
**"Tricks of the Windows Game Programming Gurus, 2nd Edition" (1999)**  
by **André LaMothe** (Russian edition: _Программирование игр для Windows. Советы профессионала_).

In the original book, the author implements a simple breakout-style game called **FreakOut**, using **C++** and **DirectDraw**. The source code was located in the `T3DCHAP01` folder and targeted older versions of **Visual Studio** and **DirectDraw**, with a screen resolution of **640x480 pixels**.

## About This Version

This is a modernized reimplementation of the original **FreakOut** game using:

- **C++**
- **Pure WinAPI (no DirectDraw or DirectX)**
- **Visual Studio 2019**
- **Software backbuffering via WinAPI functions**
- **Screen resolution: 800x600 pixels**

All rendering is done using GDI functions and in-memory backbuffering, without any external libraries or dependencies.

## How to Build

1. Open the solution in **Visual Studio 2019**.
2. Build the project using or **Debug x86** configuration.
3. Run the resulting `.exe` file.

<img src="https://github.com/ed-kurlyak/freakout-remastered/blob/main/pics/freakout.png">
