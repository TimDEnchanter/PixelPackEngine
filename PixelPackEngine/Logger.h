#pragma once

#include <string>
#include <iostream>
#include <fstream>

/*
Logger

Singelton used to display logs. keeps track of surrent message level and output destination.
*/

namespace LogLevel
{
	enum Level : std::uint8_t
	{
		errors = 0,
		info = 4,
		actions = 8,
		classes = 12,
		full = 16,
	};
}

class Logger
{
	private:
		Logger() : fileOut("/logs/log.txt"), 
			       currentLogLevel(LogLevel::errors) 
		{};

		std::ofstream fileOut;
		LogLevel::Level currentLogLevel;

	public:
		static Logger& getInstance()
		{
			static Logger instance;
			return instance;
		}

		~Logger();

		Logger(Logger const&)            = delete;
		void operator=(Logger const&)    = delete;

		//setters
		void setFileOut(std::string filename);
		void setCurrentLogLevel(LogLevel::Level newLogLevel) { currentLogLevel = newLogLevel; }

		// message functions
		void log(std::string msg);
		void log(std::string msg, LogLevel::Level msgLevel);
};

