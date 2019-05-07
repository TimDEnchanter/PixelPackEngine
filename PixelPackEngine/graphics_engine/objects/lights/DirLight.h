#ifndef DIR_LIGHT_H
#define DIR_LIGHT_H

#include "Light.h"

namespace pxpk
{
	class DirLight : public pxpk::Light
	{
	public:
		virtual void draw(int);
	};
}

#endif  // !DIR_LIGHT_H