/*
	File: Logging.h

	Win32 api console + txt file loggin

	Author: Phazer, 2020-2023
*/

#pragma once

#include <codecvt>
#include <fstream>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>
#include <Windows.h>
#include "..\..\CommonLibs\Timer\SystemClockChecker.h"
#include "..\Tools\tools.h"

namespace logging {

	inline bool emptyBool() { return false; }
	inline void empty() {  }
#if 0
#define LogActionColor(message) empty()
#define LogActionError(msg) emptyBool()
#define LogInfo(msg) empty()
#define LogAction(message) empty()
#define LogActionWarning(message) emptyBool()

#define LogActionSuccess(message) empty()

#define LogException(msg) empty()

#define FULL_FUNC_INFO(message) std::string("")

#define FULL_FUNC_INFO_W(message) std::wstring(L"")

#define FULL_FUNC_INFO_W_EXC(message) std::wstring(L"")
#else


#define FULL_FUNC_INFO(message) std::string(std::string(__func__) +  std::string(": Line: ") + std::to_string(__LINE__) + ": " + message)

#define FULL_FUNC_INFO_W(message) libtools::string_to_wstring(__func__) +  std::wstring(L": Line: ") + std::to_wstring(__LINE__) + L": " + message

	//#define FULL_FUNC_INFO_W_EXC(message) std::string(__func__) +  std::string(L": Line: ") + std::to_string(__LINE__) + L": " + message
#define FULL_FUNC_INFO_W_EXC(message) ToWString(__DATE__) + L":" + ToWString(__TIME__) + L":  " + ToWString(__func__) + L"()" + std::wstring(L": Line: ") + std::to_wstring(__LINE__) + L": " + message





#define LogException(what) ImplLog::LogException( FULL_FUNC_INFO(what) );

#define LogActionError(msg) ImplLog::LogActionErrorFalse( FULL_FUNC_INFO(msg) );

#define LogInfo(msg) ImplLog::LogActionInfo( FULL_FUNC_INFO(msg) );

#define LogAction(message)  ImplLog::LogActionInfo( \
	std::string(__func__) +  std::string(": Line: ") + std::to_string(__LINE__) + ": " + message);\

#define LogActionColor(message)  ImplLog::LogSimpleWithColor( \
	std::string(__func__) +  std::string(": Line: ") + std::to_string(__LINE__) + ": " + message);\

#define LogActionSuccess(message) ImplLog::LogAction_success(message);

#define LogActionWarning(message) ImplLog::LogAction_warning(message);

#endif

#include "inl\WinConsoleColors.inl"



	class WinConsole
	{
	public:
		static void Print(const std::wstring& str, WORD wColorFlags = BG_BLACK | FG_WHITE);
		static void PrintLn(const std::wstring& str, WORD color = BG_BLACK | FG_WHITE);
	};

	// TODO: finish making this into a neat singleton
	class ImplLog
	{
	public:
		// TODO clean this up with better encapsulation, setters/getters
		static void SetLogFolder(const std::wstring& path);

		static std::wstring sm_loggingFolder;
		static std::wstring sm_logFileName;

		static std::wstring sm_prefix;

		static std::wstring GetOutLogFilePath() { return sm_loggingFolder + sm_logFileName; }

		static void Log_Exception(const std::string& strMsg);
		static void LogActionInfo(const std::string& strMsg);
		static void DoLog(const std::string& strMsg,
						  const std::string& strTag = "ACTION: ",
						  WORD color = FG_WHITE | BG_BLACK,
						  WORD tagColor = FG_WHITE | BG_BLACK);
		static void LogSimpleWithColor(const std::string& strMsg, WORD wColorFlags = BG_BLACK | FG_WHITE);
		static void LogAction_success(const std::string& strMsg);


		static bool LogActionErrorFalse(const std::string& strMsg);
		//static bool LogActionErrorFalse(const std::string& strMsg);


		static bool LogAction_warning(const std::string& strMsg);
		static void LogWrite(const std::string& strMsg);
		static void WriteToLogFile(const std::string& logString);

		static void LogActionTimedBegin(const std::string& strMsg);
		static void LogActionTimedEnd(const std::string& strMsg);


		static ImplLog& GetInstance() {
			if (!m_poInstance)
			{
				m_poInstance = std::make_unique<ImplLog>();
			}
			else
			{
				return *m_poInstance;
			}
		};

		static timer::SystemClockChecker m_globalClock;

	private:
		static std::unique_ptr<ImplLog> m_poInstance;
	};

	class TimeLogAction
	{
	public:
		static TimeLogAction MakeObject() { return TimeLogAction(); };


		TimeLogAction() { m_clock.ResetLocalTime(); };

		static TimeLogAction PrintStart(const std::string& messageToShow)
		{
			TimeLogAction newInstance;
			newInstance.m_message = messageToShow;
			ImplLog::LogSimpleWithColor(NarrowStr(ImplLog::sm_prefix) + newInstance.m_message + ".", BG_BLACK | FG_WHITE);

			return newInstance;
		};

		void PrintDone(const std::string& messageToShow = "")
		{
			ImplLog::LogSimpleWithColor(NarrowStr(ImplLog::sm_prefix) + m_message + ". Done.", BG_BLACK | FG_WHITE);

			auto timeElapsedMessageString = "Time Elapsed: " + std::to_string(m_clock.GetLocalTime()) + " seconds";
			ImplLog::LogSimpleWithColor(timeElapsedMessageString, BG_BLACK | FG_GREEN);
		};

		void PrintDoneSuccess(const std::string& messageToShow = "")
		{
			ImplLog::LogSimpleWithColor(NarrowStr(ImplLog::sm_prefix) + m_message + ". Completed Succesfully.", BG_BLACK | FG_WHITE);

			auto timeElapsedMessageString = "Time Elapsed: " + std::to_string(m_clock.GetLocalTime()) + " seconds";
			ImplLog::LogSimpleWithColor(timeElapsedMessageString, BG_BLACK | FG_GREEN);
		};

		timer::SystemClockChecker& GetClock() { return m_clock; };
		double GetLocalTime() { return m_clock.GetLocalTime(); };

	private:
		timer::SystemClockChecker m_clock;
		std::string m_message = "";
	};



	//namespace ConsoleForeground
	//{
	//    enum {
	//        BLACK = 0,
	//        DARKBLUE = FOREGROUND_BLUE,
	//        DARKGREEN = FOREGROUND_GREEN,
	//        DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
	//        DARKRED = FOREGROUND_RED,
	//        DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
	//        DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
	//        DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	//        GRAY = FOREGROUND_INTENSITY,
	//        BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
	//        GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	//        CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
	//        RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
	//        MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
	//        YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
	//        WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	//    };
	//}
	//
	//namespace ConsoleBackground
	//{
	//    enum ConsoleBackground {
	//        BLACK = 0,
	//        DARKBLUE = BACKGROUND_BLUE,
	//        DARKGREEN = BACKGROUND_GREEN,
	//        DARKCYAN = BACKGROUND_GREEN | BACKGROUND_BLUE,
	//        DARKRED = BACKGROUND_RED,
	//        DARKMAGENTA = BACKGROUND_RED | BACKGROUND_BLUE,
	//        DARKYELLOW = BACKGROUND_RED | BACKGROUND_GREEN,
	//        DARKGRAY = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	//        GRAY = BACKGROUND_INTENSITY,
	//        BLUE = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
	//        GREEN = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
	//        CYAN = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
	//        RED = BACKGROUND_INTENSITY | BACKGROUND_RED,
	//        MAGENTA = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
	//        YELLOW = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
	//        WHITE = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	//    };
	//};


}