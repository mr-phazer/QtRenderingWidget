


#include <CustomExceptions\CustomExceptions.h>
#include "..\..\DirectXTK\Inc\DirectXHelpers.h"
#include "..\..\DXUT\Core\DXUT.h"
#include "Logger\Logger.h"
#include "d3d11.h"

#include "..\..\..\DirectXTK\Src\PlatformHelpers.h"



// Author
#include "DxTexture.h"

#include <DirectXHelpers.h>

//#include "..\Managers\ResourceManager\DxResourceManager.h"

/*void rldx::DxTexture::SetToActiveTargetView(ID3D11Deviceentext* deviceContext)
{
	auto pRTV = GetRenderTargetView();
	ID3D11RenderTargetView* const ppRTV[] = { pRTV };
	deviceContext->OMSetRenderTargets(1, ppRTV, NULL);
}

void rldx::DxTexture::clearPixels(ID3D11DeviceContext* pDeviceContext, sm::Color vColor)
{
	pDeviceContext->ClearRenderTargetView(m_cpoRenderTargetView.Get(), vColor);
}*/

bool rldx::DxTexture::LoadFileFromDisk(ID3D11Device* poD3DDevice, const std::wstring& fileName, const std::string& objectName)
{
	ID3D11Resource* poTextureResource = nullptr;
	HRESULT hrTextureCreateResult = //DirectX::CreateDDSTextureFromFileEx(


		DirectX::CreateDDSTextureFromFileEx(
			poD3DDevice,
			fileName.c_str(),
			0,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE,
			0,
			D3D11_RESOURCE_MISC_TEXTURECUBE,
			DirectX::DX11::DDS_LOADER_FORCE_SRGB,
			&poTextureResource,
			&m_cpoShaderResourceView);

	if (FAILED(hrTextureCreateResult))
	{
		using namespace logger;
		Logger::LogActionWarning(L"Failed to load texture file: " + fileName);
		return false;
	}

	DXUT_SetDebugName(poTextureResource, std::string("DDS:TEX " + objectName).c_str());
	DXUT_SetDebugName(m_cpoShaderResourceView.Get(), std::string("DDS:SRV " + objectName).c_str());

	return PlaceResourceBuffer(poTextureResource);
}

bool rldx::DxTexture::LoadCubeMap(ID3D11Device* poD3DDevice, const std::wstring& fileName, const std::string& objectName)
{
	// TODO: get to work, get texture data into "m_cpoTexture"
	ID3D11Resource* poTextureResource = nullptr;
	DirectX::CreateDDSTextureFromFileEx(
		poD3DDevice,
		fileName.c_str(),
		0,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0,
		D3D11_RESOURCE_MISC_TEXTURECUBE,
		DirectX::DX11::DDS_LOADER_DEFAULT,
		&poTextureResource,
		&m_cpoShaderResourceView);

	DXUT_SetDebugName(poTextureResource, std::string("DDS:TEX " + objectName).c_str());
	DXUT_SetDebugName(m_cpoShaderResourceView.Get(), std::string("DDS:SRV " + objectName).c_str());

	// Assume that pResource is a valid pointer to an ID3D11Resource object.		
	return PlaceResourceBuffer(poTextureResource);
}

bool rldx::DxTexture::LoadCubeMap(ID3D11Device* poD3DDevice, const uint8_t* pbinarFileData, size_t dataSize, const std::string& objectName)
{

	// TODO: get to work, get texture data into "m_cpoTexture"
	ID3D11Resource* poTextureResource = nullptr;
	DirectX::CreateDDSTextureFromMemoryEx(
		poD3DDevice,
		pbinarFileData,
		dataSize,
		0,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0,
		D3D11_RESOURCE_MISC_TEXTURECUBE,
		DirectX::DX11::DDS_LOADER_DEFAULT,
		&poTextureResource,
		&m_cpoShaderResourceView);


	DXUT_SetDebugName(poTextureResource, std::string("DDS:TEX " + objectName).c_str());
	DXUT_SetDebugName(m_cpoShaderResourceView.Get(), std::string("DDS:SRV " + objectName).c_str());

	return PlaceResourceBuffer(poTextureResource);
}

bool rldx::DxTexture::LoadFileFromMemory(ID3D11Device* poD3DDevice, const uint8_t* pbinarFileData, size_t dataSize, const std::string& objectName)
{
	// TODO: get to work, get texture data into "m_cpoTexture"
	ID3D11Resource* poTextureResource = nullptr;
	DirectX::CreateDDSTextureFromMemory(poD3DDevice, pbinarFileData, dataSize, &poTextureResource, &m_cpoShaderResourceView);


	DXUT_SetDebugName(poTextureResource, std::string("DDS:TEX " + objectName).c_str());
	DXUT_SetDebugName(m_cpoShaderResourceView.Get(), std::string("DDS:SRV " + objectName).c_str());

	// Assume that pResource is a valid pointer to an ID3D11Resource object.		
	return PlaceResourceBuffer(poTextureResource);
}

bool rldx::DxTexture::PlaceResourceBuffer(ID3D11Resource* poTextureResource)
{
	if (!poTextureResource)
		return false;

	// Check dimension of texture resource
	D3D11_RESOURCE_DIMENSION resourceType;
	poTextureResource->GetType(&resourceType);

	if (resourceType == D3D11_RESOURCE_DIMENSION_TEXTURE2D)
	{
		// WHY do you need to store a 2d texture??
		HRESULT hrResult = poTextureResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&m_cpoTexture);
		DirectX::ThrowIfFailed(hrResult);

		m_cpoTexture->GetDesc(&m_textureDesc);

		return true;
	}
	else if (resourceType == D3D11_RESOURCE_DIMENSION_TEXTURE3D)
	{
		HRESULT hrResult = poTextureResource->QueryInterface(__uuidof(ID3D11Texture3D), (void**)&m_cpoCubeMapTexture);
		DirectX::ThrowIfFailed(hrResult);

		// TODO: is this needed?

		return true;
	}

	throw std::exception("Texture resource is not a 2D or 3D texture.");

	return false;
}



bool rldx::DxTexture::CreateBuffers(ID3D11Device* poD3DDevice, UINT width, UINT height, DXGI_FORMAT format, UINT sampleCount, const std::string& objectName)
{
	HRESULT hr = S_OK;

	Create2dTextureBuffer(poD3DDevice, width, height, format, sampleCount);
	CreateRenderTargetViewBuffer(poD3DDevice);
	CreateShaderResourceViewBuffer(poD3DDevice);


	return true;
}

HRESULT rldx::DxTexture::Create2dTextureBuffer(ID3D11Device* poD3DDevice, UINT width, UINT height, DXGI_FORMAT format, UINT sampleCount)
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

HRESULT rldx::DxTexture::CreateShaderResourceViewBuffer(ID3D11Device* poD3DDevice)
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

HRESULT rldx::DxTexture::CreateRenderTargetViewBuffer(ID3D11Device* poD3DDevice)
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

rldx::ResourceTypeEnum rldx::DxTexture::GetType() const
{
	return ResourceTypeEnum::Texture;
}

std::wstring rldx::DxTexture::GetTypeString() const
{
	return L"DxTexture";
}

void rldx::DxTexture::Resize(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, unsigned int width, unsigned int height)
{
	CreateBuffers(
		poDevice,
		m_textureDesc.Width,
		m_textureDesc.Height,
		DXGI_FORMAT_R32_TYPELESS,
		1, "Texture"
	);
}

UINT rldx::DxTexture::GetHeight()
{
	return m_textureDesc.Height;
}

UINT rldx::DxTexture::GetWidth()
{
	return m_textureDesc.Width;
}

UINT rldx::DxTexture::GetSampleCount()
{
	return m_textureDesc.SampleDesc.Count;
}

float rldx::DxTexture::GetAspectRatio()
{
	return static_cast<float>(GetWidth()) / static_cast<float>(GetHeight());
}



