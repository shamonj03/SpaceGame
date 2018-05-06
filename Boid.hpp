#ifndef BOID_HPP
#define BOID_HPP


#include <iostream>
#include <glm\glm.hpp>
#include <vector>
#include <GL/glew.h>
#include "Shader.h"
#include "Entity.hpp"
#include "Util.hpp"
#include "World.hpp"


class Boid : public Entity {
private:
	static glm::vec3 vertices[4];
	static GLuint indices[6];
	static GLfloat colors[16];

public:
	float maxSepartion;
	float fov;

	glm::vec3 center;

	Boid(glm::vec3 position_);
	~Boid();

	virtual void initializeBuffers(GLuint shader);
	virtual void update(float dt);
	virtual void draw(float dt);

	glm::vec3 seperation();
	glm::vec3 alignment();
	glm::vec3 cohesion();
};


/*
* Each line is a unique vertex of the polygon.
*/
glm::vec3 Boid::vertices[4] = {
	glm::vec3(-0.5f, -0.5f, 0.0f) * 0.5f,
	glm::vec3(0.0f, -0.25f, 0.0f) * 0.5f,
	glm::vec3(0.0f, 0.75f, 0.0f) * 0.5f,
	glm::vec3(0.5f, -0.5f, 0.0f) * 0.5f,
};

/*
* The order to draw the vertices.
*/
GLuint Boid::indices[6] = {
	0, 1, 2, 2, 3, 1
};

/*
* Each line is a color for that vertex.
*/
GLfloat Boid::colors[4 * 4] = {
	0, 1, 0, 1,
	0, 1, 0, 1,
	0, 1, 0, 1,
	0, 1, 0, 1,
};

Boid::Boid(glm::vec3 position_) :
	Entity(position_),
	maxSepartion(1.0f),
	fov(5.0f) {
	velocity = glm::vec3(-5 + rand() % 5, -5 + (rand() % 5), 0);
	color = glm::vec4(Util::randf(), Util::randf(), Util::randf(), 1.0f);
}


Boid::~Boid() {
}


inline void Boid::initializeBuffers(GLuint shader) {
	glUseProgram(shader);
	Shader::bindArray(GL_ELEMENT_ARRAY_BUFFER, indexBuffer, sizeof(indices), &indices[0], GL_STATIC_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, vertexBuffer, sizeof(glm::vec3) * sizeof(Boid::vertices), &Boid::vertices[0], GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, positionBuffer, sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, colorBuffer, sizeof(colors), &colors[0], GL_STATIC_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, normalBuffer, sizeof(glm::vec3), &normal, GL_STATIC_DRAW);
	glUseProgram(0);
}



inline void Boid::draw(float dt) {
	glPushMatrix();
	Shader::sendArray(0, 3, vertexBuffer);
	Shader::sendArray(1, 3, normalBuffer);
	Shader::sendArray(2, 3, positionBuffer, sizeof(glm::vec3), &position[0], GL_STREAM_DRAW);
	//Shader::sendArray(2, 2, texCoordBuffer);
	Shader::sendArray(3, 4, colorBuffer);


	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1); // normal : one per quad -> 1
	glVertexAttribDivisor(2, 1); // positions : one per quad (its center) -> 1
	glVertexAttribDivisor(3, 1); // color : one per quad -> 1


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glPopMatrix();
}


inline void Boid::update(float dt) {
	glm::vec3 force;

	acceleration += seperation() * 2.5f;
	acceleration += alignment() * 1.0f;
	acceleration += cohesion() * 1.0f;


	for (auto obj : World::obstacles3D) {
		float dist = glm::distance(position, glm::vec3(obj->center, 0));

		if ((dist > 0) && (dist < obj->dimensions.x + (size * 2.0f))) {
			glm::vec3 diff = glm::normalize(position - glm::vec3(obj->center, 0));
			position += diff * dist * dt;
		}
	}

	velocity += acceleration * dt;
	position += velocity * dt;
	center = position + glm::vec3(size / 2.0f, size / 2.0f, 0);
	acceleration *= 0;

	if (position.x > World::bounds3D->top.x) {
		position.x = World::bounds3D->bottom.x;
	}
	if (position.y > World::bounds3D->top.y) {
		velocity *= -1.25f;
	//	position.y = World::bounds3D->bottom.y;
	}
	//if (position.z > World::bounds3D->top.z) {
	//	position.z = World::bounds3D->bottom.z;
	//}

	if (position.x < World::bounds3D->bottom.x) {
		position.x = World::bounds3D->top.x;
	}
	if (position.y < World::bounds3D->bottom.y) {
		velocity *= -1.25f;
	//	position.y = World::bounds3D->bottom.y;
	}
	//if (position.z < World::bounds3D->bottom.z) {
	//	position.z = World::bounds3D->top.z;
	//}
}

inline glm::vec3 Boid::seperation() {
	glm::vec3 steer(0, 0, 0);
	int count = 0;

	std::vector<Node*> neighbors;
	World::tree->withinArea(BoundingBox2D(position, maxSepartion), neighbors);
	//std::cout << neighbors.size() << std::endl;
	for (auto boid : neighbors) {
	//std::vector<Boid*> neighbors;
		if (boid == this) {
			continue;
		}
		float dist = glm::distance(center, boid->position);

		if ((dist > 0) && (dist < maxSepartion)) {
			glm::vec3 diff = glm::normalize(center - boid->position);
			steer += (diff / dist);
			count++;
		}
	}

	if (count > 0) {
		steer /= (float) count;
	}

	if (glm::length(steer) > 0) {
		steer = glm::normalize(steer);
		steer = (steer * maxSpeed) - velocity;
	}
	return steer;
}

inline glm::vec3 Boid::alignment() {
	glm::vec3 sum(0, 0, 0);
	int count = 0;
	Boid* b;

	std::vector<Node*> neighbors;
	World::tree->withinArea(BoundingBox2D(position, maxSepartion), neighbors);
	for (auto boid : neighbors) {
		if (boid == this) {
			continue;
		}
		float dist = glm::distance(center, boid->position);

		if ((dist > 0) && (dist < fov)) {
			b = static_cast<Boid*>(boid);
			sum += b->velocity;
			count++;
		}
	}
	if (count > 0) {
		sum /= (float) count;
		sum = glm::normalize(sum) * maxSpeed;

		glm::vec3 steer(0, 0, 0);
		steer = sum - velocity;
		return steer;
	} else {
		return glm::vec3(0, 0, 0);
	}
}

inline glm::vec3 Boid::cohesion() {
	glm::vec3 sum(0, 0, 0);
	int count = 0;

	std::vector<Node*> neighbors;
	World::tree->withinArea(BoundingBox2D(position, fov), neighbors);
	for (auto boid : neighbors) {
		if (boid == this) {
			continue;
		}
		float dist = glm::distance(center, boid->position);

		if ((dist > 0) && (dist < fov)) {
			sum += boid->position;
			count++;
		}
	}
	if (count > 0) {
		glm::vec3 steer(0, 0, 0);
		sum /= (float) count;

		sum = glm::normalize(sum - center) * maxSpeed;
		steer = sum - velocity;
		return steer;
	} else {
		return glm::vec3(0, 0, 0);
	}
}
#endif // BOID_HPP