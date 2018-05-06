#include <vector>

#include <iostream>
#include <glm\glm.hpp>
#include <GL/glew.h>

#include "Util.hpp"
#include "Shader.h"

#include "Boid.h"
#include "World.h"
#include "Node.h"
#include "BoundingBox2D.h"
#include "QuadTree.h"
#include "ParticleSystem.h"

Boid::Boid(World* world_, glm::vec3 position_) :
	Ship(world_, position_),
	maxSepartion(1.0f),
	fov(2.0f) {
	velocity = glm::vec3(-5 + (Util::randf() * 10), -5 + (Util::randf() * 10), 0);
	colors = new GLfloat[16]{
		0, 1, 0, 1,
		0, 1, 0, 1,
		0, 1, 0, 1,
		0, 1, 0, 1 };
}

Boid::~Boid() {
}


void Boid::initializeBuffers(GLuint shader_) {
	shader = shader_;
	for (int i = 0; i < 4; i++) {
		vertices[i] *= size;
	}
	glUseProgram(shader);
	for (auto particle : emitters) {
		particle->initializeBuffers(shader);
	}
	Shader::bindArray(GL_ELEMENT_ARRAY_BUFFER, indexBuffer, sizeof(indices), &indices[0], GL_STATIC_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, vertexBuffer, sizeof(glm::vec3) * sizeof(vertices), &vertices[0], GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, positionBuffer, sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, colorBuffer, 4 * sizeof(vertices), &colors[0], GL_STATIC_DRAW);
	Shader::bindArray(GL_ARRAY_BUFFER, normalBuffer, sizeof(glm::vec3), &normal, GL_STATIC_DRAW);
	glUseProgram(0);
}

void Boid::update(float dt) {
	glm::vec3 force;

	acceleration += seperation() * 2.5f;
	acceleration += alignment() * 1.0f;
	acceleration += cohesion() * 1.0f;

	velocity += acceleration * dt;
	position += velocity * dt;
	center = position + glm::vec3(size / 2.0f, size / 2.0f, 0);
	acceleration *= 0;

	if (position.x > world->bounds3D->top.x) {
		position.x = world->bounds3D->bottom.x;
	}
	if (position.y > world->bounds3D->top.y) {
		velocity *= -1.25f;
	//	position.y = world->bounds3D->bottom.y;
	}
	//if (position.z > world->bounds3D->top.z) {
	//	position.z = world->bounds3D->bottom.z;
	//}

	if (position.x < world->bounds3D->bottom.x) {
		position.x = world->bounds3D->top.x;
	}
	if (position.y < world->bounds3D->bottom.y) {
		velocity *= -1.25f;
	//	position.y = world->bounds3D->bottom.y;
	}
	//if (position.z < world->bounds3D->bottom.z) {
	//	position.z = world->bounds3D->top.z;
	//}
	

	glm::vec3 dir = glm::normalize(velocity);
	angle = glm::degrees(glm::atan(-dir.y, -dir.x)) + 90;
	Util::rotate(Boid::vertices, 4, angle);
	Shader::bindArray(GL_ARRAY_BUFFER, vertexBuffer, sizeof(glm::vec3) * sizeof(Boid::vertices), &Boid::vertices[0], GL_STREAM_DRAW);
	Util::rotate(Boid::vertices, 4, -angle);

	
	for (auto particle : emitters) {
		particle->generate(dt);
		particle->update(dt);
		particle->decay(dt);

		if (!particle->alive) {
			//emitters.erase(std::remove(emitters.begin(), emitters.end(), particle), emitters.end());
		}
	}
}

glm::vec3 Boid::seperation() {
	glm::vec3 steer(0, 0, 0);
	int count = 0;

	std::vector<Node*> neighbors;
	world->tree->withinArea(new BoundingBox2D(position, maxSepartion), neighbors);

	for (auto boid : neighbors) {
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
	angle = atan2(steer.y, steer.x);
	return steer;
}

glm::vec3 Boid::alignment() {
	glm::vec3 sum(0, 0, 0);
	int count = 0;
	Boid* b;

	std::vector<Node*> neighbors;
	world->tree->withinArea(new BoundingBox2D(position, maxSepartion), neighbors);
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

glm::vec3 Boid::cohesion() {
	glm::vec3 sum(0, 0, 0);
	int count = 0;

	std::vector<Node*> neighbors;
	world->tree->withinArea(new BoundingBox2D(position, fov), neighbors);
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