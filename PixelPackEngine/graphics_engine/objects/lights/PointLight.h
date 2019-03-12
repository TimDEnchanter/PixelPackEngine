#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"

namespace pxpk
{
	class PointLight : public pxpk::Light
	{
	private:
		//attenuation rate values
		float constant = 1.0f;
		float linear = 0.022f;
		float quadratic = 0.0019f;

	public:
		float getConstant();
		float getLinear();
		float getQuadratic();

		void setConstant(float);
		void setLinear(float);
		void setQuadratic(float);

		virtual void draw(int);
	};
}

#endif // !POINT_LIGHT_H
