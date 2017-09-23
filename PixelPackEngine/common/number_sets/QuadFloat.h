#ifndef QUAD_FLOAT_H
#define QUAD_FLOAT_H

#include <iostream>
#include "TriFloat.h";

class QuadFloat
{
	protected:
		float data[4] = { 0 };

	private:
		QuadFloat& add(float input[4]);
		QuadFloat& subtract(float input[4]);

	public:
		QuadFloat();
		QuadFloat(float val);
		QuadFloat(float val[4]);

		~QuadFloat();

		TriFloat asTri();

		friend std::ostream& operator<<(std::ostream& out, const QuadFloat& rhs);

		QuadFloat& operator+(QuadFloat& input);
		QuadFloat& operator+(float input[4]);
		QuadFloat& operator+(float input);

		QuadFloat& operator-(QuadFloat& input);
		QuadFloat& operator-(float input[4]);
		QuadFloat& operator-(float input);
};

#endif // !QUAD_FLOAT_H