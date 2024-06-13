#include "Logger.h"
#include <fstream>
#include <sstream>
#include "..\Utils\StrUtils.h"

//---------------------------------------------------------------------//
// Logger implementation
//---------------------------------------------------------------------//

namespace logger {

	using namespace utils;

	Logger& Logger::GetInstance() {
		if (!m_poInstance)
		{
			m_poInstance = std::make_unique<Logger>();
			sm_logFileName = L"RenderViewLog.txt";
			sm_prefix = L"[QRenderingView Debug:] ";
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

		WriteToLogFile(strTag + L": " + strMsg);
	}

	void Logger::LogSimpleWithColor(const std::wstring& strMsg, WORD wColorFlags)
	{
		WinConsole::Print(strMsg, wColorFlags);
		WinConsole::Print(L"\r\n");

		WriteToLogFile(strMsg);
	}

	void Logger::LogActionSuccess(const std::wstring& strMsg)
	{
		DoLog(strMsg, L"SUCCESS", BG_BLACK | FG_WHITE, BG_BLACK | FG_GREEN);
	}

	void Logger::LogActionInfo(const std::wstring& strMsg)
	{
		DoLog(strMsg, L"ACTION");
	}

	void Logger::LogActionWarning(const std::wstring& strMsg)
	{
		DoLog(strMsg, L"SUCCESS", BG_BLACK | FG_WHITE, BG_BLACK | FG_YELLOW);
	}

	void Logger::LogActionError(const std::wstring& strMsg)
	{
		DoLog(strMsg, L"SUCCESS", BG_BLACK | FG_WHITE, BG_BLACK | FG_RED);
	}

	void Logger::LogException(const std::wstring& strMsg)
	{
		DoLog(strMsg, L"EXCEPTION Caught", BG_BLACK | FG_WHITE, BG_BLACK | FG_RED);
	}

	void Logger::WriteToLogFile(const std::wstring& strMsg)
	{
		std::wstringstream logString;
		logString << std::endl << sm_prefix + strMsg;

		std::ofstream oOutFile(GetOutLogFilePath(), std::ios::app);
		oOutFile << ToString(logString.str());
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

		auto timeMessage = L"Time Elapsed: " + std::to_wstring(timeElapsed) + L" seconds.\n";

		WinConsole::Print(timeMessage, BG_BLACK | FG_GREEN);
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

	SystemClockChecker& TimeLogAction::GetClock() {
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
}