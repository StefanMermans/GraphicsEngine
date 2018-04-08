#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <string>
#include <vector>
#include <fstream>
#include "Vertex.h"
#include "Mesh/Mesh.h"

namespace Core {
	class ObjLoader {
	public:
		static Mesh loadObj(const std::string &filePath);
	private:
		static Mesh generateObj(std::ifstream &dataStream);
		static std::string cleanLine(std::string &line);
		static std::string replace(std::string str, const std::string &toReplace, const std::string &replacement);
	};
}


#endif // !OBJ_LOADER_H
