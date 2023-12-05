#pragma once

#include <DirectXMath.h>
#include <wrl\client.h>
#include <d3d11.h>
#include "..\..\..\..\DirectXTK\Inc\DDSTextureLoader.h"
#include "..\..\..\..\DirectXTK\Inc\SimpleMath.h"
#include "IDrawable.h"

#include <string>



namespace Rldx
{


	class IDxTexture
	{
		virtual ID3D11Texture2D* GetTexture() const = 0;
		virtual ID3D11RenderTargetView* GetRenderTargetView() const = 0;
		virtual ID3D11ShaderResourceView* GetShaderResourceView() const = 0;
		virtual ID3D11RenderTargetView** GetAddressOfRenderTargetView() = 0;
		virtual ID3D11ShaderResourceView** getAddressOfShaderResourceView() = 0;
		virtual ID3D11Texture2D** getGetAddressOfTexture() = 0;
	};	

	class DxTexture : public IResizable
	{
	public:
		


		virtual void Reset(ID3D11Device* _poDevice, ID3D11DeviceContext* _poDeviceContext, unsigned int width, unsigned int height) override;
		UINT GetHeight();
		UINT GetWidth();
		UINT GetSampleCount();
		UINT GetSlot();
		virtual ID3D11RenderTargetView* GetRenderTargetView() const /*override*/ { return m_cpoRenderTargetView.Get();};
		virtual ID3D11ShaderResourceView* GetShaderResourceView() const  /*override*/ { return m_cpoShaderResourceView.Get();};
		virtual ID3D11RenderTargetView** GetAddressOfRenderTargetView() /*override*/ { return m_cpoRenderTargetView.GetAddressOf(); };
		virtual ID3D11ShaderResourceView** GetAddressOfShaderResourceView()  /*override*/ { return m_cpoShaderResourceView.GetAddressOf(); };

		virtual ID3D11Texture2D* GetTexture() const  /*override*/ { return m_cpoTexture.Get();};
		virtual ID3D11Texture2D** GetGetAddressOfTexture() /*override*/ { return m_cpoTexture.GetAddressOf(); };
				
		virtual Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetComPtrRenderTargetView()  /*override*/ {
			return m_cpoRenderTargetView;
		};

		virtual Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetComPtrShaderResourceView() /*override*/{
			return m_cpoShaderResourceView;
		};

		virtual Microsoft::WRL::ComPtr<ID3D11Texture2D>& GetComPtrTexture() /*override*/{
			return m_cpoTexture;
		};

		void BindAsRenderTargetView(ID3D11DeviceContext* deviceContext)
		{
			auto pRTV = GetRenderTargetView();
			ID3D11RenderTargetView* const ppRTV[] = { pRTV };
			deviceContext->OMSetRenderTargets(1, ppRTV, NULL);
		}

		void BindAsShaderResourceView(ID3D11DeviceContext* deviceContext, UINT slot)
		{
			auto pSRV = GetShaderResourceView();
			deviceContext->PSSetShaderResources(slot, 1, &pSRV);
		}

		void ClearPixels(ID3D11DeviceContext* pDeviceContext, sm::Color vColor)
		{
			pDeviceContext->ClearRenderTargetView(m_cpoRenderTargetView.Get(), vColor);
		}

		bool LoadFile(ID3D11Device* poD3DDevice, const std::wstring& fileName, const std::string& objectName = "");
		bool LoadFile(ID3D11Device* poD3DDevice, const uint8_t* pbinarFileData, size_t dataSize, const std::string& objectName = "");

		bool CreateBuffers(ID3D11Device* poD3DDevice, UINT width, UINT height, DXGI_FORMAT format, UINT sampleCount, const std::string& objectName = "");

	private:
		HRESULT Create2dTextureBuffer(ID3D11Device* poD3DDevice, UINT width, UINT height, DXGI_FORMAT format, UINT sampleCount = 1);
		//HRESULT Create3dTextureBuffer(ID3D11Device* poD3DDevice, UINT width, UINT height, UINT depth, DXGI_FORMAT format, UINT sampleCount);
		HRESULT CreateShaderResourceViewBuffer(ID3D11Device* poD3DDevice);
		HRESULT CreateRenderTargetViewBuffer(ID3D11Device* poD3DDevice);

	private:
		D3D11_TEXTURE2D_DESC m_textureDesc;  // Texture data descrtion 
		D3D11_SHADER_RESOURCE_VIEW_DESC m_shaderResourceViewDesc; // For use in input in shader
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc; // Render Target

		// Texture buffer
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_cpoTexture;

		// Interfaces to use texture memory "render to/from"
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_cpoRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_cpoShaderResourceView;
		
		// Depth buffer/stencil interfaces
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_cpoDepthStencilTexture;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_poDepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_cpoDepthStencilView_ReadOnly;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_cpoDepthShaderResourceView;

	};

	/// <summary>
	/// Convenience class for easily RTVs to the set render target d3d method
	/// </summary>
	class RTVContainer	{

		RTVContainer(const std::vector<ID3D11RenderTargetView*>& input)
		{
			m_vecRTVs = input;
		}

		operator ID3D11RenderTargetView* const* ()
		{
			m_vecRTVs.data();
		}

	private:
		std::vector<ID3D11RenderTargetView*> m_vecRTVs;

	};


}