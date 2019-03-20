#include "dx.h"

struct CB_WVP
{
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
};

DX::DX(HWND hWnd)
	:hWnd(hWnd)
{

}

DX::~DX()
{

}

HRESULT DX::CreateD3D11DeviceAndImmediateContext()
{
	HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( hWnd, &rc );
    wndWidth = rc.right - rc.left;
    wndHeight = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        D3D_DRIVER_TYPE driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDevice( nullptr, driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
                                D3D11_SDK_VERSION, &pDevice, &featureLevel, &pImmediateContext );

        if ( hr == E_INVALIDARG )
        {
            // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
            hr = D3D11CreateDevice( nullptr, driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                                    D3D11_SDK_VERSION, &pDevice, &featureLevel, &pImmediateContext );
        }

        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

	return hr;
}

HRESULT DX::CreateD3D11DeviceDeferredContext(ID3D11DeviceContext *pDeferredContext)
{
	return pDevice->CreateDeferredContext(0, &pDeferredContext);
}

HRESULT DX::CheckMTSupport()
{
	D3D11_FEATURE_DATA_THREADING fdth;
	return pDevice->CheckFeatureSupport(D3D11_FEATURE_THREADING, &fdth, sizeof(D3D11_FEATURE_DATA_THREADING));
}

HRESULT DX::CreateSwapChain()
{
	HRESULT hr = S_OK;

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
    IDXGIFactory1* dxgiFactory = nullptr;
    {
        IDXGIDevice* dxgiDevice = nullptr;
        hr = pDevice->QueryInterface( __uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice) );
        if (SUCCEEDED(hr))
        {
            IDXGIAdapter* adapter = nullptr;
            hr = dxgiDevice->GetAdapter(&adapter);
            if (SUCCEEDED(hr))
            {
                hr = adapter->GetParent( __uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory) );
                adapter->Release();
            }
            dxgiDevice->Release();
        }
    }
    if (FAILED(hr))
        return hr;

    // Create swap chain
    IDXGIFactory2* dxgiFactory2 = nullptr;
    hr = dxgiFactory->QueryInterface( __uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2) );
    if ( dxgiFactory2 )
    {
        // DirectX 11.1 or later
        hr = pDevice->QueryInterface( __uuidof(ID3D11Device1), reinterpret_cast<void**>(&pDevice1) );
        if (SUCCEEDED(hr))
        {
            (void) pImmediateContext->QueryInterface( __uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&pImmediateContext1) );
        }

        DXGI_SWAP_CHAIN_DESC1 sd = {};
        sd.Width = wndWidth;
        sd.Height = wndHeight;
        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;

        hr = dxgiFactory2->CreateSwapChainForHwnd( pDevice, hWnd, &sd, nullptr, nullptr, &pSwapChain1 );
        if (SUCCEEDED(hr))
        {
            hr = pSwapChain1->QueryInterface( __uuidof(IDXGISwapChain), reinterpret_cast<void**>(&pSwapChain) );
        }

        dxgiFactory2->Release();
    }
    else
    {
        // DirectX 11.0 systems
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 1;
        sd.BufferDesc.Width = wndWidth;
        sd.BufferDesc.Height = wndHeight;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;

        hr = dxgiFactory->CreateSwapChain( pDevice, &sd, &pSwapChain );
    }

    // Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
    dxgiFactory->MakeWindowAssociation( hWnd, DXGI_MWA_NO_ALT_ENTER );

    dxgiFactory->Release();

    if (FAILED(hr))
        return hr;

	return hr;
}

HRESULT DX::CreateRTVAndDSV()
{
	HRESULT hr = S_OK;



	// Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &pBackBuffer ) );
    if( FAILED( hr ) )
        return hr;

    hr = pDevice->CreateRenderTargetView( pBackBuffer, nullptr, &pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;

    pImmediateContext->OMSetRenderTargets( 1, &pRenderTargetView, nullptr );


	//// Create a render target view
 //   ID3D11Texture2D* pBackBuffer = nullptr;
 //   hr = pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &pBackBuffer ) );
 //   if( FAILED( hr ) )
 //       return hr;

 //   hr = pDevice->CreateRenderTargetView( pBackBuffer, nullptr, &pRenderTargetView );
 //   pBackBuffer->Release();
 //   if( FAILED( hr ) )
 //       return hr;

	//// Create depth stencil texture
 //   D3D11_TEXTURE2D_DESC descDepth = {};
 //   descDepth.Width = wndWidth;
 //   descDepth.Height = wndHeight;
 //   descDepth.MipLevels = 1;
 //   descDepth.ArraySize = 1;
 //   descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
 //   descDepth.SampleDesc.Count = 1;
 //   descDepth.SampleDesc.Quality = 0;
 //   descDepth.Usage = D3D11_USAGE_DEFAULT;
 //   descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
 //   descDepth.CPUAccessFlags = 0;
 //   descDepth.MiscFlags = 0;
 //   hr = pDevice->CreateTexture2D( &descDepth, nullptr, &pDepthStencil );
 //   if( FAILED( hr ) )
 //       return hr;

 //   // Create the depth stencil view
 //   D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
 //   descDSV.Format = descDepth.Format;
 //   descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
 //   descDSV.Texture2D.MipSlice = 0;
 //   hr = pDevice->CreateDepthStencilView( pDepthStencil, &descDSV, &pDepthStencilView );
 //   if( FAILED( hr ) )
 //       return hr;

	//pImmediateContext->OMSetRenderTargets( 1, &pRenderTargetView, pDepthStencilView );

	return hr;
}

void DX::SetViewport()
{
	// Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)wndWidth;
    vp.Height = (FLOAT)wndHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    pImmediateContext->RSSetViewports( 1, &vp );
}

void DX::SetWVP()
{
	// Initialize the model / world matrix
	world = DirectX::XMMatrixIdentity();

    // Initialize the view matrix
	DirectX::XMVECTOR Eye = DirectX::XMVectorSet( 0.0f, 1.0f, -5.0f, 0.0f );
	DirectX::XMVECTOR At = DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	DirectX::XMVECTOR Up = DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	view = DirectX::XMMatrixLookAtLH( Eye, At, Up );

    // Initialize the projection matrix
	projection = DirectX::XMMatrixPerspectiveFovLH( DirectX::XM_PIDIV2, wndWidth / (FLOAT)wndHeight, 0.01f, 100.0f );
}

HRESULT DX::CreateCB_WVP()
{
	HRESULT hr = S_OK;

	// Create the constant buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CB_WVP);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
    hr = pDevice->CreateBuffer( &bd, nullptr, &pConstantBuffer );
    
	if( FAILED( hr ) )
        return hr;

	return hr;
}

HRESULT DX::CreateCB_Color(Surface *surface)
{
	HRESULT hr = S_OK;

	// Create the constant buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CB_Color);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pDevice->CreateBuffer( &bd, nullptr, &surface->pCB_Color );
    
	if( FAILED( hr ) )
        return hr;

	return hr;
}


#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
HRESULT CompileShaderFromFile( const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;

    // Disable optimizations to further improve shader debugging
    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ID3DBlob* pErrorBlob = nullptr;
    hr = D3DCompileFromFile( szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob );
    if( FAILED(hr) )
    {
        if( pErrorBlob )
        {
            OutputDebugStringA( reinterpret_cast<const char*>( pErrorBlob->GetBufferPointer() ) );
            pErrorBlob->Release();
        }
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}

HRESULT DX::CreateSurface(Surface *surface)
{
	HRESULT hr = S_OK;

		// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
    hr = CompileShaderFromFile( L"surface_basic.hlsl", "main", "vs_4_0", &pVSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( nullptr,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// Create the vertex shader
	hr = pDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &surface->pVertexShader );
	if( FAILED( hr ) )
	{	
		pVSBlob->Release();
        return hr;
	}

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        //{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE( layout );

    // Create the input layout
	hr = pDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),
                                          pVSBlob->GetBufferSize(), &surface->pVertexLayout );
	pVSBlob->Release();
	if( FAILED( hr ) )
        return hr;

    // Set the input layout
    pImmediateContext->IASetInputLayout( surface->pVertexLayout );

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
    hr = CompileShaderFromFile( L"surface_basic_PS.hlsl", "main", "ps_4_0", &pPSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( nullptr,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// Create the pixel shader
	hr = pDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &surface->pPixelShader );
	pPSBlob->Release();
    if( FAILED( hr ) )
        return hr;

	D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( DirectX::XMFLOAT3 ) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = surface->vertices;
	hr = pDevice->CreateBuffer( &bd, &InitData, &surface->pVertexBuffer );
    if( FAILED( hr ) )
        return hr;

    // Set vertex buffer
    UINT stride = sizeof( DirectX::XMFLOAT3 );
    UINT offset = 0;
    pImmediateContext->IASetVertexBuffers( 0, 1, &surface->pVertexBuffer, &stride, &offset );

	bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( WORD ) * 6;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
    InitData.pSysMem = indices;
    hr = pDevice->CreateBuffer( &bd, &InitData, &surface->pIndexBuffer );
    if( FAILED( hr ) )
        return hr;

    // Set index buffer
    pImmediateContext->IASetIndexBuffer( surface->pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );

	return hr;
}

void DX::DrawSurface(Surface *surface)
{
	CB_WVP cbwvp;
	cbwvp.mWorld = XMMatrixTranspose( world );
	cbwvp.mView = XMMatrixTranspose( view );
	cbwvp.mProjection = XMMatrixTranspose( projection );
	pImmediateContext->UpdateSubresource( pConstantBuffer, 0, nullptr, &cbwvp, 0, 0 );

	CB_Color cbcolor;
	cbcolor.color = DirectX::XMFLOAT4(surface->color);

	// Clear the back buffer 
	pImmediateContext->ClearRenderTargetView( pRenderTargetView, surface->color );

	pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    // Render a triangle
	pImmediateContext->VSSetShader( surface->pVertexShader, nullptr, 0 );
	pImmediateContext->VSSetConstantBuffers( 0, 1, &pConstantBuffer );
	pImmediateContext->PSSetShader( surface->pPixelShader, nullptr, 0 );
	pImmediateContext->PSSetConstantBuffers( 0, 1, &surface->pCB_Color );
    pImmediateContext->DrawIndexed( 6, 0, 0 );
    //pImmediateContext->Draw(4,0);

    // Present the information rendered to the back buffer to the front buffer (the screen)
    pSwapChain->Present( 0, 0 );
}

void DX::ClearScreen(float color[4])
{
	// Clear the back buffer 
	pImmediateContext->ClearRenderTargetView( pRenderTargetView, color );

	// Present the information rendered to the back buffer to the front buffer (the screen)
	pSwapChain->Present( 0, 0 );
}