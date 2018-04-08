#ifndef INDEXED_MESH_H
#define INDEXED_MESH_H

#include "../../OpenGLEngine/GLEstdafx.h"
#include <vector>

struct IndexedVertex {
	int position;
	int normal;
	int texcoord;

	bool hasTexCoords = false;
	bool hasNormals = false;
};

struct Face {
	std::vector<IndexedVertex> vertices;
};

struct IndexedObjGroup {
	std::string name;
	int materialIndex;
	std::vector <Face> faces;
};

class IndexedMesh {
public:
	inline IndexedMesh() = default;
	inline ~IndexedMesh() = default;

	std::vector<glm::vec3> _vertices;
	std::vector<glm::vec3> _normals;
	std::vector<glm::vec2> _texcoords;
	std::vector<IndexedObjGroup> _groups;
};

#endif // !INDEXED_MESH_H
