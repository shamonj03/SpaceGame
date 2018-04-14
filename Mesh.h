#pragma once

#include <glm\glm.hpp>
#include <gl\glew.h>

#include "Engine.h"
#include "Texture.h"

class Mesh {
public:
	glm::vec3 offset;

	int rows, cols;
	int v_x, v_y;
	float quad_size;

	int index_count;
	int* indices;

	int vertex_count;
	glm::vec3* verticies;
	glm::vec3* normals;
	glm::vec2* textCoords;
	glm::vec4* colors;

	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint positionBuffer;
	GLuint texCoordBuffer;
	GLuint indexBuffer;
	GLuint colorBuffer;

	virtual void draw(GLenum mode);

	Mesh(int rows_, int cols_, float quad_size_);

	void initializeBuffers(GLuint shader);

	~Mesh() {}
};

