#include "Entity.h"
#include "World.h"
#include "Node.h"

Entity::Entity(World* world_, GLfloat shader_, glm::vec3 position_) : Node(position_), shader(shader_), world(world_), alive(true), maxSpeed(15.0f), maxForce(0.9f), angle(0), velocity(0), acceleration(0), color(1), mass(1), size(0.5f), normal(glm::vec3(0, 1, 0)) {

}

Entity::~Entity() {

}