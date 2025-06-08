#ifndef _MODEL_H
#define _MODEL_H

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "geometry.h"

struct Face
{
	std::vector<int> pointIndices;
	std::vector<int> textureIndices;
	std::vector<int> normalIndices;
};

void readOBJ(const std::string& filePath,
	std::vector<Vec3f>& points,
	std::vector<Vec2f>& texCoords,
	std::vector<Vec3f>& normals,
	std::vector<Face>& faces);

void readobj(const std::string& filePath);



#endif