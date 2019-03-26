#include "ui.h"

Surface::Surface()
	:startIndex(0),
	baseVertex(0)
{

}

Surface::Surface(DirectX::XMFLOAT2 pos, float width, float height,  float col[4])
	:startIndex(0),
	baseVertex(0)
{
	// position is top left of rect
	position = pos;

	color[0] = col[0];
	color[1] = col[1];
	color[2] = col[2];
	color[3] = col[3];

	/*
		-0.5,0.5	0.5,0.5
		-0.5,-0.5	0.5,-0.5
	*/

	vertices[0].x = pos.x; vertices[0].y = pos.y; vertices[0].z = 0.0f;
	vertices[1].x = pos.x + width; vertices[1].y = pos.y; vertices[1].z = 0.0f;
	vertices[2].x = pos.x + width; vertices[2].y = pos.y - height; vertices[2].z = 0.0f;
	vertices[3].x = pos.x; vertices[3].y = pos.y - height; vertices[3].z = 0.0f;

}

Surface::~Surface()
{

}
