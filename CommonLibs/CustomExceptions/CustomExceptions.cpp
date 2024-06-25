#include "CustomExceptions.h"

#include "..\Logger\Logger.h"

using namespace logging;
using namespace utils;

ConLogException::ConLogException(const std::wstring& message)
{
	Logger::LogActionError(L"Exception: " + message);
}

const char* ConLogException::what() const throw()
{
	return m_message.c_str();
}

// --------------------------------------------------------------------------------

ConLogExceptionVerbose::ConLogExceptionVerbose(const std::wstring& message) : m_message(FULL_FUNC_INFO(ToString(message)))
{
	Logger::LogActionError(L"Exception: " + message);
}

const char* ConLogExceptionVerbose::what() const throw()
{
	return m_message.c_str();
}
