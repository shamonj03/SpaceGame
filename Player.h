#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include "Ship.h"

class Player : public Ship {

public:
	bool released;

	Player(class World* world, glm::vec3 position_);
	~Player();

	virtual void onKeyDown(SDL_KeyboardEvent& e);
	virtual void onKeyUp(SDL_KeyboardEvent& e);

	virtual void update(float dt);
};

#endif // PLAYER_H