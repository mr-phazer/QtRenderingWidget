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

void DxSwapChain::Reset(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height)
{
	//ID3D11Texture2D* pBackBuffer = nullptr;

	////m_oSwapChainData.m_pRenderTargetView->AddRef();
	////if (m_oSwapChainData.m_pRenderTargetView->Release() > 0)
	////{
	////	m_oSwapChainData.m_pRenderTargetView->Release();
	////}
	////else
	////	m_oSwapChainData.m_pRenderTargetView = nullptr;


	//*m_BackBufferTexture.GetComPtrRenderTargetView().ReleaseAndGetAddressOf() = nullptr;
	////m_BackBufferTexture.GetComPtrTexture().ReleaseAndGetAddressOf();
	////m_oBackBuffer.m_cpoRenderTargetView = nullptr;



	//////m_poDepthStencilView->AddRef();
	////m_poDepthStencilView.ReleaseAndGetAddressOf();
	////m_poDepthStencilView = nullptr;
	////m_cpoDepthShaderResourceView.ReleaseAndGetAddressOf();
	////m_cpoDepthShaderResourceView = nullptr;

	////m_cpoDepthViewTexture.ReleaseAndGetAddressOf();

	//deviceContext->ClearState();
	//deviceContext->Flush();

	////HRESULT hr = m_oSwapChainData.m_pSwapChain1->GetBuffer(0, __uuidof(*m_oSwapChainData.m_cpoBackBuffer.GetAddressOf()), (void**)m_oSwapChainData.m_cpoBackBuffer.GetAddressOf());
	////auto po = m_oSwapChainData.m_cpoBackBuffer.Reset();

	//HRESULT hrResize = (m_cpoSwapChain1->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0));
	////std::wstring wstrErrorMsghrResize = _com_error(hrResize).ErrorMessage();

	//HRESULT hrGetBuffer = (m_cpoSwapChain1->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)));
	////std::wstring	wstrErrorMsghrhrGetBuffer = _com_error(hrResize).ErrorMessage();

	//HRESULT hrCreateRRTV = device->CreateRenderTargetView(pBackBuffer, NULL, &m_BackBufferTexture.GetComPtrRenderTargetView());
	////HRESULT hrCreateRRTV = device->CreateRenderTargetView(pBackBuffer, NULL, &m_oBackBuffer.m_cpoRenderTargetView);
	//pBackBuffer->Release();

	//m_BackBufferTexture.InitDepthStencilView(device, width, height);

	//// TODO: are those to be part of 'DXSwapChain' ?
	////updateViewPort();
	////createDeptBufferAndhStencil();


			//ID3D11Texture2D* pBackBuffer = Q_NULLPTR;			
	ID3D11Texture2D* pBackBuffer = nullptr;

	//ReleaseObject(m_oSwapChainData.m_pRenderTargetView);
	m_BackBufferTexture.GetComPtrRenderTargetView().ReleaseAndGetAddressOf();

	//DXCall(m_pSwapChain->ResizeBuffers(0, width(), height(), DXGI_FORMAT_UNKNOWN, 0));
	m_cpoSwapChain1->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

	//DXCall(m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)));
	m_cpoSwapChain1->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

	//DXCall(m_cpoDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_oSwapChainData.m_pRenderTargetView));
	device->CreateRenderTargetView(pBackBuffer, NULL, &m_BackBufferTexture.GetComPtrRenderTargetView());

	pBackBuffer->Release();

	m_BackBufferTexture.InitDepthStencilView(device, width, height);

}

void DxSwapChain::Present(ID3D11DeviceContext* poDXDeviceContext)
{
	m_cpoSwapChain1->Present(1, 0);
}

inline void DxSwapChain::UpdateViewPort(ID3D11DeviceContext* _pDeviceContext, QWidget* _renderView)
{
	//// Set viewport
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.Width = (FLOAT)_renderView->width();
	m_viewPort.Height = (FLOAT)_renderView->height();
	m_viewPort.MinDepth = 0.f;
	m_viewPort.MaxDepth = 1.f;
	_pDeviceContext->RSSetViewports(1, &m_viewPort);
}
