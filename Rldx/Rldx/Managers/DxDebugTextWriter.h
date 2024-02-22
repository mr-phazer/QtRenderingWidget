#pragma once
#include <memory>
#include <string>
#include <vector>

#include <D3d11.h>

#include <SpriteFont.h>
#include <SpriteBatch.h>
#include <SimpleMath.h>

#include "..\..\Rldx\Rldx\Tools\tools.h"

namespace rldx {

	class RenderTextString
	{
		std::wstring m_string = L"";
		sm::Color m_color = { 1,1,1,1 };
		libtools::SystemClock m_clock;
		float m_clock_timeOut = 0;
		float m_ratioOfTimeOut = 1.0f;

	public:
		RenderTextString(const std::wstring& string, sm::Color color = { 1,1,1,1 }, float fadeOutInSeconds = 5.0f)
			: m_string(string), m_color(color), m_clock_timeOut(fadeOutInSeconds)
		{
			Start(fadeOutInSeconds);
		}

		bool IsDone() const
		{
			auto localTime = (float)m_clock.GetLocalTime();
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
			m_ratioOfTimeOut = 1.0f -std::min<float>(1.0f,  localTime / m_clock_timeOut);
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
	public:
		static std::unique_ptr<DxDebugTextWriter> Create(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext);

		void AddString(const std::wstring& _string, DirectX::XMFLOAT4 color = { 1,1,1,1 }, float timeOut = 5.0f);
				
		void RenderText();

	private:
		
		void RemoveStrings();

		std::unique_ptr<DirectX::SpriteBatch> m_upoSpriteBatch;
		std::unique_ptr<DirectX::SpriteFont> m_upoFont;
		std::vector<std::wstring> m_stringRenderQueue = { L"D3D\nIt Works\nNOW: Scenegrapdh" };

		std::vector<RenderTextString> m_stringExtQueue;/* = {
			{L"Test String1", {1,1,1,1},  9.0f },
			{L"Test String2", {1,1,1,1},  5.0f },
			{L"Test String3", {1,1,1,1},  15.0f },
			{L"Test String4", {1,1,1,1},  12.0f },
			{L"Test String5", {1,1,1,1},  7.0f },
			{L"Test String6", {1,1,1,1},  4.0f }
			,
		};*/

		size_t m_maxStringQueueSize = 20;
	};
};