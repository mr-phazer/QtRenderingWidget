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
	static std::unique_ptr<DxDebugTextWriter> Create(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext)
	{
		auto newInstance = std::make_unique<DxDebugTextWriter>();

		// TODO: handle exception where?		
		if (!(newInstance->m_upoFont = std::make_unique<DirectX::SpriteFont>(poDevice, L"myfile.spritefont"))) {
			throw std::exception("Error Loading Font.");
		}

		if (!(newInstance->m_upoSpriteBatch = std::make_unique<DirectX::SpriteBatch>(poDeviceContext))) {
			throw std::exception("Error Creating SpriteBatch.");
		}

		return newInstance;
	}

	void AddString(const std::wstring& _string) 
	{
		if (m_stringRenderQueue.size() >= m_maxStringQueueSize)
		{
			m_stringRenderQueue.clear();
		}

		m_stringRenderQueue.push_back(_string);
	}

	void RenderText()
	{
		
		m_upoSpriteBatch->Begin();

		DirectX::XMFLOAT2 v2TextPosition(0.f, 0.f);

		auto lineSpacing = m_upoFont->GetLineSpacing();	
		
		for (const auto& itString : m_stringRenderQueue)
		{
			m_upoFont->DrawString(m_upoSpriteBatch.get(), itString.c_str(), v2TextPosition);
			v2TextPosition.y += lineSpacing;
		}

		m_upoSpriteBatch->End();
	}


private:
	std::unique_ptr<DirectX::SpriteBatch> m_upoSpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_upoFont;
	std::vector<std::wstring> m_stringRenderQueue = { L"D3D\nIt Works\nNOW: Scenegrapdh" };

	size_t m_maxStringQueueSize = 1;
};


