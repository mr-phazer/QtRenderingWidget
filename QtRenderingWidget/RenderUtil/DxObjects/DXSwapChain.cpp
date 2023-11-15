#include "DXSwapChain.h"
#include <d3d11.h>


using namespace Rldx;

inline void DxSwapChain::Reset(UINT width, UINT height, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	ID3D11Texture2D* pBackBuffer = nullptr;

	//m_oSwapChainData.m_pRenderTargetView->AddRef();
	//if (m_oSwapChainData.m_pRenderTargetView->Release() > 0)
	//{
	//	m_oSwapChainData.m_pRenderTargetView->Release();
	//}
	//else
	//	m_oSwapChainData.m_pRenderTargetView = nullptr;


	*m_BackBufferTexture.GetRenderTargetViewCPO().ReleaseAndGetAddressOf() = nullptr;
	m_BackBufferTexture.GetTextureCPO().ReleaseAndGetAddressOf();
	//m_oBackBuffer.m_cpoRenderTargetView = nullptr;



	////m_poDepthStencilView->AddRef();
	//m_poDepthStencilView.ReleaseAndGetAddressOf();
	//m_poDepthStencilView = nullptr;
	//m_cpoDepthShaderResourceView.ReleaseAndGetAddressOf();
	//m_cpoDepthShaderResourceView = nullptr;

	//m_cpoDepthViewTexture.ReleaseAndGetAddressOf();

	deviceContext->ClearState();
	deviceContext->Flush();

	//HRESULT hr = m_oSwapChainData.m_pSwapChain1->GetBuffer(0, __uuidof(*m_oSwapChainData.m_cpoBackBuffer.GetAddressOf()), (void**)m_oSwapChainData.m_cpoBackBuffer.GetAddressOf());
	//auto po = m_oSwapChainData.m_cpoBackBuffer.Reset();

	HRESULT hrResize = (m_cpoSwapChain1->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0));
	//std::wstring wstrErrorMsghrResize = _com_error(hrResize).ErrorMessage();

	HRESULT hrGetBuffer = (m_cpoSwapChain1->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)));
	//std::wstring	wstrErrorMsghrhrGetBuffer = _com_error(hrResize).ErrorMessage();

	HRESULT hrCreateRRTV = device->CreateRenderTargetView(pBackBuffer, NULL, &m_BackBufferTexture.GetRenderTargetViewCPO());
	//HRESULT hrCreateRRTV = device->CreateRenderTargetView(pBackBuffer, NULL, &m_oBackBuffer.m_cpoRenderTargetView);
	pBackBuffer->Release();

	// TODO: are those to be part of 'DXSwapChain' ?
	//updateViewPort();
	//createDeptBufferAndhStencil();

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
