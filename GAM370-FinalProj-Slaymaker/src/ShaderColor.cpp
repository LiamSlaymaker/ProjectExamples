#include "ShaderColor.h"
#include "d3dUtil.h"
#include <d3d11.h>
#include <assert.h>


ShaderColor::ShaderColor(ID3D11Device* device)
	: ShaderBase(device, L"../Assets/Shaders/ColorSelected3D.hlsl")
{
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);  
	this->CreateInputLayout(layout, numElements);

	HRESULT hr;  

	// View Projection buffer
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CamMatrices);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferCamMatrices);
	assert(SUCCEEDED(hr));

	// Color buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Data_WorldColor);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBuffWordColor);
	assert(SUCCEEDED(hr));

}

ShaderColor::~ShaderColor()
{
	ReleaseAndDeleteCOMobject(mpBuffWordColor);
	ReleaseAndDeleteCOMobject(mpBufferCamMatrices);
}

void ShaderColor::SendCamMatrices( const Matrix& view, const Matrix& proj)
{
	CamMatrices	mCamMatrices;
	mCamMatrices.View = view;  
	mCamMatrices.Projection = proj;

	this->GetContext()->UpdateSubresource(mpBufferCamMatrices, 0, nullptr, &mCamMatrices, 0, 0);
}

void ShaderColor::SendWorldColor(const Matrix& world, const Vect& col)
{
	Data_WorldColor wc;
	wc.World = world;
	wc.Color = col;

	this->GetContext()->UpdateSubresource(mpBuffWordColor, 0, nullptr, &wc, 0, 0);
}

void ShaderColor::SetToContext(ID3D11DeviceContext* devcon)
{
	ShaderBase::SetContext(devcon);
	ShaderBase::SetToContext_VS_PS_InputLayout();

	devcon->VSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	devcon->VSSetConstantBuffers(1, 1, &mpBuffWordColor);
}


