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

		glm::vec3 lookPoint;

	public:
		GLfloat getFov();
		GLfloat getNearDist();
		GLfloat getFarDist();
		glm::vec3 getLookPoint();

		void setFov(GLfloat);
		void setNearDist(GLfloat);
		void setFarDist(GLfloat);
		void setLookPoint(glm::vec3);

		virtual void init();
		virtual void draw();

		glm::mat4 getProjectionMatrix();
		glm::mat4 getViewMatrix();
};

#endif  //  !CAMERA_H