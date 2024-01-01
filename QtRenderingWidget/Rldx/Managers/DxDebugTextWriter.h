#pragma once
#include <memory>
#include <string>
#include <vector>
#include <exception>

#include <D3d11.h>

#include <SpriteFont.h>
#include <SpriteBatch.h>

/// <summary>
/// Very Simple Text Renderer for Debugging Purposes
/// </summary>
struct DxDebugTextWriter
{
public:
	static std::unique_ptr<DxDebugTextWriter> Create(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext);

	void AddString(const std::wstring& _string);
	void RenderText();

private:
	std::unique_ptr<DirectX::SpriteBatch> m_upoSpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_upoFont;
	std::vector<std::wstring> m_stringRenderQueue = { L"D3D\nIt Works\nNOW: Scenegrapdh" };

	size_t m_maxStringQueueSize = 1;
};


