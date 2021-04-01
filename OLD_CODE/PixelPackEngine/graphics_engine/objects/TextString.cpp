#include "TextString.h"

std::shared_ptr<pxpk::FontObject> pxpk::TextString::getFontPtr()
{
	return fontPtr;
}

std::string pxpk::TextString::getdata()
{
	return data;
}

glm::vec3 pxpk::TextString::getTextColor()
{
	return textColor;
}

void pxpk::TextString::setFontPtr(std::shared_ptr<pxpk::FontObject> input)
{
	fontPtr = input;
}

void pxpk::TextString::setData(std::string input)
{
	data = input;
}

void pxpk::TextString::setTextColor(glm::vec3 input)
{
	textColor = input;
}

void pxpk::TextString::draw()
{
	//set current shader
	shaderPtr->use();

	//set uniforms
	shaderPtr->setVec3("textColor", textColor);

	//draw each character individually
	glm::vec2 cursor = position;
	for (char& c : data)
	{
		//give new data to OpenGL
		cursor.x += fontPtr->updateCharQuad(cursor.x, cursor.y, c, pixelSize.y);

		//bind font data
		fontPtr->bindResource();

		//set focus to element buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fontPtr->getCharIndexId());

		//draw the character
		glDrawElements(
			GL_TRIANGLES,
			sizeof(GLuint) * 6,
			GL_UNSIGNED_INT,
			(void*)0
		);
	}
}
