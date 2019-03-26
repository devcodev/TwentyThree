#include "app.h"

App::App()
{
	state=GAMESTATE::STARTED;

}

App::~App()
{

}

void App::CreateMainWindow()
{
	WNDCLASSEX wcex={0};
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.lpfnWndProc = WndProc;
	wcex.hInstance = GetModuleHandle(NULL);
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszClassName = L"classname";
    RegisterClassEx( &wcex );
    hWnd = CreateWindow( L"classname", L"193",
                           WS_OVERLAPPED,
						   CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, wcex.hInstance,
                           this );
}

void App::ShowMainWindow()
{
	if(hWnd) ShowWindow(hWnd,SW_SHOWDEFAULT);
}

LRESULT App::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	App *pThis = (App*)GetWindowLongPtr(hWnd,GWLP_USERDATA);
	PAINTSTRUCT ps;
	HDC hdc;
	switch( message )
	{
	case WM_CREATE:
		SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR) ((CREATESTRUCT*)lParam)->lpCreateParams);
		break;
	case WM_PAINT:
		hdc = BeginPaint( hWnd, &ps );
		EndPaint( hWnd, &ps );
		break;
	case WM_CHAR:
		switch (wParam)
		{
		case 'w':
			break;
		case 's':
			break;
		case 'a':
			break;
		case 'd':
			break;
		}
	case WM_KEYDOWN: 
		switch (wParam) 
		{ 
		case VK_LEFT:
			break;
		case VK_RIGHT:
			break;
		case VK_UP:
			break;
		case VK_DOWN:
			break;
		}
	case WM_LBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_RBUTTONUP:
		break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}

void App::MsgPump(DX *pdx)
{
	if(state!=GAMESTATE::PAUSED){
		while(WM_QUIT!=msg.message){
			if(PeekMessage(&msg,hWnd,0,0,PM_REMOVE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else{
				float bg_color[4] = { 0.2f,0.2f,0.2f,1.0f };
				pdx->ClearScreen(bg_color);

				pdx->DrawSurface(&ListSurface[0]);
				pdx->DrawSurface(&ListSurface[1]);
				pdx->DrawSurface(&ListSurface[2]);
				
				pdx->Present();
			}
		}
	}
}