#include "ShaderFiles.h"

inline void VertexShaderFile::SetShader(ID3D11VertexShader* poShaderCode) {
	m_cpoShaderCode = poShaderCode;
}

ID3D11VertexShader* VertexShaderFile::GetShader() {
	return m_cpoShaderCode.Get();
}

inline void PixelShaderFile::SetShader(ID3D11PixelShader* poShaderCode) {
	m_cpoShaderCode = Microsoft::WRL::ComPtr<ID3D11PixelShader>(poShaderCode);
}

ID3D11PixelShader* PixelShaderFile::GetShader() {
	return m_cpoShaderCode.Get();
}
