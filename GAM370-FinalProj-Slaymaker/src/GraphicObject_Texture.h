
#ifndef _GraphicObject_Texture
#define _GraphicObject_Texture

#include "GraphicObject_Base.h"
#include "Vect.h"
#include "ShaderTexture.h"

class GraphicObject_Texture : public GraphicObject_Base
{
	friend class ShaderTexture;

public:
	GraphicObject_Texture(const GraphicObject_Texture&) = delete;				 // Copy constructor
	GraphicObject_Texture(GraphicObject_Texture&&) = default;                    // Move constructor
	GraphicObject_Texture& operator=(const GraphicObject_Texture&) & = default;  // Copy assignment operator
	GraphicObject_Texture& operator=(GraphicObject_Texture&&) & = default;       // Move assignment operator
	~GraphicObject_Texture();		  											 // Destructor

	GraphicObject_Texture() = delete;

	void SetTexture( Texture* tex);
	void SetTexture(Texture* tex, int meshnum);
	void SetWorld(const Matrix& m);
	virtual void Render() override;

	GraphicObject_Texture(ShaderTexture* shader, Model* mod);

private:
	ShaderTexture* pShader;
	Texture** MeshTexs;
	Matrix							World;

};

#endif _GraphicObject_Color
