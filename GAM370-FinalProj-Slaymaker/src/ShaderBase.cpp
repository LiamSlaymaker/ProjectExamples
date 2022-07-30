#include "ShaderBase.h"

#include <d3d11.h>
#include "d3dUtil.h"
#include <d3dcompiler.h>
#include <string>
#include <assert.h>

ShaderBase::ShaderBase(ID3D11Device* device, WCHAR* filename, LPCSTR vsModel, LPCSTR psModel)
	: mDevice(device)
{
	assert(device != nullptr);

	mContext = nullptr;

	mpVertexShader = nullptr;
	mpPixelShader = nullptr;
	mpVertexLayout = nullptr;

	privBuildShaders(filename, vsModel, psModel);
}

ID3D11Device* ShaderBase::GetDevice() 
{ 
	assert(mDevice != nullptr && "ERROR: Device not set.");
	return mDevice; 
}

ID3D11DeviceContext* ShaderBase::GetContext() 
{ 
	assert(mContext != nullptr && "ERROR: Context not set.");
	return mContext; 
}

ShaderBase::~ShaderBase()
{ 
	ReleaseAndDeleteCOMobject(pVSBlob);
	ReleaseAndDeleteCOMobject(pPSBlob);
	ReleaseAndDeleteCOMobject(mpVertexShader);
	ReleaseAndDeleteCOMobject(mpPixelShader);
	ReleaseAndDeleteCOMobject(mpVertexLayout);
}  

// Compile and send the VS and PS shaders to the GPU
void ShaderBase::privBuildShaders(WCHAR* filename, LPCSTR vsModel, LPCSTR psModel)
{
	pVSBlob = nullptr;
	HRESULT hr = CompileShaderFromFile( filename, "VS", vsModel, &pVSBlob);
	assert(SUCCEEDED(hr));

	hr = mDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &mpVertexShader);
	assert(SUCCEEDED(hr));

	pPSBlob = nullptr;
	hr = CompileShaderFromFile(filename, "PS", psModel, &pPSBlob);
	assert(SUCCEEDED(hr));

	// Create the pixel shader
	hr = mDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mpPixelShader);
	assert(SUCCEEDED(hr));
} 

void ShaderBase::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layoutdesc, UINT size)
{
	// Create the input layout
	HRESULT hr = mDevice->CreateInputLayout(layoutdesc, size, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &mpVertexLayout);
	assert(SUCCEEDED(hr));
}

// Set the point to the current DX context
void ShaderBase::SetContext(ID3D11DeviceContext* devcon)
{
	assert(devcon != nullptr);
	mContext = devcon;
}

// Sets the DX context to use these VS, PS and input layout
void ShaderBase::SetToContext_VS_PS_InputLayout()
{
	assert( mContext != nullptr && "ERROR: Context not set.");
	mContext->VSSetShader(mpVertexShader, nullptr, 0);
	mContext->PSSetShader(mpPixelShader, nullptr, 0);
	mContext->IASetInputLayout(mpVertexLayout);
}

HRESULT ShaderBase::CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
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
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}
