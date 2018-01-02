#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include "SharedVariables.h"
#include "../utility/debugging/Logger.h"
#include "../dependencies/glm/vec3.hpp"
#include "../dependencies/glm/mat4x4.hpp"
#include "../dependencies/glm/gtc/quaternion.hpp"
#include "../dependencies/glm/gtc/matrix_transform.hpp"

#include <vector>

/*
RenderObject

the 3D object rendered in OpeGL. 
Usually part of another object
*/

class RenderObject
{
	protected:
		bool isHidden = false;                          //determines if object is rendered
		glm::vec3 position = glm::vec3();                        //location of origin point
		glm::quat orientation = glm::quat(0.0, 0.0, 0.0, 1.0);                  //orientation in quaternions
		glm::vec3 scale = glm::vec3(1.0);

	private:
		GLenum drawMode = GL_TRIANGLES;                        //draw mode of vertices
		glm::vec3 objColor = glm::vec3(0.5);                         //base color of the object if no texture

		//vertex and index data for indexed VBO
		std::vector<GLfloat> vertexVector;
		std::vector<GLuint> indexVector;
		std::vector<GLfloat> colorVector;

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
		glm::vec3 getObjColor();
		glm::vec3 getPosition();
		glm::vec3 getOrientation();
		glm::vec3 getScale();
		std::vector<GLfloat> getVertexVector();
		std::vector<GLuint> getIndexVector();
		std::vector<GLfloat> getColorVector();

		glm::mat4 getModelMatrix();

		void setIsHidden(bool);
		void setDrawMode(GLenum);
		void setObjColor(glm::vec3);
		void setPosition(glm::vec3);
		void setOrientation(glm::vec3);
		void setScale(glm::vec3);
		void setVertexVector(std::vector<GLfloat>);
		void setIndexVector(std::vector<GLuint>);
		void setColorVector(std::vector<GLfloat>);

		void loadOBJ(std::string);
		virtual void init();
		virtual void draw();
};

#endif // !RENDER_OBJECT_H