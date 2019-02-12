#pragma once
#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H
#include <vector>
#include <mutex>
#include <string>
#include <algorithm>

#include "timer.h"
namespace search {
	bool t();
	bool checkStringByFlags(std::string left, std::string right, int flags);
	typedef std::vector<std::string> StringArray;
	struct thread_data
	{
		std::vector<StringArray> *ats;
		timer::Timer *timer1;
		unsigned int *threadId;
		int maxThreadCount;
		std::mutex *searchLock;
		std::string folder;
		std::string search;
		double factor;
		double *mainFactor;
		int m_id;
		bool child;
		bool ISthread;
		bool firstRun;
		std::vector<int> *status;

		int flags;
		thread_data(int id) : m_id(id) {}
	};

	void search_folder(thread_data td);
}

#endif // !SEARCHENGINE_H
