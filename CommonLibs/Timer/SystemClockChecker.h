#pragma once

#include <chrono>

namespace timer {

	/// <summary>
	/// Uses the CPUs high resolution clock, to count time intervals
	/// </summary>
	class SystemClockChecker
	{
		typedef std::chrono::high_resolution_clock Time;

	public:

		SystemClockChecker();

		/// <summary>
		/// Resets the local timer to "now"
		/// </summary>
		void ResetLocalTime();

		/// <summary>
		///  Get "local" time, 
		/// </summary>
		/// <returns>"state time" - "now" </returns>
		double GetLocalTime() const;

		/// <summary>
		/// Rerturn the CPU "ticks"
		/// </summary>		
		long long GetSystemTick() const;

	private:
		std::chrono::steady_clock::time_point m_startTime;
	};
}