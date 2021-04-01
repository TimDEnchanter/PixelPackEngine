#ifndef FONT_OBJECT_H
#define FONT_OBJECT_H

#define PXPK_FONT_DEFAULT_HEIGHT 15.0

#include "../../dependencies/stb/stb_truetype.h"
#include "ObjectResource.h"

namespace pxpk
{
	class FontObject : public ObjectResource
	{
	private:
		GLuint fontId = -1;
		GLuint charVertId = -1;
		GLuint charUVId = -1;
		GLuint charIndexId = -1;

		stbtt_packedchar charData[96];
	public:
		virtual void bindResource();
		virtual void freeResource();
		virtual void deleteResource();

		FontObject(std::string);
		virtual ~FontObject();

		GLuint getFontId();
		GLuint getCharVertId();
		GLuint getCharUVId();
		GLuint getCharIndexId();

		float updateCharQuad(float, float, char, float);
	};
}

#endif // !FONT_OBJECT_H
