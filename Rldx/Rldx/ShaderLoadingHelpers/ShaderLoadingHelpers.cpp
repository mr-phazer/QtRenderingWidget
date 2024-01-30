#include "ShaderLoadingHelpers.h"
#include "..\Tools\tools.h"
#include <exception>

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxguid.lib")

std::vector<uint8_t> rldx::ShaderLoaderHelper::GetRawDataFromDisk(const std::wstring& shaderPath)
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

ID3D11InputLayout* rldx::VertexShaderLoader::CreateInputLayoutDescFromVertexShaderSignature(ID3D11Device* pD3DDevice, void* pMem, size_t buffer_size)

{
	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = NULL;
	if (FAILED(D3DReflect(pMem, buffer_size, IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		return nullptr;
	}

	// Get shader info
	D3D11_SHADER_DESC shaderDesc;
	pVertexShaderReflection->GetDesc(&shaderDesc);

	// Read input layout description from shader info
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (uint32_t i = 0; i < shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

		// fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		// determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		//save element desc
		inputLayoutDesc.push_back(elementDesc);
	}

	// Try to create Input Layout
	ID3D11InputLayout* pInputLayout = NULL;
	HRESULT hr = pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], static_cast<UINT>(inputLayoutDesc.size()), pMem, buffer_size, &pInputLayout);

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();

	if (FAILED(hr)) {
		return nullptr;
	}

	return pInputLayout;
};