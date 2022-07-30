
cbuffer bufColor
{
	float4 ActiveCol;
}

// Vertex Shader
float4 VS(float4 Pos : POSITION) : SV_POSITION
{
	return Pos;
}

// Pixel Shader
float4 PS(float4 pix : SV_POSITION) : SV_Target
{
	return ActiveCol;
}