#ifndef EXPLOSION_EMITTER_HPP
#define EXPLOSION_EMITTER_HPP

#include "Util.h"
#include "ParticleSystem.h"

class ExplosionEmitter : public ParticleSystem {
public:
	glm::vec3 target;
	int index;

	ExplosionEmitter(glm::vec3 target_, int genRate_, int maxParticles_) : ParticleSystem(genRate_, maxParticles_), target(target_), index(0) {
		vertices = new glm::vec3[4] {
			glm::vec3(-0.5f, -0.5f, 0.0f),
			glm::vec3(0.5f, -0.5f, 0.0f),
			glm::vec3(-0.5f, 0.5f, 0.0f),
			glm::vec3(0.5f, 0.5f, 0.0f),
		};

		for (int i = 0; i < 4; i++) {
			vertices[i] *= 0.1f;
		}
	}
	~ExplosionEmitter() {}

	virtual void emit(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt);
	virtual void update(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt);
	virtual void destroy(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt);
};

void ExplosionEmitter::emit(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt) {
	static int slices = 20;

	if (index >= slices) {
		return;
	}
	index++;
	position = target;
	velocity = glm::vec3(0, -7, 0);
	color = glm::vec4(1, 0, 0, 1);
	life = 0.25f;

	int slice = (index) % slices;
	float angle = (360.0f / slices) * slice;

	Util::rotate(vertices, 4, angle);
	Util::rotate(velocity, angle);
}

void ExplosionEmitter::destroy(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt) {
	if (index >= 20) {
		alive = false;
	}
}

void ExplosionEmitter::update(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt) {

}
#endif EXPLOSION_EMITTER_HPP