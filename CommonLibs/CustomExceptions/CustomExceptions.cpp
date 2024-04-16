#include "CustomExceptions.h"

#include "..\..\Rldx\Rldx\Logging\Logging.h"

ConLogException::ConLogException(const std::string& message) : m_message(message)
{
	logging::LogActionError("Exception: " + message);
}

const char* ConLogException::what() const throw()
{
	return m_message.c_str();
}
;