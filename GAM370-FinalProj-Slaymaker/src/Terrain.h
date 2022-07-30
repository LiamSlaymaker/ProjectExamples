#ifndef _Terrain
#define _Terrain

#include "GraphicObject_Base.h"
#include "Texture.h"
#include "Vect.h"
#include "TerrainModel.h"
#include "ShaderTextureLight.h"

class Terrain : public GraphicObject_Base
{
	//friend class ShaderTextureLight;

public:
	Terrain(ShaderTextureLight* shader, TerrainModel* terr);

	Terrain(const Terrain&) = delete;				 // Copy constructor
	Terrain(Terrain&&) = default;                    // Move constructor
	Terrain& operator=(const Terrain&) & = default;  // Copy assignment operator
	Terrain& operator=(Terrain&&) & = default;       // Move assignment operator
	~Terrain();		  											 // Destructor

	Terrain() = delete;

	void SetMaterial(const Vect& amb, const Vect& diff, const Vect& spec);
	void SetTexture(Texture* tex);
	void SetWorld(const Matrix& m);
	virtual void Render() override;



private:
	ShaderTextureLight* pShader;
	Texture* Tex;
	Vect Amb;
	Vect Diff;
	Vect Spec;
	Matrix World;
	TerrainModel* pTerr;

};

#endif _Terrain