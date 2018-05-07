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

public:
	class World * world;

	GLuint shader;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 normal;
	glm::vec4 color;

	bool alive;
	float angle;
	float maxSpeed;
	float maxForce;
	float mass;
	float size;

	Entity(class World* world, GLfloat shader, glm::vec3 position_);
	~Entity();

	virtual void update(float dt) = 0;
	virtual void draw(float dt) = 0;
	virtual void initializeBuffers(GLfloat shader_) = 0;
};
#endif // ENTITY_H