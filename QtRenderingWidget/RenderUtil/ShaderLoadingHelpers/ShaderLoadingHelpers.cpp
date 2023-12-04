#include "ShaderLoadingHelpers.h"
#include "..\..\Tools\tools.h"

inline std::vector<uint8_t> ShaderLoaderHelper::GetRawDataFromDisk(const std::wstring& shaderPath)
{
	size_t shaderCodeSize = tools::getFileSize(shaderPath);
	std::vector<uint8_t> shaderCodeRaw(shaderCodeSize);
	std::ifstream in(shaderPath, std::ios::binary);
	in.read((char*)shaderCodeRaw.data(), shaderCodeSize);
	in.close();

	return shaderCodeRaw;
}
