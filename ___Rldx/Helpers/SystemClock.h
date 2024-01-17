#pragma once

#include <vector>
#include <Windows.h>
#include <chrono>

namespace sys_lib
{
	class SystemClock
	{
	public:

		SystemClock(double _max_time);

		SystemClock();

		void wait(double t_second);

		void setFrameCount(size_t _frame_count);

		void setFrameRate(double _frame_rate);

		void setMaxTime(double _max_time);

		void setIncrement(double _inc_time);

		void setTime(double _time);

		void pause(double _add = 0);

		void stop();

		bool isStopped();

		void resume();

		void skipFrameForward();

		void skipFrameBackward();

		static void initController();

		static double getSeconds();
		static long long getTicks();
		static double getSecondsSinceAppStart();

		double frameLength();

		double animLength();

		void toEnd();

		void toStart();

		// get the current local time
		// for timing animations, etc
		double getClockTime() const;

		void resetLocalTicks();

		uint64_t getTicksLocal();

		// start the timer ass the specificed time
		void resetLocalTime(double _start_at = 0.0);

		double getLocalTime() const;

		// Get time for DISPLAYING timer values,
		// NOTE: do NOT use it for ACTUAL timing stuff, use 'getAnimTimePosition' instead
		double getDisplayTime_animTimePosition();

		size_t getFrameCount();

		double getFrameRate();

		void setLoop(bool _bState);

		bool loop();

		bool endReached();

	protected:
		double m_start_time = 0; // reference point

		double m_stop_time = 0;

		double m_max_time = 0;
		double m_increment_time = 0;

		double m_start_at = 0;

		double m_frame_rate = 10.0f;

		size_t m_frame_count = 0;

		bool m_bStopFlag = false;
		bool m_bLoop = true;
		bool m_bEndReached = false;

		long long m_start_ticks = 0;

		//QSlider* m_poSlider = nulltr;

		static long long sm_qwFrencuency;
		static long long sm_qwAppStartValue;
	};
};