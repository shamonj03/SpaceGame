#include "ParticleSystem.h"

#include <algorithm>
#include <vector>
#include <GL\glew.h>

#include "Util.h"
#include "Shader.h"

ParticleSystem::ParticleSystem(int genRate_, int maxParticles_) : 
	alive(true), 
	genRate(genRate_), 
	maxParticles(maxParticles_), 
	particleCount(0), 
	normal(glm::vec3(0, 1, 0)),
	positions(new glm::vec3[maxParticles_]),
	velocities(new glm::vec3[maxParticles_]),
	colors(new glm::vec4[maxParticles_]),
	lifes(new float[maxParticles_]) {
}

ParticleSystem::~ParticleSystem() {
	delete[] velocities;
	delete[] positions;
	delete[] colors;
	delete[] lifes;
	delete[] vertices;
}


void ParticleSystem::update(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt) {}

void ParticleSystem::initializeBuffers(GLuint shader) {
	glUseProgram(shader);
	Shader::bindArray(GL_ARRAY_BUFFER, vertexBuffer, maxParticles * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, positionBuffer, maxParticles * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, colorBuffer, maxParticles * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, normalBuffer, sizeof(glm::vec3), &normal, GL_STATIC_DRAW);
	glUseProgram(0);
}

void ParticleSystem::decay(float dt) {
	for (int i = 0; i < particleCount; i++) {

		float life = lifes[i];

		if ((life - (1 * dt)) <= 0.0f) {
			lifes[i] = 0;
			destroy(positions[i], velocities[i], colors[i], lifes[i], dt);
			destroy(i);
		} else {
			lifes[i] -= 1 * dt;
		}
	}
}


void ParticleSystem::destroy(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt) {

}

void ParticleSystem::generate(float dt) {
	if (particleCount >= maxParticles) {
		return;
	}

	float toGen = genRate * dt;

	if (toGen < 1.0f) {
		toGen = genRate;
	}

	for (int i = 0; i < toGen; i++) {
		emit(positions[particleCount], velocities[particleCount], colors[particleCount], lifes[particleCount], dt);
		particleCount++;
	}
}

void ParticleSystem::destroy(int index) {
	int i = particleCount - 1;
	if (i <= 0) {
		i = 0;
	}
	if (i >= maxParticles) {
		i = maxParticles;
	}
	positions[index] = positions[i];
	velocities[index] = velocities[i];
 	colors[index] = colors[i];
	lifes[index] = lifes[i];
	particleCount--;
}


void ParticleSystem::update(float dt) {
	for (int i = 0; i < particleCount; i++) {
		glm::vec3* pos = &positions[i];
		glm::vec3* vel = &velocities[i];

		*pos = *pos + *vel * dt;
		update(positions[i], velocities[i], colors[i], lifes[i], dt);
	}
}

void ParticleSystem::draw(float dt) {
		glPushMatrix();
		Shader::sendArray(0, 3, vertexBuffer, sizeof(glm::vec3) * sizeof(vertices), &vertices[0], GL_STREAM_DRAW);
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
