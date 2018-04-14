#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <glm\glm.hpp>
#include <GL\glew.h>

#include "Shader.h"

class ParticleSystem {
	glm::vec3* velocities;
	glm::vec3* positions;
	glm::vec4* colors;
	glm::vec3 normal;

	static glm::vec3 vertices[4];

	float* sizes;
	float* lifes;

	int particleCount;
	int genRate;

	GLuint indexBuffer;
	GLuint vertexBuffer;
	GLuint positionBuffer;
	GLuint normalBuffer;
	GLuint colorBuffer;
	GLuint vao;

	int MAX_PARTICLES;

	virtual void emit();
	virtual void destroy(int index);

public:
	ParticleSystem(int genRate_, int maxParticles_);
	~ParticleSystem();

	virtual void initializeBuffers(GLuint shader);
	virtual void decay(float dt);
	virtual void generate(float dt);
	virtual void update(float dt);
	virtual void draw(float dt);
};

glm::vec3 ParticleSystem::vertices[4] = {
	glm::vec3(-0.5f, -0.5f, 0.0f) * 0.2f,
	glm::vec3(0.5f, -0.5f, 0.0f) * 0.2f,
	glm::vec3(-0.5f, 0.5f, 0.0f) * 0.2f,
	glm::vec3(0.5f, 0.5f, 0.0f) * 0.2f,
};

ParticleSystem::ParticleSystem(int genRate_, int maxParticles_) : genRate(genRate_), MAX_PARTICLES(maxParticles_), particleCount(0), normal(glm::vec3(0, 1, 0)) {
	positions = new glm::vec3[MAX_PARTICLES];
	velocities = new glm::vec3[MAX_PARTICLES];
	colors = new glm::vec4[MAX_PARTICLES];
	sizes = new float[MAX_PARTICLES];
	lifes = new float[MAX_PARTICLES];
}



void ParticleSystem::initializeBuffers(GLuint shader) {
	glUseProgram(shader);
	Shader::bindArray(GL_ARRAY_BUFFER, vertexBuffer, sizeof(glm::vec3) * sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, positionBuffer, MAX_PARTICLES * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, colorBuffer, MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, normalBuffer, sizeof(glm::vec3), &normal, GL_STATIC_DRAW);
	glUseProgram(0);
}

void ParticleSystem::emit() {
	if (particleCount >= MAX_PARTICLES) {
		return;
	}
	positions[particleCount] = glm::vec3(-25 + rand() % 50, rand() % 10, 0);
	colors[particleCount] = glm::vec4(1, 1, 1, 1);
	velocities[particleCount] = glm::vec3(0, -1, 0) * 5.0f;
	lifes[particleCount] = 10;
	particleCount++;
}


void ParticleSystem::decay(float dt) {
	for (int i = 0; i < particleCount; i++) {
		float life = lifes[i];

		if (life <= 0.0f) {
			destroy(i);
		}
		else {
			lifes[i] -= 1 * dt;
		}
	}
}

void ParticleSystem::generate(float dt) {
	float toGen = genRate * dt;

	for (int i = 0; i < toGen; i++) {
		emit();
	}
}

void ParticleSystem::destroy(int index) {
	positions[index] = positions[particleCount];
	velocities[index] = velocities[particleCount];
	colors[index] = colors[particleCount];
	lifes[index] = lifes[particleCount];

	particleCount--;
}


void ParticleSystem::update(float dt) {
	for (int i = 0; i < particleCount; i++) {
		glm::vec3* pos = &positions[i];
		glm::vec3* vel = &velocities[i];

		*pos = *pos + *vel * dt;
	}

	std::cout << particleCount << "/" << MAX_PARTICLES << ":" << genRate << std::endl;
}

void ParticleSystem::draw(float dt) {
	glPushMatrix();
	Shader::sendArray(0, 3, vertexBuffer);
	Shader::sendArray(1, 3, normalBuffer);
	Shader::sendArray(2, 3, positionBuffer, MAX_PARTICLES * sizeof(glm::vec3), &positions[0], GL_STREAM_DRAW);
	Shader::sendArray(3, 4, colorBuffer, MAX_PARTICLES * sizeof(glm::vec4), &colors[0], GL_STREAM_DRAW);

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

ParticleSystem::~ParticleSystem() {
}

#endif // PARTICLE_SYSTEM_HPP