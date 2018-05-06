#include <algorithm> 
#include <vector>

#include "Ship.h"
#include "World.h"
#include "Shader.h"

#include "BoundingBox2D.h"
#include "ParticleSystem.h"
#include "Util.hpp"

Ship::Ship(class World* world_, glm::vec3 position_) : Entity(world_, position_),
colors(new GLfloat[16] {
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1}),
indices{0, 1, 2, 2, 3, 1},
vertices{
	glm::vec3(-0.5f, -0.5f, 0.0f),
	glm::vec3(0.0f, -0.25f, 0.0f),
	glm::vec3(0.0f, 0.75f, 0.0f),
	glm::vec3(0.5f, -0.5f, 0.0f)} {

}

Ship::~Ship() {
}

void Ship::initializeBuffers(GLuint shader_) {
	shader = shader_;
	for (int i = 0; i < 4; i++) {
		vertices[i] *= size;
	}
	glUseProgram(shader);
	for (auto particle : emitters) {
		particle->initializeBuffers(shader);
	}
	Shader::bindArray(GL_ELEMENT_ARRAY_BUFFER, indexBuffer, sizeof(indices), &indices[0], GL_STATIC_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, vertexBuffer, sizeof(glm::vec3) * sizeof(vertices), &vertices[0], GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, positionBuffer, sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, colorBuffer, 4 * sizeof(vertices), &colors[0], GL_STATIC_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, normalBuffer, sizeof(glm::vec3), &normal, GL_STATIC_DRAW);
	glUseProgram(0);
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

	Util::rotate(Ship::vertices, 4, angle);
	Shader::bindArray(GL_ARRAY_BUFFER, vertexBuffer, sizeof(glm::vec3) * sizeof(Ship::vertices), &Ship::vertices[0], GL_STREAM_DRAW);
	Util::rotate(Ship::vertices, 4, -angle);

	for (auto particle : emitters) {
		particle->generate(dt);
		particle->update(dt);
		particle->decay(dt);

		if (!particle->alive) {
			emitters.erase(std::remove(emitters.begin(), emitters.end(), particle), emitters.end());
		}
	}
}

void Ship::draw(float dt) {
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

	for (auto particle : emitters) {
		particle->draw(dt);
	}
}