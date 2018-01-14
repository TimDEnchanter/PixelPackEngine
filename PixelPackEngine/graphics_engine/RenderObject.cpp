#include "RenderObject.h"



void RenderObject::initBuffers()
{
	
	Logger::getInstance().log("loading buffers", LogLevel::info);
	
	//setup vertex buffer
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertexVector.size() * sizeof(GLfloat),
		&vertexVector.front(),
		GL_STATIC_DRAW
	);

	//setup color buffer
	glGenBuffers(1, &colorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
	glBufferData(
		GL_ARRAY_BUFFER,
		colorVector.size() * sizeof(GLfloat),
		&colorVector.front(),
		GL_STATIC_DRAW
	);
	
	//setup element buffer
	glGenBuffers(1, &elementBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indexVector.size() * sizeof(GLuint),
		&indexVector.front(),
		GL_STATIC_DRAW
	);
}

glm::quat RenderObject::quatToVector(glm::vec3 start, glm::vec3 dest)
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

RenderObject::RenderObject()
{
}

RenderObject::RenderObject(const RenderObject & input)
{
	isHidden = input.isHidden;
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
	normalBufferID = input.normalBufferID;
	colorBufferID = input.colorBufferID;
}


RenderObject::~RenderObject()
{
}

bool RenderObject::getIsHidden()
{
	return isHidden;
}

GLenum RenderObject::getDrawMode()
{
	return drawMode;
}

glm::vec3 RenderObject::getObjColor()
{
	return objColor;
}

glm::vec3 RenderObject::getPosition()
{
	return position;
}

glm::quat RenderObject::getOrientaion()
{
	return orientation;
}

glm::vec3 RenderObject::getOrientationEuler()
{
	return glm::eulerAngles(orientation);
}

glm::vec3 RenderObject::getScale()
{
	return scale;
}

std::vector<GLfloat> RenderObject::getVertexVector()
{
	return vertexVector;
}

std::vector<GLuint> RenderObject::getIndexVector()
{
	return indexVector;
}

std::vector<GLfloat> RenderObject::getColorVector()
{
	return colorVector;
}

glm::mat4 RenderObject::getModelMatrix()
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

void RenderObject::setIsHidden(bool input)
{
	isHidden = input;
}

void RenderObject::setDrawMode(GLenum input)
{
	drawMode = input;
}

void RenderObject::setObjColor(glm::vec3 input)
{
	objColor = input;
}

void RenderObject::setPosition(glm::vec3 input)
{
	position = input;
}

void RenderObject::setOrientation(glm::quat input)
{
	orientation = input;
}

void RenderObject::setOrientationEuler(glm::vec3 eulerInput)
{
	orientation = glm::quat(eulerInput);
}

void RenderObject::setScale(glm::vec3 input)
{
	scale = input;
}

void RenderObject::setVertexVector(std::vector<GLfloat> input)
{
	vertexVector = input;
}

void RenderObject::setIndexVector(std::vector<GLuint> input)
{
	indexVector = input;
}

void RenderObject::setColorVector(std::vector<GLfloat> input)
{
	colorVector = input;
}


void RenderObject::translate(glm::vec3 input)
{
	position += input;
}

void RenderObject::rotateEuler(glm::vec3 input)
{
	glm::quat inQuat = glm::quat(input);
	orientation = inQuat * orientation;
}

void RenderObject::rotate(glm::quat input)
{
	orientation = input * orientation;
}

void RenderObject::lookAt(glm::vec3 target)
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
	//Logger::getInstance().log("targetOrientation: " + std::to_string(orientation.x) + "    " + std::to_string(orientation.y) + "    " + std::to_string(orientation.z) + "    " + std::to_string(orientation.w));
}

void RenderObject::loadOBJ(std::string filePath)
{
}

void RenderObject::init()
{
	if (colorVector.empty()) {
		int numVerts = vertexVector.size() / 3;
		for (int i = 0; i < numVerts; i++) colorVector.insert(colorVector.end(), { objColor.x, objColor.y, objColor.z });
	}
	initBuffers();
}

void RenderObject::draw()
{
	//Logger::getInstance().log("drawing Object", LogLevel::info);
	
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