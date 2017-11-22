#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include "SharedVariables.h"
#include "../common/Color.h"

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
		std::vector<TriFloat> vertexVector;
		std::vector<GLuint> indexVector;

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
		std::vector<TriFloat> getVertexVector();
		std::vector<GLuint> getIndexVector();

		void setIsHidden(bool);
		void setDrawMode(GLenum);
		void setObjColor(Color);
		void setVertexVector(std::vector<TriFloat>);
		void setIndexVector(std::vector<GLuint>);

		void init();
		void draw();
};

#endif // !RENDER_OBJECT_H