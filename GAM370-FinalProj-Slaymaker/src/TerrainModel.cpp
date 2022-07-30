#include "TerrainModel.h"
#include "Model.h"
#include "d3dUtil.h"
#include "DirectXTex.h"
#include <assert.h>


TerrainModel::TerrainModel(ID3D11Device* dev, LPCWSTR heightmapFile, float len, float maxheight, float ytrans, int RepeatU, int RepeatV)
{
	DirectX::ScratchImage scrtTex;
	HRESULT hr = LoadFromTGAFile(heightmapFile, nullptr, scrtTex);
	assert(SUCCEEDED(hr));

	const DirectX::Image* hgtmap = scrtTex.GetImage(0, 0, 0);
	assert(hgtmap->height == hgtmap->width);

	size_t side = hgtmap->height;	// the image should be square
	size_t pixel_width = 4;			// 4 bytes RGBA per pixel
	uint8_t h_val = hgtmap->pixels[pixel_width * (5 * side + 3)]; // the 'R' byte of the pixel at (3,5)


	// ** much work to add below **

	float nverts = side * side;
	StandardVertex* pVerts = new StandardVertex[nverts];

	float ntri = (side - 1) * 2;
	ntri *= (side - 1);
	TriangleByIndex* pTriList = new TriangleByIndex[ntri];

	float x;
	float y;
	float z;
	float u;
	float v;
	float s = side;
	Vect a;
	Vect b;
	Vect n1;
	Vect n2;
	Vect n3;
	Vect n4;
	Vect n5;
	Vect n6;
	Vect nor;
	for (int i = 0; i < side; i++) {
		for (int j = 0; j < side; j++) {


			x = (j / (s - 1)) * len;
			u = x * RepeatU / len;
			x -= (len * 0.5);
			//if ()
			y = hgtmap->pixels[TexelIndex(side, j, i)];
			y = maxheight * y / 255;
			y += ytrans;
			z = (i / (s - 1)) * len;
			v = z * RepeatV / len;
			z -= (len * 0.5);
			pVerts[i * side + j].set(x, y, z, u, v);
		}
	}
	for (int i = 1; i < side - 1; i++) {
		for (int j = 1; j < side - 1; j++) {
			//n1
			a = pVerts[(i - 1) * side + j].Pos - pVerts[(i)*side + j].Pos;
			b = pVerts[(i)*side + j - 1].Pos - pVerts[(i)*side + j].Pos;
			n1 = a.cross(b);
			n1.norm();

			//n2
			a = pVerts[(i)*side + j - 1].Pos - pVerts[(i)*side + j].Pos;
			b = pVerts[(i + 1) * side + j - 1].Pos - pVerts[(i)*side + j].Pos;
			n2 = a.cross(b);
			n2.norm();

			//n3
			a = pVerts[(i + 1) * side + j - 1].Pos - pVerts[(i)*side + j].Pos;
			b = pVerts[(i + 1) * side + j].Pos - pVerts[(i)*side + j].Pos;
			n3 = a.cross(b);
			n3.norm();

			//n4
			a = pVerts[(i + 1) * side + j].Pos - pVerts[(i)*side + j].Pos;
			b = pVerts[i * side + j + 1].Pos - pVerts[(i)*side + j].Pos;
			n4 = a.cross(b);
			n4.norm();

			//n5
			a = pVerts[i * side + j + 1].Pos - pVerts[(i)*side + j].Pos;
			b = pVerts[(i - 1) * side + j - 1].Pos - pVerts[(i)*side + j].Pos;
			n5 = a.cross(b);
			n5.norm();

			//n6
			a = pVerts[(i - 1) * side + j - 1].Pos - pVerts[(i)*side + j].Pos;
			b = pVerts[(i - 1) * side + j].Pos - pVerts[(i)*side + j].Pos;
			n6 = b.cross(a);
			n6.norm();

			nor = n1 + n2 + n3 + n4 + n5 + n6;
			nor.norm();
			nor.set(nor.X(), nor.Y(), nor.Z(), 0);
			pVerts[i * side + j].normal = nor;
		}
	}

	int count = (side - 1) * 2;
	for (int i = 0; i < side - 1; i++) {
		for (int j = 0; j < side - 1; j++) {

			pTriList[(j * 2) + (count * i)].set(j + (i * side), j + ((i + 1) * side), j + (i * side) + 1);
			pTriList[(j * 2) + (count * i) + 1].set(j + (i * side) + 1, j + ((i + 1) * side), j + ((i + 1) * side) + 1);
		}
	}

	uint8_t test = hgtmap->pixels[TexelIndex(side, 0, 15)];
	test / 2;

	pModTerrain = new Model(dev, pVerts, nverts, pTriList, ntri);

	delete[] pVerts;
	delete[] pTriList;
}

int TerrainModel::TexelIndex(int side, int i, int j) {
	return 4 * (j * side + i);
}

TerrainModel::~TerrainModel()
{
	delete pModTerrain;
}

void TerrainModel::Render(ID3D11DeviceContext* context)
{
	pModTerrain->SetToContext(context);
	pModTerrain->Render(context);
}