#include "Mesh.h"

using namespace Core;


Mesh::Mesh(const IndexedMesh &indexedMesh)
{
	for (IndexedObjGroup iGroup : indexedMesh._groups) {
		ObjGroup group{};

		for (Face face : iGroup.faces) {
			for (IndexedVertex iVertex : face.vertices) {
				glm::vec2 texCoord = { };
				if (iVertex.hasTexCoords) {
					texCoord = indexedMesh._texcoords[iVertex.texcoord];
				}

				Vertex vertex{
					indexedMesh._vertices[iVertex.position],
					{1.0f, 1.0f, 1.0f, 0.6f}, // plain grey...
					texCoord,
					indexedMesh._normals[iVertex.normal]
				};

				group.vertices.push_back(vertex);
			}
		}

		_groups.push_back(group);
	}
}

Mesh::Mesh(const std::vector<ObjGroup>& groups)
{
	_groups = groups;
}

void Mesh::draw()
{
	for (ObjGroup group : _groups) {
		// Positions
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (float*)group.vertices.data());
		glEnableVertexAttribArray(0);
		// colors
		glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), (float*)group.vertices.data() + 3);
		glEnableVertexAttribArray(1);
		// TexCoords
		glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (float*)group.vertices.data() + 7);
		glEnableVertexAttribArray(2);
		// Normals
		glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(Vertex), (float*)group.vertices.data() + 9);
		glEnableVertexAttribArray(3);

		glDrawArrays(GL_TRIANGLES, 0, group.vertices.size());
	}
}
