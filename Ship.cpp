#include <algorithm> 
#include <vector>

#include "Ship.h"
#include "World.h"
#include "Shader.h"

#include "BoundingBox2D.h"
#include "ParticleSystem.h"
#include "Util.h"
#include "StandardEmitter.hpp"

Ship::Ship(class World* world_, GLfloat shader_, glm::vec3 position_) : Entity(world_, shader_, position_),
colors{
	glm::vec4(1, 1, 1, 1),
	glm::vec4(1, 1, 1, 1),
	glm::vec4(1, 1, 1, 1),
	glm::vec4(1, 1, 1, 1) },
indices{0, 1, 2, 2, 3, 1},
vertices{
	glm::vec3(-0.5f, -0.5f, 0.0f),
	glm::vec3(0.0f, -0.25f, 0.0f),
	glm::vec3(0.0f, 0.75f, 0.0f),
	glm::vec3(0.5f, -0.5f, 0.0f)} {

	for (int i = 0; i < 4; i++) {
		vertices[i] *= size;
	}
	addEmitter(new StandardEmitter(this, 5, 20), shader_);
}

Ship::~Ship() {
	//delete[] colors;
}


void Ship::initializeBuffers(GLfloat shader_) {
	glUseProgram(shader_);
	Shader::bindArray(GL_ELEMENT_ARRAY_BUFFER, indexBuffer, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, vertexBuffer, sizeof(glm::vec3) * vertices.size(), NULL, GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, positionBuffer, sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, colorBuffer, sizeof(glm::vec4) * colors.size(), &colors[0], GL_STATIC_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, normalBuffer, sizeof(glm::vec3), &normal, GL_STATIC_DRAW);
	glUseProgram(0);
}

void Ship::addEmitter(ParticleSystem* emitter, GLfloat Shader) {
	try {
		emitters.push_back(emitter);
		emitter->initializeBuffers(Shader);
	} catch (std::exception e) {
		// code to handle ExceptionName exception
	}
}

void Ship::update(float dt) {
	velocity += acceleration * dt;
	velocity = Util::limit(velocity, maxSpeed);

	// TODO: This rotation method sucks! But it works.
	Util::rotate(velocity, angle);
	position += velocity * dt;

	if (position.x > world->bounds3D->top.x) {
		position.x = world->bounds3D->bottom.x;
	}
	if (position.x < world->bounds3D->bottom.x) {
		position.x = world->bounds3D->top.x;
	}

	if (position.y > world->bounds3D->top.y) {
		position.y = world->bounds3D->bottom.y;
	}
	if (position.y < world->bounds3D->bottom.y) {
		position.y = world->bounds3D->top.y;
	}

	Util::rotate(velocity, -angle);
	//Shader::bindArray(GL_ARRAY_BUFFER, vertexBuffer, sizeof(glm::vec3) * sizeof(vertices), &vertices[0], GL_STREAM_DRAW);
//	Util::rotate(vertices, 4, -angle);

	for (auto particle : emitters) {
		particle->generate(dt);
		particle->update(dt);
		particle->decay(dt);
	}
	for (auto particle : emitters) {
		if (!particle->alive) {
			emitters.erase(std::remove(emitters.begin(), emitters.end(), particle), emitters.end());
		}
	}
}

void Ship::draw(float dt) {
	glPushMatrix();
	Util::rotate(vertices, 4, angle);
	Shader::sendArray(0, 3, vertexBuffer, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STREAM_DRAW);
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
	Util::rotate(vertices, 4, -angle);
	glPopMatrix();

	for (auto particle : emitters) {
		particle->draw(dt);
	}
}