#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Node.h"

class Entity : public Node {
protected:
	GLuint indexBuffer;
	GLuint vertexBuffer;
	GLuint positionBuffer; 
	GLuint texCoordBuffer;
	GLuint normalBuffer;
	GLuint colorBuffer;

public:
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 normal;
	glm::vec4 color;

	float angle;

	Entity(glm::vec3 position_);
	~Entity() {}

	virtual void initializeBuffers(GLuint shader) = 0;
	virtual void update(float dt) = 0;
	virtual void draw(float dt) = 0;
};

Entity::Entity(glm::vec3 position_) : Node(position_), angle(0), velocity(0), acceleration(0), color(1), normal(glm::vec3(0, 1, 0)) {

}

#endif // ENTITY_HPP