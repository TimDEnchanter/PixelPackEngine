#define TINYOBJLOADER_IMPLEMENTATION

#include <string>
#include <iostream>
#include <unordered_map>
#include <limits>
#include "tiny_obj_loader.h"

void OBJtoPXO(std::string filepath)
{
	std::cout << "Reading OBJ file: " << filepath << std::endl;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filepath.c_str());

	if (!err.empty())
	{
		std::cout << "Error loading file " << filepath << "\n" << err << std::endl;
	}
	if (!success)
	{
		exit(1);
	}

	//convert to indexed data
	std::unordered_map<std::string, int> face_pts;

	std::vector<uint32_t> indexes;
	std::vector<float> vertexes_indexed;
	std::vector<float> normals_indexed;
	std::vector<float> uvs_indexed;

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

	std::cout << "OBJ file read. writing to PXO file." << std::endl;

	//write data to file
	int filenameIndex = filepath.find_last_of('/') != std::string::npos ? filepath.find_last_of('/') : filepath.find_last_of('\\');
	int extensionIndex = filepath.find_last_of('.');
	std::string filename = filepath.substr(filenameIndex + 1, extensionIndex - (filenameIndex + 1));

	std::ofstream outFile(".\\output\\" + filename + ".pxo", std::ios::binary);

	size_t vertSize = vertexes_indexed.size() * sizeof(float);
	size_t normSize = normals_indexed.size() * sizeof(float);
	size_t uvSize = uvs_indexed.size() * sizeof(float);
	size_t indexSize = indexes.size() * sizeof(uint32_t);

	//list container sizes
	outFile.write(reinterpret_cast<char*>(&vertSize), sizeof(vertSize));
	outFile.write(reinterpret_cast<char*>(&normSize), sizeof(normSize));
	outFile.write(reinterpret_cast<char*>(&uvSize), sizeof(uvSize));
	outFile.write(reinterpret_cast<char*>(&indexSize), sizeof(indexSize));

	//write container binary data
	outFile.write(reinterpret_cast<char*>(vertexes_indexed.data()), vertSize);
	outFile.write(reinterpret_cast<char*>(normals_indexed.data()), normSize);
	outFile.write(reinterpret_cast<char*>(uvs_indexed.data()), uvSize);
	outFile.write(reinterpret_cast<char*>(indexes.data()), indexSize);

	outFile.close();

	std::cout << filename << ".pxo created!" << std::endl;
}

int main(int argc, char **argv)
{
	std::string filepath;
	std::string extension;

	std::cout << "Enter file path: " << std::endl;
	std::getline(std::cin, filepath);

	int extensionIndex = -1;
	extensionIndex = filepath.find_last_of('.');
	extension = filepath.substr(extensionIndex);

	if (extension == ".obj")
	{
		OBJtoPXO(filepath);
	}
	else
	{
		std::cout << "EROR: Unable to convert file. Invalid filepath: " << filepath << std::endl;
		return 1;
	}

	std::cout << "processing complete. Press enter to continue." << std::endl;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	return 0;
}