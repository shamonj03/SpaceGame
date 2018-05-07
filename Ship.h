#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <GL/glew.h>
#include <glm\glm.hpp>
#include "Entity.h"

class Ship : public Entity {
protected:
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	std::vector<glm::vec4> colors;

public:
	std::vector<class ParticleSystem*> emitters;

	Ship(class World* world_, GLfloat shader_, glm::vec3 position_);
	~Ship();

	virtual void update(float dt);
	virtual void draw(float dt);

	void addEmitter(class ParticleSystem* emitter, GLfloat Shader);

	virtual void initializeBuffers(GLfloat shader_);
};
#endif // SHIP_H