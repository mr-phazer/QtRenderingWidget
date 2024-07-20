#include "ShaderFiles.h"

#include <DXUT\Core\DXUT.h>

inline void VertexShaderFile::SetShader(ID3D11VertexShader* poShaderCode) {
	m_cpoVertexShader = poShaderCode;
}

ID3D11VertexShader* VertexShaderFile::GetShader() const {
	return m_cpoVertexShader.Get();
}

ID3D11InputLayout* VertexShaderFile::GetInputLayout() const {
	return m_cpoInputLayout.Get();
}

void PixelShaderFile::SetShader(ID3D11PixelShader* poShaderCode) {
	m_cpoShaderCode = Microsoft::WRL::ComPtr<ID3D11PixelShader>(poShaderCode);
}

ID3D11PixelShader* PixelShaderFile::GetShader() {
	return m_cpoShaderCode.Get();
}
