#include "TriFloat.h"



TriFloat & TriFloat::add(float input[3])
{
	for (int i : data)
		data[i] += input[i];

	return *this;
}

TriFloat & TriFloat::subtract(float input[3])
{
	for (int i : data)
		data[i] -= input[i];

	return *this;
}

TriFloat::TriFloat()
{
}

TriFloat::TriFloat(float val)
{
	for (int i : data)
		data[i] = val;
}

TriFloat::TriFloat(float val[3])
{
	for (int i : data)
		data[i] = val[i];
}


TriFloat::~TriFloat()
{
}

TriFloat & TriFloat::operator+(TriFloat & input)
{
	return add(input.data);
}

TriFloat & TriFloat::operator+(float input[3])
{
	return add(input);
}

TriFloat & TriFloat::operator+(float input)
{
	float temp[3] = { input };
	return add(temp);
}

TriFloat & TriFloat::operator-(TriFloat & input)
{
	return subtract(input.data);
}

TriFloat & TriFloat::operator-(float input[3])
{
	return subtract(input);
}

TriFloat & TriFloat::operator-(float input)
{
	float temp[3] = { input };
	return add(temp);
}

std::ostream & operator<<(std::ostream & out, const TriFloat & rhs)
{
	return out << "["
		<< rhs.data[0] << ", "
		<< rhs.data[1] << ", "
		<< rhs.data[2]
		<< "]";
}
