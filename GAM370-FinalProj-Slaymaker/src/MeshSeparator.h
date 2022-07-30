// MeshSeperator
// Andre Berthiaume, July 2016

#ifndef _MeshSeperator
#define _MeshSeperator

#include <map>
#include <vector>
struct StandardVertex;
struct TriangleByIndex;



class MeshSeparator
{
private:
	struct MeshIndexData
	{
		int offset;
		int numTri;
	};

	MeshIndexData* meshdata;
	int meshcount;

public:
	MeshSeparator(const MeshSeparator&) = delete;				 // Copy constructor
	MeshSeparator(MeshSeparator&&) = default;                    // Move constructor
	MeshSeparator& operator=(const MeshSeparator&) & = default;  // Copy assignment operator
	MeshSeparator& operator=(MeshSeparator&&) & = default;       // Move assignment operator
	~MeshSeparator();									 // Destructor

	MeshSeparator() = delete;
	MeshSeparator(StandardVertex* pVerts, int nverts, TriangleByIndex*& pTriList, int ntri);

	int GetMeshCount();
	void GetMeshTriCountAndOffset(int meshnum, int& count, int& offset);

};

#endif _MeshSeperator

