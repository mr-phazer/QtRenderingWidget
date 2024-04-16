#pragma once

#include <exception>
#include <string>

class ConLogException : public std::exception
{
	std::string m_message;

public:
	ConLogException(const std::string& message);
	virtual const char* what() const throw();
};