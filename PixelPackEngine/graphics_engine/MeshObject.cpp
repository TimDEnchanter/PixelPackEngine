#include "MeshObject.h"

void pxpk::MeshObject::createResource(
	std::vector<GLfloat> vertexData, 
	std::vector<GLfloat> normalData, 
	std::vector<GLfloat> uvData, 
	std::vector<GLuint>  indexData)
{
	LOG("Creating model VBOs in OpenGL", pxpk::INFO_LOG);

	//create vertex buffer
	glGenBuffers(1, &vertexID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexID);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertexData.size() * sizeof(GLfloat),
		&vertexData.front(),
		GL_STATIC_DRAW
	);

	//create normal buffer
	glGenBuffers(1, &normalID);
	glBindBuffer(GL_ARRAY_BUFFER, normalID);
	glBufferData(
		GL_ARRAY_BUFFER,
		normalData.size() * sizeof(GLfloat),
		&normalData.front(),
		GL_STATIC_DRAW
	);

	//create UV buffer
	glGenBuffers(1, &uvID);
	glBindBuffer(GL_ARRAY_BUFFER, uvID);
	glBufferData(
		GL_ARRAY_BUFFER,
		uvData.size() * sizeof(GLfloat),
		&uvData.front(),
		GL_STATIC_DRAW
	);

	//create index buffer
	glGenBuffers(1, &indexID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indexData.size() * sizeof(GLuint),
		&indexData.front(),
		GL_STATIC_DRAW
	);

	//store size of index buffer
	indexSize = indexData.size();
}

void pxpk::MeshObject::bindResource()
{
	//enable use of vertex attribute
	glEnableVertexAttribArray(pxpk::vertexAttributeID);
	//set focus to this object's vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexID);
	//point attribute to buffer
	glVertexAttribPointer(
		pxpk::vertexAttributeID,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	//enable use of normal attribute
	glEnableVertexAttribArray(pxpk::normalAttributeID);
	//set focus to this object's normal buffer
	glBindBuffer(GL_ARRAY_BUFFER, normalID);
	//point attribute to buffer
	glVertexAttribPointer(
		pxpk::normalAttributeID,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	//enable use of UV attribute
	glEnableVertexAttribArray(pxpk::UVAttributeID);
	//set focus to this object's UV buffer
	glBindBuffer(GL_ARRAY_BUFFER, uvID);
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

void pxpk::MeshObject::freeResource()
{
	glDisableVertexAttribArray(pxpk::vertexAttributeID);
	glDisableVertexAttribArray(pxpk::normalAttributeID);
	glDisableVertexAttribArray(pxpk::UVAttributeID);
}

void pxpk::MeshObject::deleteResource()
{
	GLuint buffers[4] = { vertexID, normalID, uvID, indexID };
	glDeleteBuffers(4, buffers);
}

pxpk::MeshObject::MeshObject(std::string filepath) : ObjectResource(filepath)
{
	std::string extension = filepath.substr(filepath.size() - 5, 4);
	//read file
	if (extension != ".obj")
	{
		LOG("Cannot load OBJ file: invalid file path " + filepath, pxpk::ERROR_LOG);
		return;
	}

	LOG("Reading OBJ file: " + filepath, pxpk::INFO_LOG);
	std::ifstream inFile(filepath);

	if (!inFile.is_open())
	{
		LOG("Cannot load OBJ file: unable to open file " + filepath, pxpk::ERROR_LOG);
		return;
	}

	std::vector<glm::vec3> vertexes_unindexed;
	std::vector<glm::vec3> normals_unindexed;
	std::vector<glm::vec2> uvs_unindexed;
	std::vector<glm::vec3> face_pts;

	std::vector<GLuint> indexes;
	std::vector<GLfloat> vertexes_indexed;
	std::vector<GLfloat> normals_indexed;
	std::vector<GLfloat> uvs_indexed;

	int indexCount = 0;

	std::string currentLine;
	while (std::getline(inFile, currentLine))
	{
		//get token
		std::stringstream lineStream(currentLine);
		std::string token;
		lineStream >> token;
		if (token == "v")  //vertex
		{
			float x, y, z;
			lineStream >> x;
			lineStream >> y;
			lineStream >> z;

			vertexes_unindexed.push_back(glm::vec3(x, y, z));
		}
		else if (token == "vn")  //normal
		{
			float x, y, z;
			lineStream >> x;
			lineStream >> y;
			lineStream >> z;

			normals_unindexed.push_back(glm::vec3(x, y, z));
		}
		else if (token == "vt")  //UV
		{
			float u, v;
			lineStream >> u;
			lineStream >> v;

			uvs_unindexed.push_back(glm::vec2(u, v));
		}
		else if (token == "f")  //face
		{
			std::string a, b, c;
			lineStream >> a;
			lineStream >> b;
			lineStream >> c;

			std::string points[3] = { a, b, c };

			int firstDelim, secondDelim;
			for (const std::string &a : points)
			{
				int a_vert, a_tex, a_norm;
				firstDelim = a.find_first_of('/');
				secondDelim = a.find_first_of('/', firstDelim + 1);
				std::string temp = a.substr(0, firstDelim);
				a_vert = std::stoi(temp);
				temp = a.substr(firstDelim + 1, (secondDelim - firstDelim) - 1);
				a_tex = std::stoi(temp);
				temp = a.substr(secondDelim + 1, a.size());
				a_norm = std::stoi(temp);

				//check if already indexed
				std::vector<glm::vec3>::iterator it = std::find(face_pts.begin(), face_pts.end(), glm::vec3(a_vert, a_tex, a_norm));
				if (it != face_pts.end())
				{
					//point is already indexed
					indexes.push_back(std::distance(face_pts.begin(), it));
				}
				else  //new point
				{
					//add vertex
					vertexes_indexed.push_back(vertexes_unindexed[a_vert-1].x);
					vertexes_indexed.push_back(vertexes_unindexed[a_vert-1].y);
					vertexes_indexed.push_back(vertexes_unindexed[a_vert-1].z);

					//add UV
					uvs_indexed.push_back(uvs_unindexed[a_tex-1].x);
					uvs_indexed.push_back(uvs_unindexed[a_tex-1].y);

					//add normal
					normals_indexed.push_back(normals_unindexed[a_norm-1].x);
					normals_indexed.push_back(normals_unindexed[a_norm-1].y);
					normals_indexed.push_back(normals_unindexed[a_norm-1].z);

					//add index
					indexes.push_back(indexCount++);
				}
			}
		}
		else  //all other tokens
		{

		}
	}

	inFile.close();

	//pass data to OpenGL
	this->createResource(vertexes_indexed, normals_indexed, uvs_indexed, indexes);
}

pxpk::MeshObject::~MeshObject()
{
}

GLuint pxpk::MeshObject::getVertexID()
{
	return vertexID;
}

GLuint pxpk::MeshObject::getNormalID()
{
	return normalID;
}

GLuint pxpk::MeshObject::getUVID()
{
	return uvID;
}

GLuint pxpk::MeshObject::getIndexID()
{
	return indexID;
}

GLsizei pxpk::MeshObject::getIndexSize()
{
	return indexSize;
}
