#include "RenderObject.h"

void pxpk::RenderObject::initVertexBuffer()
{
	LOG("initializing vertex buffer", pxpk::INFO_LOG);
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertexVector.size() * sizeof(GLfloat),
		&vertexVector.front(),
		GL_STATIC_DRAW
	);
}

void pxpk::RenderObject::initElementuffer()
{
	glGenBuffers(1, &elementBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indexVector.size() * sizeof(GLuint),
		&indexVector.front(),
		GL_STATIC_DRAW
	);
}

void pxpk::RenderObject::initColorBuffer()
{
	glGenBuffers(1, &colorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
	glBufferData(
		GL_ARRAY_BUFFER,
		colorVector.size() * sizeof(GLfloat),
		&colorVector.front(),
		GL_STATIC_DRAW
	);
}

glm::quat pxpk::RenderObject::quatToVector(glm::vec3 start, glm::vec3 dest)
{
	start = glm::normalize(start);
	dest = glm::normalize(dest);

	float cosTheta = glm::dot(start, dest);
	glm::vec3 rotationAxis;
	if (cosTheta < -1 + 0.001f)
	{
		//rotate 180 around any perpendicular vector
		rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (std::powf(rotationAxis.length(), 2.0f) < 0.01f) //check if arbitrary axis is parallel
			rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = glm::normalize(rotationAxis);
		return glm::angleAxis(glm::radians(180.0f), rotationAxis);
	}

	rotationAxis = glm::cross(start, dest);
	float s = sqrt((1.0f + cosTheta) * 2.0f);
	float invs = 1.0f / s;

	return glm::quat(
		s*0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
}

pxpk::RenderObject::RenderObject()
{
}

pxpk::RenderObject::RenderObject(const RenderObject & input)
{
	position = input.position;
	orientation = input.orientation;
	scale = input.scale;
	drawMode = input.drawMode;
	objColor = input.objColor;
	vertexVector = input.vertexVector;
	indexVector = input.indexVector;
	colorVector = input.colorVector;
	vertexBufferID = input.vertexBufferID;
	elementBufferID = input.elementBufferID;
	colorBufferID = input.colorBufferID;
}


pxpk::RenderObject::~RenderObject()
{
}

GLenum pxpk::RenderObject::getDrawMode()
{
	return drawMode;
}

glm::vec3 pxpk::RenderObject::getObjColor()
{
	return objColor;
}

glm::vec3 pxpk::RenderObject::getPosition()
{
	return position;
}

glm::quat pxpk::RenderObject::getOrientaion()
{
	return orientation;
}

glm::vec3 pxpk::RenderObject::getOrientationEuler()
{
	return glm::eulerAngles(orientation);
}

glm::vec3 pxpk::RenderObject::getScale()
{
	return scale;
}

std::vector<GLfloat> pxpk::RenderObject::getVertexVector()
{
	return vertexVector;
}

std::vector<GLuint> pxpk::RenderObject::getIndexVector()
{
	return indexVector;
}

std::vector<GLfloat> pxpk::RenderObject::getColorVector()
{
	return colorVector;
}

glm::mat4 pxpk::RenderObject::getModelMatrix()
{
	// create translation matrix
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0), position);

	//create scale matrix
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), scale);

	//create rotation matrix
	glm::mat4 rotationMatrix = glm::mat4_cast(orientation);

	//multiply together **ORDER IS IMPORTANT**
	glm::mat4 modelOut =  translationMatrix * rotationMatrix * scaleMatrix;
	return modelOut;
}

void pxpk::RenderObject::setDrawMode(GLenum input)
{
	drawMode = input;
}

void pxpk::RenderObject::setObjColor(glm::vec3 input)
{
	objColor = input;

	size_t numVerts = vertexVector.size() / 3;
	colorVector.clear();
	for (int i = 0; i < numVerts; i++) colorVector.insert(colorVector.end(), { objColor.x, objColor.y, objColor.z });
	initColorBuffer();
}

void pxpk::RenderObject::setPosition(glm::vec3 input)
{
	position = input;
}

void pxpk::RenderObject::setOrientation(glm::quat input)
{
	orientation = input;
}

void pxpk::RenderObject::setOrientationEuler(glm::vec3 eulerInput)
{
	orientation = glm::quat(eulerInput);
}

void pxpk::RenderObject::setScale(glm::vec3 input)
{
	scale = input;
}

void pxpk::RenderObject::setVertexVector(std::vector<GLfloat> input)
{
	vertexVector = input;

	initVertexBuffer();
}

void pxpk::RenderObject::setIndexVector(std::vector<GLuint> input)
{
	indexVector = input;

	initElementuffer();
}

void pxpk::RenderObject::setColorVector(std::vector<GLfloat> input)
{
	colorVector = input;

	initColorBuffer();
}


void pxpk::RenderObject::freeVertexVector()
{
	glDeleteBuffers(1, &vertexBufferID);
	vertexVector.clear();
}

void pxpk::RenderObject::freeIndexVector()
{
	glDeleteBuffers(1, &elementBufferID);
	indexVector.clear();
}

void pxpk::RenderObject::freeColorVector()
{
	glDeleteBuffers(1, &colorBufferID);
	colorVector.clear();
}

void pxpk::RenderObject::translate(glm::vec3 input)
{
	position += input;
}

void pxpk::RenderObject::rotateEuler(glm::vec3 input)
{
	glm::quat inQuat = glm::quat(input);
	orientation = inQuat * orientation;
}

void pxpk::RenderObject::rotate(glm::quat input)
{
	orientation = input * orientation;
}

void pxpk::RenderObject::lookAt(glm::vec3 target)
{
	glm::vec3 targetVector = target - position;
	glm::quat rotation = quatToVector(glm::vec3(0.0f,0.0f,1.0f), targetVector);

	glm::vec3 targetUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::cross(targetVector, targetUp);
	targetUp = glm::cross(right, targetVector);
	glm::vec3 newUp = rotation * glm::vec3(0.0f, 1.0f, 0.0f);

	glm::quat upRotation = quatToVector(newUp, targetUp);

	//ORDER IMPORTANT
	orientation = upRotation * rotation;
}

void pxpk::RenderObject::draw()
{	
	//determine sides from drawMode
	GLint sides = 3; //default to triangles
	//if (drawMode == GL_QUADS) sides = 4;

	//enable use of vertex attribute
	glEnableVertexAttribArray(pxpk::vertexAttributeID);

	//set focus to this object's vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	//point attribute to buffer
	glVertexAttribPointer(
		pxpk::vertexAttributeID,
		sides,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);


	//enable use of color attribute
	glEnableVertexAttribArray(pxpk::colorAttributeID);

	//set focus to this object's color buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);

	//point to buffer
	glVertexAttribPointer(
		pxpk::colorAttributeID,
		sides,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	
	//set focus to element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);

	//draw the object
	glDrawElements(
		drawMode,
		indexVector.size(),
		GL_UNSIGNED_INT,
		(void*)0
	);
	

	//disable use of vertex attribute
	glDisableVertexAttribArray(pxpk::vertexAttributeID);
}