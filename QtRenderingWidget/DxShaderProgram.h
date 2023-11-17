#pragma once

namespace Rldx {


	class VertexShader;
	class PixelShader;


	class DxShaderProgram
	{
	protected:
		VertexShader* m_pVertexShader;
		PixelShader* m_pPixelShader;
	};

}; // namespace Rldx

