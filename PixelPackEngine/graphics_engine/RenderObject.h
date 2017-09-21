#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "../common/Color.h"

/*
RenderObject

the phyical 3d object rendered in OpeGL. 
Usually part of another object
*/

class RenderObject
{
	private:
		bool isHidden;
		GLenum drawMode;
		Color objColor;

	public:
		RenderObject();
		~RenderObject();

		void draw();
};

#endif // !RENDER_OBJECT_H