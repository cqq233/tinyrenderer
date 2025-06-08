#include <iostream>
#include <fstream>
#include <vector>

#include "tgaimage.h"
#include "model.h"
#include "geometry.h"


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

const int WIDTH = 800;
const int HEIGHT = 800;

float zBuffer[WIDTH * HEIGHT];

std::vector<Vec3f> vertices;
std::vector<Vec2f> texCoords;
std::vector<Vec3f> normals;
std::vector<Face> faces;


std::string filePath = "african_head.obj";



int main(int argc, char** argv) {

	readOBJ(filePath, vertices, texCoords, normals, faces);

	TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
	//TGAImage image(64, 64, TGAImage::RGB);
	Vec3f lightDir = Vec3f(0, 0, -1);
	for (int i = 0; i < HEIGHT * WIDTH; i++)
	{
		zBuffer[i] = -9999.0;
	}
	int count = 0;
	for (int i = 0; i < faces.size(); i++)
	{
		Vec3i screen_coords[3];
		Vec3f world_coords[3];
		for (int j = 0; j < 3; j++)
		{
			Vec3f world_coord = vertices[faces[i].pointIndices[j]];
			world_coords[j] = world_coord;
			screen_coords[j] = Vec3i((world_coord.x + 1.) * WIDTH / 2., (world_coord.y + 1.) * HEIGHT / 2., 0);
		}

		Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
		n.normalize();

		float diffuse = lightDir * n;
		//triangle_z(world_coords, screen_coords, zBuffer, image, TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
		if (diffuse > 0) {
			count++;
			triangle_z(world_coords, screen_coords, zBuffer, image, TGAColor(diffuse * 255, diffuse * 255, diffuse * 255, 255));
		}
	}
	//std::cout << faces.size() << std::endl;
	
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output_human07.tga");
	return 0;
}