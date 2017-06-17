#pragma once
#include <string>
#include <time.h>

class FileUtil
{
public:
	static FileUtil* getInstance();
	static void destroyInstance();
	std::string getStringFromFile(const std::string& filename);
	bool writeLog(const std::string& str);
	void setDir(std::string dir);
private:
	bool init();
private:
	static FileUtil* m_instance;
	FILE* m_logFile = nullptr;
	time_t m_logTime;
	std::string m_dir;
};