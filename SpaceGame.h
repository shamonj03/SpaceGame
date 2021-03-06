// SpaceGame.cpp : Defines the entry point for the console application.
//
#ifndef SPACE_GAME_H
#define SPACE_GAME_H

#include "Engine.h"

class SpaceGame : public Engine {

public:
	class World* world;
	class Boid* b;

	class Mesh* grid;
	SpaceGame(float width, float height);
	virtual void update(float dt);
	virtual void drawStandard(float dt);
	virtual void drawTextured(float dt);
	virtual void onKeyDown(SDL_KeyboardEvent& e);
	virtual void onKeyUp(SDL_KeyboardEvent& e);
	virtual void onMouseButtonDown(SDL_MouseButtonEvent& e);

	void drawTree(class QuadTree* tree);
};

#endif // SPACE_GAME_H