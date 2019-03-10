#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include "ObjectResource.h"
#include "../../dependencies/glm/vec3.hpp"
#include "../../dependencies/glm/mat4x4.hpp"

namespace pxpk 
{
	class ShaderObject : public ObjectResource
	{
	private:
		GLuint programID;

		GLuint vertShaderID;
		GLuint fragShaderID;

	public:
		ShaderObject(std::string);

		void use();

		void setBool(const std::string&, bool);
		void setInt(const std::string&, int);
		void setFloat(const std::string&, float);
		void setVec3(const std::string&, glm::vec3);
		void setMat4(const std::string&, glm::mat4);
	};
}

#endif // !SHADER_OBJECT_H
