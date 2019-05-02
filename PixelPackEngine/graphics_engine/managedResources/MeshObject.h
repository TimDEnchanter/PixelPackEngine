#ifndef MESH_OBJECT_H
#define MESH_OBJECT_H

#include <algorithm>
#include <unordered_map>

#include "ObjectResource.h"
#include "..\..\dependencies\glm\vec3.hpp"
#include "..\..\dependencies\glm\vec2.hpp"
#include "..\..\dependencies\tiny_OBJ_loader\tiny_obj_loader.h"

namespace pxpk
{
	class MeshObject : public ObjectResource
	{
	private:
		GLuint vertexID = -1;
		GLuint normalID = -1;
		GLuint uvID = -1;
		GLuint indexID = -1;

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

		void readOBJ(std::string);
		void readPXO(std::string);
	};
}

#endif //MESH_OBJECT_H
