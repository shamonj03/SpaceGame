#include <vector>
#include <algorithm>

#include <iostream>
#include <glm\glm.hpp>
#include <GL/glew.h>

#include "Util.h"
#include "Shader.h"

#include "Boid.h"
#include "World.h"
#include "Node.h"
#include "BoundingBox2D.h"
#include "QuadTree.h"
#include "ParticleSystem.h"
#include "Player.h"

Boid::Boid(World* world_, GLfloat shader_, glm::vec3 position_) :
	Ship(world_, shader_, position_),
	maxSepartion(1.5f),
	fov(2.0f) {
	velocity = glm::vec3(-5 + (Util::randf() * 10), -5 + (Util::randf() * 10), 0);
}

Boid::~Boid() {
}

void Boid::initializeBuffers(GLfloat shader_) {
	colors = {
		glm::vec4(0, 1, 0, 1),
		glm::vec4(0, 1, 0, 1),
		glm::vec4(0, 1, 0, 1),
		glm::vec4(0, 1, 0, 1) };
	Ship::initializeBuffers(shader_);
}

void Boid::update(float dt) {
	acceleration += seperation() * 2.5f;
	acceleration += alignment() * 1.0f;
	acceleration += cohesion() * 1.0f;

	center = position + glm::vec3(size / 2.0f, size / 2.0f, 0);


	velocity += acceleration * dt;
	velocity = Util::limit(velocity, maxSpeed);
	position += velocity * dt;

	acceleration *= 0;


	glm::vec3 dir = glm::normalize(velocity);
	angle = glm::degrees(glm::atan(-dir.y, -dir.x)) + 90;

	if (position.x > world->bounds3D->top.x) {
		position.x = world->bounds3D->bottom.x;
	}
	if (position.x < world->bounds3D->bottom.x) {
		position.x = world->bounds3D->top.x;
	}

	if (position.y > world->bounds3D->top.y) {
		position.y = world->bounds3D->bottom.y;
	}
	if (position.y < world->bounds3D->bottom.y) {
		position.y = world->bounds3D->top.y;
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



	float dist2 = glm::distance(center, world->player->position);
	if ((dist2 > 0) && (dist2 < maxSepartion)) {
		glm::vec3 diff = glm::normalize(world->player->position - center);
		steer = (diff / dist2);
		count = 1;
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


	float dist2 = glm::distance(center, world->player->position);
	if ((dist2 > 0) && (dist2 < fov * 5)) {
		sum = world->player->position;
		count = 1;
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