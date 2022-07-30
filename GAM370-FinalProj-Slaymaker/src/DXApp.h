// DXApp
// Andre Berthiaume, June 2016
// Note: DX 11 SDK https://www.microsoft.com/en-us/download/details.aspx?id=6812
// Note on weird stuff with swap chain (1s and 2s) https://msdn.microsoft.com/en-us/library/windows/desktop/jj863687(v=vs.85).aspx

#ifndef _DXApp
#define _DXApp

#include <d3d11.h>
#include "d3dUtil.h"
#include "Align16.h"
#include "GameTimer.h"
#include <string>

// New includes for demo
#include "Vect.h"
#include "Matrix.h"
#include "Camera.h"
#include "ShaderColor.h"
#include "ShaderColorLight.h"
#include "ShaderTexture.h"
#include "ShaderTextureLight.h"

class Model;
class Texture;
class FlatPlane;
class TerrainModel;
class GraphicObject_Color;
class ShaderTexture;
class GraphicObject_Texture;
class GraphicObject_ColorLight;
class GraphicObject_TextureLight;
class Terrain;
class SkyBox;

class DXApp : public Align16
{
private:	
	// Main window handle
	HWND      mhMainWnd;

	Vect BackgroundColor;

	// DX application elements
	ID3D11Device* md3dDevice;					// Connects to the graphics card
	ID3D11DeviceContext* md3dImmediateContext;	// Settings for the GPU to use
	IDXGISwapChain* mSwapChain;					// image buffers used for rendering
	ID3D11RenderTargetView* mRenderTargetView;	// Where to send rendring operations (typically: points to one of the swap buffers)
	ID3D11DepthStencilView* mpDepthStencilView; // Needed to force depth-buffer operations

	GameTimer mTimer;
	std::string mMainWndCaption;
	int mClientWidth;
	int mClientHeight;

	void InitDirect3D();
	void InitDemo();
	void UpdateScene();
	void DrawScene();
	void CalculateFrameStats();

	// Shader loading utility. SHould be moved elsewhere later...
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	// Debug utility
	void ReportLiveDXObjects();

	// Demo specific additions
	Camera mCam;

	Model* pModSword;
	Model* pModTree;
	Model* pModPine;
	Model* pModRock1;
	Model* pModPedastal;
	Model* pModWhiteTree;
	Model* pModGhost;
	Model* pModLBug;
	TerrainModel* pTerrain;
	SkyBox* pSky;

	Matrix mWorld1;	
	Matrix mWorld2;
	Matrix mWorld3;
	Matrix mWorldSword;
	Matrix mWorldTree;
	Matrix mWorldWhiteTree;
	Matrix mWorldPine;
	Matrix mWorldRock1;
	Matrix mWorldPedastal;
	Matrix mWorldGhost;
	Matrix mWorldLBug;
	Matrix mWorldSky;
	Matrix mWorldT;
	
	Vect pos2;

	Texture* pTexPlane;
	Texture* pTexSky;
	Texture* pTexSword;
	Texture* pTexTree;
	Texture* pTexWhiteTree;
	Texture* pTexGhost;
	Texture* pTexRock1;
	Texture* pTexPedastal;

	ShaderColor* pShaderCol;
	ShaderColorLight* pShaderColLight;
	ShaderTextureLight* pShaderTexLight;
	ShaderTexture*			pShaderTex;

	//GraphicObject_Color* GraphObj1;
	//GraphicObject_Color* GraphObj2;
	GraphicObject_Color* GraphObjLBug;
	GraphicObject_ColorLight*	GraphObjPine;
	//GraphicObject_ColorLight*	GraphObj2;
	GraphicObject_TextureLight* GraphObjTree;
	GraphicObject_TextureLight*	GraphObjWhiteTree;
	GraphicObject_TextureLight* GraphObjSword;
	GraphicObject_TextureLight* GraphObjRock1;
	GraphicObject_TextureLight* GraphObjPedastal;
	GraphicObject_TextureLight* GraphObjGhost;
	//GraphicObject_TextureLight*	GraphObj2;
	//GraphicObject_Texture*	GraphObj1;
	//GraphicObject_Texture*	GraphObj2;
	Terrain* TerrainObj;

	//Texture*				pTex1;
	//Texture*				pTex2;

	ID3D11BlendState* TransparentBS;

public:
	DXApp(HWND hwnd);
	virtual ~DXApp();
	
	void FrameTick();

	// overrides for handling mouse input.
	void OnMouseDown(WPARAM btnState, int xval, int yval);
	void OnMouseUp(WPARAM btnState, int xval, int yval);
	void OnMouseMove(WPARAM btnState, int xval, int yval);
	void OnMouseWheel(short delta);


};

#endif _DXApp