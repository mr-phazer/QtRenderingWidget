#include <string>

#include "DxDebugTextWriter.h"
#include "..\Tools\tools.h"
#include "..\..\ImportExport\Helpers\ByteStream.h"

using namespace rldx;

std::unique_ptr<DxDebugTextWriter> DxDebugTextWriter::Create(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext)
{
	auto newInstance = std::make_unique<DxDebugTextWriter>();
	const auto fontPath = /*libtools::GetExePath() + */std::wstring(L"myfile.spritefont");

	ByteStream fontData(fontPath);

	// TODO: handle exception where?		
	if (!(newInstance->m_upoFont = std::make_unique<DirectX::SpriteFont>(poDevice, fontData.GetBufferPtr(), fontData.GetBufferSize())))
	{
		throw std::exception("Error Loading Font.");
	}

	if (!(newInstance->m_upoSpriteBatch = std::make_unique<DirectX::SpriteBatch>(poDeviceContext))) {
		throw std::exception("Error Creating SpriteBatch.");
	}

	return newInstance;
}

// TODO: don't use this or?
//void DxDebugTextWriter::AddString(const std::wstring& _string)
//{
//	if (m_stringRenderQueue.size() >= m_maxStringQueueSize)
//	{
//		m_stringRenderQueue.clear();
//	}
//
//	m_stringRenderQueue.push_back(_string);
//}

void DxDebugTextWriter::AddString(const std::wstring& _string, DirectX::XMFLOAT4 color, float timeOut)
{
	m_stringExtQueue.push_back({ _string, color, timeOut });
}

void DxDebugTextWriter::RenderText()
{
	DirectX::XMFLOAT2 v2TextPosition(0.f, 0.f);
	auto lineSpacing = m_upoFont->GetLineSpacing();

	m_upoSpriteBatch->Begin();

	for (auto& itString : m_stringExtQueue)
	{
		m_upoFont->DrawString(
			m_upoSpriteBatch.get(),
			itString.GetString().c_str(),
			v2TextPosition,
			itString.GetColor());

		v2TextPosition.y += lineSpacing;

		itString.Update();
	}

	m_upoSpriteBatch->End();

	RemoveStrings();
};

void DxDebugTextWriter::RemoveStrings()
{
	for (int iString = (int)m_stringExtQueue.size() - 1; iString >= 0; --iString) 
	{
		if (m_stringExtQueue[iString].IsDone())
		{
			m_stringExtQueue.erase(m_stringExtQueue.begin() + iString);
		}
	}
}

