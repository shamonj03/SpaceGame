#ifndef WORLD_H
#define WORLD_H

#include <vector>

class World {
public:
	class BoundingBox2D * bounds3D;
	class QuadTree* tree;
	std::vector<class Boid*> flock;

	World();
	~World();
};

#endif // WORLD_H