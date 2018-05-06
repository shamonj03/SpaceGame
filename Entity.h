#ifndef ENTITY_H
#define ENTITY_H

#include <glm\glm.hpp>
#include <GL\glew.h>
#include "Node.h"

class Entity : public Node {
protected:
	GLuint indexBuffer;
	GLuint vertexBuffer;
	GLuint positionBuffer; 
	GLuint texCoordBuffer;
	GLuint normalBuffer;
	GLuint colorBuffer;
	GLuint shader;

public:
	class World * world;

	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 normal;
	glm::vec4 color;

	float angle;
	float maxSpeed;
	float maxForce;
	float mass;
	float size;

	Entity(class World* world, glm::vec3 position_);
	~Entity() {}

	virtual void initializeBuffers(GLuint shader) = 0;
	virtual void update(float dt) = 0;
	virtual void draw(float dt) = 0;
};
#endif // ENTITY_H