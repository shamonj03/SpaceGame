#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <GL/glew.h>
#include "Shader.h"

#include "Entity.hpp"

class Player : public Entity {
private:
	/*
	* Each line is a unique vertex of the polygon.
	*/
	glm::vec3 vertices[4] = {
		glm::vec3(-0.5f, -0.5f, 0.0f) * 0.5f,
		glm::vec3(0.0f, -0.25f, 0.0f) * 0.5f,
		glm::vec3(0.0f, 0.75f, 0.0f) * 0.5f,
		glm::vec3(0.5f, -0.5f, 0.0f) * 0.5f,
	};

	/*
	* Each line is a color for that vertex.
	*/
	GLfloat colors[4 * 4] = {
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1
	};

	/*
	* The order to draw the vertices.
	*/
	GLuint indices[6] = {
		0, 1, 2, 2, 3, 1
	};


public:

	Player(glm::vec3 position_);
	~Player();

	virtual void initializeBuffers(GLuint shader);
	virtual void update(float dt);
	virtual void draw(float dt);
	virtual void onKeyDown(SDL_KeyboardEvent& e);
};

Player::Player(glm::vec3 position_) : Entity(position_) {
}


Player::~Player() {
}


void Player::onKeyDown(SDL_KeyboardEvent& e) {
	if (e.keysym.sym == SDLK_w) { // Accel forward
		acceleration += glm::vec3(0, 0.1f, 0);
	}
	if (e.keysym.sym == SDLK_s) { // Deaccel
		acceleration -= glm::vec3(0, 0.1f, 0);
	}
	if (e.keysym.sym == SDLK_a) { // Rotate Left

	}
	if (e.keysym.sym == SDLK_d) { // Rotate Right

	}
}


void Player::initializeBuffers(GLuint shader) {
	glUseProgram(shader);
	Shader::bindArray(GL_ELEMENT_ARRAY_BUFFER, indexBuffer, sizeof(indices), &indices[0], GL_STATIC_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, vertexBuffer, sizeof(glm::vec3) * sizeof(vertices), NULL, GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, colorBuffer, sizeof(colors), &colors[0], GL_STATIC_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, normalBuffer, sizeof(glm::vec3), &normal, GL_STATIC_DRAW);
	glUseProgram(0);
}

void Player::update(float dt) {
	velocity += acceleration * dt;
	position += velocity * dt;

	for (int i = 0; i < sizeof(vertices); i++) {
		vertices[i] = vertices[i] + position;
	}
}

void Player::draw(float dt) {
	glPushMatrix();
	Shader::sendArray(0, 3, vertexBuffer, sizeof(glm::vec3) * sizeof(vertices), &vertices[0], GL_STREAM_DRAW);
	Shader::sendArray(1, 3, normalBuffer);
	//Shader::sendArray(2, 2, texCoordBuffer);
	Shader::sendArray(3, 4, colorBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glPopMatrix();
}

#endif // PLAYER_HPP