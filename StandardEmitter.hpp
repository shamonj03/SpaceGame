#ifndef STANARD_EMITTER_HPP
#define STANARD_EMITTER_HPP

#include "Util.h"
#include "ParticleSystem.h"
#include "Entity.h"

class StandardEmitter : public ParticleSystem {
public:
	Entity* player;

	StandardEmitter(Entity* player_, int genRate_, int maxParticles_) : ParticleSystem(genRate_, maxParticles_), player(player_) {
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

	virtual void emit(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt);
};

inline void StandardEmitter::emit(glm::vec3& position, glm::vec3& velocity, glm::vec4& color, float& life, float& dt) {
	position = player->position;
	velocity = glm::vec3(0, -1, 0) + glm::vec3(-1.0f + (Util::randf() * 2.0f), 0, 0);
	color = glm::vec4(1, 1, 1, 1);
	life = 0.1f;


	glm::vec3 dir = glm::normalize(velocity);
	float angle = glm::degrees(glm::atan(-dir.y, -dir.x)) + 90;

	Util::rotate(vertices, 4, angle);
	Util::rotate(velocity, angle);
}

#endif // STANARD_EMITTER_HPP