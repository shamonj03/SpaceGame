#pragma once
#include <glm\glm.hpp>

class Emitter {

public:
	int genRate;
	int maxParticles;

	Emitter(int genRate_, int maxParticles_) : genRate(genRate_), maxParticles(maxParticles_) {}

	virtual bool emit(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, glm::vec3* verticies) = 0;
	virtual bool update(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, glm::vec3* verticies) { return true; }
};