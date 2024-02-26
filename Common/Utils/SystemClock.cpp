#include "pch.h"
#include "SystemClock.h"

namespace Utils {

	SystemClock::SystemClock()
	{
		ResetLocalTime();
	}

	void SystemClock::ResetLocalTime()
	{
		m_startTime = std::chrono::high_resolution_clock::now();
	}

	double SystemClock::GetLocalTime() const
	{
		auto timeElapsed = std::chrono::high_resolution_clock::now();

		auto value = std::chrono::duration<float, std::chrono::seconds::period>(timeElapsed - m_startTime);

		float retValue = value.count();

		return retValue;
	}
}