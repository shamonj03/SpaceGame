#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <algorithm>
#include <vector>
#include <glm\glm.hpp>
#include <GL\glew.h>

#include "Util.hpp"
#include "Shader.h"
#include "Emitter.h"

#define MAX_PARTICLES 100000

class ParticleSystem {
	std::vector<Emitter*> emitters;

	glm::vec3* velocities;
	glm::vec3* positions;
	glm::vec4* colors;
	glm::vec3 normal;
	float* lifes;

	int particleCount;

	GLuint indexBuffer;
	GLuint vertexBuffer;
	GLuint positionBuffer;
	GLuint normalBuffer;
	GLuint colorBuffer;
	GLuint vao;

	virtual void destroy(int index);

public:
	ParticleSystem();
	~ParticleSystem() {}

	void addEmitter(Emitter* emitter_);

	void removeEmitter(Emitter* emitter_);

	virtual void initializeBuffers(GLuint shader);
	virtual void decay(float dt);
	virtual void generate(float dt);
	virtual void update(float dt);
	virtual void draw(float dt);
};

ParticleSystem::ParticleSystem() : particleCount(0), normal(glm::vec3(0, 1, 0)) {
	positions = new glm::vec3[MAX_PARTICLES];
	velocities = new glm::vec3[MAX_PARTICLES];
	colors = new glm::vec4[MAX_PARTICLES];
	lifes = new float[MAX_PARTICLES];
}


void ParticleSystem::addEmitter(Emitter* emitter_) {
	emitters.push_back(emitter_);
}

void ParticleSystem::removeEmitter(Emitter* emitter_) {
	emitters.erase(std::remove(emitters.begin(), emitters.end(), emitter_), emitters.end());
}

void ParticleSystem::initializeBuffers(GLuint shader) {
	glUseProgram(shader);
	Shader::bindArray(GL_ARRAY_BUFFER, vertexBuffer, sizeof(glm::vec3) * particleCount, NULL, GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, positionBuffer, particleCount * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, colorBuffer, particleCount * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, normalBuffer, sizeof(glm::vec3), &normal, GL_STATIC_DRAW);
	glUseProgram(0);
}

void ParticleSystem::decay(float dt) {
	for (int i = 0; i < particleCount; i++) {
		float life = lifes[i];

		if ((life - (1 * dt)) <= 0.0f) {
			destroy(i);
		} else {
			lifes[i] -= 1 * dt;
		}
	}
}

void ParticleSystem::generate(float dt) {
	if (particleCount >= MAX_PARTICLES) {
		return;
	}

	for (auto emitter : emitters) {
		float toGen = emitter->genRate * dt;

		if (toGen < 1.0f) {
			toGen = emitter->genRate;
		}

		for (int i = 0; i < toGen; i++) {
			if (emitter->emit(positions[particleCount], velocities[particleCount], colors[particleCount], lifes[particleCount])) {
				particleCount++;
			} else {
				removeEmitter(emitter);
			}
		}
	}
}

void ParticleSystem::destroy(int index) {
	positions[index] = positions[particleCount - 1];
	velocities[index] = velocities[particleCount - 1];
	colors[index] = colors[particleCount - 1];
	lifes[index] = lifes[particleCount - 1];
	particleCount--;
}


void ParticleSystem::update(float dt) {
	for (int i = 0; i < particleCount; i++) {
		glm::vec3* pos = &positions[i];
		glm::vec3* vel = &velocities[i];

		*pos = *pos + *vel * dt;

		for (auto emitter : emitters) {
			emitter->update(positions[particleCount], velocities[particleCount], colors[particleCount], lifes[particleCount]);
		}
	}
}

void ParticleSystem::draw(float dt) {
	for (auto emitter : emitters) {
		glPushMatrix();
		Shader::sendArray(0, 3, vertexBuffer, sizeof(glm::vec3) * sizeof(emitter->vertices), &emitter->vertices[0], GL_STREAM_DRAW);
		Shader::sendArray(1, 3, normalBuffer);
		Shader::sendArray(2, 3, positionBuffer, particleCount * sizeof(glm::vec3), &positions[0], GL_STREAM_DRAW);
		Shader::sendArray(3, 4, colorBuffer, particleCount * sizeof(glm::vec4), &colors[0], GL_STREAM_DRAW);

		glVertexAttribDivisor(0, 0);
		glVertexAttribDivisor(1, 1); // normal : one per quad -> 1
		glVertexAttribDivisor(2, 1); // positions : one per quad (its center) -> 1
		glVertexAttribDivisor(3, 1); // color : one per quad -> 1

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glPopMatrix();
	}
}

#endif // PARTICLE_SYSTEM_HPP