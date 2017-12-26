#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include "SharedVariables.h"
#include "../common/Color.h"
#include "../utility/debugging/Logger.h"
#include "../dependencies/glm/vec3.hpp"

#include <vector>

/*
RenderObject

the 3D object rendered in OpeGL. 
Usually part of another object
*/

class RenderObject
{
	private:
		bool isHidden;                          //determines if object is rendered
		GLenum drawMode;                        //draw mode of vertices
		Color objColor;                         //base color of the object if no texture
		TriFloat origin;                        //location of origin point
		QuadFloat orientation;                  //orientation in quaternions

		//vertex and index data for indexed VBO
		std::vector<glm::vec3> vertexVector;
		std::vector<GLuint> indexVector;

		GLuint vertexbuffer;

		//buffer IDs
		GLuint vertexBufferID;
		GLuint elementBufferID;
		GLuint normalBufferID;
		GLuint colorBufferID;

		void initBuffers();

	public:
		RenderObject();
		~RenderObject();

		bool getIsHidden();
		GLenum getDrawMode();
		Color getObjColor();
		std::vector<glm::vec3> getVertexVector();
		std::vector<GLuint> getIndexVector();

		void setIsHidden(bool);
		void setDrawMode(GLenum);
		void setObjColor(Color);
		void setVertexVector(std::vector<glm::vec3>);
		void setIndexVector(std::vector<GLuint>);

		void init();
		void draw();
};

#endif // !RENDER_OBJECT_H