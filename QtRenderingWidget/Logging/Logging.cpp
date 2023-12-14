
#include "Logging.h"

using namespace logging;

std::wstring ImplLog::prefix = L"[QRenderingView Debug:] ";

void ImplLog::LogActionInfo(const std::string& _strMsg)
{
	WinConsole::Print(prefix);
	WinConsole::Print(L" ");
	WinConsole::Print(WidenStr(_strMsg));
	WinConsole::Print(L"\r\n");

	std::stringstream logString;
	logString << std::endl << "ACTION: " << (_strMsg).c_str();

	WriteToLogFile(logString.str());    
}

void ImplLog::LogSimpleWithColor(const std::string& _strMsg, WORD wColorFlags)
{
	WinConsole::Print(WidenStr(_strMsg), wColorFlags);	
    WinConsole::Print(L"\r\n");

	std::stringstream logString;
	logString << std::endl << (_strMsg).c_str();

	WriteToLogFile(logString.str());
}


void ImplLog::LogAction_success(const std::string& _strMsg)
{	
	WinConsole::Print(prefix + L"SUCCESS:", BG_BLACK | FG_GREEN);
	WinConsole::Print(L" ");
	WinConsole::Print(WidenStr(_strMsg));
	//WinConcole::Print(L"Success.", BG_BLUE | FG_WHITE);
	WinConsole::Print(L"\r\n");

	std::stringstream logString;
	logString << std::endl << tools::wstring_to_string(prefix) << _strMsg << ". Success.";
	
	WriteToLogFile(logString.str());	
}

bool ImplLog::LogActionErrorFalse(const std::string& _strMsg)
{	
	WinConsole::Print(prefix + L"ERROR:", BG_BLACK | FG_RED);
	WinConsole::Print(L" ");
	WinConsole::Print(WidenStr(_strMsg));
	WinConsole::Print(L"\r\n");

	std::stringstream logString;
	logString << std::endl << tools::wstring_to_string(prefix) +  "ERROR: " << _strMsg;

	WriteToLogFile(logString.str());

	return false;
}

bool ImplLog::LogAction_warning(const std::string& _strMsg)
{
	WinConsole::Print(prefix + L"WARNING:", BG_BLACK | FG_YELLOW);
	WinConsole::Print(L" ");
	WinConsole::Print(WidenStr(_strMsg));
	WinConsole::Print(L"\r\n");

	std::stringstream logString;
	logString << std::endl << tools::wstring_to_string(prefix) + "WARNING:: " << _strMsg;

	 WriteToLogFile(logString.str());

	return false;
}

void ImplLog::LogWrite(const std::string& _strMsg)
{
	WriteToLogFile(_strMsg);
}

void ImplLog::WriteToLogFile(const std::string& logString)
{
    std::ofstream oOutFile(L"log.txt", std::ios::app);
    oOutFile << logString;
    oOutFile.close();
}

void ImplLog::LogActionTimedBegin(const std::string& _strMsg)
{
    m_globalClock.ResetLocalTime();
    LogActionInfo(_strMsg);
}

void ImplLog::LogActionTimedEnd(const std::string& _strMsg)
{
    auto timeElapsed = m_globalClock.GetLocalTime();
    LogActionInfo(_strMsg);
    
    auto timeMessage = "Time Elapsed: " + std::to_string(timeElapsed) + " seconds.\n";

    WinConsole::Print(WidenStr(timeMessage), BG_BLACK | FG_GREEN);
}

void WinConsole::Print(const std::wstring& str, WORD wColorFlags)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    /*
     * Set the new color information
     */
    SetConsoleTextAttribute(h, wColorFlags);
    DWORD dwChars = 0;
    WriteConsole(h, str.data(), (DWORD)str.size(), &dwChars, NULL);

    /*
    * Reset to default color
    */
    SetConsoleTextAttribute(h, BG_BLACK | FG_WHITE);
}

void WinConsole::PrintLn(const std::wstring& str, WORD color)
{
    Print(str + L"\n", color);
}

tools::SystemClock ImplLog::m_globalClock;