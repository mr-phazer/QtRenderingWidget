#pragma once

// --- Direct3d 11 Headers
#include <d3d11.h>
#include <d3d11_1.h>
#include <D3Dcompiler.h>

//  WRL wrappers from COM Objects - smart pointers for Direct3d resources
#include <wrl/client.h>
#include <wrl/wrappers/corewrappers.h>

// Standard lib headers
#include <assert.h>
#include <stdexcept>
#include <memory>
#include <functional>
#include <vector>


// qt headers
#include <QtWidgets/qwidget.h>

#include <DirectXMath.h>

// author headers:
#include "..\..\..\Logging\Logging.h"

// TODO: use new or old?
//#include "DxTextureView.h"
#include "DxTexture.h"

#include "..\..\Interfaces\IDrawable.h"


namespace Rldx
{
	class DxSwapChain
	{
	public:
		// TODO: is this relly needeed? How often do you need use the smart pointer type?
		using UniquePtr = std::unique_ptr<DxSwapChain>;
	public:
		DxSwapChain()
		{
			m_backgroundColor = Debug_GetRandomColor();
		}

		static DirectX::XMFLOAT4 Debug_GetRandomColor()
		{
			DirectX::XMFLOAT4 randomColor{ };
			const float divider = 100000;
			randomColor.x = ((float)(rand() % (int)divider)) / divider;
			randomColor.y = ((float)(rand() % (int)divider)) / divider;
			randomColor.z = ((float)(rand() % (int)divider)) / divider;
			randomColor.w = 1.0f;

			return randomColor;
		}

		static UniquePtr CreateForHWND(ID3D11Device* poDevice, HWND hWindow, UINT width = 1024, UINT height = 1024);

		void Resize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height);
		void Present(ID3D11DeviceContext* poDXDeviceContext);
		DxTexture* GetBackBuffer() { return &m_BackBufferTexture; };
		void UpdateViewPort(ID3D11DeviceContext* _pDeviceContext, QWidget* _renderView);
		
		// TODO: should be in camera?		
		D3D11_VIEWPORT m_viewPort;
		D3D11_TEXTURE2D_DESC m_textureDesc = { 0 };

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_cpoSwapChain1;
		DxTexture m_BackBufferTexture;
		DXGI_SWAP_CHAIN_DESC1 m_SwapChainDescription = { 0 };
		DirectX::XMFLOAT4 m_backgroundColor;

		

	};
};
