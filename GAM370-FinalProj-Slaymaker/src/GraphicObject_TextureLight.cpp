#include "GraphicObject_TextureLight.h"
#include "Model.h"
#include "ShaderTextureLight.h"
#include <assert.h>

GraphicObject_TextureLight::GraphicObject_TextureLight(ShaderTextureLight* shader, Model* mod)
{
	SetModel(mod);
	pShader = shader;

	World = Matrix(IDENTITY);

	int n = mod->GetMeshCount();
	MeshTexs = new Texture* [n];
	MeshAmb = new Vect[n];
	MeshDiff = new Vect[n];
	MeshSpec = new Vect[n];
	for (int i = 0; i < n; i++) {
		MeshAmb[i] = Vect(0, 0, 0);
		MeshDiff[i] = Vect(0, 0, 0);
		MeshSpec[i] = Vect(0, 0, 0);
	}
}

GraphicObject_TextureLight::~GraphicObject_TextureLight()
	{
		delete[] MeshTexs;
		delete[] MeshAmb;
		delete[] MeshDiff;
		delete[] MeshSpec;
	}


void GraphicObject_TextureLight::SetTexture(Texture* tex)
{
	for (int i = 0; i < this->pModel->GetMeshCount(); i++)
		MeshTexs[i] = tex;
}

void GraphicObject_TextureLight::SetTexture(Texture* tex, int meshnum)
{
	assert(pModel->ValidMeshNum(meshnum));
	MeshTexs[meshnum] = tex;
}

void GraphicObject_TextureLight::SetMaterial(const Vect& amb, const Vect& diff, const Vect& spec)
{
	for (int i = 0; i < this->pModel->GetMeshCount(); i++) {
		MeshAmb[i] = amb;
		MeshDiff[i] = diff;
		MeshSpec[i] = spec;
	}
}

void GraphicObject_TextureLight::SetMaterial(const Vect& amb, const Vect& diff, const Vect& spec, int meshnum)
{
	assert(pModel->ValidMeshNum(meshnum));

	MeshAmb[meshnum] = amb;
	MeshDiff[meshnum] = diff;
	MeshSpec[meshnum] = spec;

}

void GraphicObject_TextureLight::SetWorld(const Matrix& m)
{
	World = m;
}

void GraphicObject_TextureLight::Render()
{

	pModel->SetToContext(pShader->GetContext());

	for (int i = 0; i < pModel->GetMeshCount(); i++) {
		pShader->SendWorldAndMaterial(World, MeshAmb[i], MeshDiff[i], MeshSpec[i]);
		MeshTexs[i]->SetToContext(pShader->GetContext());
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}


