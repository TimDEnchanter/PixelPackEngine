#include "QuadFloat.h"



QuadFloat & QuadFloat::add(float input[4])
{
	for (int i : data)
		data[i] += input[i];

	return *this;
}

QuadFloat & QuadFloat::subtract(float input[4])
{
	for (int i : data)
		data[i] -= input[i];

	return *this;
}

QuadFloat::QuadFloat()
{
}

QuadFloat::QuadFloat(float val)
{
	for (int i : data)
		data[i] = val;
}

QuadFloat::QuadFloat(float val[4])
{
	for (int i : data)
		data[i] = val[i];
}


QuadFloat::~QuadFloat()
{
}

TriFloat QuadFloat::asTri()
{
	return TriFloat(data);
}

QuadFloat & QuadFloat::operator+(QuadFloat & input)
{
	return add(input.data);
}

QuadFloat & QuadFloat::operator+(float input[4])
{
	return add(input);
}

QuadFloat & QuadFloat::operator+(float input)
{
	float temp[4] = { input };
	return add(temp);
}

QuadFloat & QuadFloat::operator-(QuadFloat & input)
{
	return subtract(input.data);
}

QuadFloat & QuadFloat::operator-(float input[4])
{
	return subtract(input);
}

QuadFloat & QuadFloat::operator-(float input)
{
	float temp[4] = { input };
	return subtract(temp);
}

std::ostream & operator<<(std::ostream & out, const QuadFloat & rhs)
{
	return out << "[" 
		<< rhs.data[0] << ", " 
		<< rhs.data[1] << ", " 
		<< rhs.data[2] << ", " 
		<< rhs.data[3] 
		<< "]";
}
