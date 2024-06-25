#pragma once
#include <memory>
#include <string>
#include <vector>

#include <D3d11.h>

#include <SimpleMath.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "Timer\SystemClockChecker.h"

namespace rldx {

	class RenderTextString
	{
		std::wstring m_string = L"";
		sm::Color m_color = { 1,1,1,1 };
		timer::SystemClockChecker m_clock;
		float m_clock_timeOut = 0;
		float m_ratioOfTimeOut = 1.0f;

	public:
		RenderTextString() = default;
		RenderTextString(const std::wstring& string, sm::Color color = { 1,1,1,1 }, float fadeOutInSeconds = 5.0f)
			: m_string(string), m_color(color), m_clock_timeOut(fadeOutInSeconds)
		{
			Start(fadeOutInSeconds);
		}

		bool IsDone() const
		{
			auto localTime = m_clock.GetLocalTime();
			return localTime > m_clock_timeOut;
		}

		void Start(float fadeOutInSeconds)
		{
			m_clock_timeOut = fadeOutInSeconds;
			m_clock.ResetLocalTime();
		}

		void Update()
		{
			auto localTime = (float)m_clock.GetLocalTime();
			m_ratioOfTimeOut = 1.0f - std::min<float>(1.0f, localTime / m_clock_timeOut);
		}

		DirectX::FXMVECTOR GetColor()
		{
			auto tranformedColor = m_color * m_ratioOfTimeOut;
			return DirectX::XMLoadFloat4(&tranformedColor);
		}

		std::wstring GetString() const { return m_string; }
	};


	/// <summary>
	/// Very Simple Text Renderer for Debugging Purposes
	/// </summary>
	struct DxDebugTextWriter
	{
		static constexpr auto SPRITEFONT_PATH = LR"(Font/Consolas18.spritefont)";

		std::unique_ptr<DirectX::SpriteBatch> m_upoSpriteBatch;
		std::unique_ptr<DirectX::SpriteFont> m_upoFont;

		std::vector<RenderTextString> m_stringRows;
		std::vector<RenderTextString> m_stringExtQueue;

		size_t m_maxStringQueueSize = 20;

	public:
		DxDebugTextWriter() = default;

		// TODO: keep this, or use create?
		//DxDebugTextWriter(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext);
		static std::unique_ptr<DxDebugTextWriter> Create(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext);

		void AddString(const std::wstring& _string, DirectX::XMFLOAT4 color = { 1,1,1,1 }, float timeOut = 2.0f);
		void SetStringRow(size_t row, const std::wstring& _string, DirectX::XMFLOAT4 color = { 1,1,1,1 }, float timeOut = 2.0f);
		void RenderText();

		void RenderStrings(std::vector<rldx::RenderTextString>& renderStrings) const;

	private:
		void RemoveStrings();



	};
};