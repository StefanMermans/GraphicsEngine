#ifndef SHADER_BUILDER_H
#define SHADER_BUILDER_H

#include <d3d12.h>
#include <string>
#include <d3dcompiler.h>

class ShaderBuilder
{
public:
	static bool buildShader(
		const std::string &fileName,
		_In_reads_opt_(_Inexpressible_(definesPtr->Name != NULL)) CONST D3D_SHADER_MACRO* definesPtr,
		_In_opt_ ID3DInclude * includePtr,
		LPCSTR entryPointPtr,
		LPCSTR targetPtr,
		UINT flags1,
		UINT flags2,
		UINT secondaryDataFlags,
		ID3DBlob ** compiledPtr,
		ID3DBlob ** errorMessagePtr
	);
private:
	static std::string getName(const std::string &fullPath);
};

#endif // SHADER_BUILDER_H
