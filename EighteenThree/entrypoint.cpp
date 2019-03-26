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

	float pos[2] = { 0.0f, 1.0f };
	float color[4] = { 1.0f, 0.3f, 0.3f, 1.0f };
	float width = 1.0f;
	float height = 1.0f;
	Surface *surface1 = new Surface(DirectX::XMFLOAT2(pos), width, height, color);
	pdx->CreateSurface(surface1);
	memcpy(&app.ListSurface[0],surface1,sizeof(Surface));

	float pos2[2] = { 0.0f, 2.0f };
	float color2[4] = { 0.3f, 0.3f, 1.0f, 1.0f };
	float width2 = 1.0f;
	float height2 = 1.0f; 
	Surface *surface2 = new Surface(DirectX::XMFLOAT2(pos2), width2, height2, color2);
	pdx->CreateSurface(surface2);
	memcpy(&app.ListSurface[1],surface2,sizeof(Surface));

	float pos3[2] = { 0.0f, 3.0f };
	float color3[4] = { 0.3f, 1.0f, 0.3f, 1.0f };
	float width3 = 1.0f;
	float height3 = 1.0f; 
	Surface *surface3 = new Surface(DirectX::XMFLOAT2(pos3), width3, height3, color3);
	pdx->CreateSurface(surface3);
	memcpy(&app.ListSurface[2],surface3,sizeof(Surface));

	pdx->SetVB();
	pdx->SetIB();

	app.MsgPump(pdx);

	
}