#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <GL/glew.h>
#include <glm\glm.hpp>
#include "Entity.h"

class Ship : public Entity {
protected:
	glm::vec3 vertices[4];
	GLuint indices[6];
	GLfloat* colors;

public:
	std::vector<class ParticleSystem*> emitters;

	Ship(class World* world_, glm::vec3 position_);
	~Ship();

	virtual void initializeBuffers(GLuint shader);
	virtual void update(float dt);
	virtual void draw(float dt);
};
#endif // SHIP_H