#include "FlatPlane.h"
#include "Model.h"
#include "d3dUtil.h"

FlatPlane::FlatPlane(ID3D11Device* dev, float size, float hrep, float vrep)
{
	int nverts = 4;
	StandardVertex* pVerts = new StandardVertex[nverts];

	int ntri = 2;
	TriangleByIndex* pTriList = new TriangleByIndex[ntri];

	float c = size / 2;

	pVerts[0].set(-c, 0, -c, hrep, vrep, 0, 1, 0);
	pVerts[1].set(-c, 0,  c, hrep, 0, 0, 1, 0);
	pVerts[2].set( c, 0,  c, 0, 0, 0, 1, 0);
	pVerts[3].set( c, 0, -c, 0, vrep, 0, 1, 0);
	
	pTriList[0].set(0, 1, 2);
	pTriList[1].set(0, 2, 3);

	pPlane = new Model(dev, pVerts, nverts, pTriList, ntri);
	//pPlane = new Model( dev, Model::PreMadeModels::UnitSphere);

	delete[] pVerts;
	delete[] pTriList;
}

FlatPlane::~FlatPlane()
{
	delete pPlane;
}

void FlatPlane::Render(ID3D11DeviceContext* context)
{
	pPlane->SetToContext(context);
	pPlane->Render(context);
}