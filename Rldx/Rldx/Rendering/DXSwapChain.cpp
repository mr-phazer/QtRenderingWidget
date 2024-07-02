#include <CustomExceptions\CustomExceptions.h>
#include "d3d11.h"
#include "DXSwapChain.h"

using namespace rldx;
using namespace logging;

void rldx::DxSwapChain::ConfigureBackBuffer(ID3D11Device* poDevice, ID3D11DeviceContext* poPC)
{
	HRESULT hr = S_OK;

	hr = m_cpoSwapChain1->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)&m_BackBufferTexture.GetComPtrTexture());



	hr = poDevice->CreateRenderTargetView(
		m_BackBufferTexture.GetComPtrTexture().Get(),
		nullptr,
		m_BackBufferTexture.GetComPtrRenderTargetView().GetAddressOf()
	);

	m_BackBufferTexture.GetComPtrTexture()->GetDesc(&m_BackBufferTexture.GetDescriptionRef());

	// Create a depth-stencil view for use with 3D rendering if needed.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		static_cast<UINT> (m_BackBufferTexture.GetDescriptionRef().Width),
		static_cast<UINT> (m_BackBufferTexture.GetDescriptionRef().Height),
		1, // This depth stencil view has only one texture.
		1, // Use a single mipmap level.
		D3D11_BIND_DEPTH_STENCIL
	);

	// TODO: Does thist work?

	poDevice->CreateTexture2D(
		&depthStencilDesc,
		nullptr,
		&m_BackBufferTexture.GetComPtrDepthStencil()
	);

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);

	hr = poDevice->CreateDepthStencilView(
		m_BackBufferTexture.GetComPtrDepthStencil().Get(),
		&depthStencilViewDesc,
		&m_BackBufferTexture.GetComPtrDepthStencilView()
	);

	ThrowAndLogIfAiled(L"Error Creating Depth Stencil", COMExceptionFormatMode::StandardLog, hr);

	ZeroMemory(&m_viewPort, sizeof(D3D11_VIEWPORT));
	m_viewPort.Height = (float)m_BackBufferTexture.GetDescriptionRef().Height;
	m_viewPort.Width = (float)m_BackBufferTexture.GetDescriptionRef().Width;
	m_viewPort.MinDepth = 0;
	m_viewPort.MaxDepth = 1;

	poPC->RSSetViewports(1, &m_viewPort);
}

DxSwapChain::Uptr rldx::DxSwapChain::CreateForHWND(ID3D11Device* poDevice, HWND hWindow, bool useSRGB, UINT width, UINT height)
{
	auto poNew = std::make_unique<DxSwapChain>();

	// Get a factory2 interface from the device
	IDXGIDevice2* pDXGIDevice = nullptr;
	HRESULT hr = poDevice->QueryInterface(__uuidof(IDXGIDevice2), (void**)&pDXGIDevice);
	ThrowAndLogIfAiled(L"Getting extended d3d 11 device", COMExceptionFormatMode::StandardLog, hr);

	IDXGIAdapter* pDXGIAdapter = nullptr;;

	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDXGIAdapter);
	ThrowAndLogIfAiled(L"Get device paranet", COMExceptionFormatMode::StandardLog, hr);

	IDXGIFactory2* pIDXGIFactory = nullptr;;
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&pIDXGIFactory);

	// TODO: Put in helper/private function: MakeWindowSwapChain(width, height)
	//--------- create swap chain ------------------
	poNew->m_SwapChainDescription = DXGI_SWAP_CHAIN_DESC1{};
	poNew->m_SwapChainDescription.Width = width;
	poNew->m_SwapChainDescription.Height = height;
	poNew->m_SwapChainDescription.Format = (useSRGB) ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM;
	poNew->m_SwapChainDescription.Stereo = FALSE;
	poNew->m_SwapChainDescription.SampleDesc.Quality = 0;
	poNew->m_SwapChainDescription.SampleDesc.Count = 1;
	poNew->m_SwapChainDescription.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	poNew->m_SwapChainDescription.BufferCount = 4; // 3 // TODO: change back to 3, INTENTIONal error
	poNew->m_SwapChainDescription.Scaling = DXGI_SCALING::DXGI_SCALING_STRETCH;
	poNew->m_SwapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
	poNew->m_SwapChainDescription.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	poNew->m_SwapChainDescription.Flags = 0;

	IDXGISwapChain1** p = &poNew->m_cpoSwapChain1;
	hr = pIDXGIFactory->CreateSwapChainForHwnd(poDevice, hWindow, &poNew->m_SwapChainDescription, NULL, NULL, &poNew->m_cpoSwapChain1);
	ThrowAndLogIfAiled(L"Getting extend d3d 11 interface...", COMExceptionFormatMode::StandardLog, hr);

	// Get swap chain's back buffer, Store it in texture class
	// TODO: create its renderQuad target view and set that view as renderQuad target (in the texture class)
	hr = poNew->m_cpoSwapChain1->GetBuffer(0, __uuidof(*poNew->m_BackBufferTexture.GetTexture2D()), (void**)&poNew->m_BackBufferTexture.GetComPtrTexture());
	ThrowAndLogIfAiled(L"m_pSwapChain1->GetBuffer().", COMExceptionFormatMode::StandardLog, hr);

	//hr = m_cpoDevice->CreateRenderTargetView(po->m_oBackBuffer.getTexture(), nullptr, po->m_oBackBuffer.m_cpoRenderTargetView.ReleaseAndGetAddressOf());
	hr = poDevice->CreateRenderTargetView(poNew->m_BackBufferTexture.GetTexture2D(), nullptr, poNew->m_BackBufferTexture.GetComPtrRenderTargetView().ReleaseAndGetAddressOf());
	ThrowAndLogIfAiled(L"CreateRenderTargetView()...", COMExceptionFormatMode::StandardLog, hr);

	// TODO: any way to this more cleverly?
	// Set the texture descriptor manually in backbuffer DxTexture, so it reports the right dimensions
	auto& textureDescriptor = poNew->m_BackBufferTexture.GetDescriptionRef();
	textureDescriptor.Width = poNew->m_SwapChainDescription.Width;
	textureDescriptor.Height = poNew->m_SwapChainDescription.Height;
	poNew->m_BackBufferTexture.InitDepthStencilView(poDevice, width, height);

	Logger::LogActionSuccess(L"Finished making Swap Chain For Window");

	return poNew;
}

void DxSwapChain::Resize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height)
{
	ReleaseBackBuffer(deviceContext);

	HRESULT hr = m_cpoSwapChain1->ResizeBuffers(
		0,                   // Number of buffers. Set this to 0 to preserve the existing setting.
		width, height,                // Width and height of the swap chain. MUST be set to a non-zero value. For example, match the window size.
		DXGI_FORMAT_UNKNOWN, // This tells DXGI to retain the current back buffer format.
		0
	);

	ConfigureBackBuffer(device, deviceContext);
}

void DxSwapChain::Present(ID3D11DeviceContext* poDXDeviceContext)
{
	m_cpoSwapChain1->Present(1, 0);
}

void DxSwapChain::UpdateViewPort(ID3D11DeviceContext* _pDeviceContext, QWidget* _renderView)
{
	m_BackBufferTexture.GetTexture2D()->GetDesc(&m_textureDesc);

	memset(&m_viewPort, 0, sizeof(D3D11_VIEWPORT));
	m_viewPort.Height = (float)m_textureDesc.Height;
	m_viewPort.Width = (float)m_textureDesc.Width;
	m_viewPort.MinDepth = 0;
	m_viewPort.MaxDepth = 1;

	_pDeviceContext->RSSetViewports(1, &m_viewPort);
}
