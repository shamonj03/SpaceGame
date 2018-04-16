#pragma once

#include "Util.hpp"
#include "Emitter.h"

class ExplosionEmitter : public Emitter {
public:
	glm::vec3 target;
	int index;

	ExplosionEmitter(glm::vec3 target_, int genRate_, int maxParticles_) : Emitter(genRate_, maxParticles_), target(target_), index(0) {}
	~ExplosionEmitter() {}

	virtual bool emit(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, glm::vec3* verticies);
};

bool ExplosionEmitter::emit(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, glm::vec3* verticies) {
	int slices = 20;

	if (index > 300) {
		return false;
	}
	index++;

	position = target;
	velocity = glm::vec3(0, -7, 0);
	color = glm::vec4(1, 1, 1, 1);
	life = 0.25f;

	int slice = (index) % slices;
	float angle = (360.0f / slices) * slice;

	Util::rotate(verticies, 4, angle);
	Util::rotate(velocity, angle);
	return true;
}
