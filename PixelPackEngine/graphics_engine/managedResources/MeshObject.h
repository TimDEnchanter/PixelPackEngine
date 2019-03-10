#ifndef MESH_OBJECT_H
#define MESH_OBJECT_H

#include <algorithm>
#include "ObjectResource.h"
#include "..\..\dependencies\glm\vec3.hpp"
#include "..\..\dependencies\glm\vec2.hpp"

namespace pxpk
{
	class MeshObject : public ObjectResource
	{
	private:
		GLuint vertexID;
		GLuint normalID;
		GLuint uvID;
		GLuint indexID;

		GLsizei indexSize;

	public:
		void createResource(
			std::vector<GLfloat>, 
			std::vector<GLfloat>, 
			std::vector<GLfloat>, 
			std::vector<GLuint>);

		virtual void bindResource();
		virtual void freeResource();
		virtual void deleteResource();

		MeshObject(std::string);
		virtual ~MeshObject();

		GLuint getVertexID();
		GLuint getNormalID();
		GLuint getUVID();
		GLuint getIndexID();
		GLsizei getIndexSize();
	};
}

#endif //MESH_OBJECT_H
