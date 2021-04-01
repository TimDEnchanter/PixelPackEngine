#define STB_TRUETYPE_IMPLEMENTATION

#include "fontObject.h"

void pxpk::FontObject::bindResource()
{
	//enable font bitmap
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fontId);

	//enable use of vertex attribute
	glEnableVertexAttribArray(pxpk::vertexAttributeID);
	//set focus to this object's vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, charVertId);
	//point attribute to buffer
	glVertexAttribPointer(
		pxpk::vertexAttributeID,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	//enable use of UV attribute
	glEnableVertexAttribArray(pxpk::UVAttributeID);
	//set focus to this object's UV buffer
	glBindBuffer(GL_ARRAY_BUFFER, charUVId);
	//point attribute to buffer
	glVertexAttribPointer(
		pxpk::UVAttributeID,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
}

void pxpk::FontObject::freeResource()
{
	glDisableVertexAttribArray(pxpk::vertexAttributeID);
}

void pxpk::FontObject::deleteResource()
{
	glDeleteTextures(1, &fontId);

	GLuint buffers[3] = { charVertId, charUVId, charIndexId };
	glDeleteBuffers(3, buffers);
}

pxpk::FontObject::FontObject(std::string filepath) : ObjectResource(filepath)
{
	LOG("Reading texture file: " + filepath, pxpk::INFO_LOG);

	//find working directory
	std::string fullPath = pxpk::getWorkingDir() + filepath;
	size_t fileSize = pxpk::getFileSize(fullPath);

	//read file into bitmap
	//unsigned char buffer[1 << 25];
	//unsigned char bitmap[1024 * 1024];
	std::vector<unsigned char> buffer(fileSize);
	std::vector<unsigned char> bitmap(1024*1024);
	FILE * inFile;
	fopen_s(&inFile, fullPath.c_str(), "rb");
	fread(&buffer[0], 1, fileSize, inFile);

	stbtt_pack_context context;
	if (!stbtt_PackBegin(&context, &bitmap[0], 1024, 1024, 0, 1, nullptr))
	{
		LOG("ERROR: Failed to initialize font", pxpk::ERROR_LOG);
		return;
	}
	stbtt_PackSetOversampling(&context, 2, 2);
	if (!stbtt_PackFontRange(&context, &buffer[0], 0, PXPK_FONT_DEFAULT_HEIGHT, 32, 96, charData))
	{
		LOG("ERROR: Failed to pack font", pxpk::ERROR_LOG);
		return;
	}
	stbtt_PackEnd(&context);

	//free(buffer);

	//load bitmap into openGL
	glGenTextures(1, &fontId);
	glBindTexture(GL_TEXTURE_2D, fontId);
	//wrapping params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//filtering params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 1024, 1024, 0, GL_ALPHA, GL_UNSIGNED_BYTE, &bitmap[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	//free(bitmap);

	//create character data buffers (empty data)
	//vertex buffer
	GLfloat vert[4][2];
	glGenBuffers(1, &charVertId);
	glBindBuffer(GL_ARRAY_BUFFER, charVertId);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(GLfloat) * 2 * 4,
		vert,
		GL_DYNAMIC_DRAW
	);
	//uv buffer
	GLfloat uv[4][2];
	glGenBuffers(1, &charUVId);
	glBindBuffer(GL_ARRAY_BUFFER, charUVId);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(GLfloat) * 2 * 4,
		uv,
		GL_DYNAMIC_DRAW
	);
	//index buffer
	GLuint index[6] = { 0,1,2, 0,2,3 };
	glGenBuffers(1, &charIndexId);
	glBindBuffer(GL_ARRAY_BUFFER, charIndexId);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(index),
		index,
		GL_STATIC_DRAW
	);
}

pxpk::FontObject::~FontObject()
{
	deleteResource();
}

GLuint pxpk::FontObject::getFontId()
{
	return fontId;
}

GLuint pxpk::FontObject::getCharVertId()
{
	return charVertId;
}

GLuint pxpk::FontObject::getCharUVId()
{
	return charUVId;
}

GLuint pxpk::FontObject::getCharIndexId()
{
	return charIndexId;
}

float pxpk::FontObject::updateCharQuad(float screenX, float screenY, char input, GLfloat pixelHeight)
{
	int charIdx = input - 32;
	//get quad data
	stbtt_aligned_quad newQuad;
	stbtt_GetPackedQuad(charData, 1024, 1024, charIdx, &screenX, &screenY, &newQuad, 1);

	//scale character to desired pixel size
	GLfloat scale = pixelHeight / PXPK_FONT_DEFAULT_HEIGHT;
	GLfloat targetWidth = (newQuad.x1 - newQuad.x0) * scale;
	GLfloat targetHeight = (newQuad.y1 - newQuad.y0) * scale;
	GLfloat targetYoffset = newQuad.y0 * scale;

	newQuad.x1 = newQuad.x0 + targetWidth;
	newQuad.y0 = screenY + targetYoffset;
	newQuad.y1 = newQuad.y0 + targetHeight;

	GLfloat vert[4][2] = 
	{
		{ newQuad.x0, newQuad.y1 },
		{ newQuad.x1, newQuad.y1 },
		{ newQuad.x1, newQuad.y0 },
		{ newQuad.x0, newQuad.y0 }
	};
	GLfloat uv[4][2] =
	{
		{ newQuad.s0, newQuad.t1 },
		{ newQuad.s1, newQuad.t1 },
		{ newQuad.s1, newQuad.t0 },
		{ newQuad.s0, newQuad.t0 } 
	};

	//update buffers in OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, charVertId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vert), vert);

	glBindBuffer(GL_ARRAY_BUFFER, charUVId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uv), uv);

	//return the advance offset for next character
	return charData[charIdx].xadvance * scale;
}
