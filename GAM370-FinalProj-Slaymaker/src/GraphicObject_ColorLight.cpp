#include "GraphicObject_ColorLight.h"
#include "Model.h"
#include "ShaderColorLight.h"
#include <assert.h>

GraphicObject_ColorLight::GraphicObject_ColorLight(ShaderColorLight* shader, Model* mod)
{
	SetModel(mod);
	pShader = shader;

	World = Matrix(IDENTITY);

	int n = mod->GetMeshCount();
	MeshAmb = new Vect[n];
	MeshDiff = new Vect[n];
	MeshSpec = new Vect[n];
	for (int i = 0; i < n; i++) {
		MeshAmb[i] = Vect(0, 0, 0);
		MeshDiff[i] = Vect(0, 0, 0);
		MeshSpec[i] = Vect(0, 0, 0);
	}
	
}

GraphicObject_ColorLight::~GraphicObject_ColorLight()
{
	delete[] MeshAmb;
	delete[] MeshDiff;
	delete[] MeshSpec;
}

void GraphicObject_ColorLight::SetMaterial(const Vect& amb, const Vect& diff, const Vect& spec)
{
	for (int i = 0; i < this->pModel->GetMeshCount(); i++) {
		MeshAmb[i] = amb;
		MeshDiff[i] = diff;
		MeshSpec[i] = spec;
	}
}

void GraphicObject_ColorLight::SetMaterial(const Vect& amb, const Vect& diff, const Vect& spec, int meshnum)
{
	assert(pModel->ValidMeshNum(meshnum));

	MeshAmb[meshnum] = amb;
	MeshDiff[meshnum] = diff;
	MeshSpec[meshnum] = spec;

}

void GraphicObject_ColorLight::SetWorld(const Matrix& m)
{
	World = m;
}

/*void GraphicObject_ColorLight::SetEyePos(Camera mCam)
{
	mCam.getPos(EyePosition);
}
*/

void GraphicObject_ColorLight::Render()
{
	pModel->SetToContext(pShader->GetContext());
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SendWorldAndMaterial(World, MeshAmb[i], MeshDiff[i], MeshSpec[i]);
		pModel->RenderMesh(pShader->GetContext(), i);

	}
}