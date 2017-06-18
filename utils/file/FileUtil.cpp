#include "FileUtil.h"

FileUtil* FileUtil::m_instance = nullptr;

FileUtil* FileUtil::getInstance()
{
	if (!m_instance)
	{
		m_instance = new FileUtil();
		if (!m_instance->init())
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}

	return m_instance;
}

void FileUtil::destroyInstance()
{
	if (m_instance)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

void FileUtil::setDir(std::string dir)
{
	m_dir = dir;
}

bool FileUtil::init()
{
	//以当前时间戳作为后缀打开一个文件
	m_logTime = time(NULL);
	return true;
}

std::string FileUtil::getStringFromFile(const std::string& filename)
{
	unsigned char* buffer = nullptr;
	long size = 0;
	size_t readsize;
	FILE *fp = fopen(filename.c_str(), "rb");
	if (!fp)
	{
		return "";
	}
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buffer = (unsigned char*)malloc(sizeof(unsigned char)* (size + 1));
	buffer[size] = '\0';
	readsize = fread(buffer, sizeof(unsigned char), size, fp);
	fclose(fp);
	return std::string((char*)buffer);
}

bool FileUtil::writeLog(const std::string& str)
{
	std::string newStr = str + "\n";
	struct tm *p;
	p = localtime(&m_logTime); /*变量t的值转换为实际日期时间的表示格式*/
	char filename[256] = { 0 };
	sprintf(filename, "%slog_%d_%02d_%02d_%02d_%02d_%02d.log", m_dir.c_str(), 1900 + p->tm_year, \
		1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
	m_logFile = fopen(filename, "ab");
	if (!m_logFile)
	{
		return false;
	}
	fseek(m_logFile,0,SEEK_END);
	long size = ftell(m_logFile);
	long max_size = max_capacity;	//单个文件一兆
	long left_size = max_size - size;
	time_t curTime = time(NULL);
	struct tm* cur_p = localtime(&curTime);
	if ((cur_p->tm_mday != p->tm_mday) || (left_size < newStr.length()))
	{
		fclose(m_logFile);
		//文件不够长了或跨天 创建新文件
		if (!this->init())
		{
			return false;
		}

		memset(filename, 0, sizeof(filename));
		sprintf(filename, "%slog_%d_%02d_%02d_%02d_%02d_%02d.log", m_dir.c_str(), 1900 + p->tm_year, \
			1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
		m_logFile = fopen(filename, "ab");
	}
	fwrite((char*)newStr.c_str(), 1, newStr.length(), m_logFile);
	fclose(m_logFile);
	return true;
}