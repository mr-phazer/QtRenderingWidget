#pragma once

#include <DirectXMath.h>
#include <wrl\client.h>
#include <d3d11.h>
#include "..\..\..\..\DirectXTK\Inc\DDSTextureLoader.h"
#include "..\..\..\..\DirectXTK\Inc\SimpleMath.h"

#include <string>



namespace Rldx
{

	// TODO: renable
	class IDxTexture
	{
		virtual ID3D11Texture2D* GetTexture() const = 0;
		virtual ID3D11RenderTargetView* GetRenderTargetView() const = 0;
		virtual ID3D11ShaderResourceView* GetShaderResourceView() const = 0;


		virtual ID3D11RenderTargetView** GetAddressOfRenderTargetView() = 0;
		virtual ID3D11ShaderResourceView** getAddressOfShaderResourceView() = 0;
		virtual ID3D11Texture2D** getGetAddressOfTexture() = 0;

	};


	class DxTexture : public IDxTexture
	{
	public:

		virtual ID3D11RenderTargetView* GetRenderTargetView() const override { return m_cpoRenderTargetView.Get();};
		virtual ID3D11ShaderResourceView* GetShaderResourceView() const  override { return m_cpoShaderResourceView.Get();};
		virtual ID3D11Texture2D* GetTexture() const  override { return m_cpoTexture.Get();};
		virtual ID3D11RenderTargetView** GetAddressOfRenderTargetView() override { return m_cpoRenderTargetView.GetAddressOf();	};
		virtual ID3D11ShaderResourceView** getAddressOfShaderResourceView()  override { return m_cpoShaderResourceView.GetAddressOf(); };
		virtual ID3D11Texture2D** getGetAddressOfTexture() override { return m_cpoTexture.GetAddressOf(); };
		

		virtual Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetRenderTargetViewCPO()  /*override */ {
			return m_cpoRenderTargetView;
		};

		virtual Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceViewCPO()  /*override*/ {
			return m_cpoShaderResourceView;
		};

		virtual Microsoft::WRL::ComPtr<ID3D11Texture2D>& GetTextureCPO()  /*override*/ {
			return m_cpoTexture;
		};

		void SetToActiveTargetView(ID3D11DeviceContext* deviceContext)
		{
			auto pRTV = GetRenderTargetView();
			ID3D11RenderTargetView* const ppRTV[] = { pRTV };
			deviceContext->OMSetRenderTargets(1, ppRTV, NULL);

		}


		void ClearPixels(ID3D11DeviceContext* pDeviceContext, sm::Color vColor)
		{
			pDeviceContext->ClearRenderTargetView(m_cpoRenderTargetView.Get(), vColor);
		}



		bool LoadFile(ID3D11Device* poD3DDevice, const std::wstring& fileName, const std::string& objectName = "");

		bool LoadFile(ID3D11Device* poD3DDevice, const uint8_t* pbinarFileData, size_t dataSize, const std::string& objectName = "");

		bool CreateBuffers(ID3D11Device* poD3DDevice, UINT width, UINT height, DXGI_FORMAT format, UINT sampleCount, const std::string& objectName = "");

		UINT GetHeight();
		UINT GetWidth();


	private:
		D3D11_TEXTURE2D_DESC m_textureDesc;  // Texture data descrtion 
		D3D11_SHADER_RESOURCE_VIEW_DESC m_shaderResourceViewDesc; // For use in input in shader
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc; // Render Target

		// buffers

		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_cpoTexture;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_cpoRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_cpoShaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_cpoDepthStencilTexture;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_poDepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_cpoDepthStencilView_ReadOnly;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_cpoDepthShaderResourceView;

	};
}