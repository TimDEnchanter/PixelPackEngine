#include "Model.h"

GLenum pxpk::Model::getDrawMode()
{
	return drawMode;
}

glm::vec3 pxpk::Model::getBaseColor()
{
	return baseColor;
}

std::shared_ptr<pxpk::MeshObject> pxpk::Model::getMeshPtr()
{
	return meshPtr;
}

std::shared_ptr<pxpk::TextureObject> pxpk::Model::getTexturePtr()
{
	return texPtr;
}

std::shared_ptr<pxpk::ShaderObject> pxpk::Model::getShaderPtr()
{
	return shaderPtr;
}

void pxpk::Model::setDrawMode(GLenum input)
{
	drawMode = input;
}

void pxpk::Model::setBaseColor(glm::vec3 input)
{
	baseColor = input;
}

void pxpk::Model::setMeshPtr(std::shared_ptr<pxpk::MeshObject> input)
{
	meshPtr = input;
}

void pxpk::Model::setTexturePtr(std::shared_ptr<pxpk::TextureObject> input)
{
	texPtr = input;
}

void pxpk::Model::setShaderPtr(std::shared_ptr<pxpk::ShaderObject> input)
{
	shaderPtr = input;
}

void pxpk::Model::draw()
{
	//set model matrix
	shaderPtr->setMat4("Model", getModelMatrix());

	//set base color
	shaderPtr->setVec3("objColor", baseColor);

	//bind mesh data
	meshPtr->bindResource();

	//bind texture data
	//texPtr->bindResource();

	//set focus to element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshPtr->getIndexID());

	//draw the object
	glDrawElements(
		drawMode,
		meshPtr->getIndexSize(),
		GL_UNSIGNED_INT,
		(void*)0
	);

	LOG_GL();

	//done with object, release
	meshPtr->freeResource();
}
