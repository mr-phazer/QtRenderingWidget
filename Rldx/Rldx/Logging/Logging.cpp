
#include "Logging.h"

#include <Timer\SystemClockChecker.h>

using namespace logging;



std::wstring ImplLog::sm_loggingFolder = L"";
std::wstring ImplLog::sm_logFileName = L"RenderViewLog.txt";

std::wstring ImplLog::sm_prefix = L"[QRenderingView Debug:] ";

void logging::ImplLog::SetLogFolder(const std::wstring& path)
{
	sm_loggingFolder = path;
	if (sm_loggingFolder.back() != '\\' && sm_loggingFolder.back() != '/')
	{
		sm_loggingFolder += '\\';
	}
}

void logging::ImplLog::Log_Exception(const std::string& strMsg)
{
	DoLog(strMsg, "EXCEPTION Caught: " + strMsg, BG_BLACK | FG_WHITE, BG_BLACK | FG_RED);
}

void ImplLog::LogActionInfo(const std::string& strMsg)
{
	DoLog(strMsg, "ACTION");
}

void logging::ImplLog::DoLog(
	const std::string& strMsg,
	const std::string& strTag,
	WORD colorFlags,
	WORD tagColorFlags)
{
	WinConsole::Print(sm_prefix + stow(strTag), tagColorFlags);
	WinConsole::Print(L" ");
	WinConsole::Print(WidenStr(strMsg), colorFlags);
	WinConsole::Print(L"\r\n");

	std::stringstream logString;
	logString << std::endl << libtools::wstring_to_string(sm_prefix) + "ERROR: " << strMsg;
	WriteToLogFile(logString.str());

}

void ImplLog::LogSimpleWithColor(const std::string& strMsg, WORD wColorFlags)
{
	WinConsole::Print(WidenStr(strMsg), wColorFlags);
	WinConsole::Print(L"\r\n");

	std::stringstream logString;
	logString << std::endl << (strMsg).c_str();

	WriteToLogFile(logString.str());
}

// TODO: Change to wstring, change whole impl, to the neater thing Frogo suggested
void ImplLog::LogAction_success(const std::string& strMsg)
{
	WinConsole::Print(sm_prefix + L"SUCCESS:", BG_BLACK | FG_GREEN);
	WinConsole::Print(L" ");
	WinConsole::Print(WidenStr(strMsg));
	//WinConcole::Print(L"Success.", BG_BLUE | FG_WHITE);
	WinConsole::Print(L"\r\n");

	std::stringstream logString;
	logString << std::endl << libtools::wstring_to_string(sm_prefix) << strMsg << ". Success.";

	WriteToLogFile(logString.str());
}

bool ImplLog::LogActionErrorFalse(const std::string& strMsg)
{
	WinConsole::Print(sm_prefix + L"ERROR:", BG_BLACK | FG_RED);
	WinConsole::Print(L" ");
	WinConsole::Print(WidenStr(strMsg));
	WinConsole::Print(L"\r\n");

	std::stringstream logString;
	logString << std::endl << wtos(sm_prefix) + "ERROR: " << strMsg;

	WriteToLogFile(logString.str());

	return false;
}

bool ImplLog::LogAction_warning(const std::string& strMsg)
{
	WinConsole::Print(sm_prefix + L"WARNING:", BG_BLACK | FG_YELLOW);
	WinConsole::Print(L" ");
	WinConsole::Print(WidenStr(strMsg));
	WinConsole::Print(L"\r\n");

	std::stringstream logString;
	logString << std::endl << libtools::wstring_to_string(sm_prefix) + "WARNING:: " << strMsg;

	WriteToLogFile(logString.str());

	return false;
}

void ImplLog::LogWrite(const std::string& strMsg)
{
	WriteToLogFile(strMsg);
}

void ImplLog::WriteToLogFile(const std::string& logString)
{
	std::ofstream oOutFile(GetOutLogFilePath(), std::ios::app);
	oOutFile << logString;
	oOutFile.close();
}

void ImplLog::LogActionTimedBegin(const std::string& strMsg)
{
	m_globalClock.ResetLocalTime();
	LogActionInfo(strMsg);
}

void ImplLog::LogActionTimedEnd(const std::string& strMsg)
{
	auto timeElapsed = m_globalClock.GetLocalTime();
	LogActionInfo(strMsg);

	auto timeMessage = "Time Elapsed: " + std::to_string(timeElapsed) + " seconds.\n";

	WinConsole::Print(WidenStr(timeMessage), BG_BLACK | FG_GREEN);
}

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

timer::SystemClockChecker ImplLog::m_globalClock;



class blah
{
	template<typename T>  // primary template
	void FuncBlah(T t)
	{
		// fallback ?
	}
};

template<>
void blah::FuncBlah(int t)
{
	// specialization for int
}

bool IsEven(int32_t i)
{
	return (i & 0b0000000000000001) == 0;
}