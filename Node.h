#ifndef NODE_H
#define NODE_H

#include <glm\glm.hpp>

class Node {
public:
	glm::vec3 position;

	Node(glm::vec3 position_);
	~Node();

	bool operator == (const Node& node) const {
		return position.x == node.position.x && position.y == node.position.y && position.z == node.position.z;
	}
};

#endif // NODE_H