#include "pch.h"
#include "Logger.h"

//---------------------------------------------------------------------//
// Logger implementation
//---------------------------------------------------------------------//

using namespace Utils;

Logger& Logger::GetInstance() {
	if (!m_poInstance)
	{
		m_poInstance = std::make_unique<Logger>();
	}
	else
	{
		return *m_poInstance;
	}
}

void Logger::SetLogFolder(const std::wstring& path)
{
    sm_loggingFolder = path;
    if (sm_loggingFolder.back() != '\\' && sm_loggingFolder.back() != '/')
    {
        sm_loggingFolder += '\\';
    }
}

std::wstring Logger::GetOutLogFilePath() {
	return sm_loggingFolder + sm_logFileName;
}

void Logger::DoLog(const std::wstring& strMsg, const std::wstring& strTag, WORD colorFlags, WORD tagColorFlags)
{
	WinConsole::Print(sm_prefix + strTag, tagColorFlags);
	WinConsole::Print(L" ");
	WinConsole::Print(strMsg, colorFlags);
	WinConsole::Print(L"\r\n");

	std::wstringstream logString;
	logString << std::endl << sm_prefix + L"ERROR: " << strMsg;
	WriteToLogFile(logString.str());
}

void Logger::LogSimpleWithColor(const std::wstring& strMsg, WORD wColorFlags)
{
	WinConsole::Print(strMsg, wColorFlags);
	WinConsole::Print(L"\r\n");

	std::wstringstream logString;
	logString << std::endl << strMsg;

	WriteToLogFile(logString.str());
}

void Logger::LogActionSuccess(const std::wstring& strMsg)
{
	WinConsole::Print(sm_prefix + L"SUCCESS:", BG_BLACK | FG_GREEN);
	WinConsole::Print(L" ");
	WinConsole::Print(strMsg);
	//WinConcole::Print(L"Success.", BG_BLUE | FG_WHITE);
	WinConsole::Print(L"\r\n");

	std::wstringstream logString;
	logString << std::endl << sm_prefix << strMsg << L". Success.";

	WriteToLogFile(logString.str());
}

void Logger::LogActionInfo(const std::wstring& strMsg)
{
	DoLog(strMsg, L"ACTION");
}

bool Logger::LogActionWarning(const std::wstring& strMsg)
{
	WinConsole::Print(sm_prefix + L"WARNING:", BG_BLACK | FG_YELLOW);
	WinConsole::Print(L" ");
	WinConsole::Print(strMsg);
	WinConsole::Print(L"\r\n");

	std::wstringstream logString;
	logString << std::endl << sm_prefix + L"WARNING:: " << strMsg;

	WriteToLogFile(logString.str());

	return false;
}

bool Logger::LogActionErrorFalse(const std::wstring& strMsg)
{
	WinConsole::Print(sm_prefix + L"ERROR:", BG_BLACK | FG_RED);
	WinConsole::Print(L" ");
	WinConsole::Print(strMsg);
	WinConsole::Print(L"\r\n");

	std::wstringstream logString;
	logString << std::endl << sm_prefix + L"ERROR: " << strMsg;

	WriteToLogFile(logString.str());

	return false;
}

void Logger::LogException(const std::wstring& strMsg)
{
	DoLog(strMsg, L"EXCEPTION Caught: " + strMsg, BG_BLACK | FG_WHITE, BG_BLACK | FG_RED);
}

void Logger::WriteToLogFile(const std::wstring& logString)
{
	std::ofstream oOutFile(GetOutLogFilePath(), std::ios::app);
	oOutFile << ToString(logString);
	oOutFile.close();
}

void Logger::LogActionTimedBegin(const std::wstring& strMsg)
{
	m_globalClock.ResetLocalTime();
	LogActionInfo(strMsg);
}

void Logger::LogActionTimedEnd(const std::wstring& strMsg)
{
	auto timeElapsed = m_globalClock.GetLocalTime();
	LogActionInfo(strMsg);

	auto timeMessage = "Time Elapsed: " + std::to_string(timeElapsed) + " seconds.\n";

	WinConsole::Print(ToWString(timeMessage), BG_BLACK | FG_GREEN);
}

//---------------------------------------------------------------------//
// TimeLogAction implementation
//---------------------------------------------------------------------//

TimeLogAction MakeObject() { 
	return TimeLogAction(); 
};

TimeLogAction::TimeLogAction() {
	m_clock.ResetLocalTime(); 
};

TimeLogAction TimeLogAction::PrintStart(const std::wstring& messageToShow)
{
	TimeLogAction newInstance;
	newInstance.m_message = messageToShow;
	Logger::LogSimpleWithColor(Logger::sm_prefix + newInstance.m_message + L".", BG_BLACK | FG_WHITE);

	return newInstance;
};

void TimeLogAction::PrintDone(const std::wstring& messageToShow)
{
	Logger::LogSimpleWithColor(Logger::sm_prefix + m_message + L". Done.", BG_BLACK | FG_WHITE);

	auto timeElapsedMessageString = L"Time Elapsed: " + std::to_wstring(m_clock.GetLocalTime()) + L" seconds";
	Logger::LogSimpleWithColor(timeElapsedMessageString, BG_BLACK | FG_GREEN);
};

void TimeLogAction::PrintDoneSuccess(const std::wstring& messageToShow)
{
	Logger::LogSimpleWithColor(Logger::sm_prefix + m_message + L". Completed Succesfully.", BG_BLACK | FG_WHITE);

	auto timeElapsedMessageString = L"Time Elapsed: " + std::to_wstring(m_clock.GetLocalTime()) + L" seconds";
	Logger::LogSimpleWithColor(timeElapsedMessageString, BG_BLACK | FG_GREEN);
};

SystemClock& TimeLogAction::GetClock() {
	return m_clock; 
};
double TimeLogAction::GetLocalTime() {
	return m_clock.GetLocalTime(); 
};

//---------------------------------------------------------------------//
// WinConsole implementation
//---------------------------------------------------------------------//

void WinConsole::Print(const std::wstring& str, WORD wColorFlags)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	/*
	 * Set the new colorFlags information
	 */
	SetConsoleTextAttribute(h, wColorFlags);
	DWORD dwChars = 0;
	WriteConsole(h, str.data(), (DWORD)str.size(), &dwChars, NULL);

	/*
	* Reset to default colorFlags
	*/
	SetConsoleTextAttribute(h, BG_BLACK | FG_WHITE);
}

void WinConsole::PrintLn(const std::wstring& str, WORD color)
{
	Print(str + L"\n", color);
}
