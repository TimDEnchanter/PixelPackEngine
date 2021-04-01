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

void pxpk::Logger::_logGL(const char * file, int line)
{
	GLenum err(glGetError());

	while (err != GL_NO_ERROR)
	{
		std::string errStr;

		switch (err)
		{
		case GL_INVALID_OPERATION: errStr = "OpenGL error - INVALID_OPERATION"; break;
		case GL_INVALID_ENUM:  errStr = "OpenGL error - INVALID_ENUM"; break;
		case GL_INVALID_VALUE:  errStr = "OpenGL error - INVALID_VALUE"; break;
		case GL_OUT_OF_MEMORY:  errStr = "OpenGL error - OUT_OF_MEMORY"; break;
		case GL_STACK_OVERFLOW:  errStr = "OpenGL error - STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:  errStr = "OpenGL error - STACK_UNDERFLOW"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  errStr = "OpenGL error - INVALID_FRAMEBUFFER_OPERATION"; break;
		case GL_CONTEXT_LOST:  errStr = "OpenGL error - CONTEXT_LOST"; break;
		case GL_TABLE_TOO_LARGE:  errStr = "OpenGL error - TABLE_TOO_LARGE"; break;
		default: errStr = "OpenGL error - Unknown"; break;
		}

		pxpk::Logger::getInstance()._log(errStr, pxpk::ERROR_LOG, file, line);

		err = glGetError();
	}
}
