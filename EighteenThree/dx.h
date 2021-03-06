#ifndef DX_H
#define DX_H

#include <d3d11_1.h>
#pragma comment(lib,"d3d11.lib")

#include "ui.h"

class DX
{
public:
	DX(HWND hWnd);
	~DX();

	HRESULT CreateD3D11DeviceAndImmediateContext();
	HRESULT CreateD3D11DeviceDeferredContext(ID3D11DeviceContext *pDeferredContext);
	HRESULT CheckMTSupport();
	HRESULT CreateSwapChain();
	HRESULT CreateRTVAndDSV(); // create render target view + depth stencil view + set OM setrendertargets
	void SetViewport();
	void SetWVP();
	HRESULT CreateCB_WVP();
	void ClearScreen(float color[4]);
	void Present();
	
	// 
	void AddVertices(void *vertices, UINT cntVertices);
	HRESULT SetVB();
	void AddIndices(WORD *indices, UINT cntIndices);
	HRESULT SetIB();
	HRESULT SetPixelShader( const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3D11PixelShader **pPixelShader );
	void SetTopo(D3D_PRIMITIVE_TOPOLOGY topo);
	void SetCBVS();

	// surface
	HRESULT CreateSurface(Surface *surface);
	HRESULT SetSurfaceShadersAndLayout( const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, Surface *surface);
	void DrawSurface(Surface *surface);
	HRESULT CreateCB_Color(Surface *surface);

private:
	HWND hWnd;
	UINT wndWidth;
	UINT wndHeight;

	ID3D11Device *pDevice;
	ID3D11Device1 *pDevice1;
	ID3D11DeviceContext *pImmediateContext;
	ID3D11DeviceContext1 *pImmediateContext1;
	IDXGISwapChain *pSwapChain;
	IDXGISwapChain1 *pSwapChain1;
	ID3D11RenderTargetView *pRenderTargetView;
	ID3D11Texture2D *pDepthStencil;
	ID3D11DepthStencilView *pDepthStencilView;
	ID3D11Buffer *pConstantBuffer;

	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;

	UINT cVertices;
	DirectX::XMFLOAT3 vData[4096];
	ID3D11Buffer *pVertexBuffer;
	UINT cIndices;
	WORD iData[4096];
	ID3D11Buffer *pIndexBuffer;


};

#endif // DX_H