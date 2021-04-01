#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include "Light.h"

namespace pxpk
{
	class SpotLight : public pxpk::Light
	{
	private:
		glm::vec3 direction = glm::vec3(0.0, -1.0, 0.0);

		float innerCutoff = glm::cos(glm::radians(12.5f));
		float outerCutoff = glm::cos(glm::radians(15.0f));

		float constant = 1.0f;
		float linear = 0.022f;
		float quadratic = 0.0019f;

	public:
		glm::vec3 getDirection();
		float getInnerCutoff();
		float getOuterCutoff();
		float getConstant();
		float getLinear();
		float getQuadratic();

		void setDirection(glm::vec3);
		void setInnerCutoff(float);
		void setOuterCutoff(float);
		void setConstant(float);
		void setLinear(float);
		void setQuadratic(float);

		virtual void draw(int);
	};
}

#endif // !SPOT_LIGHT_H

