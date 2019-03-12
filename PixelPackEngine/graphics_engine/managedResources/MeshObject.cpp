#define TINYOBJLOADER_IMPLEMENTATION
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

	if (!normalData.empty())
	{
		//create normal buffer
		glGenBuffers(1, &normalID);
		glBindBuffer(GL_ARRAY_BUFFER, normalID);
		glBufferData(
			GL_ARRAY_BUFFER,
			normalData.size() * sizeof(GLfloat),
			&normalData.front(),
			GL_STATIC_DRAW
		);
	}

	if (!uvData.empty())
	{
		//create UV buffer
		glGenBuffers(1, &uvID);
		glBindBuffer(GL_ARRAY_BUFFER, uvID);
		glBufferData(
			GL_ARRAY_BUFFER,
			uvData.size() * sizeof(GLfloat),
			&uvData.front(),
			GL_STATIC_DRAW
		);
	}

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

	if (normalID != -1)
	{
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
	}

	if (uvID != -1)
	{
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
}

void pxpk::MeshObject::freeResource()
{
	glDisableVertexAttribArray(pxpk::vertexAttributeID);
	if (normalID != -1)
		glDisableVertexAttribArray(pxpk::normalAttributeID);
	if (uvID != -1)
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
	//std::ifstream inFile(filepath);

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filepath.c_str());

	if (!err.empty())
	{
		LOG("Error loading file " + filepath + "\n" + err, pxpk::ERROR_LOG);
	}
	if (!success)
	{
		exit(1);
	}

	//convert to indexed data
	std::unordered_map<std::string, int> face_pts;

	std::vector<GLuint> indexes;
	std::vector<GLfloat> vertexes_indexed;
	std::vector<GLfloat> normals_indexed;
	std::vector<GLfloat> uvs_indexed;

	//reserve worst-case max size
	face_pts.reserve(shapes[0].mesh.indices.size());
	indexes.reserve(shapes[0].mesh.indices.size());
	vertexes_indexed.reserve(attrib.vertices.size());
	normals_indexed.reserve(attrib.normals.size());
	uvs_indexed.reserve(attrib.texcoords.size());

	int indexedCount = 0;
	for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
	{
		int vert_idx = shapes[0].mesh.indices[i].vertex_index;
		int tex_idx = shapes[0].mesh.indices[i].texcoord_index;
		int norm_idx = shapes[0].mesh.indices[i].normal_index;

		//check if already indexed
		std::string findThis;
		findThis = std::to_string(vert_idx) + '/' + std::to_string(tex_idx) + '/' + std::to_string(norm_idx);
		
		if (face_pts.find(findThis) != face_pts.end())
		{
			//point is already indexed
			indexes.push_back(face_pts[findThis]);
		}
		else  //new point
		{
			//add vertex
			if (vert_idx != -1)
			{
				vertexes_indexed.push_back(attrib.vertices[3 * vert_idx + 0]);
				vertexes_indexed.push_back(attrib.vertices[3 * vert_idx + 1]);
				vertexes_indexed.push_back(attrib.vertices[3 * vert_idx + 2]);
			}

			//add UV
			if (tex_idx != -1)
			{
				uvs_indexed.push_back(attrib.texcoords[2 * tex_idx + 0]);
				uvs_indexed.push_back(attrib.texcoords[2 * tex_idx + 1]);
			}

			//add normal
			if (norm_idx != -1)
			{
				normals_indexed.push_back(attrib.normals[3 * norm_idx + 0]);
				normals_indexed.push_back(attrib.normals[3 * norm_idx + 1]);
				normals_indexed.push_back(attrib.normals[3 * norm_idx + 2]);
			}

			//add index
			indexes.push_back(indexedCount);

			//add to face records
			face_pts.insert({ findThis, indexedCount++ });
		}
	}

	//trim data footprint
	vertexes_indexed.shrink_to_fit();
	normals_indexed.shrink_to_fit();
	uvs_indexed.shrink_to_fit();
	indexes.shrink_to_fit();

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
