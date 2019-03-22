cbuffer cbCol
{
	float4 color;
}

float4 main() : SV_TARGET
{
	return /*float4(1.0f, 0.2f, 0.0f, 1.0f);*/color;
}