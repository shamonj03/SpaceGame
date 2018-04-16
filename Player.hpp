#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <GL/glew.h>
#include "Shader.h"

#include "Entity.hpp"
#include "Util.hpp"

class Player : public Entity {
private:
	static glm::vec3 vertices[4];
	static GLuint indices[6];
	static GLfloat colors[16];

public:
	bool released;

	Player(glm::vec3 position_);
	~Player();

	virtual void initializeBuffers(GLuint shader);
	virtual void update(float dt);
	virtual void draw(float dt);
	virtual void onKeyDown(SDL_KeyboardEvent& e);
	virtual void onKeyUp(SDL_KeyboardEvent& e);
};

/*
* Each line is a unique vertex of the polygon.
*/
glm::vec3 Player::vertices[4] = {
	glm::vec3(-0.5f, -0.5f, 0.0f) * 0.5f,
	glm::vec3(0.0f, -0.25f, 0.0f) * 0.5f,
	glm::vec3(0.0f, 0.75f, 0.0f) * 0.5f,
	glm::vec3(0.5f, -0.5f, 0.0f) * 0.5f,
};

/*
* The order to draw the vertices.
*/
GLuint Player::indices[6] = {
	0, 1, 2, 2, 3, 1
};

/*
* Each line is a color for that vertex.
*/
GLfloat Player::colors[4 * 4] = {
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1
};


Player::Player(glm::vec3 position_) : Entity(position_) {
}


Player::~Player() {
}


void Player::onKeyDown(SDL_KeyboardEvent& e) {
	if (e.keysym.sym == SDLK_w) { // Accel forward
		acceleration += glm::vec3(0, 0.75f, 0);
		released = false;
	}
	if (e.keysym.sym == SDLK_s) { // Deaccel
		acceleration -= glm::vec3(0, 0.75f, 0);
		released = false;
	}

	if (e.keysym.sym == SDLK_a) { // Rotate Left
		angle += 5.0f;
	}
	if (e.keysym.sym == SDLK_d) { // Rotate Right
		angle -= 5.0f;
	}

	if (e.keysym.sym == SDLK_SPACE) { // FIRE THE LASER!

	}
}


void Player::onKeyUp(SDL_KeyboardEvent& e) {
	if (e.keysym.sym == SDLK_w || e.keysym.sym == SDLK_s) { // Accel forward
		acceleration = glm::vec3(0, 0, 0);
		released = true;
	}

	if (e.keysym.sym == SDLK_SPACE) { // Stop the laser :C

	}
}


void Player::initializeBuffers(GLuint shader) {
	glUseProgram(shader);
	Shader::bindArray(GL_ELEMENT_ARRAY_BUFFER, indexBuffer, sizeof(indices), &indices[0], GL_STATIC_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, vertexBuffer, sizeof(glm::vec3) * sizeof(vertices), &vertices[0], GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, positionBuffer, sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, colorBuffer, sizeof(colors), &colors[0], GL_STATIC_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, normalBuffer, sizeof(glm::vec3), &normal, GL_STATIC_DRAW);
	glUseProgram(0);
}

void Player::update(float dt) {
	velocity += acceleration * dt;
	velocity = Util::limit(velocity, maxSpeed);

	// TODO: This rotation method sucks! But it works.
	Util::rotate(velocity, angle);
	position += velocity * dt;
	Util::rotate(velocity, -angle);

	Util::rotate(vertices, 4, angle);
	Shader::bindArray(GL_ARRAY_BUFFER, vertexBuffer, sizeof(glm::vec3) * sizeof(vertices), &vertices[0], GL_STREAM_DRAW);
	Util::rotate(vertices, 4, -angle);

	// Slows down if key released.
	if (released) {
		velocity *= 0.99f;
	}
}

void Player::draw(float dt) {
	glPushMatrix();
	Shader::sendArray(0, 3, vertexBuffer);
	Shader::sendArray(1, 3, normalBuffer);
	Shader::sendArray(2, 3, positionBuffer, sizeof(glm::vec3), &position[0], GL_STREAM_DRAW);
	//Shader::sendArray(2, 2, texCoordBuffer);
	Shader::sendArray(3, 4, colorBuffer);


	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1); // normal : one per quad -> 1
	glVertexAttribDivisor(2, 1); // positions : one per quad (its center) -> 1
	glVertexAttribDivisor(3, 1); // color : one per quad -> 1


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glPopMatrix();
}

#endif // PLAYER_HPP