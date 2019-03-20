#include "app.h"
#include "dx.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
	App app;
	app.CreateMainWindow();
	app.ShowMainWindow();
	
	DX *pdx = new DX(app.GetHWND());
	pdx->CreateD3D11DeviceAndImmediateContext();
	pdx->CreateSwapChain();
	pdx->CreateRTVAndDSV();
	pdx->CreateCB_WVP();
	pdx->SetViewport();
	pdx->SetWVP();

	float pos[2] = { 0.4f, 0.1f };
	float color[4] = { 0.8f, 0.6f, 0.5f, 1.0f };
	float width = 0.3;
	float height = 0.2;
	Surface *surface1 = new Surface(DirectX::XMFLOAT2(pos), width, height, color);
	pdx->CreateSurface(surface1);
	memcpy(&app.ListSurface[0],surface1,sizeof(Surface));

	//float pos2[2] = { -0.1f, 0.1f };
	//float color2[4] = { 0.8f, 1.0f, 0.5f, 1.0f };
	//float width2 = 0.1;
	//float height2 = 0.2; 
	//Surface *surface2 = new Surface(DirectX::XMFLOAT2(pos2), width2, height2, color2);
	//pdx->CreateSurface(surface2);
	//memcpy(&app.ListSurface[1],surface2,sizeof(Surface));

	app.MsgPump(pdx);

	
}