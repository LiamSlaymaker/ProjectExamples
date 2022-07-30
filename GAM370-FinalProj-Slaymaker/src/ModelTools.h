// ModelTools
// Support tools to compute bounding volumes on models
// Ported/adapted from Keenan's FBX Converter code
// Andre Berthiaume, June 2016

#ifndef _ModelTools
#define _ModelTools

#include "Vect.h"

struct StandardVertex;
struct TriangleByIndex;

class ModelTools
{

public:
	static void CreateUnitBox(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri);
	static void CreateUnitBoxRepTexture(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri);

	//static void CreateUnitPyramid(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri);
	static void CreateUnitSphere(int vslice, int hslice, StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri);

	static void CreateUnitBoxSixFacesTexture(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri);
};



#endif _ModelTools