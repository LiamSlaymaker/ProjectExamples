#include "DXApp.h"
#include <windows.h>
#include <sstream>
#include <assert.h>
#include "d3dUtil.h"

// needed to load shaders from file
#include <d3dcompiler.h>

#include "Model.h"
#include "ShaderColorLight.h"
#include "DirectXTex.h"
#include "Texture.h"
#include "FlatPlane.h"
#include "TerrainModel.h"
#include "GraphicObject_Color.h"
#include "GraphicObject_ColorLight.h"
#include "GraphicObject_Texture.h"
#include "GraphicObject_TextureLight.h"
#include "Terrain.h"
#include "SkyBox.h"

void DXApp::InitDemo()
{
	pShaderTexLight = new ShaderTextureLight(md3dDevice);
	pShaderColLight = new ShaderColorLight(md3dDevice);
	pShaderTex = new ShaderTexture(md3dDevice);
	pShaderCol = new ShaderColor(md3dDevice);

	pShaderTexLight->SetDirectionalLightParameters(Vect(-1, -1, 1).getNorm(),
		Vect(0.1f, 0.1f, 0.2f), Vect(0.1f, 0.1f, 0.2f), .5 * Vect(1, 1, 1));
	
	pShaderColLight->SetDirectionalLightParameters(Vect(-1, -1, 1).getNorm(),
		Vect(0.1f, 0.1f, 0.2f), Vect(0.1f, 0.1f, 0.2f), .5 * Vect(1, 1, 1));


	pShaderTexLight->SetSpotLightParameters(Vect(-10,58,90), 100, .5 * Vect(1, 0, 0),
		Vect(-40, 8, 60) -Vect(-10, 58, 90), 50,
		0.5*Colors::LightYellow, 0.5*Colors::LightYellow, 1 * Vect(1, 1, 1, 150));

	pShaderColLight->SetSpotLightParameters(Vect(-10, 58, 90), 100, .5 * Vect(1, 0, 0),
		Vect(-40, 8, 60) - Vect(-10, 58, 90), 50,
		0.5 * Colors::LightYellow, 0.5 * Colors::LightYellow, 1 * Vect(1, 1, 1, 150));


	//Create Textures
	pTexPlane = new Texture(md3dDevice, L"../Assets/Textures/mountainTex.tga", D3D11_FILTER_ANISOTROPIC, 8, true);
	pTexSky = new Texture(md3dDevice, L"../Assets/Textures/SkyBox.tga", D3D11_FILTER_MIN_MAG_MIP_POINT);
	pTexSword = new Texture(md3dDevice, L"../Assets/Textures/Sword.tga", D3D11_FILTER_MIN_MAG_MIP_POINT);
	pTexRock1 = new Texture(md3dDevice, L"../Assets/Textures/rock.tga", D3D11_FILTER_MIN_MAG_MIP_POINT);
	pTexPedastal = new Texture(md3dDevice, L"../Assets/Textures/Pedastal.tga", D3D11_FILTER_MIN_MAG_MIP_POINT);
	pTexTree = new Texture(md3dDevice, L"../Assets/Textures/tree.tga", D3D11_FILTER_MIN_MAG_MIP_POINT);
	pTexWhiteTree = new Texture(md3dDevice, L"../Assets/Textures/WhiteTree.tga", D3D11_FILTER_MIN_MAG_MIP_POINT);
	pTexGhost = new Texture(md3dDevice, L"../Assets/Textures/Ghost.tga", D3D11_FILTER_MIN_MAG_MIP_POINT);
	
	//Create Models
	pModSword = new Model(md3dDevice, "../Assets/Models/Sword.azul");
	pModTree = new Model(md3dDevice, "../Assets/Models/tree.azul");
	pModWhiteTree = new Model(md3dDevice, "../Assets/Models/WhiteTree.azul");
	pModPine = new Model(md3dDevice, "../Assets/Models/Pine.azul");
	pModRock1 = new Model(md3dDevice, "../Assets/Models/Rock1.azul");
	pModPedastal = new Model(md3dDevice, "../Assets/Models/Pedastal.azul");
	pModGhost = new Model(md3dDevice, "../Assets/Models/Ghost.azul");
	pModLBug = new Model(md3dDevice, Model::PreMadeModels::UnitSphere);
	pTerrain = new TerrainModel(md3dDevice, L"../Assets/Textures/HMtest5.tga", 350, 80, 0, 8, 8);
	pSky = new SkyBox(md3dDevice, 850, pShaderTex);

	//Define World Matrices
	mWorldTree = Matrix(TRANS, -10, 0, 90);
	mWorldWhiteTree = Matrix(ROT_Y, -3.1415 / 1.8) * Matrix(TRANS, -60, 0, 55);
	mWorldSword = Matrix(ROT_X, -3.1415 / 2) * Matrix(TRANS, -40, 8, 60);
	mWorldRock1 = Matrix(ROT_X, -3.1415 / 2) * Matrix(TRANS, -7, 0, 95);
	mWorldPine = Matrix(ROT_X, -3.1415 / 2) * Matrix(TRANS, -40, 10, 85);
	mWorldGhost =  Matrix(ROT_Y, 3.1415 / 1.5) * Matrix(TRANS, -20, 4, 92);
	mWorldPedastal =  Matrix(TRANS, -40, 0, 60);
	mWorldLBug = Matrix(SCALE, .2, .2, .2);
	mWorld1 =  Matrix(IDENTITY);
	mWorld2 = Matrix(SCALE, .5f, .5f, .5f) * Matrix(TRANS, 5, 2, 4);
	mWorld3 = Matrix(IDENTITY);
	mWorldSky = Matrix(ROT_Z, -3.1415 / 2);

	//Create Objs
	GraphObjTree = new GraphicObject_TextureLight(pShaderTexLight, pModTree);
	GraphObjWhiteTree = new GraphicObject_TextureLight(pShaderTexLight, pModWhiteTree);
	GraphObjSword = new GraphicObject_TextureLight(pShaderTexLight, pModSword);
	GraphObjRock1 = new GraphicObject_TextureLight(pShaderTexLight, pModRock1);
	GraphObjPedastal = new GraphicObject_TextureLight(pShaderTexLight, pModPedastal);
	GraphObjGhost = new GraphicObject_TextureLight(pShaderTexLight, pModGhost);
	GraphObjPine = new GraphicObject_ColorLight(pShaderColLight, pModPine);
	GraphObjLBug = new GraphicObject_Color(pShaderCol, pModLBug);
	TerrainObj = new Terrain(pShaderTexLight, pTerrain);

	//Set Materials
	GraphObjTree->SetMaterial(Colors::LightBlue, Colors::LightBlue, Vect(0.2, 0.2, 0.2, 1));
	GraphObjWhiteTree->SetMaterial(Colors::LightBlue, Colors::LightBlue, Vect(0.2, 0.2, 0.2, 1));
	GraphObjSword->SetMaterial(Colors::LightBlue, Colors::LightBlue, Vect(1, 1, 1, 500));
	GraphObjRock1->SetMaterial(Colors::LightBlue, Colors::LightBlue, Vect(0.2, 0.2, 0.2, 100));
	GraphObjPedastal->SetMaterial(Colors::LightBlue, Colors::LightBlue, Vect(0.2, 0.2, 0.2, 100));
	GraphObjPine->SetMaterial(Colors::ForestGreen, Colors::ForestGreen, Vect(0.2, 0.2, 0.2, 10),0);
	GraphObjPine->SetMaterial(Colors::Brown, Colors::Brown, Vect(0.2, 0.2, 0.2, 10), 1);
	GraphObjGhost->SetMaterial(Colors::LightBlue, Colors::LightBlue, Vect(0.5, 0.5, 0.5, 100));
	GraphObjLBug->SetColor(Colors::Goldenrod);
	TerrainObj->SetMaterial(Vect(1, 1, 1), Vect(1, 1, 1), Vect(0.05f, 0.05f, 0.05f));

	//Set WorldMatrix
	GraphObjTree->SetWorld(mWorldTree);
	GraphObjWhiteTree->SetWorld(mWorldWhiteTree);
	GraphObjSword->SetWorld(mWorldSword);
	GraphObjRock1->SetWorld(mWorldRock1);
	GraphObjPedastal->SetWorld(mWorldPedastal);
	GraphObjPine->SetWorld(mWorldPine);
	GraphObjGhost->SetWorld(mWorldGhost);
	GraphObjLBug->SetWorld(mWorldLBug);
	TerrainObj->SetWorld(mWorld1);
	pSky->SetWorld(mWorldSky);
	
	//Set Textures
	GraphObjTree->SetTexture(pTexTree);
	GraphObjWhiteTree->SetTexture(pTexWhiteTree);
	GraphObjSword->SetTexture(pTexSword);
	GraphObjRock1->SetTexture(pTexRock1);
	GraphObjPedastal->SetTexture(pTexPedastal);
	GraphObjGhost->SetTexture(pTexGhost);
	TerrainObj->SetTexture(pTexPlane);
	pSky->SetTexture(pTexSky);
	
	mCam.setPerspective( 3.14159/3, mClientWidth / (float)mClientHeight, 1.0f, 1000.0f);
	mCam.setOrientAndPosition(Vect(0, 1, 0), Vect(-40, 8, 60), Vect(25, 3, 75));


	mTimer.Reset();
}

void DXApp::UpdateScene()
{
	mWorld2 *= Matrix(ROT_Y, 0.0003);
	//mWorld2 *= Matrix
	mWorld3 *= Matrix(ROT_Y, 0.004);
	pos2 = mWorld2.get(MatrixRowType::ROW_3);
	//GraphObj2->SetWorld(mWorld2);

	float camSpeed = 0.015f;
	if (GetKeyState('W') & 0x08000)
	{
		mCam.TranslateFwdBack(camSpeed);
	}
	else if (GetKeyState('S') & 0x08000)
	{
		mCam.TranslateFwdBack(-camSpeed);
	}

	if (GetKeyState('A') & 0x08000)
	{
		mCam.TranslateLeftRight(-camSpeed);
	}
	else if (GetKeyState('D') & 0x08000)
	{
		mCam.TranslateLeftRight(camSpeed);
	}

	float rotSpeed = 0.001f;
	if (GetKeyState(VK_LEFT) & 0x08000)
	{
		mCam.TurnLeftRight(rotSpeed);
	}
	else if (GetKeyState(VK_RIGHT) & 0x08000)
	{
		mCam.TurnLeftRight(-rotSpeed);
	}

	if (GetKeyState(VK_UP) & 0x08000)
	{
		mCam.TiltUpDown(rotSpeed);
	}
	else if (GetKeyState(VK_DOWN) & 0x08000)
	{
		mCam.TiltUpDown(-rotSpeed);
	}

	mCam.updateCamera();
}

void DXApp::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, VasA(BackgroundColor));
	md3dImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Plane/box display
	

	pShaderTexLight->SetToContext(md3dImmediateContext);
	pShaderTexLight->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());
	pShaderTexLight->SendFog(5, 150, 25, Vect(.85, .85, .95));

	Vect eyepos;
	mCam.getPos(eyepos);
	pShaderTexLight->SendLightParameters(eyepos);

	// Render pntlights
	Matrix LBbase = mWorldLBug * mWorld2;
	Matrix LB1 = LBbase * Matrix(TRANS, -20, 4, 92) * Matrix(TRANS, 0, 2 * cosf(mTimer.TotalTime()), 0);
	Matrix LB2 = LBbase * Matrix(TRANS, 5, 6, 50) * Matrix(TRANS, 0, 1.8 * cosf(1.2*mTimer.TotalTime()), 0);
	Matrix LB3 = LBbase * Matrix(TRANS, 10, 5, 65) * Matrix(TRANS, 0, 1 * cosf(2 * mTimer.TotalTime()), 0);

	pShaderTexLight->SetPointLightParameters(Vect(LB1.M12(), LB1.M13(), LB1.M14()), 10, 0.5 * Vect(0, 1, 0),
		0.1* Colors::Goldenrod, 0.05 * Colors::Goldenrod, .5 * Vect(1, 1, 1));

	pShaderTexLight->SetPointLightParameters2(Vect(LB2.M12(), LB2.M13(), LB2.M14()), 10, 0.5 * Vect(0, 1, 0),
		0.1 * Colors::Goldenrod, 0.05 * Colors::Goldenrod, .5 * Vect(1, 1, 1));

	pShaderTexLight->SetPointLightParameters3(Vect(LB3.M12(), LB3.M13(), LB3.M14()), 10, 0.5 * Vect(0, 1, 0),
		0.1 * Colors::Goldenrod, 0.05 * Colors::Goldenrod, .5 * Vect(1, 1, 1));

	// --- Render Object #1;


	GraphObjTree->SetWorld(mWorldTree);
	GraphObjTree->Render();
	GraphObjTree->SetWorld(mWorldTree * Matrix(TRANS, 20, 0, -25));
	GraphObjTree->Render();

	GraphObjWhiteTree->Render();
	GraphObjSword->SetWorld( Matrix(ROT_X, -3.1415 / 2) * mWorld3 * Matrix(TRANS, -40, 8, 60) );
	GraphObjSword->Render();
	GraphObjRock1->Render();
	GraphObjGhost->Render();
	GraphObjPedastal->Render();
	
	TerrainObj->Render();


//ColLight Renders
	pShaderColLight->SetToContext(md3dImmediateContext);
	pShaderColLight->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());
	pShaderColLight->SendFog(5, 150, 25, Vect(.85, .85, .95));

	mCam.getPos(eyepos);
	pShaderColLight->SendLightParameters(eyepos);

	pShaderColLight->SetPointLightParameters(Vect(LB1.M12(), LB1.M13(), LB1.M14()), 10, 0.5 * Vect(0, 1, 0),
		0.1* Colors::Goldenrod, 0.05 * Colors::Goldenrod, .5 * Vect(1, 1, 1));

	pShaderColLight->SetPointLightParameters2(Vect(LB2.M12(), LB2.M13(), LB2.M14()), 10, 0.5 * Vect(0, 1, 0),
		0.1 * Colors::Goldenrod, 0.05 * Colors::Goldenrod, .5 * Vect(1, 1, 1));

	pShaderColLight->SetPointLightParameters3(Vect(LB3.M12(), LB3.M13(), LB3.M14()), 10, 0.5 * Vect(0, 1, 0),
		0.1 * Colors::Goldenrod, 0.05 * Colors::Goldenrod, .5 * Vect(1, 1, 1));

	GraphObjPine->SetWorld(mWorldPine);
	GraphObjPine->Render();

	GraphObjPine->SetWorld(mWorldPine * Matrix(TRANS, 45, 0, -35));
	GraphObjPine->Render();

	GraphObjPine->SetWorld(mWorldPine * Matrix(TRANS, 25, 0, -33));
	GraphObjPine->Render();

	//Skybox Render
	pShaderTex->SetToContext(md3dImmediateContext);
	pShaderTex->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());
	pSky->Render(md3dImmediateContext);

	//Lighting Bugs
	pShaderCol->SetToContext(md3dImmediateContext);
	pShaderCol->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());
	GraphObjLBug->SetWorld(LB1);
	GraphObjLBug->Render();
	GraphObjLBug->SetWorld(LB2);
	GraphObjLBug->Render();
	GraphObjLBug->SetWorld(LB3);
	GraphObjLBug->Render();


	// Switches the display to show the now-finished back-buffer
	mSwapChain->Present(0, 0);
}


DXApp::DXApp(HWND hwnd)
{
	assert(hwnd);
	mhMainWnd = hwnd;

	BackgroundColor = Colors::MidnightBlue;

	md3dDevice = nullptr;
	md3dImmediateContext = nullptr;
	mSwapChain = nullptr;
	mRenderTargetView = nullptr;

	// Get window data through the window handle
	RECT rc;
	BOOL err = GetClientRect(mhMainWnd, &rc);  // Seriously MS: Redifining BOOL as int? Confusing much?
	assert(err);

	// get width/hight
	mClientWidth = rc.right - rc.left;
	mClientHeight = rc.bottom - rc.top;

	// Get window caption
	const int MAX_LABEL_LENGTH = 100; // probably overkill...
	CHAR str[MAX_LABEL_LENGTH];
	GetWindowText(mhMainWnd, str, MAX_LABEL_LENGTH);
	mMainWndCaption = str;

	// Initialize DX11
	this->InitDirect3D();

	// Demo initialization
	this->InitDemo();
}

DXApp::~DXApp()
{
	delete pModTree;
	delete pModWhiteTree;
//	delete pModel2;
	delete pModPine;
	delete pModSword;
	delete pModRock1; 
	delete pModPedastal;
	delete pModGhost;
	delete pModLBug;
	delete pSky;
	delete pTerrain;

	delete pShaderTexLight;
	delete pShaderTex;
	delete pShaderColLight;
	delete pShaderCol;

	delete GraphObjTree;
	delete GraphObjWhiteTree;
	delete GraphObjSword;
	delete GraphObjPine;
	delete GraphObjRock1;
	delete GraphObjPedastal;
	delete GraphObjGhost;
	delete GraphObjLBug;
	delete TerrainObj;
	//delete GraphObj2;

	delete pTexTree;
	delete pTexWhiteTree;
	delete pTexSword;
	delete pTexPlane;
	delete pTexRock1;
	delete pTexPedastal;
	delete pTexGhost;
	delete pTexSky;

	//delete pTex1;
	//delete pTex2;
	

	ReleaseAndDeleteCOMobject(mRenderTargetView);
	ReleaseAndDeleteCOMobject(mpDepthStencilView);
	ReleaseAndDeleteCOMobject(mSwapChain);
	ReleaseAndDeleteCOMobject(md3dImmediateContext);

	// Must be done BEFORE the device is released
	ReportLiveDXObjects();		// See http://masterkenth.com/directx-leak-debugging/

	ReleaseAndDeleteCOMobject(md3dDevice);
}

// See http://masterkenth.com/directx-leak-debugging/
void DXApp::ReportLiveDXObjects()
{
#ifdef _DEBUG
	HRESULT hr = S_OK;

	// Now we set up the Debug interface, to be queried on shutdown
	ID3D11Debug* debugDev;
	hr = md3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDev));

	debugDev->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	ReleaseAndDeleteCOMobject(debugDev);
#endif
}

void DXApp::InitDirect3D()
{
	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// This is a *greatly* simplified process to create a DX device and context:
	// We force the use of DX11 feature level since that's what CDM labs are limited to.
	// For real-life applications would need to test what's the best feature level and act accordingly
	hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, nullptr, 0, D3D11_SDK_VERSION, &md3dDevice, nullptr, &md3dImmediateContext);
	assert(SUCCEEDED(hr));

	// Now we obtain the associated DXGIfactory1 with our device 
	// Many steps...
	IDXGIDevice* dxgiDevice = nullptr;
	hr = md3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
	assert(SUCCEEDED(hr));

	IDXGIAdapter* adapter = nullptr;
	hr = dxgiDevice->GetAdapter(&adapter);
	assert(SUCCEEDED(hr));

	IDXGIFactory1* dxgiFactory1 = nullptr;
	hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory1));
	assert(SUCCEEDED(hr));
	// See also note on weird stuff with factories and swap chains (1s and 2s)
	// https://msdn.microsoft.com/en-us/library/windows/desktop/jj863687(v=vs.85).aspx

	// We are done with these now...
	ReleaseAndDeleteCOMobject(adapter);
	ReleaseAndDeleteCOMobject(dxgiDevice);

	// Controls MSAA option:
	// - 4x count level garanteed for all DX11 
	// - MUST be the same for depth buffer!
	// - We _need_ to work with the depth buffer because reasons... (see below)
	DXGI_SAMPLE_DESC sampDesc;
	sampDesc.Count = 1;
	sampDesc.Quality = static_cast<UINT>(D3D11_CENTER_MULTISAMPLE_PATTERN);  // MS: what's with the type mismtach?

	DXGI_MODE_DESC buffdesc;				// https://msdn.microsoft.com/en-us/library/windows/desktop/bb173064(v=vs.85).aspx
	ZeroMemory(&buffdesc, sizeof(buffdesc));
	buffdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Next we create a swap chain. 
	// Useful thread: http://stackoverflow.com/questions/27270504/directx-creating-the-swapchain
	// Note that this is for a DirectX 11.0: in a real app, we should test the feature levels and act accordingly

	DXGI_SWAP_CHAIN_DESC sd;				// See MSDN: https://msdn.microsoft.com/en-us/library/windows/desktop/bb173075(v=vs.85).aspx
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;						// Much confusion about this number... see http://www.gamedev.net/topic/633807-swap-chain-buffer-count/
	sd.BufferDesc = buffdesc;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = mhMainWnd;
	sd.SampleDesc = sampDesc;
	sd.Windowed = TRUE;

	hr = dxgiFactory1->CreateSwapChain(md3dDevice, &sd, &mSwapChain);
	assert(SUCCEEDED(hr));
	ReleaseAndDeleteCOMobject(dxgiFactory1);

	// Create a render target view		https://msdn.microsoft.com/en-us/library/windows/desktop/ff476582(v=vs.85).aspx
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	assert(SUCCEEDED(hr));;

	hr = md3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mRenderTargetView);
	ReleaseAndDeleteCOMobject(pBackBuffer);
	assert(SUCCEEDED(hr));

	/**********************************************************/

	// First we fix what it means for triangles to be front facing.
	// Requires setting a whole new rasterizer state
	//*
	D3D11_RASTERIZER_DESC rd;
	rd.FillMode = D3D11_FILL_SOLID;  // Also: D3D11_FILL_WIREFRAME
	rd.CullMode = D3D11_CULL_BACK;
	rd.FrontCounterClockwise = true; // true for RH forward facing
	rd.DepthBias = 0;
	rd.SlopeScaledDepthBias = 0.0f;
	rd.DepthBiasClamp = 0.0f;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = true;  // Does not in fact turn on/off multisample: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476198(v=vs.85).aspx
	rd.AntialiasedLineEnable = true;

	ID3D11RasterizerState* rs;
	md3dDevice->CreateRasterizerState(&rd, &rs);

	md3dImmediateContext->RSSetState(rs);
	ReleaseAndDeleteCOMobject(rs); // we can release this resource since we won't be changing it any further
	//*/

	// We must turn on the abilty to process depth during rendering.
	// Done through depth stencils (see https://msdn.microsoft.com/en-us/library/windows/desktop/bb205074(v=vs.85).aspx)
	// Below is a simplified version
	//*
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = mClientWidth;
	descDepth.Height = mClientHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc = sampDesc;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ID3D11Texture2D* pDepthStencil;
	hr = md3dDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	assert(SUCCEEDED(hr));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0;;

	hr = md3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &mpDepthStencilView);
	assert(SUCCEEDED(hr));
	ReleaseAndDeleteCOMobject(pDepthStencil);
	//*/

	/**********************************************************/

	//md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);  // to use without depth stencil
	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mpDepthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)mClientWidth;
	vp.Height = (FLOAT)mClientHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	md3dImmediateContext->RSSetViewports(1, &vp);
}

void DXApp::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::ostringstream outs;
		outs.precision(6);
		outs << mMainWndCaption << "    "
			<< "FPS: " << fps << "    "
			<< "Frame Time: " << mspf << " (ms)";
		SetWindowText(mhMainWnd, outs.str().c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void DXApp::FrameTick()
{
	mTimer.Tick();
	CalculateFrameStats();

	this->UpdateScene();
	this->DrawScene();
}

void DXApp::OnMouseDown(WPARAM btnState, int xval, int yval)
{
	UNREFERENCED_PARAMETER(btnState);
	UNREFERENCED_PARAMETER(xval);
	UNREFERENCED_PARAMETER(yval);
}

void DXApp::OnMouseUp(WPARAM btnState, int xval, int yval)
{
	UNREFERENCED_PARAMETER(btnState);
	UNREFERENCED_PARAMETER(xval);
	UNREFERENCED_PARAMETER(yval);
}

void DXApp::OnMouseMove(WPARAM btnState, int xval, int yval)
{
	UNREFERENCED_PARAMETER(btnState);
	UNREFERENCED_PARAMETER(xval);
	UNREFERENCED_PARAMETER(yval);
}

void  DXApp::OnMouseWheel(short delta)
{
	UNREFERENCED_PARAMETER(delta);
}


// Shader loading utility. Will be moved elsewhere later...
// Needs to be moved. Requires
HRESULT DXApp::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
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
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
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