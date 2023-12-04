#pragma once


#include <wrl/client.h>
#include <d3d11.h>

class VertexShaderFile
{
public:
	VertexShaderFile() = default;
	VertexShaderFile(ID3D11VertexShader* poShaderCode) : m_cpoShaderCode(poShaderCode) {};
	
	void SetShader(ID3D11VertexShader* poShaderCode);;

	ID3D11VertexShader* GetShader();;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_cpoShaderCode;
};

class PixelShaderFile
{
public:
	PixelShaderFile() = default;
	PixelShaderFile(ID3D11PixelShader* poShaderCode) : m_cpoShaderCode(poShaderCode) {};

	void SetShader(ID3D11PixelShader* poShaderCode);;

	ID3D11PixelShader* GetShader();;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_cpoShaderCode;
};

