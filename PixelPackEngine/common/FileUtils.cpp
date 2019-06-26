#include "FileUtils.h"

std::string pxpk::getWorkingDir()
{
	//std::experimental::filesystem::v1::path p = std::experimental::filesystem::v1::current_path();

	//find working directory
	std::vector<char> buf;
	buf.resize(buf.size() + MAX_PATH);
	GetModuleFileName(NULL, &buf.at(0), buf.size());
	buf.shrink_to_fit();
	return std::string(buf.begin(), buf.end()).substr(0, std::string(buf.begin(), buf.end()).find_last_of("\\/"));
}

size_t pxpk::getFileSize(std::string input)
{
	return std::experimental::filesystem::v1::file_size(std::experimental::filesystem::v1::path(input));
}
