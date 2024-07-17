#pragma once

#include <exception>
#include <string>
#include "..\Logger\Logger.h"
#include "COMerrors.h"

// TODO: This needs a revision to ensure it works as expected.
enum class COMExceptionFormatMode {
	StandardNoLog,
	StandardLog,
	StandardLogVerbose,
	COM
};

// TODO: and change name to "LogException", keep rest. 
// TODO: Edit this to fit C++ "convention", 
class COMException : public std::exception
{
	HRESULT m_hrResult;
	std::string m_message;
	COMExceptionFormatMode m_formatMode;

public:
	COMException(const std::wstring& message, const COMExceptionFormatMode formatMode = COMExceptionFormatMode::StandardLog, const HRESULT hrResult = S_OK);
	virtual const char* what() const throw();
	HRESULT GetResult() const;
	operator HRESULT() { return m_hrResult; };
};

#define ThrowAndLogIfAiled(message, formatMode, hrResult) \
	if (!SUCCEEDED(hr)) throw COMException(message, formatMode, hrResult); \
	else LogSucess(message) \

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

