#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <glm\glm.hpp>
#include <GL\glew.h>
#include <vector>

class ParticleSystem {
protected:
	glm::vec3* velocities;
	glm::vec3* positions;
	glm::vec4* colors;
	glm::vec3* vertices;
	float* lifes;
	glm::vec3 normal;

	int genRate;
	int maxParticles;
	int particleCount;

	GLuint indexBuffer;
	GLuint vertexBuffer;
	GLuint positionBuffer;
	GLuint normalBuffer;
	GLuint colorBuffer;
	GLuint vao;

	virtual void destroy(int index);

public:
	bool alive;

	ParticleSystem(int genRate_, int maxParticles_);
	~ParticleSystem();

	virtual void initializeBuffers(GLuint shader);
	virtual void decay(float dt);
	virtual void generate(float dt);
	virtual void update(float dt);
	virtual void draw(float dt);


	virtual void emit(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt) = 0;

	virtual void update(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt);

	virtual void destroy(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt);
};

#endif // PARTICLE_SYSTEM_H