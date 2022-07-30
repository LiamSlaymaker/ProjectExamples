#include "GraphicObject_Texture.h"
#include "Model.h"
#include "ShaderColor.h"
#include <assert.h>

GraphicObject_Texture::GraphicObject_Texture(ShaderTexture* shader, Model* mod)
{
	SetModel(mod);
	pShader = shader;

	World = Matrix(IDENTITY);

	int n = mod->GetMeshCount();
	//Texture* MeshTexs[4];
	MeshTexs = new Texture*[n];
	//for (int i = 0; i < n; i++)
	//	MeshTexs[i] = Vect(0, 0, 0);
}

GraphicObject_Texture::~GraphicObject_Texture()
{
	delete[] MeshTexs;
}


void GraphicObject_Texture::SetTexture(Texture* tex)
{
	for (int i = 0; i < this->pModel->GetMeshCount(); i++)
		MeshTexs[i] = tex;
}

void GraphicObject_Texture::SetTexture(Texture* tex, int meshnum)
{
	assert(pModel->ValidMeshNum(meshnum));
	MeshTexs[meshnum] = tex;
}


void GraphicObject_Texture::SetWorld(const Matrix& m)
{
	World = m;
}

void GraphicObject_Texture::Render()
{
	
	pModel->SetToContext(pShader->GetContext());

	for (int i = 0; i < pModel->GetMeshCount(); i++) {
		pShader->SendWorld(World);
		MeshTexs[i]->SetToContext(pShader->GetContext());
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}