#include <fountain/ft_time.h>

using ftTime::Clock;

static double allStartT = 0.0;

#ifdef linux

// Linux
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif
#include <sys/time.h>
#include <unistd.h>
void littleSleep()
{
	usleep(1);
}

double floatTime()
{
	static struct timeval now;
	gettimeofday(&now, NULL);
	return (1000000 * now.tv_sec + now.tv_usec) / 1000000.0 - allStartT;
}

// Linux end
#endif

#ifdef _WIN32
// Win32
#include <time.h>
#include <windows.h>
void littleSleep()
{
	Sleep(1);
}

double floatTime()
{
	LARGE_INTEGER tickPerSecond, tick;
	QueryPerformanceFrequency(&tickPerSecond);
	QueryPerformanceCounter(&tick);
	return tick.QuadPart / (double)tickPerSecond.QuadPart - allStartT;
}

// Win32 end
#endif

void ftTime::init()
{
	floatTime();
}

Clock::Clock(double fps)
{
	if (fps == 0.0)
		secondPerFrame = 0.0;
	else
		secondPerFrame = 1.0 / fps;
	isPaused = 1;
	deltaT = 0.0;
	timeScale = 1.0;
}

void Clock::init()
{
	firstT = beginT = continueT = pauseT = floatTime();
	isPaused = 0;
}

void Clock::tick()
{
	endT = floatTime();
	while ((deltaT = endT - beginT) < secondPerFrame) {
		littleSleep();
		endT = floatTime();
	}
	if (secondPerFrame != 0 && deltaT > secondPerFrame)
		deltaT = secondPerFrame;
	deltaT *= timeScale;
	if (isPaused == 1)
		deltaT = 0.0;
	beginT = endT;
}

double Clock::getDeltaT()
{
	return deltaT;
}

double Clock::secondsFromInit()
{
	return floatTime() - firstT;
}

double Clock::secondsFromPause()
{
	return floatTime() - pauseT;
}

double Clock::secondsFromContinue()
{
	return floatTime() - continueT;
}

void Clock::Pause()
{
	pauseT = floatTime();
	isPaused = 1;
}

void Clock::Continue()
{
	beginT = continueT = floatTime();
	isPaused = 0;
}
