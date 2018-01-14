#ifndef LOGGER_H
#define LOGGER_H


#include <string>
#include <iostream>
#include <fstream>

/*
Logger

Singelton used to display logs. keeps track of surrent message level and output destination.
*/

namespace pxpk
{
	enum LogLevel : std::uint8_t
	{
		ERROR_LOG = 0,
		INFO_LOG = 4,
		ACTIONS_LOG = 8,
		CLASSES_LOG = 12,
		FULL_LOG = 16,
	};


	class Logger
	{
	private:
		Logger() : fileOut("/logs/log.txt"),
			currentLogLevel(LogLevel::ERROR_LOG)
		{};

		std::ofstream fileOut;
		pxpk::LogLevel currentLogLevel;

	public:
		static Logger& getInstance()
		{
			static Logger instance;
			return instance;
		}

		~Logger();

		Logger(Logger const&) = delete;
		void operator=(Logger const&) = delete;

		//setters
		void setFileOut(std::string filename);
		void setCurrentLogLevel(pxpk::LogLevel newLogLevel) { currentLogLevel = newLogLevel; }

		// message functions
		void log(std::string msg);
		void log(std::string msg, pxpk::LogLevel msgLevel);
	};
}

#endif // !LOGGER_H