/*
	File: Log.h

	Win32 api console + txt file loggin

	Author: Phazer, 2020-2023
*/

#pragma once

#include <Windows.h>
#include <consoleapi2.h>

#include <string>
#include <memory>

#include "..\Timer\SystemClockChecker.h"
#include "..\Utils\StrUtils.h"

namespace logger {

	using namespace timer;
	using namespace utils;

	enum ConsoleColorFG
	{
		FG_BLACK = 0,
		FG_DARKBLUE = FOREGROUND_BLUE,
		FG_DARKGREEN = FOREGROUND_GREEN,
		FG_DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
		FG_DARKRED = FOREGROUND_RED,
		FG_DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
		FG_DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
		FG_DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		FG_GRAY = FOREGROUND_INTENSITY,
		FG_BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
		FG_GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		FG_CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
		FG_RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
		FG_MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
		FG_YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
		FG_WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	};

	enum ConsoleColorBG
	{
		BG_BLACK = 0,
		BG_DARKBLUE = BACKGROUND_BLUE,
		BG_DARKGREEN = BACKGROUND_GREEN,
		BG_DARKCYAN = BACKGROUND_GREEN | BACKGROUND_BLUE,
		BG_DARKRED = BACKGROUND_RED,
		BG_DARKMAGENTA = BACKGROUND_RED | BACKGROUND_BLUE,
		BG_DARKYELLOW = BACKGROUND_RED | BACKGROUND_GREEN,
		BG_DARKGRAY = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
		BG_GRAY = BACKGROUND_INTENSITY,
		BG_BLUE = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
		BG_GREEN = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
		BG_CYAN = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
		BG_RED = BACKGROUND_INTENSITY | BACKGROUND_RED,
		BG_MAGENTA = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
		BG_YELLOW = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
		BG_WHITE = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	};

#define FULL_FUNC_INFO(message) std::string(__func__) +  std::string(", line ") + std::to_string(__LINE__) + ": " + message;
#define FULL_FUNC_INFO_W(message) ToWString(__func__) +  std::wstring(L": Line: ") + ToWString(__LINE__) + L": " + message;
#define FULL_FUNC_INFO_W_EXC(message) std::string(__func__) +  std::string(L": Line: ") + std::to_string(__LINE__) + L": " + message;
#define LogInfo(msg) LogActionInfo( FULL_FUNC_INFO(msg) );
#define LogAction(message)  LogActionInfo(std::string(__func__) +  std::string(": Line: ") + std::to_string(__LINE__) + ": " + message);
#define LogActionColor(message)  LogSimpleWithColor(std::string(__func__) +  std::string(": Line: ") + std::to_string(__LINE__) + ": " + message);

	class WinConsole
	{
	public:
		static void Print(const std::wstring& str, WORD wColorFlags = BG_BLACK | FG_WHITE);
		static void PrintLn(const std::wstring& str, WORD color = BG_BLACK | FG_WHITE);
	};

	// TODO: finish making this into a neat singleton
	// TODO Frodo: This generates very big logs. We need a way to clear them.
	class Logger
	{
	public:
		static Logger& GetInstance();

		// TODO clean this up with better encapsulation, setters/getters
		static void SetLogFolder(const std::wstring& path);
		static std::wstring GetOutLogFilePath();

		static void DoLog(const std::wstring& strMsg, const std::wstring& strTag = L"ACTION", WORD color = FG_WHITE | BG_BLACK, WORD tagColor = FG_WHITE | BG_BLACK);
		static void LogSimpleWithColor(const std::wstring& strMsg, WORD wColorFlags = BG_BLACK | FG_WHITE);

		static void LogActionSuccess(const std::wstring& strMsg);
		static void LogActionInfo(const std::wstring& strMsg);
		static void LogActionWarning(const std::wstring& strMsg);
		static void LogActionError(const std::wstring& strMsg);
		static void LogException(const std::wstring& strMsg);

		static void WriteToLogFile(const std::wstring& logString);

		static void LogActionTimedBegin(const std::wstring& strMsg);
		static void LogActionTimedEnd(const std::wstring& strMsg);

		static std::wstring sm_loggingFolder;
		static std::wstring sm_logFileName;
		static std::wstring sm_prefix;
		static SystemClockChecker m_globalClock;

	private:
		static std::unique_ptr<Logger> m_poInstance;
	};

	class TimeLogAction
	{
	public:
		static TimeLogAction MakeObject();

		TimeLogAction();

		static TimeLogAction PrintStart(const std::wstring& messageToShow);
		void PrintDone(const std::wstring& messageToShow = L"");
		void PrintDoneSuccess(const std::wstring& messageToShow = L"");

		SystemClockChecker& GetClock();
		double GetLocalTime();

	private:
		SystemClockChecker m_clock;
		std::wstring m_message = L"";
	};
}