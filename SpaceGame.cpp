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
	Boid* b;

	Mesh* grid;
	SpaceGame(float width, float height);
	virtual void update(float dt);
	virtual void drawStandard(float dt);
	virtual void drawTextured(float dt);
	virtual void onKeyDown(SDL_KeyboardEvent& e);
	virtual void onKeyUp(SDL_KeyboardEvent& e);

	void drawTree(QuadTree* tree) {
		if (tree->isLeaf()) {
			glUseProgram(0);
			glPushMatrix();
			glColor4f(0, 1, 1, 1);
			glTranslatef(tree->box.bottom.x, tree->box.bottom.y, 0);
			glScalef(tree->box.dimensions.x, tree->box.dimensions.y, 1);
			glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(1.0, 0.0, 0.0);

			glVertex3f(1.0, 0.0, 0.0);
			glVertex3f(1.0, 1.0, 0.0);

			glVertex3f(1.0, 1.0, 0.0);
			glVertex3f(0.0, 1.0, 0.0);

			glVertex3f(0.0, 1.0, 0.0);
			glVertex3f(0.0, 0.0, 0.0);
			glEnd();
			glPopMatrix();
		} else {
			for (int i = 0; i < CHILD_COUNT; i++) {
				drawTree(tree->children[i]);
			}
		}
	}
};

SpaceGame::SpaceGame(float width, float height) : Engine(width, height) {
	SDL_ShowCursor(SDL_FALSE);
	srand(time(NULL));
	camera->enabled = false;
	grid = new Mesh(100, 100, 1.0f);
	player = new Player(glm::vec3(0, 0, 0));

	grid->initializeBuffers(standardShader);
	player->initializeBuffers(standardShader);

	for (int i = 0; i < 200; i++) {
		int x = -50 + (rand() % 100);
		int y = -50 + (rand() % 100);
		Boid* boid = new Boid(glm::vec3(x, y, 0));
		boid->initializeBuffers(standardShader);

		//emitter = new StandardEmitter(boid, 1, 2);
		//particles->addEmitter(emitter);
		
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

	player->update(dt);

	for (auto ent : World::flock) {
		ent->update(dt);
	}


	camera->position = player->position;
	camera->updateView();
}


void SpaceGame::drawStandard(float dt) {
	glUseProgram(standardShader);

	for (auto ent : World::flock) {
		b = static_cast<Boid*>(ent);
		b->draw(dt);
	}

	player->draw(dt);

	glUseProgram(0);
	drawTree(World::tree);

	glUseProgram(standardShader);
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