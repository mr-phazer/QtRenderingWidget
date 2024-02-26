#pragma once

#include <chrono>

namespace Utils {

	/// Uses the CPUs high resolution clock, to count time intervals
	class SystemClock
	{
		typedef std::chrono::high_resolution_clock Time;

	public:

		SystemClock();

		/// Resets the local timer to "now"
		void ResetLocalTime();

		///  Get "local" time, "state time" - "now"
		double GetLocalTime() const;

	private:
		std::chrono::steady_clock::time_point m_startTime;
	};
}