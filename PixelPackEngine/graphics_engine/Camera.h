#ifndef CAMERA_H
#define CAMERA_H

#include "SharedVariables.h"
#include "RenderObject.h"

class Camera : public RenderObject
{
	private:
		GLfloat fov;
		GLfloat nearDist;
		GLfloat farDist;


	public:
		GLfloat getFov();
		GLfloat getNearDist();
		GLfloat getFarDist();

		void setFov(GLfloat);
		void setNearDist(GLfloat);
		void setFarDist(GLfloat);

		virtual void lookAt(glm::vec3, glm::vec3);
		virtual void init();
		virtual void draw();

		glm::mat4 getProjectionMatrix();
		glm::mat4 getViewMatrix();
};

#endif  //  !CAMERA_H