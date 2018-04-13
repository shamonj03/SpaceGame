#include "Mesh.h"
#include "Shader.h"
#include <iostream>

Mesh::Mesh(int rows_, int cols_, float quad_size_, glm::vec3 offset_) : rows(rows_), cols(cols_), v_x(cols_ + 1), v_y(rows_ + 1), quad_size(quad_size_), offset(offset_) {
	vertex_count = v_x * v_y;
	index_count = rows * cols * 6;

	indices = new int[index_count];
	verticies = new glm::vec3[vertex_count];
	normals = new glm::vec3[vertex_count];
	textCoords = new glm::vec2[vertex_count];
	colors = new glm::vec4[vertex_count];

	for (int y = 0; y < v_y; y++) {
		for (int x = 0; x < v_x; x++) {
			int index = x + (y * v_x);
			glm::vec3 pos(x * quad_size, 0, y * quad_size);
			verticies[index] = offset + pos;
			colors[index] = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
			normals[index] = glm::vec3(0, 1, 0);
			textCoords[index] = glm::vec2((float) x / (float) (cols), (float) y / (float)(rows));
		}
	}

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			int index = (x + (y * v_x));
			int index2 = (x + (y * cols)) * 6;

											 // Top Left Triangle.
			indices[index2 + 0] = index; // Top Left
			indices[index2 + 1] = index + 1; // Top Right
			indices[index2 + 2] = index + v_x + 1; // Bottom Right

													   // Bottom Right Triangle.
			indices[index2 + 3] = index + v_x + 1; // Bottom Right
			indices[index2 + 4] = index + v_x; // Bottom Left
			indices[index2 + 5] = index; // Top Left
		}
	}
}

void Mesh::initializeBuffers(GLuint shader) {
	glUseProgram(shader);
	Shader::bindArray(GL_ELEMENT_ARRAY_BUFFER, indexBuffer, index_count * sizeof(int), &indices[0], GL_STATIC_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, vertexBuffer, vertex_count * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, colorBuffer, vertex_count * sizeof(glm::vec4), &colors[0], GL_STATIC_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, normalBuffer, vertex_count * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, texCoordBuffer, vertex_count * sizeof(glm::vec2), &textCoords[0], GL_STATIC_DRAW);
	glUseProgram(0);
}

void Mesh::draw(GLenum mode) {
	glPushMatrix();
	Shader::sendArray(0, 3, vertexBuffer, vertex_count * sizeof(glm::vec3), &verticies[0], GL_STREAM_DRAW);
	Shader::sendArray(1, 3, normalBuffer, vertex_count * sizeof(glm::vec3), &normals[0], GL_STREAM_DRAW);
	Shader::sendArray(2, 2, texCoordBuffer);
	Shader::sendArray(3, 4, colorBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(mode, index_count, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glPopMatrix();
}
