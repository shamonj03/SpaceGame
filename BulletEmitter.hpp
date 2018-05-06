#ifndef BULLET_EMITTER_HPP
#define BULLET_EMITTER_HPP

#include <iostream>

#include "ParticleSystem.h"
#include "Util.hpp"
#include "Entity.h"
#include "Boid.h"

class BulletEmitter : public ParticleSystem {
public:
	class Entity * player;

	BulletEmitter(class Entity* player_) : ParticleSystem(1, 1), player(player_) {
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
};

inline void BulletEmitter::emit(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt) {
	position = player->position;
	velocity = glm::vec3(0, 30, 0);
	color = glm::vec4(1, 1, 1, 1);
	life = 1.0f;
	Util::rotate(vertices, 4, player->angle);
	Util::rotate(velocity, player->angle);
}


inline void BulletEmitter::update(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt) {
	for (auto boid : player->world->flock) {
		glm::vec3 dp = glm::abs(position - boid->position);

		if (glm::length(dp) < 0.25f) {
			life = 0;
		}
	}

	if (life <= 0) {
		alive = false;
	}
}

#endif // BULLET_EMITTER_HPP