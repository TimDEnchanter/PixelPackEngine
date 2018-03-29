#include "Logger.h"




pxpk::Logger::~Logger()
{
}


void pxpk::Logger::setFileOut(std::string filename)
{
	if (fileOut.is_open()) fileOut.close();
	fileOut.open(filename);
}

void pxpk::Logger::_log(std::string msg, const char *file, int line)
{
	std::stringstream outStream;
	std::stringstream locationStream;
	locationStream << "[" << file << ":" << line << "]";
	outStream << std::left << std::setw(25) << locationStream.str() << msg << std::endl;
	std::cout << outStream.str();
	if (fileOut.is_open()) fileOut << outStream.str();
}

void pxpk::Logger::_log(std::string msg, pxpk::LogLevel msgLevel, const char *file, int line)
{
	if (currentLogLevel >= msgLevel) _log(msg, file, line);
}
