#ifndef MESH_H
#define MESH_H

#include "../../OpenGLEngine/GLEstdafx.h"
#include "../../Core/Mesh/IndexedMesh.h"
#include "../Vertex.h"

struct ObjGroup {
	std::vector<Core::Vertex> vertices;
};

class Mesh {
public:
	inline Mesh() = default;
	explicit Mesh(const std::vector<ObjGroup> &groups);
	explicit Mesh(const IndexedMesh &indexedMesh);
	inline ~Mesh() = default;


	std::vector<ObjGroup> _groups;
	void draw();
};

#endif // !MESH_H
