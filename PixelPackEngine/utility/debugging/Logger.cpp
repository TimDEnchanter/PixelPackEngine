#include "Logger.h"




Logger::~Logger()
{
}


void Logger::setFileOut(std::string filename)
{
	if (fileOut.is_open()) fileOut.close();
	fileOut.open(filename);
}

void Logger::log(std::string msg)
{
	std::cout << msg << std::endl;
	if (fileOut.is_open()) fileOut << msg << std::endl;
}

void Logger::log(std::string msg, LogLevel::Level msgLevel)
{
	if (currentLogLevel >= msgLevel) log(msg);
}
