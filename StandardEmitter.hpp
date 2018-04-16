#ifndef STANARD_EMITTER_HPP
#define STANARD_EMITTER_HPP

#include "Util.hpp"
#include "Emitter.h"
#include "Player.hpp"

class StandardEmitter : public Emitter {
public:
	Player* player;

	StandardEmitter(Player* player_, int genRate_, int maxParticles_) : Emitter(genRate_, maxParticles_), player(player_) {}
	~StandardEmitter() {}

	virtual bool emit(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, glm::vec3* verticies);
};

bool StandardEmitter::emit(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, glm::vec3* verticies) {
	position = player->position;
	velocity = glm::vec3(0, -1, 0);
	color = glm::vec4(1, 1, 1, 1);
	life = 1.0f;
	Util::rotate(verticies, 4, player->angle);
	Util::rotate(velocity, player->angle);
	return true;
}

#endif // STANARD_EMITTER_HPP