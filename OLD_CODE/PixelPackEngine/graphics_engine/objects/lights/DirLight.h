#ifndef DIR_LIGHT_H
#define DIR_LIGHT_H

#include "Light.h"

namespace pxpk
{
	class DirLight : public pxpk::Light
	{
	private:
		glm::vec3 direction = glm::vec3(0.0, -1.0, 0.0);
	public:
		glm::vec3 getDirection();

		void setDirection(glm::vec3);

		virtual void draw(int);
	};
}

#endif  // !DIR_LIGHT_H