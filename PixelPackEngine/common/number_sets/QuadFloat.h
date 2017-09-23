#ifndef QUAD_FLOAT_H
#define QUAD_FLOAT_H

#include "TriFloat.h";

class QuadFloat
{
	protected:
		float data[4] = { 0 };

	private:
		QuadFloat add(float input[4]);
		QuadFloat subtract(float input[4]);

	public:
		QuadFloat();
		~QuadFloat();

		TriFloat asTri();

		QuadFloat& operator+(QuadFloat& input);
		QuadFloat& operator+(float input[4]);
		QuadFloat& operator+(float input);

		QuadFloat& operator-(QuadFloat& input);
		QuadFloat& operator-(float input[4]);
		QuadFloat& operator-(float input);
};

#endif // !QUAD_FLOAT_H