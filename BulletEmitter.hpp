#ifndef BULLET_EMITTER_HPP
#define BULLET_EMITTER_HPP

#include <iostream>

#include "ParticleSystem.h"
#include "Util.h"
#include "Entity.h"
#include "Boid.h"

class BulletEmitter : public ParticleSystem {
public:
	class Ship * player;

	BulletEmitter(class Ship* player_) : ParticleSystem(1, 1), player(player_) {
		vertices = new glm::vec3[4]{
			glm::vec3(-0.5f, -0.5f, 0.0f),
			glm::vec3(0.5f, -0.5f, 0.0f),
			glm::vec3(-0.5f, 0.5f, 0.0f),
			glm::vec3(0.5f, 0.5f, 0.0f),
		};

		for (int i = 0; i < 4; i++) {
			vertices[i] *= 0.1f;
		}
	}
	~BulletEmitter() {}

	virtual void emit(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt);

	virtual void update(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt);

	virtual void destroy(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt);
};

void BulletEmitter::emit(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt) {
	position = player->position;
	velocity = glm::vec3(0, 30, 0);
	color = glm::vec4(1, 1, 1, 1);
	life = 0.5f;
	Util::rotate(vertices, 4, player->angle);
	Util::rotate(velocity, player->angle);
}


void BulletEmitter::update(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt) {
	for (auto boid : player->world->flock) {
		if (!boid) {
			continue;
		}
		glm::vec3 dp = glm::abs(position - boid->position);

		if (glm::length(dp) < 0.25f) {
			life = 0;
			boid->alive = false;
			break;
		}
	}
}

void BulletEmitter::destroy(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt) {
	alive = false;
}

#endif // BULLET_EMITTER_HPP