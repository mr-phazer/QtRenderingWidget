#include "CustomExceptions.h"

#include "..\Logger\Logger.h"

using namespace logger;
ConLogException::ConLogException(const std::wstring& message)
{
	Logger::LogActionError(L"Exception: " + message);
}

const char* ConLogException::what() const throw()
{
	return m_message.c_str();
}
;