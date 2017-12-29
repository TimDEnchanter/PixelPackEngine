#ifndef CAMERA_H
#define CAMERA_H

#include "SharedVariables.h"
#include "RenderObject.h"

class Camera : public RenderObject
{
	private:
		GLfloat fov;


	public:
		virtual void init();
		virtual void draw();

		glm::mat4 getProjectionMatrix();
		glm::mat4 getViewMatrix();
};

#endif  //  !CAMERA_H