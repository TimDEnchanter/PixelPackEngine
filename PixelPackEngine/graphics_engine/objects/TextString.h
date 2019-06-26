#ifndef TEXT_STRING_H
#define TEXT_STRING_H

#include <string>
#include "ScreenSpaceElement.h"

namespace pxpk
{
	class TextString : public pxpk::ScreenSpaceElement
	{
	private:
		std::shared_ptr<pxpk::FontObject> fontPtr;

		std::string data = "[PLACEHOLDER TEXT]";
		glm::vec3 textColor = glm::vec3(1.0);

	public:
		std::shared_ptr<pxpk::FontObject> getFontPtr();
		std::string getdata();
		glm::vec3 getTextColor();

		void setFontPtr(std::shared_ptr<pxpk::FontObject>);
		void setData(std::string);
		void setTextColor(glm::vec3);

		void draw();
	};
}

#endif // !TEXT_STRING_H