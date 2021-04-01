#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <vector>
#include <Windows.h>
#include <filesystem>

namespace pxpk
{
	std::string getWorkingDir();
	size_t getFileSize(std::string);
}

#endif // !FILE_UTILS_H