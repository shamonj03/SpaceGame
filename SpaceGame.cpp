// SpaceGame.cpp : Defines the entry point for the console application.
//
#pragma once
#include <ctime>

#include "Engine.h"
#include "Mesh.h"

#include "World.hpp"
#include "Boid.hpp"
#include "Player.hpp"
#include "ParticleSystem.hpp"
#include "StandardEmitter.hpp"
#include "ExplosionEmitter.hpp"

class SpaceGame : public Engine {

public:
	Player* player;
	ParticleSystem* particles;
	StandardEmitter* emitter;
	ExplosionEmitter* explosion;

	Mesh* grid;
	SpaceGame(float width, float height);
	virtual void update(float dt);
	virtual void drawStandard(float dt);
	virtual void drawTextured(float dt);
	virtual void onKeyDown(SDL_KeyboardEvent& e);
	virtual void onKeyUp(SDL_KeyboardEvent& e);
};

SpaceGame::SpaceGame(float width, float height) : Engine(width, height) {
	SDL_ShowCursor(SDL_FALSE);
	srand(time(NULL));
	camera->enabled = false;
	grid = new Mesh(100, 100, 1.0f);
	player = new Player(glm::vec3(0, 0, 0));

	emitter = new StandardEmitter(player, 5, 50);
	explosion = new ExplosionEmitter(glm::vec3(2, 2, 0), 15, 300);

	particles = new ParticleSystem();
	particles->addEmitter(emitter);
	particles->addEmitter(explosion);

	grid->initializeBuffers(standardShader);
	player->initializeBuffers(standardShader);
  	particles->initializeBuffers(standardShader);

	for (int i = 0; i < 200; i++) {
		int x = -50 + (rand() % 100);
		int y = -50 + (rand() % 100);
		Boid* boid = new Boid(glm::vec3(x, y, 0));
		boid->initializeBuffers(standardShader);
		
		World::flock.push_back(boid);
	}
}


void SpaceGame::onKeyDown(SDL_KeyboardEvent& e) {
	if (paused) {
		player->onKeyDown(e);
	}
}

void SpaceGame::onKeyUp(SDL_KeyboardEvent& e) {
	if (paused) {
		player->onKeyUp(e);
	}
}

void SpaceGame::update(float dt) {
	delete World::tree;
	World::tree = new QuadTree(*World::bounds3D);

	for (auto ent : World::flock) {
		World::tree->insert(ent);
	}

	particles->generate(dt);
	particles->decay(dt);
	particles->update(dt);

	player->update(dt);

	for (auto ent : World::flock) {
		ent->update(dt);
	}


	camera->position = player->position;
	camera->updateView();
}


void SpaceGame::drawStandard(float dt) {
	particles->draw(dt);

	for (auto ent : World::flock) {
		ent->draw(dt);
	}

	player->draw(dt);


	grid->draw(GL_LINES);
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
	SpaceGame game(800, 450);
	game.start();
    return EXIT_SUCCESS;
}