#ifndef APP_H
#define APP_H

#include <Windows.h>
#include "dx.h"

enum GAMESTATE{STARTED,RUNNING,PAUSED,STOPPED};

class App
{
public:
	App();
	~App();

	GAMESTATE state;
	Surface ListSurface[32];

	void CreateMainWindow();
	void ShowMainWindow();
	void MsgPump(DX *pdx);
	
	HWND GetHWND() const { return hWnd; }

private:
	HINSTANCE hInstance;
	HWND hWnd;
	MSG msg;

	static LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

};

#endif // APP_H