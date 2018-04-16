#pragma once
#include <glm\glm.hpp>

class Emitter {

public:
	virtual void emit(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, glm::vec3* verticies) = 0;
	virtual void update(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, glm::vec3* verticies) {}
};