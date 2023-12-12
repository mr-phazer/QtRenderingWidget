#pragma once


#include <wrl/client.h>
#include <d3d11.h>

class VertexShaderFile
{
public:
	VertexShaderFile() = default;
	VertexShaderFile(ID3D11VertexShader* poVertexShader) : m_cpoVertexShader(poVertexShader) {};
	VertexShaderFile(ID3D11VertexShader* poShaderCode, ID3D11InputLayout* poInputlayout) : m_cpoVertexShader(poShaderCode), m_pInputLayout(poInputlayout) {};
	
	void SetShader(ID3D11VertexShader* poVertexShader);;

	ID3D11VertexShader* GetShader();;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_cpoVertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>  m_pInputLayout;
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

