#ifndef MESH_OBJECT_H
#define MESH_OBJECT_H

#include <algorithm>
#include "ObjectResource.h"
#include "..\dependencies\glm\vec3.hpp"
#include "..\dependencies\glm\vec2.hpp"

namespace pxpk
{
	class MeshObject : public ObjectResource
	{
	private:
		GLuint vertexID;
		GLuint colorID;
		GLuint normalID;
		GLuint uvID;
		GLuint indexID;

	public:
		void createResource(
			std::vector<GLfloat>, 
			std::vector<GLfloat>, 
			std::vector<GLfloat>, 
			std::vector<GLfloat>, 
			std::vector<GLuint>);

		virtual void bindResource();
		virtual void freeResource();
		virtual void deleteResource();

		MeshObject(std::string);
		virtual ~MeshObject();
	};
}

#endif //MESH_OBJECT_H
