#include "SystemClock.h"

sys_lib::SystemClock::SystemClock(double _max_time)
{
	m_max_time = _max_time;
	initController();
	resetLocalTime();
	resetLocalTicks();
}

sys_lib::SystemClock::SystemClock()
{
	initController();
	resetLocalTime();
	resetLocalTicks();
}

void sys_lib::SystemClock::wait(double t_seconds)
{
	auto startTime = getSeconds();


	while (startTime - getSeconds() < t_seconds);
}

void sys_lib::SystemClock::setFrameCount(size_t _frame_count)
{
	m_frame_count = _frame_count;

	/*if(m_poSlider)
	{
		m_poSlider->setMinimum(0);
		m_poSlider->setMinimum(_frame_count);
	}*/
}

void sys_lib::SystemClock::setFrameRate(double _frame_rate)
{
	m_frame_rate = _frame_rate;
}

void sys_lib::SystemClock::setMaxTime(double _max_time)
{
	m_max_time = _max_time;
	//	resetLocalTime();
}

void sys_lib::SystemClock::setIncrement(double _inc_time)
{
	m_increment_time = _inc_time;
	//	resetLocalTime();
}

void sys_lib::SystemClock::setTime(double _time)
{
	m_stop_time = _time;
	m_bStopFlag = true;
}

void sys_lib::SystemClock::pause(double _add)
{
	if (m_bStopFlag) return;

	m_stop_time = this->getLocalTime();

	m_bStopFlag = true;
}
void sys_lib::SystemClock::stop()
{
	m_stop_time = 0;
	m_bStopFlag = true;

	// TODO: CHANGE/REMOVE/FIX
	//m_bStopFlag = false;
}

bool sys_lib::SystemClock::isStopped()
{
	return m_bStopFlag;
}

void sys_lib::SystemClock::resume()
{
	if (!m_bLoop)
	{
		resetLocalTime(0);
		m_bStopFlag = false;
		return;
	}

	resetLocalTime(m_stop_time);
	m_bStopFlag = false;
}

void sys_lib::SystemClock::skipFrameForward()
{
	if (!m_bStopFlag)
	{
		m_stop_time = getLocalTime();
		m_bStopFlag = true;
	}
	else
	{
		m_stop_time += (1.0f / m_frame_rate);
	}
}

void sys_lib::SystemClock::skipFrameBackward()

{
	if (!m_bStopFlag)
	{
		m_stop_time = getLocalTime();
		m_bStopFlag = true;
	}
	else
	{
		m_stop_time -= (1.0f / m_frame_rate); // go a frame back

		if (m_stop_time < 0)
		{
			// if "frame time" < 0
			// set frame time to "end time" + "the amount of time < 0"
			m_stop_time = (double)m_frame_count * (1.0f / m_frame_rate) + abs(m_stop_time - ((int)m_stop_time));
		}
	}
}
void sys_lib::SystemClock::initController()
{
	// get the frequence of clock
	LARGE_INTEGER liTemp;
	QueryPerformanceFrequency(&liTemp);
	sm_qwFrencuency = liTemp.QuadPart;

	// get current clock tick value
	QueryPerformanceCounter(&liTemp);

	sm_qwAppStartValue = liTemp.QuadPart;
}

double sys_lib::SystemClock::getSeconds()
{
	typedef std::chrono::high_resolution_clock Time;
	typedef std::chrono::duration<float> fsec;

	//auto t = Time::now();
	double ticks = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	double period = std::chrono::high_resolution_clock::period::den;
	
	
	double seconds = ticks / period;

	
	/*double nanoSeconds = clockTime.time_since_epoch().count();
	double period = std::chrono::time_point<std::chrono::high_resolution_clock>::period::num;*/

	//double seconds = nanoSeconds / period;

	return seconds;

	//LARGE_INTEGER liTemp;
	//QueryPerformanceCounter(&liTemp);

	//uint64_t qw = liTemp.QuadPart;

	//double dRet = (double)qw / double(sm_qwFrencuency);

	//return dRet;
}

long long sys_lib::SystemClock::getTicks()
{
	
	const std::chrono::time_point<std::chrono::high_resolution_clock> clockTime = std::chrono::high_resolution_clock::now();
	
	int64_t test = clockTime.time_since_epoch().count();

	return clockTime.time_since_epoch().count();


		


	//LARGE_INTEGER liTemp;
	//QueryPerformanceCounter(&liTemp);

	//uint64_t qw = liTemp.QuadPart - sm_qwAppStartValue;

	////double dRet = (double)qw / double(sm_qwFrencuency);

	//return qw;
}

double sys_lib::SystemClock::getSecondsSinceAppStart()
{
	LARGE_INTEGER liTemp;
	QueryPerformanceCounter(&liTemp);

	uint64_t qw = liTemp.QuadPart - sm_qwAppStartValue;

	double dRet = (double)qw / double(sm_qwFrencuency);

	return dRet;
}

double sys_lib::SystemClock::frameLength()
{
	if (m_frame_rate == 0.0 || m_frame_rate < 0.0)
		return 0.0;

	return (1.0 / m_frame_rate);
}

double sys_lib::SystemClock::animLength()
{
	if (m_frame_rate == 0.0f || m_frame_rate < 0.0f)
		return 0.0f;

	return (1.0f / m_frame_rate) * (double)m_frame_count;
}

void sys_lib::SystemClock::toEnd()
{
	// set the animation at the end - 1/20 framelength´
	resetLocalTime(animLength() - frameLength() * 1.1f);
	m_stop_time = animLength() - frameLength() * 1.1f;
}

void sys_lib::SystemClock::toStart()
{
	resetLocalTime(0);
	m_stop_time = 0;
}

double sys_lib::SystemClock::getClockTime() const
{
	// TODO: REMOVE DEBUGGIN CODE
	//return getLocalTime();
	// ENB: debugging code

	double _time = (m_bStopFlag) ? (m_stop_time) : getLocalTime(); // get local time / the stopped timed

	return _time;

	//if (m_frame_count < 1)
	//	return 0.0f;

	//// make sure to always restart a 0

	//double fAnimLen = ((double)(m_frame_count - 1.0f) * (1.0f / m_frame_rate));

	//double time = (m_bStopFlag) ? (m_stop_time) : getLocalTime(); // get local time / the stopped timed

	//int number_of_anim_lenghts = static_cast<int>(time / fAnimLen); // trucante number of anim lengths

	//if (number_of_anim_lenghts > 0)
	//	int _debug = 1;

	//double local_time = time - number_of_anim_lenghts * fAnimLen;

	//if (!m_bLoop && local_time >= fAnimLen)
	//{
	//	m_bEndReached = true;
	//	if (!m_bStopFlag)
	//		pause();

	//	return fAnimLen - frameLength() / 5;
	//}

	//if (local_time < 0)
	//{
	//	local_time = fAnimLen + local_time;
	//	int i = 1;
	//}
	////m_poSlider->setSliderPosition((int)local_time);

	//return local_time; // substract  time to get time in "local anim time"
}

void sys_lib::SystemClock::resetLocalTicks()
{
	m_start_ticks = getTicks();
}

uint64_t sys_lib::SystemClock::getTicksLocal()
{
	return getTicks() - m_start_ticks;
}

void sys_lib::SystemClock::resetLocalTime(double _start_at)
{
	m_start_time = getSeconds() - _start_at;
}


double sys_lib::SystemClock::getLocalTime() const
{
	double time_elapsed = getSeconds() - m_start_time; // +m_start_at;
	return time_elapsed;
}

double sys_lib::SystemClock::getDisplayTime_animTimePosition()

{
	if (m_frame_count < 1)
		return 0.0f;

	// make sure to always restart a 0

	double fAnimLen = ((double)(m_frame_count - 1.0f) * (1.0f / m_frame_rate));

	double time = (m_bStopFlag) ? (m_stop_time) : getLocalTime(); // get local time / the stopped timed

	int number_of_anim_lenghts = static_cast<int>(time / fAnimLen); // trucante number of anim lengths

	if (number_of_anim_lenghts > 0)
		int _debug = 1;

	double local_time = time - number_of_anim_lenghts * fAnimLen;

	if (!m_bLoop && local_time >= fAnimLen)
	{
		m_bEndReached = true;
		if (!m_bStopFlag)
			pause();

		return fAnimLen - frameLength() / 5;
	}

	if (local_time < 0)
	{
		local_time = fAnimLen + local_time;
		int i = 1;
	}
	//m_poSlider->setSliderPosition((int)local_time);

	return local_time; // substract  time to get time in "local anim time"
}
size_t sys_lib::SystemClock::getFrameCount()
{
	return m_frame_count;
}

double sys_lib::SystemClock::getFrameRate()
{
	return m_frame_rate;
}

void sys_lib::SystemClock::setLoop(bool _bState)
{
	m_bLoop = _bState;
}

bool sys_lib::SystemClock::loop()
{
	return m_bLoop;
}

bool sys_lib::SystemClock::endReached()
{
	return m_bEndReached;
}

int64_t sys_lib::SystemClock::sm_qwFrencuency;
int64_t sys_lib::SystemClock::sm_qwAppStartValue;