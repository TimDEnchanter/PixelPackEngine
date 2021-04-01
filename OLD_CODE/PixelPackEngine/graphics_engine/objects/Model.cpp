#include "Model.h"

GLenum pxpk::Model::getDrawMode()
{
	return drawMode;
}

glm::vec3 pxpk::Model::getAmbient()
{
	return ambient;
}

glm::vec3 pxpk::Model::getDiffuse()
{
	return diffuse;
}

glm::vec3 pxpk::Model::getSpecular()
{
	return specular;
}

float pxpk::Model::getShininess()
{
	return shininess;
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

void pxpk::Model::setAmbient(glm::vec3 input)
{
	ambient = input;
}

void pxpk::Model::setDiffuse(glm::vec3 input)
{
	diffuse = input;
}

void pxpk::Model::setSpecular(glm::vec3 input)
{
	specular = input;
}

void pxpk::Model::setShininess(float input)
{
	shininess = input;
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
	//set current shader
	shaderPtr->use();

	//set model matrix
	shaderPtr->setMat4("Model", getModelMatrix());

	//set material
	shaderPtr->setVec3("material.ambient", ambient);
	//shaderPtr->setVec3("material.diffuse", diffuse);
	shaderPtr->setVec3("material.specular", specular);
	shaderPtr->setFloat("material.shininess", shininess);

	//bind mesh data
	meshPtr->bindResource();

	//bind texture data
	texPtr->bindResource();

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
