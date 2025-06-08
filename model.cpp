#include "model.h"



void readOBJ(const std::string& filePath,
	std::vector<Vec3f>& vertices,
	std::vector<Vec2f>& texCoords,
	std::vector<Vec3f>& normals,
	std::vector<Face>& faces)
{
	std::ifstream file(filePath);
	if (!file.is_open()) 
	{
		std::cerr << "fail to open file: " << filePath << std::endl;
		return;
	}
	std::string line;

	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string type;
		iss >> type;

		if (type == "v")
		{
			Vec3f v;
			iss >> v.x >> v.y >> v.z;
			vertices.push_back(v);
		}
		else if (type == "vt")
		{
			Vec2f texCoord;
			iss >> texCoord.u >> texCoord.v;
			texCoords.push_back(texCoord);
		}
		else if (type == "vn")
		{
			Vec3f normal;
			iss >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (type == "f")
		{
			Face face;
			std::string vertexData;
			while (iss >> vertexData)
			{
				std::istringstream viss(vertexData);
				std::string v, vt, vn;

				if (std::getline(viss, v, '/') && !v.empty()) 
					face.pointIndices.push_back(std::stoi(v) - 1);

				if (std::getline(viss, vt, '/') && !v.empty())
					face.textureIndices.push_back(std::stoi(vt) - 1);

				if (std::getline(viss, vn, '/') && !v.empty())
					face.normalIndices.push_back(std::stoi(vn) - 1);

			}
			faces.push_back(face);
		}
	}

	file.close();
}

void readTex(const std::string& filePath, Vec3f color)
{

}