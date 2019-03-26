#ifndef UI_H
#define UI_H

#include <Windows.h>
#include <DirectXMath.h>

#include <d3d11_1.h>

//const DirectX::XMFLOAT3 v[4] = 
//{
//	DirectX::XMFLOAT3( 0.5f, 0.5f, 0.0f ),
//	DirectX::XMFLOAT3( 0.5f, -0.5f, 0.0f ),
//	DirectX::XMFLOAT3( -0.5f, -0.5f, 0.0f ),
//	DirectX::XMFLOAT3( -0.5f, 0.5f, 0.0f ),
//};

const WORD indices[6] =
{
	0,1,2,
    0,2,3,
};

struct CB_Color
{
	DirectX::XMFLOAT4 color;
};

#define SURFACEVERTEXCOUNT 4
#define SURFACEINDEXCOUNT 6

class Surface
{
public:
	Surface();
	Surface(DirectX::XMFLOAT2 pos, float width, float height, float col[4]);
	~Surface();

//private:
	DirectX::XMFLOAT2 position; // top left of rect
	float width;
	float height;
	float color[4];

	DirectX::XMFLOAT3 vertices[4];

	ID3D11VertexShader *pVertexShader;
	ID3D11PixelShader *pPixelShader;
	ID3D11InputLayout *pVertexLayout;
	ID3D11Buffer *pCB_Color;

	UINT startIndex;
	int baseVertex;

};

#endif // UI_H