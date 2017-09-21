#ifndef COLOR_H
#define COLOR_H

#include <vector>

class Color
{
	private:
		float red;
		float green;
		float blue;
		float alpha;

	public:
		Color();
		~Color();

		float getRed()               { return red; }
		float getGreen()             { return green; }
		float getBlue()              { return blue; }
		float getAlpha()             { return alpha; }

		std::vector<float> getRGB()  { return std::vector<float> {red, green, blue}; }
		std::vector<float> getRGBA() { return std::vector<float> {red, green, blue, alpha}; }

};

#endif // !COLOR_H