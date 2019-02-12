#pragma once
#ifndef TIMER_H
#define TIMER_H
#include <vector>
#include <string>
#include <Windows.h>
namespace timer
{

	class Timer
	{
	private:
		__int64 value;
		__int64 PauseValue;
		std::string time;
		bool StartCounter();
		bool paused;
		bool reset;
		__int64 GetCounter();
		unsigned long long int PCFreq;
		__int64 CounterStart;
		__int64 CounterIntermediate;
		__int64 getIntermediateValue();
		std::string toString(__int64 value, std::string type);
	public:
		Timer();
		__int64 getValue();
		std::string getTime(std::string type);
		void setValue(__int64 value);
		bool start();
		std::string getIntermediate(std::string type);
		bool pause();
		bool stop();
	};
}

#endif