#ifndef INPUTS_H
#define INPUTS_H

#include <unordered_map>

#include "../dependencies/freeGLUT/include/GL/freeglut.h"

namespace pxpk
{
	class InputsPC
	{
	private:
		int mouseX;
		int mouseY;

		std::unordered_map<unsigned char, bool> keys;

		InputsPC();

	public:
		static InputsPC & getInstance()
		{
			static InputsPC instance;
			return instance;
		}

		InputsPC(InputsPC const &) = delete;
		void operator=(InputsPC const &) = delete;

		void initInput();

		void mouseFunc(int, int);
		void keyFunc(unsigned char);
		void keyUpFunc(unsigned char);

		int getmouseX();
		int getmouseY();

		bool isKeyPressed(unsigned char);
	};

	static void mouseCallback(int, int);
	static void keyboardCallback(unsigned char, int, int);
	static void keyboardUpCallback(unsigned char, int, int);

	namespace
	{
		InputsPC * inputsPCInstance = NULL;
	}
}

#endif  // !INPUTS_H