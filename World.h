#ifndef WORLD_H
#define WORLD_H

#include <GL\glew.h>
#include <vector>

class World {
public:
	class BoundingBox2D * bounds3D;
	class QuadTree* tree;
	std::vector<class Boid*> flock;
	class Player* player;

	std::vector<class ParticleSystem*> emitters;

	World();
	~World();

	void addEmitter(class ParticleSystem* emitter, GLfloat shader);
};

#endif // WORLD_H