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
	std::stringstream temp;
	temp << msg << std::endl;
	std::cout << temp.str();
	if (fileOut.is_open()) fileOut << temp.str();
}

void pxpk::Logger::log(std::string msg, pxpk::LogLevel msgLevel)
{
	if (currentLogLevel >= msgLevel) log(msg);
}
