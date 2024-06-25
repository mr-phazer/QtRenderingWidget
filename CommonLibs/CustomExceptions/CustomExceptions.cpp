#include "CustomExceptions.h"

#include "..\Logger\Logger.h"

using namespace logging;
ConLogException::ConLogException(const std::wstring& message)
{
	Logger::LogActionError(L"Exception: " + message);
}

const char* ConLogException::what() const throw()
{
	return m_message.c_str();
}

// --------------------------------------------------------------------------------

ConLogExceptionVerbose::ConLogExceptionVerbose(const std::string& message) : m_message(FULL_FUNC_INFO(message))
{
	logging::LogActionError("Exception: " + message);
}

const char* ConLogExceptionVerbose::what() const throw()
{
	return m_message.c_str();
}
