#include "..\..\Rldx\Rldx\Logging\Logging.h"
#include "CustomExceptions.h"

ConLogException::ConLogException(const std::string& message) : m_message(message)
{
	logging::LogActionError("Exception: " + message);
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
