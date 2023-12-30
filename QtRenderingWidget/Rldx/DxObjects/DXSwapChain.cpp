#include "DXSwapChain.h"
#include <d3d11.h>


using namespace Rldx;

DxSwapChain::UPtr Rldx::DxSwapChain::CreateForHWND(ID3D11Device* poDevice, HWND hWindow, UINT width, UINT height)
{
	auto poNew = std::make_unique<DxSwapChain>();

	//----------------- get Factory2 interface ---------------
	IDXGIDevice2* pDXGIDevice = nullptr;
	HRESULT hr = poDevice->QueryInterface(__uuidof(IDXGIDevice2), (void**)&pDXGIDevice);
	assert(SUCCEEDED(hr));

	IDXGIAdapter* pDXGIAdapter = nullptr;;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDXGIAdapter);
	assert(SUCCEEDED(hr));

	IDXGIFactory2* pIDXGIFactory = nullptr;;
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&pIDXGIFactory);

	//--------- create swap chain ------------------

	poNew->m_SwapChainDescription = DXGI_SWAP_CHAIN_DESC1{};
	poNew->m_SwapChainDescription.Width = width;
	poNew->m_SwapChainDescription.Height = height;
	poNew->m_SwapChainDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	poNew->m_SwapChainDescription.Stereo = FALSE;
	poNew->m_SwapChainDescription.SampleDesc.Quality = 0;
	poNew->m_SwapChainDescription.SampleDesc.Count = 1;
	poNew->m_SwapChainDescription.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	poNew->m_SwapChainDescription.BufferCount = 3;
	poNew->m_SwapChainDescription.Scaling = DXGI_SCALING::DXGI_SCALING_STRETCH;
	poNew->m_SwapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	poNew->m_SwapChainDescription.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	poNew->m_SwapChainDescription.Flags = 0;

	IDXGISwapChain1** p = &poNew->m_cpoSwapChain1;
	hr = pIDXGIFactory->CreateSwapChainForHwnd(poDevice, hWindow, &poNew->m_SwapChainDescription, NULL, NULL, &poNew->m_cpoSwapChain1);
	assert(SUCCEEDED(hr));
	logging::LogActionSuccess("Created Swap Chain.");

	// Get swap chain's back buffer, create its renderQuad target view and set that view as renderQuad target
	hr = poNew->m_cpoSwapChain1->GetBuffer(0, __uuidof(*poNew->m_BackBufferTexture.GetTexture()), (void**)&poNew->m_BackBufferTexture.GetComPtrTexture());
	assert(SUCCEEDED(hr));

	logging::LogActionSuccess("m_pSwapChain1->GetBuffer().");

	//hr = m_cpoDevice->CreateRenderTargetView(po->m_oBackBuffer.getTexture(), nullptr, po->m_oBackBuffer.m_cpoRenderTargetView.ReleaseAndGetAddressOf());
	hr = poDevice->CreateRenderTargetView(poNew->m_BackBufferTexture.GetTexture(), nullptr, poNew->m_BackBufferTexture.GetComPtrRenderTargetView().ReleaseAndGetAddressOf());
	assert(SUCCEEDED(hr));

	// TODO: any way to this more cleverly?
	// Set the texture descriptor manually in backbuffer DxTexture, so it reports the right dimensions
	auto& textureDescriptor = poNew->m_BackBufferTexture.GetDescriptionRef();
	textureDescriptor.Width = poNew->m_SwapChainDescription.Width;
	textureDescriptor.Height = poNew->m_SwapChainDescription.Height;

	logging::LogActionSuccess("CreateRenderTargetView().");

	//m_oSwapChainData.m_cpoBackBuffer->Release();
	logging::LogActionSuccess("m_oSwapChainData.m_pBackBuffer->Release().");


	poNew->m_BackBufferTexture.InitDepthStencilView(poDevice, width, height);

	// set render targer
	//deviceContext()->OMSetRenderTargets(1, po->m_oBackBuffer.getRenderTargetViewCPO(), NULL);

	//LogActionSuccess("m_pDeviceContext->OMSetRenderTargets(1, &m_oSwapChainData.m_pRenderTargetView, NULL).");

	return poNew;
}

void DxSwapChain::Resize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height)
{
	deviceContext->OMSetRenderTargets(0, 0, 0);

	// Release all outstanding references to the swap chain's buffers.
	//g_pRenderTargetView->Release();
	m_BackBufferTexture.GetComPtrRenderTargetView().Reset();
	m_BackBufferTexture.GetComPtrRenderTargetView().Reset();
	m_BackBufferTexture.GetComPtrTexture().Reset();
	m_BackBufferTexture.GetComPtrDepthStencil().Reset();

	HRESULT hr; 
	// Preserve the existing buffer count and format.
	// Automatically choose the width and height to match the client rect for HWNDs.
	//hr = g_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

	hr = m_cpoSwapChain1->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

	// Perform error handling here!
	assert(SUCCEEDED(hr));	

	// Get buffer and create a render-target-view.
	//ID3D11Texture2D* pBuffer;
	//hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
	//	(void**)&pBuffer);
	hr = m_cpoSwapChain1->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(void**)&m_BackBufferTexture.GetComPtrTexture());
	// Perform error handling here!
	

	assert(SUCCEEDED(hr));

	//hr = g_pd3dDevice->CreateRenderTargetView(pBuffer, NULL,
	//	&g_pRenderTargetView);
	hr = device->CreateRenderTargetView(m_BackBufferTexture.GetComPtrTexture().Get(), NULL,
		&m_BackBufferTexture.GetComPtrRenderTargetView());
	// Perform error handling here!

	//pBuffer->Release();

	UpdateViewPort(deviceContext, nullptr );

	m_BackBufferTexture.InitDepthStencilView(device, m_textureDesc.Width, m_textureDesc.Height);

	//g_pd3dDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
	//auto tempRTV = m_BackBufferTexture.GetRenderTargetView();

	//deviceContext->OMSetRenderTargets(1, &tempRTV, m_BackBufferTexture.GetDepthStencilView());

	//// Set up the viewport.
	//D3D11_VIEWPORT vp;
	//vp.Width = width;
	//vp.Height = height;
	//vp.MinDepth = 0.0f;
	//vp.MaxDepth = 1.0f;
	//vp.TopLeftX = 0;
	//vp.TopLeftY = 0;
	//deviceContext->RSSetViewports(1, &vp);
}

void DxSwapChain::Present(ID3D11DeviceContext* poDXDeviceContext)
{
	m_cpoSwapChain1->Present(1, 0);
}

void DxSwapChain::UpdateViewPort(ID3D11DeviceContext* _pDeviceContext, QWidget* _renderView)
{
	m_BackBufferTexture.GetTexture()->GetDesc(&m_textureDesc);
	
	memset(&m_viewPort, 0, sizeof(D3D11_VIEWPORT));
	m_viewPort.Height = (float)m_textureDesc.Height;
	m_viewPort.Width = (float)m_textureDesc.Width;
	m_viewPort.MinDepth = 0;
	m_viewPort.MaxDepth = 1;

	_pDeviceContext->RSSetViewports(1, &m_viewPort);
}
