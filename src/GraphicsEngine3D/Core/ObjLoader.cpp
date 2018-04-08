#include "ObjLoader.h"

#include "BasicAssetLoader.h"
#include "Mesh\IndexedMesh.h"
#include "Mesh/Mesh.h"
#include <algorithm>

using namespace Core;

/**
* Splits a string into substrings, based on a seperator
*/
static std::vector<std::string> split(std::string str, const std::string &seperator)
{
	std::vector<std::string> ret;
	size_t index;
	while (true)
	{
		index = str.find(seperator);
		if (index == std::string::npos)
			break;
		ret.push_back(str.substr(0, index));
		str = str.substr(index + 1);
	}
	ret.push_back(str);
	return ret;
}

/**
* Turns a string to lowercase
*/
static std::string toLower(std::string data)
{
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}

Mesh ObjLoader::loadObj(const std::string &filePath)
{
	bool succes = false;
	std::ifstream data = BasicAssetLoader::loadStream(filePath, succes);
	if (succes) {
		return ObjLoader::generateObj(data);
	}
	
	throw std::exception{ "Failed to load Obj file" };
}

Mesh Core::ObjLoader::generateObj(std::ifstream &dataStream)
{
	IndexedMesh indexedMesh{};
	IndexedObjGroup currentGroup{};
	bool initial = true;

	while (!dataStream.eof()) {
		std::string line;
		std::getline(dataStream, line);
		line = cleanLine(line);
		if (line == "" || line[0] == '#') {
			continue;
		}

		std::vector<std::string>params = split(line, " ");
		params[0] = toLower(params[0]);

		if (params[0] == "o") {
			if (initial) {
				initial = false;
			}
			else {
				indexedMesh._groups.push_back(currentGroup);
			}

			currentGroup = IndexedObjGroup{};
			currentGroup.name = params[1];
		} else if (params[0] == "v") {
			indexedMesh._vertices.push_back({
				atof(params[1].c_str()),
				atof(params[2].c_str()),
				atof(params[3].c_str())
			});
		}
		else if (params[0] == "vn") {
			indexedMesh._normals.push_back({
				atof(params[1].c_str()),
				atof(params[2].c_str()),
				atof(params[3].c_str())
			});
		}
		else if (params[0] == "vt") {
			indexedMesh._texcoords.push_back({
				atof(params[1].c_str()),
				atof(params[2].c_str())
			});
		}
		else if (params[0] == "f") {
			for (size_t ii = 4; ii <= params.size(); ii++) {
				Face face{};

				for (size_t i = ii - 3; i < ii; i++) {
					IndexedVertex vertex;
					std::vector<std::string> indices = split(
						params[i == (ii - 3) ? 1 : i], 
						"/"
					);
					if (indices.size() >= 1) {
						vertex.position = atoi(indices[0].c_str()) - 1;
					}
					if (indices.size() == 2) {
						vertex.texcoord = atoi(indices[1].c_str()) - 1;
						vertex.hasTexCoords = true;
					}
					if (indices.size() == 3)
					{
						if (indices[1] != "") {
							vertex.texcoord = atoi(indices[1].c_str()) - 1;
							vertex.hasTexCoords = true;
						}
						vertex.normal = atoi(indices[2].c_str()) - 1;
						vertex.hasNormals = true;
					}
					face.vertices.push_back(vertex);
				}
				currentGroup.faces.push_back(face);
			}
		}
	}
 	indexedMesh._groups.push_back(currentGroup);

	return Mesh{ indexedMesh };
}

std::string Core::ObjLoader::cleanLine(std::string & line)
{
	line = replace(line, "\t", " ");
	while (line.find("  ") != std::string::npos)
		line = replace(line, "  ", " ");
	if (line == "")
		return "";
	if (line[0] == ' ')
		line = line.substr(1);
	if (line == "")
		return "";
	if (line[line.length() - 1] == ' ')
		line = line.substr(0, line.length() - 1);
	return line;
}

std::string Core::ObjLoader::replace(std::string str, const std::string & toReplace, const std::string & replacement)
{
	size_t index = 0;
	while (true)
	{
		index = str.find(toReplace, index);
		if (index == std::string::npos)
			break;
		str.replace(index, toReplace.length(), replacement);
		++index;
	}
	return str;
}

