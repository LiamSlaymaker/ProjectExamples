#include "GraphicObject_Color.h"
#include "Model.h"
#include "ShaderColor.h"
#include <assert.h>

GraphicObject_Color::GraphicObject_Color(ShaderColor* shader, Model* mod)
{
	SetModel(mod);
	pShader = shader;

	//Color = Vect(1, 1, 1);
	World = Matrix(IDENTITY);

	int n = mod->GetMeshCount();
	MeshColors = new Vect[n];
	for (int i = 0; i < n; i++)
		MeshColors[i] = Vect(0, 0, 0);
}

GraphicObject_Color::~GraphicObject_Color()
{
	delete[] MeshColors;
}

void GraphicObject_Color::SetColor(const Vect& col)
{
	for (int i = 0; i < this->pModel->GetMeshCount(); i++)
		MeshColors[i] = col;
}

void GraphicObject_Color::SetColor(const Vect& col, int meshnum)
{
	assert(pModel->ValidMeshNum(meshnum));
	MeshColors[meshnum] = col;
}

void GraphicObject_Color::SetWorld(const Matrix& m)
{
	World = m;
}

void GraphicObject_Color::Render()
{
	pModel->SetToContext(pShader->GetContext());

	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SendWorldColor(World, MeshColors[i]);
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}