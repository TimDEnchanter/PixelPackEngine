#ifndef COLOR_H
#define COLOR_H

#include "number_sets\QuadFloat.h"

class Color : QuadFloat
{
	private:


	public:
		Color();
		~Color();

		float getRed()     { return data[0]; }
		float getGreen()   { return data[1]; }
		float getBlue()    { return data[2]; }
		float getAlpha()   { return data[3]; }

		TriFloat getRGB()  { return asTri(); }
};

#endif // !COLOR_H