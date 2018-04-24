#ifndef STANARD_EMITTER_HPP
#define STANARD_EMITTER_HPP

#include "Util.hpp"
#include "Emitter.h"
#include "Player.hpp"

class StandardEmitter : public Emitter {
public:
	Player* player;

	StandardEmitter(Player* player_, int genRate_, int maxParticles_) : Emitter(genRate_, maxParticles_), player(player_) {
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
	~StandardEmitter() {}

	virtual bool emit(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life);
};

inline bool StandardEmitter::emit(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life) {
	position = player->position;
	velocity = glm::vec3(0, -1, 0) + glm::vec3(-1.0f + (Util::randf() * 2.0f), 0, 0);
	color = glm::vec4(1, 1, 1, 1);
	life = 0.1f;
	Util::rotate(vertices, 4, player->angle);
	Util::rotate(velocity, player->angle);
	return true;
}

#endif // STANARD_EMITTER_HPP