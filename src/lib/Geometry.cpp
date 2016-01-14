#include "Geometry.hpp"
//manual test cube mesh

float Cube[] = {

	//left
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.5f,
	0.0f, 0.5f, 0.0f,
	0.0f, 0.5f, 0.5f,

	//front
	0.0f, 0.0f, 0.5f,
	0.5f, 0.0f, 0.5f,
	0.0f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,

	//right
	0.5f, 0.0f, 0.5f,
	0.5f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.5f,

	//back
	0.0f, 0.0f, 0.0f,
	0.5f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,

	//top
	0.0f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
	0.0f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,

	//bottom
	0.0f, 0.0f, 0.0f,
	0.5f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.5f,
	0.5f, 0.0f, 0.5f
};