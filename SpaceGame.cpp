// SpaceGame.cpp : Defines the entry point for the console application.
//
#include "SpaceGame.h"
#include <ctime>
#include <vector>
#include <algorithm>

#include "QuadTree.h"
#include "Mesh.h"
#include "Boid.h"
#include "Player.h"
#include "ParticleSystem.h"
#include "BoundingBox2D.h"
#include "World.h"
#include "Camera.hpp"
#include "Shader.h"
#include "ExplosionEmitter.hpp"

SpaceGame::SpaceGame(float width, float height) : Engine(width, height) {
	SDL_ShowCursor(SDL_FALSE);
	srand(time(NULL));

	camera->enabled = false;

	world = new World();
	grid = new Mesh(100, 100, 1.0f);
	grid->initializeBuffers(standardShader);

	world->player = new Player(world, standardShader, glm::vec3(0, 0, 0));
	world->player->initializeBuffers(standardShader);

	Boid* boid;
	for (int i = 0; i < 200; i++) {
		int x = -45 + (rand() % 90);
		int y = -45 + (rand() % 90);
		boid = new Boid(world, standardShader, glm::vec3(x, y, 0));
		boid->initializeBuffers(standardShader);

		world->flock.push_back(boid); 
		world->tree->insert(boid);
	}

	glm::vec2 test(0, 0);
	test = test + 2.0f;
	std::cout << test.x << " " << test.y << std::endl;
}

void SpaceGame::drawTree(QuadTree* tree) {
	if (tree->isLeaf()) {
		glUseProgram(0);
		glPushMatrix();
		glColor4f(0, 1, 1, 1);
		glTranslatef(tree->box->bottom.x, tree->box->bottom.y, 0);
		glScalef(tree->box->dimensions.x, tree->box->dimensions.y, 1);
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
	}
	else {
		for (int i = 0; i < CHILD_COUNT; i++) {
			drawTree(tree->children[i]);
		}
	}
}

void SpaceGame::onKeyDown(SDL_KeyboardEvent& e) {
	if (paused) {
		world->player->onKeyDown(e);
	}
}

void SpaceGame::onKeyUp(SDL_KeyboardEvent& e) {
	if (paused) {
		world->player->onKeyUp(e);
	}
}


void SpaceGame::onMouseButtonDown(SDL_MouseButtonEvent& e) {
	if (paused) {
		world->player->onMouseButtonDown(e);
	}
}

void SpaceGame::update(float dt) {
	delete world->tree;
	world->tree = new QuadTree(world->bounds3D);

	for (auto ent : world->flock) {
		world->tree->insert(ent);
	}

	world->player->update(dt);

	for (auto ent : world->flock) {
		ent->update(dt);

		if (!ent->alive) {
			world->player->addEmitter(new ExplosionEmitter(ent->position, 20, 20), standardShader);
		}
	}

	for (auto ent : world->flock) {
		if (!ent->alive) {
			world->flock.erase(std::remove(world->flock.begin(), world->flock.end(), ent), world->flock.end());
		}
	}

	camera->position = world->player->position;
	camera->updateView();
}


void SpaceGame::drawStandard(float dt) {
	BoundingBox2D* view = new BoundingBox2D(world->player->position, 15);

	glUseProgram(0);
	glPushMatrix();
	glColor4f(0, 0, 1, 1);
	glTranslatef(view->bottom.x, view->bottom.y, 0);
	glScalef(view->dimensions.x, view->dimensions.y, 1);
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

	glUseProgram(standardShader);

	std::vector<Node*> neighbors;
	world->tree->withinArea(view, neighbors);

	for (auto ent : neighbors) {
		b = static_cast<Boid*>(ent);
		b->draw(dt);
	}

	glUseProgram(standardShader);
	world->player->draw(dt);

	glUseProgram(0);
	drawTree(world->tree);

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