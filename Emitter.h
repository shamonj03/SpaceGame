#pragma once
#include <glm\glm.hpp>

class Emitter {

public:
	int genRate;
	int maxParticles;

	glm::vec3* vertices;

	Emitter(int genRate_, int maxParticles_) : genRate(genRate_), maxParticles(maxParticles_) {}

	virtual bool emit(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life) = 0;
	virtual bool update(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life) { return true; }
};