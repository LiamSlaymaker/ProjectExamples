
#ifndef _SkyBox
#define _SkyBox

#include "Matrix.h"
#include "ShaderTexture.h"
#include "Texture.h"


class Model;
struct ID3D11DeviceContext;
struct ID3D11Device;

class SkyBox : public Align16
{
private:
	Model* sBox;

public:
	SkyBox(const SkyBox&) = delete;				// Copy constructor
	SkyBox(Model&&) = delete;						// Move constructor
	SkyBox& operator=(const SkyBox&) & = delete;  // Copy assignment operator
	SkyBox& operator=(SkyBox&&) & = delete;       // Move assignment operator
	~SkyBox();

	SkyBox(ID3D11Device* dev, float len, ShaderTexture* shader);

	void SetTexture(Texture* tex);
	void SetWorld(const Matrix& m);
	void Render(ID3D11DeviceContext* context);



private:
	ShaderTexture* pShader;
	Texture* pTex; 
	Matrix World;


};
#endif _SkyBox

