
#include "World.h"
#include "QuadTree.h"
#include "BoundingBox2D.h"
#include "Boid.h"


World::World() :
	bounds3D(new BoundingBox2D(glm::vec2(-50, -50), glm::vec2(50, 50))),
	tree(new QuadTree(World::bounds3D)) {
}

World::~World() {}