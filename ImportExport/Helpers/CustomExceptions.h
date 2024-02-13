#pragma once

#include <exception>
#include <string>
#include "..\Libs\PugiXML\pugixml.hpp"

class XMLException : public std::exception
{
public:
 
    explicit XMLException(pugi::xml_parse_result xmlParseResult)
        : XMLException((std::string("XML Parsing Error: ") + xmlParseResult.description()).c_str()) {}      

    explicit XMLException(const char* message)
        : m_messageString(message) {}

    
    explicit XMLException(const std::string& message)
        : m_messageString(message) {}
    
    virtual ~XMLException() noexcept {}
        
    virtual const char* what() const noexcept 
    {
        return m_messageString.c_str();
    }

protected:    
    std::string m_messageString;
};