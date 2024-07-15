#include "SystemClockChecker.h"

namespace timer {

	SystemClockChecker::SystemClockChecker()
	{
		ResetLocalTime();
	}

	void SystemClockChecker::ResetLocalTime()
	{
		m_startTime = std::chrono::high_resolution_clock::now();
	}

	float SystemClockChecker::GetLocalTime() const
	{
		auto timeElapsed = std::chrono::high_resolution_clock::now();

		auto value = std::chrono::duration<float, std::chrono::seconds::period>(timeElapsed - m_startTime);

		float retValue = value.count();

		return retValue;
	}

	long long SystemClockChecker::GetSystemTick() const
	{
		return ::std::chrono::high_resolution_clock::now().time_since_epoch().count();
	}
}