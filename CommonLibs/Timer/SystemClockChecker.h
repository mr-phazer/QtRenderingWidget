#pragma once

#include <chrono>

namespace timer
{

	/// <summary>
		/// Uses the CPUs high resolution clock, to count time intervals
		/// </summary>
	class SystemClockChecker
	{
		typedef std::chrono::high_resolution_clock Time;

	public:
		SystemClockChecker()
		{
			ResetLocalTime();
		}

		/// <summary>
		/// Resets the local timer to "now"
		/// </summary>
		void ResetLocalTime()
		{
			m_startTime = std::chrono::high_resolution_clock::now();
		}

		/// <summary>
		///  Get "local" time, 
		/// </summary>
		/// <returns>"state time" - "now" </returns>
		float GetLocalTime() const
		{
			auto timeElapsed = std::chrono::high_resolution_clock::now();

			auto value = std::chrono::duration<float, std::chrono::seconds::period>(timeElapsed - m_startTime);

			float retValue = value.count();

			return retValue;
		}

		/// <summary>
		/// Rerturn the CPU "ticks"
		/// </summary>		
		long long GetSystemTick() const
		{
			return ::std::chrono::high_resolution_clock::now().time_since_epoch().count();
		}

	private:
		::std::chrono::steady_clock::time_point m_startTime;
	};
}