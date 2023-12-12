#include "ShaderFiles.h"

inline void VertexShaderFile::SetShader(ID3D11VertexShader* poShaderCode) {
	m_cpoVertexShader = poShaderCode;
}

ID3D11VertexShader* VertexShaderFile::GetShader() {
	return m_cpoVertexShader.Get();
}

inline void PixelShaderFile::SetShader(ID3D11PixelShader* poShaderCode) {
	m_cpoShaderCode = Microsoft::WRL::ComPtr<ID3D11PixelShader>(poShaderCode);
}

ID3D11PixelShader* PixelShaderFile::GetShader() {
	return m_cpoShaderCode.Get();
}
