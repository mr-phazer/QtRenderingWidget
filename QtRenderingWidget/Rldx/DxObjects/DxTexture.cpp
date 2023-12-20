
#include "..\..\..\DirectXTK\Src\PlatformHelpers.h"

#include "d3d11.h"

// Author
#include "DxTexture.h"
#include "..\Managers\ResourceManager\DxResourceManager.h"

/*inline void Rldx::DxTexture::SetToActiveTargetView(ID3D11Deviceentext* deviceContext)
{
	auto pRTV = GetRenderTargetView();
	ID3D11RenderTargetView* const ppRTV[] = { pRTV };
	deviceContext->OMSetRenderTargets(1, ppRTV, NULL);
}

inline void Rldx::DxTexture::clearPixels(ID3D11DeviceContext* pDeviceContext, sm::Color vColor)
{
	pDeviceContext->ClearRenderTargetView(m_cpoRenderTargetView.Get(), vColor);
}*/

inline bool Rldx::DxTexture::LoadFile(ID3D11Device* poD3DDevice, const std::wstring& fileName, const std::string& objectName)
{
	// TODO: get to work, get texture data into "m_cpoTexture"
	ID3D11Resource* poTextureResource = nullptr;
	DirectX::CreateDDSTextureFromFile(poD3DDevice, fileName.c_str(), &poTextureResource, &m_cpoShaderResourceView);

	// Assume that pResource is a valid pointer to an ID3D11Resource object.
	
	HRESULT hr = poTextureResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&m_cpoTexture);

	// TODO: check
	if (SUCCEEDED(hr)) {

		auto DEBUG_1 = 1;
		// pTexture2D now points to the ID3D11Texture2D interface of the resource.
	}
	else {
		auto DEBUG_ERRROR = 1;
		// Handle failure here.
	}

	// TODO: fix
	//DxResourceManager::Instance()->GetTextures()->AddResource(this, objectName);
}

inline bool Rldx::DxTexture::LoadFile(ID3D11Device* poD3DDevice, const uint8_t* pbinarFileData, size_t dataSize, const std::string& objectName)
{
	// TODO: get to work, get texture data into "m_cpoTexture"
	//DirectX::CreateDDSTextureFromMemory(poD3DDevice, pbinarFileData, dataSize, &m_cpoTexture, &m_cpoShaderResourceView);

	return true;
}

inline bool Rldx::DxTexture::CreateBuffers(ID3D11Device* poD3DDevice, UINT width, UINT height, DXGI_FORMAT format, UINT sampleCount, const std::string& objectName)
{
	HRESULT hr = S_OK;

	Create2dTextureBuffer(poD3DDevice,  width, height, format, sampleCount);
	CreateRenderTargetViewBuffer(poD3DDevice);
	CreateShaderResourceViewBuffer(poD3DDevice);

	return true;
}

inline HRESULT Rldx::DxTexture::Create2dTextureBuffer(ID3D11Device* poD3DDevice, UINT width, UINT height, DXGI_FORMAT format, UINT sampleCount)
{
	ZeroMemory(&m_textureDesc, sizeof(m_textureDesc));
	ZeroMemory(&m_shaderResourceViewDesc, sizeof(m_shaderResourceViewDesc));

	m_textureDesc.Width = width;
	m_textureDesc.Height = height;
	m_textureDesc.MipLevels = 1;
	m_textureDesc.ArraySize = 1;
	m_textureDesc.Format = format;
	m_textureDesc.SampleDesc.Count = sampleCount;
	m_textureDesc.SampleDesc.Quality = 0;
	m_textureDesc.Usage = D3D11_USAGE_DEFAULT;

	if (format == DXGI_FORMAT_R32_TYPELESS)
		m_textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	else
		m_textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	m_textureDesc.CPUAccessFlags = 0;
	m_textureDesc.MiscFlags = 0;

	// Create the texture
	HRESULT hr = poD3DDevice->CreateTexture2D(&m_textureDesc, NULL, m_cpoTexture.GetAddressOf());

	DirectX::ThrowIfFailed(hr);

	return hr;
}

inline HRESULT Rldx::DxTexture::CreateShaderResourceViewBuffer(ID3D11Device* poD3DDevice)
{
	m_shaderResourceViewDesc.Format = m_textureDesc.Format;
	m_shaderResourceViewDesc.ViewDimension =
		(m_textureDesc.SampleDesc.Count > 1) ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
	m_shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	m_shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	HRESULT hr = poD3DDevice->CreateShaderResourceView(m_cpoTexture.Get(), &m_shaderResourceViewDesc, &m_cpoShaderResourceView);
	
	DirectX::ThrowIfFailed(hr);

	return hr;
}

inline HRESULT Rldx::DxTexture::CreateRenderTargetViewBuffer(ID3D11Device* poD3DDevice)
{
	renderTargetViewDesc.Format = m_textureDesc.Format;
	renderTargetViewDesc.ViewDimension =
		(m_textureDesc.SampleDesc.Count > 1) ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;

	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the renderQuad target view.
	HRESULT hr = poD3DDevice->CreateRenderTargetView(m_cpoTexture.Get(), &renderTargetViewDesc, &m_cpoRenderTargetView);
	
	DirectX::ThrowIfFailed(hr);

	return hr;
}

Rldx::SceneNodeTypeEnum Rldx::DxTexture::GetType() const
{
	return SceneNodeTypeEnum::Texture;
}

void Rldx::DxTexture::Reset(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, unsigned int width, unsigned int height)
{	
	CreateBuffers(
		poDevice, 
		m_textureDesc.Width,
		m_textureDesc.Height, 		
		DXGI_FORMAT_R32_TYPELESS, 
		1, "Texture"
	);
}

UINT Rldx::DxTexture::GetHeight()
{
    return m_textureDesc.Height;
}

UINT Rldx::DxTexture::GetWidth()
{
    return m_textureDesc.Width;
}

UINT Rldx::DxTexture::GetSampleCount()
{
	return m_textureDesc.SampleDesc.Count;
}

float Rldx::DxTexture::GetAspectRatio()
{
	return static_cast<float>(GetWidth()) / static_cast<float>(GetHeight());
}

