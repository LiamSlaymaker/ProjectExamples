#include "Terrain.h"
#include "TerrainModel.h"
#include "ShaderTextureLight.h"
#include <assert.h>

Terrain::Terrain(ShaderTextureLight* shader, TerrainModel* terr)
{
	pTerr = terr;
	pShader = shader;

	World = Matrix(IDENTITY);

	Amb = Vect(0.5, 0.5, 0.5);
	Diff = Vect(0.5, 0.5, 0.5);
	Spec = Vect(0.5, 0.5, 0.5);

}

Terrain::~Terrain()
{

}

void Terrain::SetTexture(Texture* tex)
{
	Tex = tex;
}

void Terrain::SetMaterial(const Vect& amb, const Vect& diff, const Vect& spec)
{
	Amb = amb;
	Diff = diff;
	Spec = spec;
}

void Terrain::SetWorld(const Matrix& m)
{
	World = m;
}

void Terrain::Render()
{

	//pTerr->SetToContext(pShader->GetContext());
	pShader->SendWorldAndMaterial(World, Amb, Diff, Spec);
	Tex->SetToContext(pShader->GetContext());
	pTerr->Render(pShader->GetContext());

}