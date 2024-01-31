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
		std::string m_string = "";
		sm::Color m_color = { 1,1,1,1 };
		tools::SystemClock m_clock;
		float m_clock_timeOut = 0;
		float m_clock_ratio = 0;

	public:
		RenderTextString(const std::string& string, sm::Color color = { 1,1,1,1 }, float fadeOutInSeconds = 5.0f)
			: m_string(string), m_color(color), m_clock_timeOut(fadeOutInSeconds)
		{
			Start();
		}

		void Start(float fadeOutInSeconds = 15.0f)
		{
			m_clock_timeOut = fadeOutInSeconds;
			m_clock.ResetLocalTime();
		}

		void Update()
		{	
			auto localTime = (float)m_clock.GetLocalTime();
			m_clock_ratio = 1.0f - std::min<float>(1.0f, localTime / m_clock_timeOut);
		}

		DirectX::FXMVECTOR GetColor()
		{
			auto tranformedColor = m_color * 1.0;// m_clock_ratio;
			return DirectX::XMLoadFloat4(&tranformedColor);
		}

		std::string GetString() const { return m_string; }

	}; 


	/// <summary>
	/// Very Simple Text Renderer for Debugging Purposes
	/// </summary>
	struct DxDebugTextWriter
	{
	public:
		static std::unique_ptr<DxDebugTextWriter> Create(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext);

		void AddString(const std::wstring& _string);
		void RenderText();

		std::unique_ptr<DirectX::SpriteBatch> m_upoSpriteBatch;
		std::unique_ptr<DirectX::SpriteFont> m_upoFont;
		std::vector<std::wstring> m_stringRenderQueue = { L"D3D\nIt Works\nNOW: Scenegrapdh" };

		std::vector<RenderTextString> m_stringExtQueue = { {"Test String", {1,0,0,1}} };

		size_t m_maxStringQueueSize = 1;
	};
};