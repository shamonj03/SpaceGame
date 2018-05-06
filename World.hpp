#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include "BoundingBox2D.h"
#include "QuadTree.hpp"

class Boid;

class World {
public:
	static std::vector<BoundingBox2D*> obstacles3D;
	static std::vector<Boid*> flock;
	static BoundingBox2D* bounds3D;
	static QuadTree* tree;
};

BoundingBox2D* World::bounds3D = new BoundingBox2D(glm::vec2(-50, -50), glm::vec2(50, 50));
std::vector<BoundingBox2D*> World::obstacles3D;
std::vector<Boid*> World::flock;
QuadTree* World::tree = new QuadTree(*World::bounds3D);

#endif // WORLD_HPP