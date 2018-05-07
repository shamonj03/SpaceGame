
#include "World.h"
#include "QuadTree.h"
#include "BoundingBox2D.h"
#include "Boid.h"
#include "ParticleSystem.h"

World::World() :
	bounds3D(new BoundingBox2D(glm::vec2(-50, -50), glm::vec2(50, 50))),
	tree(new QuadTree(World::bounds3D)) {
}

World::~World() {}

void World::addEmitter(ParticleSystem* emitter, GLfloat shader) {
	emitters.push_back(emitter);
	emitter->initializeBuffers(shader);
}