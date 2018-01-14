#include "Logger.h"




pxpk::Logger::~Logger()
{
}


void pxpk::Logger::setFileOut(std::string filename)
{
	if (fileOut.is_open()) fileOut.close();
	fileOut.open(filename);
}

void pxpk::Logger::log(std::string msg)
{
	std::cout << msg << std::endl;
	if (fileOut.is_open()) fileOut << msg << std::endl;
}

void pxpk::Logger::log(std::string msg, pxpk::LogLevel msgLevel)
{
	if (currentLogLevel >= msgLevel) log(msg);
}
