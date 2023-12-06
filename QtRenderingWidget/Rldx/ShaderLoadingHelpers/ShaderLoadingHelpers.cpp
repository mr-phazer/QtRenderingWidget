#include "ShaderLoadingHelpers.h"
#include "..\..\Tools\tools.h"
#include <exception>

std::vector<uint8_t> Rldx::ShaderLoaderHelper::GetRawDataFromDisk(const std::wstring& shaderPath)
{
	if (!tools::DoesFileExist(shaderPath))	{
		throw std::runtime_error("Shader file: '" + tools::wstring_to_string(shaderPath) + "' not found");
	}

	size_t shaderCodeSize = tools::GetFileSize(shaderPath);
	std::vector<uint8_t> shaderCodeRaw(shaderCodeSize);

	std::ifstream inputFile(shaderPath, std::ios::binary);
	inputFile.read((char*)shaderCodeRaw.data(), shaderCodeSize);
	inputFile.close();

	return shaderCodeRaw;
}
