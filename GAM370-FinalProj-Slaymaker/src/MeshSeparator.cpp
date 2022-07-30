#include "MeshSeparator.h"
#include <assert.h>
#include "Model.h"

// Improtant: the tri index list may be modified (reordered!)
MeshSeparator::MeshSeparator(StandardVertex* pVerts, int nverts, TriangleByIndex*& pTriList, int ntri)
{
	// create a temporary map to sort the indices
	using KEY = int;
	using MESHINDICES = std::vector<TriangleByIndex>;
	std::map<KEY, MESHINDICES> meshes;

	// Create the different list of indices for meshes
	nverts;
	for (int i = 0; i < ntri; ++i)
	{
		TriangleByIndex ind = pTriList[i];

		// triangles in one mesh shouldn't share vertices with another mesh
		assert((pVerts[ind.v0].meshNum == pVerts[ind.v1].meshNum) && (pVerts[ind.v0].meshNum == pVerts[ind.v2].meshNum));

		KEY key = (KEY)pVerts[ind.v0].meshNum;
		meshes[key].push_back(ind);
	}

	// we save the mesh count for later validation
	meshcount = meshes.size();

	// Reconsruct a new tri index list ordered by mesh number
	TriangleByIndex* templist = new TriangleByIndex[ntri];
	meshdata = new MeshIndexData[meshes.size()];
	int offsetval = 0;

	for (size_t i = 0; i < meshes.size(); ++i)
	{
		meshdata[i].numTri = meshes[i].size();
		meshdata[i].offset = offsetval;

		// Collect all the tri indices for the i'th mesh
		size_t meshsize = meshes[i].size();
		for (size_t j = 0; j < meshsize; ++j)
		{
			templist[offsetval + j] = meshes[i][j];
		}

		offsetval += meshes[i].size();
	}

	delete[] pTriList;		// Delete the original triangle array
	pTriList = templist;	// Replace it with new re-ordered one

}

MeshSeparator::~MeshSeparator()
{
	delete[] meshdata;
}

void MeshSeparator::GetMeshTriCountAndOffset(int meshnum, int& count, int& offset)
{
	assert((meshnum >= 0) && (meshnum < GetMeshCount()));

	count = meshdata[meshnum].numTri;
	offset = meshdata[meshnum].offset;
}

int MeshSeparator::GetMeshCount()
{
	return meshcount;
}

