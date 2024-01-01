#pragma once

#include <DirectXMath.h>
#include <wrl\client.h>
#include <d3d11.h>
#include "..\..\..\..\DirectXTK\Inc\DDSTextureLoader.h"
#include "..\..\..\..\DirectXTK\Inc\SimpleMath.h"

#include "..\..\Rldx\Interfaces\IDrawable.h"
#include "..\..\Rldx\Interfaces\IResizable.h"

#include <string>

#include "..\Managers\ResourceManager\IDxResouce.h"


namespace Rldx {

	class IDxTexture
	{
		virtual ID3D11Texture2D* GetTexture() const = 0;
		virtual ID3D11RenderTargetView* GetRenderTargetView() const = 0;
		virtual ID3D11ShaderResourceView* GetShaderResourceView() const = 0;
		virtual ID3D11RenderTargetView** GetAddressOfRenderTargetView() = 0;
		virtual ID3D11ShaderResourceView** getAddressOfShaderResourceView() = 0;
		virtual ID3D11Texture2D** getGetAddressOfTexture() = 0;
	};	

	class DxTexture : public IResizable, public IDxResource
	{
	public:		
		ResourceTypeEnum GetType() const override;
		std::string GetTypeString() const override;

		virtual void Resize(ID3D11Device* _poDevice, ID3D11DeviceContext* _poDeviceContext, unsigned int width, unsigned int height) override;
		UINT GetHeight();
		UINT GetWidth();
		UINT GetSampleCount();		
		float GetAspectRatio();

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

		virtual Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetComPtrDepthStencil() /*override*/{
			return m_cpoDepthStencilView;
		};

		virtual Microsoft::WRL::ComPtr<ID3D11Texture2D>& GetComPtrTexture() /*override*/{
			return m_cpoTexture;
		};

		ID3D11DepthStencilView* GetDepthStencilView() const
		{
			return m_cpoDepthStencilView.Get();
		}

		void BindAsRenderTargetView(ID3D11DeviceContext* deviceContext)
		{
			auto pRTV = GetRenderTargetView();
			ID3D11RenderTargetView* const ppRTV[] = { pRTV };
			deviceContext->OMSetRenderTargets(1, ppRTV, NULL);
		}

		void BindAsRenderTargetViewWithDepthBuffer(ID3D11DeviceContext* deviceContext)
		{
			auto pRTV = GetRenderTargetView();
			ID3D11RenderTargetView* const ppRTV[] = { pRTV };
			deviceContext->OMSetRenderTargets(1, ppRTV, m_cpoDepthStencilView.Get());
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

		void ClearPixelsAndDepthBuffer(ID3D11DeviceContext* pDeviceContext, sm::Color vColor)
		{
			pDeviceContext->ClearRenderTargetView(m_cpoRenderTargetView.Get(), vColor);

			pDeviceContext->ClearDepthStencilView(
				m_cpoDepthStencilView.Get(),
				D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
				1.0f,
				0);
		}

		bool LoadFile(ID3D11Device* poD3DDevice, const std::wstring& fileName, const std::string& objectName = "");
		bool LoadFile(ID3D11Device* poD3DDevice, const uint8_t* pbinarFileData, size_t dataSize, const std::string& objectName = "");

		bool CreateBuffers(ID3D11Device* poD3DDevice, UINT width, UINT height, DXGI_FORMAT format, UINT sampleCount, const std::string& objectName = "");

		// TODO: more elegant way of updating dimensions when used a backbuffer in swapchain?
		D3D11_TEXTURE2D_DESC& GetDescriptionRef() { return m_textureDesc; }

		bool InitDepthStencilView(
			ID3D11Device* poDevice,
			UINT width, UINT height,
			DXGI_FORMAT Format = DXGI_FORMAT_R32_TYPELESS,
			UINT sampleCount = 1,
			const char* szObjectName = nullptr)
		{
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Create Depth buffer TEXTURE
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			HRESULT hr = S_OK;
			D3D11_TEXTURE2D_DESC depthTextureDesc;
			ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
			depthTextureDesc.Width = width;
			depthTextureDesc.Height = height;
			depthTextureDesc.MipLevels = 1;
			depthTextureDesc.ArraySize = 1;

			depthTextureDesc.SampleDesc = { sampleCount, 0 };
			depthTextureDesc.Format = DXGI_FORMAT_R32_TYPELESS;//  DXGI_FORMAT_R24G8_TYPELESS; //DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			depthTextureDesc.SampleDesc.Count = sampleCount;
			depthTextureDesc.SampleDesc.Quality = 0;
			depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;

			hr = poDevice->CreateTexture2D(&depthTextureDesc, NULL,
				m_cpoDepthStencilTexture.GetAddressOf());
			assert(SUCCEEDED(hr));

			//string szDepthTextureName = string("DepthTeX_") + std::to_string(sm_NextId);
			/*m_cpoDepthStencilTexture->SetPrivateData(WKPDID_D3DDebugObjectName,
													 szDepthTextureName.size() - 1, szDepthTextureName.data());*/
													 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

													 /////////////////////////////////////////////////////////////////////////////////////////////////////////
													 // Create Depth  SHADER RESOURCE VIEW
													 /////////////////////////////////////////////////////////////////////////////////////////////////////////
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT; //DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			shaderResourceViewDesc.ViewDimension =
				(sampleCount > 1) ?
				D3D11_SRV_DIMENSION_TEXTURE2DMS :
				D3D11_SRV_DIMENSION_TEXTURE2D;

			shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;

			// Create the shader resource view.
			hr = poDevice->CreateShaderResourceView(m_cpoDepthStencilTexture.Get(),
				&shaderResourceViewDesc, m_cpoDepthShaderResourceView.GetAddressOf());
			assert(SUCCEEDED(hr));

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Create Depth Stencil VIEW
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
			::ZeroMemory(&dsvDesc, sizeof(dsvDesc));

			dsvDesc.Flags = 0;
			dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
			dsvDesc.ViewDimension =
				(sampleCount > 1) ?
				D3D11_DSV_DIMENSION_TEXTURE2DMS :
				D3D11_DSV_DIMENSION_TEXTURE2D;

			dsvDesc.Texture2DArray.ArraySize = 1;
			dsvDesc.Texture2DArray.FirstArraySlice = 0;
			dsvDesc.Texture2DArray.MipSlice = 0;

			hr = poDevice->CreateDepthStencilView(m_cpoDepthStencilTexture.Get(), &dsvDesc,
				m_cpoDepthStencilView.GetAddressOf());
			assert(SUCCEEDED(hr));

			//////////////////////////////////////////////////////
			// Create READ ONLY Depth Stencil VIEW
			//////////////////////////////////////////////////////
			dsvDesc.Flags = D3D11_DSV_READ_ONLY_DEPTH;
			dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // DXGI_FORMAT_D24_UNORM_S8_UINT;
			hr = poDevice->CreateDepthStencilView(m_cpoDepthStencilTexture.Get(), &dsvDesc,
				m_cpoDepthStencilView_ReadOnly.GetAddressOf());
			assert(SUCCEEDED(hr));

			/************************
			* 	Debug naming
			************************/
			//string szDepthStencilViewNameReadOny = "DSV_RO_" + std::to_string(sm_NextId);
			//m_cpoDepthStencilView_ReadOnly->SetPrivateData(WKPDID_D3DDebugObjectName,
			//											   szDepthStencilViewNameReadOny.size() - 1, szDepthStencilViewNameReadOny.data());

													   ///*=============================================================
													   //	Create Stencil
													   //=============================================================*/
													   //D3D11_DEPTH_STENCIL_DESC dsDesc;
													   //// Depth initLog parameters
													   //dsDesc.DepthEnable = true;
													   //dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
													   //dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

													   //// Stencil initLog parameters
													   //dsDesc.StencilEnable = false;
													   //dsDesc.StencilReadMask = 0x00;
													   //dsDesc.StencilWriteMask = 0x00;

													   //// Stencil operations if pixel is front-facing
													   //dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
													   //dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
													   //dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
													   //dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

													   //// Stencil operations if pixel is back-facing
													   //dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
													   //dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
													   //dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
													   //dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

													   //// Create depth stencil state
													   //ID3D11DepthStencilState* pDSState;
													   //hr = _poScene->getDevice()->CreateDepthStencilState(&dsDesc, &m_poDepthStencilState_On);

													   //dsDesc.DepthEnable = false;
													   //hr = _poScene->getDevice()->CreateDepthStencilState(&dsDesc, &m_poDepthStencilState_Off);

													   //assert(SUCCEEDED(hr));

													   //_poScene->getDeviceContext()->OMSetDepthStencilState(m_poDepthStencilState_On, 1);
													   //m_width = _width;
													   //m_height = _height;

			return SUCCEEDED(hr);
		}



	private:
		HRESULT Create2dTextureBuffer(ID3D11Device* poD3DDevice, UINT width, UINT height, DXGI_FORMAT format, UINT sampleCount = 1);
		//HRESULT Create3dTextureBuffer(ID3D11Device* poD3DDevice, UINT width, UINT height, UINT depth, DXGI_FORMAT format, UINT sampleCount);
		HRESULT CreateShaderResourceViewBuffer(ID3D11Device* poD3DDevice);
		HRESULT CreateRenderTargetViewBuffer(ID3D11Device* poD3DDevice);



	private:
		D3D11_TEXTURE2D_DESC m_textureDesc = {0};  // Texture data descrtion 
		D3D11_SHADER_RESOURCE_VIEW_DESC m_shaderResourceViewDesc = { DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM_SRGB, D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D }; // For use in input in shader
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = { DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM_SRGB, D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D }; // For use in input in shader; // Render Target

		// Texture buffer
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_cpoTexture;

		// Interfaces to use texture memory "render to/from"
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_cpoRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_cpoShaderResourceView;
		
		// Depth buffer/stencil interfaces
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_cpoDepthStencilTexture;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_cpoDepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_cpoDepthStencilView_ReadOnly;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_cpoDepthShaderResourceView;
	};

	///// <summary>
	///// Convenience class for easily RTVs to the set render target d3d method
	///// </summary>
	//class RTVContainer	{

	//	RTVContainer(const std::vector<ID3D11RenderTargetView*>& input)
	//	{
	//		m_vecRTVs = input;
	//	}

	//	operator ID3D11RenderTargetView* const* ()
	//	{
	//		m_vecRTVs.data();
	//	}

	//private:
	//	std::vector<ID3D11RenderTargetView*> m_vecRTVs;

	//};


}