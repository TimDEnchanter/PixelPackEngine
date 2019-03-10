#include "ShaderObject.h"

pxpk::ShaderObject::ShaderObject(std::string input) : pxpk::ObjectResource(input)
{
	//create shaders
	vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//find working directory
	char buf[1000];
	GetModuleFileName(NULL, buf, 1000);
	std::string dir = std::string(buf).substr(0, std::string(buf).find_last_of("\\/"));
	LOG("shader directory: " + dir, pxpk::ERROR_LOG);

	//separate input into filepaths
	int delim = input.find_first_of('|');
	std::string vertFile = input.substr(0, delim);
	std::string fragFile = input.substr(delim + 1, input.size());

	// read vertex shader from file
	std::string vertCode;
	std::ifstream vertStream(dir + vertFile, std::ios::in);
	if (vertStream.is_open())
	{
		vertCode = std::string(std::istreambuf_iterator<char>(vertStream), std::istreambuf_iterator<char>());
		vertStream.close();
		//pxpk::Logger::getInstance().log(vertCode, pxpk::ERROR_LOG);
	}
	else
	{
		LOG("unable to open the vertex shader file", pxpk::ERROR_LOG);
	}

	// read fragment shader from file
	std::string fragCode;
	std::ifstream fragStream(dir + fragFile, std::ios::in);
	if (fragStream.is_open())
	{
		fragCode = std::string(std::istreambuf_iterator<char>(fragStream), std::istreambuf_iterator<char>());
		fragStream.close();
		//pxpk::Logger::getInstance().log(fragCode, pxpk::ERROR_LOG);
	}
	else
	{
		LOG("unable to open the fragment shader file", pxpk::ERROR_LOG);
	}

	// compile vertex shader
	const char* vertPointer = vertCode.c_str();
	glShaderSource(vertShaderID, 1, &vertPointer, NULL);
	glCompileShader(vertShaderID);
	GLint vertStatus;
	glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &vertStatus);
	if (vertStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(vertShaderID, infoLogLength, NULL, strInfoLog);

		std::string logStr = strInfoLog;

		LOG("Compile failure in vertex shader: " + logStr, pxpk::ERROR_LOG);
		delete[] strInfoLog;
	}

	// compile fragment shader
	const char* fragPointer = fragCode.c_str();
	glShaderSource(fragShaderID, 1, &fragPointer, NULL);
	glCompileShader(fragShaderID);
	GLint fragStatus;
	glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &fragStatus);
	if (fragStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(fragShaderID, infoLogLength, NULL, strInfoLog);

		std::string logStr = strInfoLog;

		LOG("Compile failure in fragment shader: " + logStr, pxpk::ERROR_LOG);
		delete[] strInfoLog;
	}

	// add shaders to program
	programID = glCreateProgram();
	glAttachShader(programID, vertShaderID);
	glAttachShader(programID, fragShaderID);

	// link program to GL
	glLinkProgram(programID);
	GLint status;
	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(programID, infoLogLength, NULL, strInfoLog);
		std::string logStr = strInfoLog;
		LOG("Linker error: " + logStr, pxpk::ERROR_LOG);
		delete[] strInfoLog;
	}

	glDetachShader(programID, vertShaderID);
	glDetachShader(programID, fragShaderID);

	glDeleteShader(vertShaderID);
	glDeleteShader(fragShaderID);

	LOG("Shaders Loaded", pxpk::INFO_LOG);
}

void pxpk::ShaderObject::use()
{
	glUseProgram(programID);
	LOG_GL();
}

void pxpk::ShaderObject::setBool(const std::string &name, bool input)
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)input);
	LOG_GL();
}

void pxpk::ShaderObject::setInt(const std::string &name, int input)
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), input);
	LOG_GL();
}

void pxpk::ShaderObject::setFloat(const std::string &name, float input)
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), input);
	LOG_GL();
}

void pxpk::ShaderObject::setVec3(const std::string &name, glm::vec3 input)
{
	glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &input[0]);
	LOG_GL();
}

void pxpk::ShaderObject::setMat4(const std::string &name, glm::mat4 input)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &input[0][0]);
	LOG_GL();
}
