#include "searchEngine.h"
#include <iostream>
bool search::t()
{
	return false;
}

bool search::checkStringByFlags(std::string left, std::string right, int flags)
{
	bool Extension = (4 & flags);
	bool matchCase = (2 & flags);
	bool caseSensitive = (1 & flags);
	if (!Extension) {
		size_t pos = right.find_last_of('.');
		if (pos != std::string::npos) {
			right.erase(pos);
		}
	}
	if (caseSensitive) {
		std::transform(right.begin(), right.end(), right.begin(), ::tolower);
		std::transform(left.begin(), left.end(), left.begin(), ::tolower);
	}
	bool match;
	if (!matchCase)
		match = right.find(left) != std::string::npos;
	else
		match = right == left;
	return match;
}

void search::search_folder(thread_data td)

{
	if (td.searchLock != nullptr && td.status != nullptr) {
		td.searchLock->lock();
		td.status->push_back(td.m_id);
		td.searchLock->unlock();
	}
	int numbOfObject = 0;
	int numbOfFiles = 0;
	int numbOfFolders = 0;
	//std::cout << (1 & flags) << " " << (2 & flags) << " " << (4 & flags) << std::endl;
	//// 000  001 Extensions, 010 Match word, 100 case sensitive
	//bool Extension = (4 & td.flags);
	//bool matchCase = (2 & td.flags);
	//bool caseSensitive = (1 & td.flags);
	std::vector<std::string> folders;
	std::string search_path = td.folder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..

			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)) {
				numbOfFiles++;
				//caseSensitive

				//std::string temp(fd.cFileName);

				//if (!Extension) {
				//	size_t pos = temp.find_last_of('.');
				//	if (pos != std::string::npos) {
				//		temp.erase(pos);
				//	}
				//}
				//if (caseSensitive) {
				//	std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
				//	std::transform(td.search.begin(), td.search.end(), td.search.begin(), ::tolower);
				//}
				//bool match;
				//if (!matchCase)
				//	match = temp.find(td.search) != std::string::npos;
				//else
				//	match = temp == td.search;
				if (checkStringByFlags(td.search, fd.cFileName, td.flags)) {

					if (td.ats != nullptr) {
						std::string res(fd.cFileName);
						StringArray ats;
						ats.push_back("");
						ats.push_back(td.folder);
						ats.push_back(res);
						ats.push_back("File");
						//ats.add(String(td.timer1->getValue()));
						if (td.searchLock != nullptr) {

							td.searchLock->lock();
							if (td.timer1 != nullptr)
								ats.push_back(std::to_string(td.timer1->getValue()));
							else
								ats.push_back("");
							if (td.ats != nullptr) {
								td.ats->push_back(ats);
							}
							td.searchLock->unlock();
						}
					}
					//std::string res(fd.cFileName);

					//searchLock.lock();
					//files.push_back( string(td.folder + "/" + res));
					//searchLock.unlock();


				}
			}
			else if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				//std::string toPush(fd.cFileName);
				//names.push_back(toPush + "/");
				
				std::string path(fd.cFileName);
				if (path != "." && path != "..")
					folders.push_back(fd.cFileName);
				if (checkStringByFlags(td.search, fd.cFileName, td.flags)) {
					if (td.ats != nullptr) {
						std::string res(fd.cFileName);
						StringArray ats;
						ats.push_back("");
						ats.push_back(td.folder);
						ats.push_back(res);
						ats.push_back("Folder");

						//ats.add(String(td.timer1->getValue()));
						if (td.searchLock != nullptr) {

							td.searchLock->lock();
							if (td.timer1 != nullptr)
								ats.push_back(std::to_string(td.timer1->getValue()));
							else
								ats.push_back("");
							if (td.ats != nullptr) {
								td.ats->push_back(ats);
							}
							td.searchLock->unlock();
						}
					}
				}





			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
		//#pragma omp parallel for
		numbOfFolders = folders.size();
		numbOfObject = numbOfFolders + numbOfFiles;
		if (numbOfObject == 0) {
			td.searchLock->lock();
			if (td.mainFactor != nullptr)*td.mainFactor += td.factor;
			td.searchLock->unlock();
		}
		else {
			td.factor /= numbOfObject;
			td.searchLock->lock();
			if (td.mainFactor != nullptr)*td.mainFactor += td.factor * numbOfFiles;
			//if(numbOfObject == 0)if (td.mainFactor != nullptr)*td.mainFactor += td.factor;
			td.searchLock->unlock();

		}
		int i = 0;

		std::vector<std::thread> threads;
		for (auto var : folders) {
			thread_data data(td);
			data.m_id++;
			data.folder += '/' + var;
			data.firstRun = false;
			//data.factor = data.factor / numbOfObject;
			if (td.maxThreadCount > (data.threadId != nullptr ? 0 : *data.threadId) && !td.child) {
				data.ISthread = true;
				//data.m_id = (data.threadId != nullptr ? 0 : ++(*data.threadId));
				//data.m_id++;
				//Reikia kad kai visi aukstesniojo lygio ciklai yra baigti
				//eiti i zemesnio lygio
				data.child = false;

				std::thread a(search_folder, data);
				a.detach();
			}
			else {
				data.ISthread = false;
				search_folder(data);
			}
		}

	}
	if (td.ISthread && td.threadId != nullptr)(*td.threadId)--;
	if (td.searchLock != nullptr && td.status != nullptr) {
		td.searchLock->lock();
		//std::cout << *td.mainFactor << std::endl;
		td.status->pop_back();
		td.searchLock->unlock();
	}
}
