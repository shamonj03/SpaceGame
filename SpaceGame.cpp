// SpaceGame.cpp : Defines the entry point for the console application.
//
#pragma once
#include <ctime>

#include "Engine.h"
#include "Player.hpp"

class SpaceGame : public Engine {

public:
	Player * player;
	SpaceGame(float width, float height);
	virtual void update(float dt);
	virtual void drawStandard(float dt);
	virtual void drawTextured(float dt);
	virtual void onKeyDown(SDL_KeyboardEvent& e);
};

SpaceGame::SpaceGame(float width, float height) : Engine(width, height) {
	SDL_ShowCursor(SDL_FALSE);
	srand(time(NULL));
	camera->enabled = false;
	player = new Player(glm::vec3(0, 0, 0));
	player->initializeBuffers(standardShader);
}


void SpaceGame::onKeyDown(SDL_KeyboardEvent& e) {
	player->onKeyDown(e);
}


void SpaceGame::update(float dt) {
	player->update(dt);
}


void SpaceGame::drawStandard(float dt) {
	glPushMatrix();
	player->draw(dt);
	glPopMatrix();


	/*glPushMatrix();
	glUseProgram(0);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f(0.0f, -0.25f);
	glVertex2f(0.0f, 0.75f);
	glVertex2f(0.5f, -0.5f);
	glVertex2f(0.0f, -0.25f);
	glVertex2f(0.0f, 0.75f);
	glEnd();
	glPopMatrix();*/
}

void SpaceGame::drawTextured(float dt) {

}

int main(int argc, char *argv[]) {
	SpaceGame game(800, 600);
	game.start();
    return EXIT_SUCCESS;
}

