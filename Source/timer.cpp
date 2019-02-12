#include "timer.h"
namespace timer
{

Timer::Timer()
{
	value = 0;
	PCFreq = 0;
	CounterStart = 0;
	CounterIntermediate = 0;
	time = "00:00:00:00"; // "hh:mm:ss:msms"
	paused = true;
	reset = true;
}

__int64 Timer::getValue()
{
	value = GetCounter();
	return this->value;
}
void Timer::setValue(__int64 value)
{
	this->value = value;
}
bool Timer::StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
	{
		return 0;
	}
	PCFreq = li.QuadPart / 1000;
	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
	return 1;
}
__int64 Timer::GetCounter()
{
	if (paused) return value;
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return (li.QuadPart - CounterStart) / PCFreq;
}
bool Timer::start()
{
	if (!paused || reset)
	{
		if (!StartCounter())return 0;
		paused = false;
	}
	else
	{
		paused = false;
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		PauseValue = li.QuadPart - PauseValue;
		CounterStart += PauseValue;
	}
	return 1;
}
bool Timer::pause()
{
	if (paused)return 0;
	reset = false;
	value = GetCounter();
	paused = true;
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	PauseValue = li.QuadPart;


	return 1;
}
bool Timer::stop()
{
	if (reset)return 0;
	reset = true;
	value = GetCounter();
	return 1;
}
__int64 Timer::getIntermediateValue()
{
	if (paused)return value;
	__int64 stamp = GetCounter();
	__int64 IntermediateValue = stamp - CounterIntermediate;
	CounterIntermediate = stamp;
	return IntermediateValue;
}
std::string Timer::toString(__int64 value, std::string type)
{
	__int64 ms = value;
	__int64 s = (value / 1000) % 60;
	__int64 min = ((value / 1000) / 60) % 60;
	__int64 h = ((value / 1000) / 3600) % 24;
	ms %= 1000;
	ms -= ms % 10;
	ms /= 10;
	std::string value_to_string = "";

	bool hours = false;
	bool minutes = false;
	bool seconds = false;
	bool miliseconds = false;

	for (std::string::iterator it = type.begin(); it < type.end(); it++)
	{
		switch (*it)
		{
		case 'h':
			if (!hours)
				if (h != 0)
				{
					if (h >= 10)
						value_to_string += std::to_string(h);
					else
						value_to_string += "0" + std::to_string(h);
				}
				else
					value_to_string += "00";
			hours = true;
			break;
		case 'm':
			if (!minutes)
				if (min != 0)
				{
					if (min >= 10)
						value_to_string += std::to_string(min);
					else
						value_to_string += "0" + std::to_string(min);
				}
				else
					value_to_string += "00";
			minutes = true;
			break;
		case 's':
			if (!seconds)
				if (s != 0)
				{
					if (s >= 10)
						value_to_string += std::to_string(s);
					else
						value_to_string += "0" + std::to_string(s);
				}
				else
					value_to_string += "00";
			seconds = true;
			break;
		case 'c':

			if (!miliseconds)
				if (ms != 0)
				{
					if (ms >= 10)
						value_to_string += std::to_string(ms);
					else
						value_to_string += "0" + std::to_string(ms);
				}
				else
					value_to_string += "00";
			miliseconds = true;
			break;
		default:
			value_to_string += *it;
			break;
		}
	}
	return value_to_string;
}
std::string Timer::getIntermediate(std::string type)
{
	return toString(getIntermediateValue(), type);
}

std::string Timer::getTime(std::string type)
{
	value = GetCounter();
	return toString(value, type);
}
}