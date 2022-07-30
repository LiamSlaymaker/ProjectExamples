//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

struct VertexData
{
	float4 Pos : POSITION;
};

struct PixelData
{
	float4 Pos : SV_POSITION;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PixelData VS( VertexData inVert )
{
    return inVert;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PixelData inPix ) : SV_Target
{
    return float4( 1.0f, 1.0f, 0.0f, 1.0f );    // Yellow, with Alpha = 1

	/*
	float red = (inPix.Pos.x - 200) / 400;
	float green = (inPix.Pos.y - 150) / 300;
	return float4(red, green, 0, 1);
	//*/
}
