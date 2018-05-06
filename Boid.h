#ifndef BOID_H
#define BOID_H

#include <glm\glm.hpp>

#include "Ship.h"

class Boid : public Ship {

public:
	float maxSepartion;
	float fov;

	glm::vec3 center;

	Boid(class World* world_, glm::vec3 position_);
	~Boid();

	virtual void initializeBuffers(GLuint shader_);
	virtual void update(float dt);

	glm::vec3 seperation();
	glm::vec3 alignment();
	glm::vec3 cohesion();
};

#endif // BOID_H