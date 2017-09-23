#ifndef TRI_FLOAT_H
#define TRI_FLOAT_H

class TriFloat
{
	protected:
		float data[3] = { 0 };

	private:
		TriFloat add(float input[3]);
		TriFloat subtract(float input[3]);

	public:
		TriFloat();
		TriFloat(float val);
		TriFloat(float val[3]);


		~TriFloat();

		TriFloat& operator+(TriFloat& input);
		TriFloat& operator+(float input[3]);
		TriFloat& operator+(float input);

		TriFloat& operator-(TriFloat& input);
		TriFloat& operator-(float input[3]);
		TriFloat& operator-(float input);
};

#endif // !TRI_FLOAT_H