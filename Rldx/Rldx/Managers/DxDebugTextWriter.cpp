#include <string>

#include <Timer\SystemClockChecker.h>
#include "..\..\ImportExport\Helpers\ByteStream.h"
#include "..\Logging\Logging.h"
#include "..\Tools\tools.h"
#include "DxDebugTextWriter.h"

using namespace rldx;

// TODO: keep or use create()?
//rldx::DxDebugTextWriter::DxDebugTextWriter(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext)
//{
//	logging::LogAction("DxDebugTextWriter::DxDebugTextWriter");
//
//	auto newInstance = std::make_unique<DxDebugTextWriter>();
//	ByteStream fontData(SPRITEFONT_PATH);
//
//	// TODO: handle exception where?		
//	if (!(newInstance->m_upoFont = std::make_unique<DirectX::SpriteFont>(poDevice, fontData.GetBufferPtr(), fontData.GetBufferSize())))
//	{
//		logging::LogActionError("Error Loading Font.");
//		throw std::exception("Error Loading Font.");
//	}
//
//	if (!(newInstance->m_upoSpriteBatch = std::make_unique<DirectX::SpriteBatch>(poDeviceContext))) {
//		logging::LogActionError("Error Loading Font.");
//		throw std::exception("Error Creating SpriteBatch.");
//	}
//
//	logging::LogActionSuccess("Sucess");
//}

std::unique_ptr<DxDebugTextWriter> DxDebugTextWriter::Create(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext)
{
	logging::LogAction("DxDebugTextWriter::Create");

	auto newInstance = std::make_unique<DxDebugTextWriter>();
	ByteStream fontData(SPRITEFONT_PATH);

	// TODO: handle exception where?		
	if (!(newInstance->m_upoFont = std::make_unique<DirectX::SpriteFont>(poDevice, fontData.GetBufferPtr(), fontData.GetBufferSize())))
	{
		logging::LogActionError("Error Loading Font.");
		throw std::exception("Error Loading Font.");
	}

	if (!(newInstance->m_upoSpriteBatch = std::make_unique<DirectX::SpriteBatch>(poDeviceContext))) {
		logging::LogActionError("Error Loading Font.");
		throw std::exception("Error Creating SpriteBatch.");
	}

	logging::LogActionSuccess("");
	return newInstance;
}

// TODO: don't use this or?
//void DxDebugTextWriter::AddString(const std::wstring& stringToDisplay)
//{
//	if (m_stringRenderQueue.size() >= m_maxStringQueueSize)
//	{
//		m_stringRenderQueue.clear();
//	}
//
//	m_stringRenderQueue.push_back(stringToDisplay);
//}

void DxDebugTextWriter::AddString(const std::wstring& _string, DirectX::XMFLOAT4 color, float timeOut)
{
	if (m_stringExtQueue.size() > 0) {
		m_stringExtQueue.clear();
	}

	//	m_stringExtQueue.push_back({ _string, color, timeOut });
}

void rldx::DxDebugTextWriter::SetStringRow(size_t row, const std::wstring& stringToDisplay, DirectX::XMFLOAT4 color, float timeOut)
{
	m_stringRows.resize(row < m_stringRows.size() ? m_stringRows.size() : row + 1);
	m_stringRows[row] = { stringToDisplay, color, timeOut };
}

void DxDebugTextWriter::RenderText()
{
	m_upoSpriteBatch->Begin();

	//RenderStrings(m_stringExtQueue);
	RenderStrings(m_stringRows);

	m_upoSpriteBatch->End();

	RemoveStrings();
}

void rldx::DxDebugTextWriter::RenderStrings(std::vector<rldx::RenderTextString>& renderStrings) const
{
	DirectX::XMFLOAT2 v2TextPosition(0.f, 0.f);
	auto lineSpacing = m_upoFont->GetLineSpacing();

	for (auto& itString : renderStrings)
	{
		m_upoFont->DrawString(
			m_upoSpriteBatch.get(),
			itString.GetString().c_str(),
			v2TextPosition,
			itString.GetColor());

		v2TextPosition.y += lineSpacing;

		itString.Update();
	}
}

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

