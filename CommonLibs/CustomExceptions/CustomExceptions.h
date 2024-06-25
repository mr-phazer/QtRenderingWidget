#pragma once

#include <exception>
#include <string>
#include "COMerrors.h"
#include "..\Logger\Logger.h"

// TODO: This needs a revision to ensure it works as expected.
enum class COMExceptionFormatMode {
	StandardNoLog,
	StandardLog,
	StandardLogVerbose,
	COM
};

class COMException : public std::exception
{
	HRESULT m_hrResult;
	std::string m_message;
	COMExceptionFormatMode m_formatMode;

public:
	COMException(const std::wstring& message, const COMExceptionFormatMode formatMode, const HRESULT hrResult);
	virtual const char* what() const throw();
	HRESULT GetResult() const;
	operator HRESULT() { return m_hrResult; };
};

#define ThrowAndLogIfAiled(message, formatMode, hrResult) \
	if (!SUCCEEDED(hr)) throw COMException(message, formatMode, hrResult); \
	else \
	using namespace logging; \
	Logger::LogActionSuccess(L"CreateRenderTargetView().")

#define COM_COND_THROW(HR) \
if (!SUCCEEDED(HR)) throw COMException(HR); \

#define LOADING_FILE(path) ("Loading: " + std::string(path))

#define COM_ASSERT_BOX(HR, op) comAssert_Box(HR, __FUNCTION__, op)

#define COM_ASSERT(HR) comAssert_LogOnly(HR, __FUNCTION__)
#define COM_ASSERT_OP(HR, op) comAssert_LogOnly(HR, __FUNCTION__, op)

#define ComMessageBox(hr, _title, _op) \
		auto message = std::wstring(ToWString(__FUNCTION__) + L"\r\n") \
		+ L"Failed: " + std::wstring(_op) + _wstrPath + L"\r\n") \
		+ L"HRESULT: " + std::to_wstring(hr) + L"\r\n" \
		+ L"String: " + GetComError(hr) + L"\r\n"; \
	MessageBox(NULL, message.c_str(), _title, MB_OK | MB_ICONERROR);\


#define VERBOSE __FUNCTION__ ": " __FILE__ "(" + std::to_string(__LINE__) + ")"

