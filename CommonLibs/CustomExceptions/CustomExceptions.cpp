#include "CustomExceptions.h"

using namespace logging;
using namespace utils;

COMException::COMException(const std::wstring& message, const COMExceptionFormatMode formatMode, const HRESULT hrResult)
{
	m_hrResult = hrResult;
	m_message = ToString(message);
	m_formatMode = formatMode;

	if (m_formatMode == COMExceptionFormatMode::StandardLog) {
		Logger::LogActionError(L"Exception: " + message);
	}
	else if (m_formatMode == COMExceptionFormatMode::StandardLogVerbose) {
		Logger::LogActionError(L"Exception: " + FULL_FUNC_INFO_W(message));
	}
	else if (m_formatMode == COMExceptionFormatMode::COM) {
		Logger::LogActionError(ToWString(GetComErrorFormated(m_hrResult, m_message)));
	}
}

const char* COMException::what() const throw()
{
	if (m_formatMode == COMExceptionFormatMode::StandardLogVerbose) {
		return FULL_FUNC_INFO(m_message).c_str();
	}
	else if (m_formatMode == COMExceptionFormatMode::COM) {
		const size_t fixedStringLength = 128;

		std::string tempString;
		tempString.resize(fixedStringLength);

		// -- add the hexadecimal error code, 
		sprintf_s((char*)tempString.data(), fixedStringLength, "Failure with HRESULT of 0x%08X", static_cast<unsigned int>(m_hrResult));

		// -- add error string
		tempString.resize(strlen(tempString.c_str())); // resize string to fit the error string
		auto comErrorMessage = GetComError(m_hrResult);
		tempString += " : " + comErrorMessage + '\0';

		return tempString.c_str();

	}
	else {
		return m_message.c_str();
	}
}

HRESULT COMException::GetResult() const { 
	return m_hrResult; 
}