#include "ShaderBuilder.h"
#include "..\Core\BasicAssetLoader.h"



bool ShaderBuilder::buildShader(const std::string &fileName, _In_reads_opt_(_Inexpressible_(definesPtr->Name != NULL)) CONST D3D_SHADER_MACRO* definesPtr, _In_opt_ ID3DInclude * includePtr, LPCSTR entryPointPtr, LPCSTR targetPtr, UINT flags1, UINT flags2, UINT secondaryDataFlags, ID3DBlob ** compiledPtr, ID3DBlob ** errorMessagePtr)
{
	
	std::string data = Core::BasicAssetLoader::readFile(fileName);

	HRESULT result = D3DCompile2(
		data.c_str(),
		data.size(),
		ShaderBuilder::getName(fileName).c_str(),
		definesPtr,
		includePtr,
		entryPointPtr,
		targetPtr,
		flags1,
		flags2,
		secondaryDataFlags,
		NULL,
		NULL,
		compiledPtr,
		errorMessagePtr
	);

	return !FAILED(result);
}

std::string ShaderBuilder::getName(const std::string &fullPath)
{
	std::string name;
	size_t end = 0;
	size_t begin = 0;

	for (size_t i = fullPath.size() - 1; i >= 0; i--) {
		if (fullPath[i] == '.') {
			end = i;
		}
		else if (fullPath[i] == '\\' || fullPath[i] == '/') {
			begin = i;
			break;
		}
	}

	return fullPath.substr(begin, end);
}
