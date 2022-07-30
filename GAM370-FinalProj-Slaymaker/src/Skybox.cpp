#include "SkyBox.h"
#include "Model.h"
#include "d3dUtil.h"

SkyBox::SkyBox(ID3D11Device* dev, float len, ShaderTexture* shader)
{
	pShader = shader;

	int nverts = 24;
	StandardVertex* pVerts = new StandardVertex[nverts];

	int ntri = 12;
	TriangleByIndex* pTriList = new TriangleByIndex[ntri];



	// Setting up faces
	// Forward
	int vind = 0;
	int tind = 0;
	pVerts[vind].set(0.5f * len, 0.5f * len, 0.5f * len, 0, 0.336f);
	pVerts[vind + 1].set(-0.5f * len, 0.5f * len, 0.5f * len, 0.25f, 0.336f);
	pVerts[vind + 2].set(-0.5f * len, -0.5f * len, 0.5f * len, 0.25f, 0.666f);
	pVerts[vind + 3].set(0.5f * len, -0.5f * len, 0.5f * len, 0, 0.666f);
	pTriList[tind].set(vind, vind + 2, vind + 1);
	pTriList[tind + 1].set(vind, vind + 3, vind + 2);

	// Back
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f * len, 0.5f * len, -0.5f * len, 0.75f, 0.335f);
	pVerts[vind + 1].set(-0.5f * len, 0.5f * len, -0.5f * len, 0.5f, 0.335f);
	pVerts[vind + 2].set(-0.5f * len, -0.5f * len, -0.5f * len, 0.5f, 0.666f);
	pVerts[vind + 3].set(0.5f * len, -0.5f * len, -0.5f * len, 0.75f, 0.666f);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Left
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f * len, 0.5f * len, -0.5f * len, 0.75f, 0.335f);
	pVerts[vind + 1].set(0.5f * len, 0.5f * len, 0.5f * len, 1, 0.335f);
	pVerts[vind + 2].set(0.5f * len, -0.5f * len, 0.5f * len, 1, 0.666f);
	pVerts[vind + 3].set(0.5f * len, -0.5f * len, -0.5f * len, 0.75f, 0.666f);
	pTriList[tind].set(vind, vind + 2, vind + 1);
	pTriList[tind + 1].set(vind, vind + 3, vind + 2);

	// Right
	vind += 4;
	tind += 2;
	pVerts[vind].set(-0.5f * len, 0.5f * len, 0.5f * len, 0.25f, 0.335f);
	pVerts[vind + 1].set(-0.5f * len, 0.5f * len, -0.5f * len, 0.5f, 0.335);
	pVerts[vind + 2].set(-0.5f * len, -0.5f * len, -0.5f * len, 0.5f, 0.666f);
	pVerts[vind + 3].set(-0.5f * len, -0.5f * len, 0.5f * len, 0.25f, 0.666f);
	pTriList[tind].set(vind, vind + 2, vind + 1);
	pTriList[tind + 1].set(vind, vind + 3, vind + 2);

	// Top
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f * len, 0.5f * len, -0.5f * len, 0.499f, 0);
	pVerts[vind + 1].set(-0.5f * len, 0.5f * len, -0.5f * len, 0.499f, 0.34f);
	pVerts[vind + 2].set(-0.5f * len, 0.5f * len, 0.5f * len, 0.25f, 0.34f);
	pVerts[vind + 3].set(0.5f * len, 0.5f * len, 0.5f * len, 0.25f, 0);
	pTriList[tind].set(vind, vind + 2, vind + 1);
	pTriList[tind + 1].set(vind, vind + 3, vind + 2);

	// Bottom
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f * len, -0.5f * len, 0.5f * len, 0.25f, 1);
	pVerts[vind + 1].set(-0.5f * len, -0.5f * len, 0.5f * len, 0.25f, 0.666f);
	pVerts[vind + 2].set(-0.5f * len, -0.5f * len, -0.5f * len, 0.5f, 0.666f);
	pVerts[vind + 3].set(0.5f * len, -0.5f * len, -0.5f * len, 0.5f, 1);
	pTriList[tind].set(vind, vind + 2, vind + 1);
	pTriList[tind + 1].set(vind, vind + 3, vind + 2);

	sBox = new Model(dev, pVerts, nverts, pTriList, ntri);

	delete[] pVerts;
	delete[] pTriList;
}


SkyBox::~SkyBox()
{
	delete sBox;
}

void SkyBox::SetTexture(Texture* tex) {
	pTex = tex;
}

void SkyBox::SetWorld(const Matrix& w) {
	World = w;
}

void SkyBox::Render(ID3D11DeviceContext* context)
{
	pShader->SendWorld(World);
	pTex->SetToContext(context);
	sBox->SetToContext(context);
	sBox->Render(context);
}