#pragma once

#include <exception>
#include <string>
#include "..\Logger\Logger.h"
#include "COMerrors.h"

class ConLogException : public std::exception
{
	std::string m_message;

public:
	ConLogException(const std::wstring& message);
	virtual const char* what() const throw();
};

class ConLogExceptionVerbose : public std::exception
{
	std::string m_message;

public:
	ConLogExceptionVerbose(const std::wstring& message);
	virtual const char* what() const throw();
};

class COMException : public std::exception
{
	HRESULT m_hrResult;
	std::string m_message;

public:
	COMException(HRESULT hr) noexcept : m_hrResult(hr)
	{
		m_message = GetComErrorFormated(m_hrResult, "");
	}

	const char* what() const override {
		return m_message.c_str();
	}

	HRESULT GetResult() const noexcept { return m_hrResult; }
};

class COMExceptionLog : public std::exception
{
	HRESULT m_hrResult = S_OK;
	std::string m_message = "";

public:
	COMExceptionLog(HRESULT hr, const std::string& message) noexcept
		:
		m_hrResult(hr),
		m_message(GetComErrorFormated(hr, message))
	{
		using namespace logging;
		Logger::LogActionError(ToWString(what()));
	}

	const char* what() const override
	{
		return m_message.c_str();
	}

	HRESULT GetResult() const noexcept { return m_hrResult; }
};

#define ThrowAndLogIfAiled(hr, message) \
	if (!SUCCEEDED(hr)) throw COMExceptionLog(hr, message); \
	else \
	using namespace logging; \
	Logger::LogActionSuccess("CreateRenderTargetView().")



