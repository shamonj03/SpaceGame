#ifndef BOUNDBOX_H
#define BOUNDBOX_H

#include <glm\glm.hpp>

class BoundingBox {
public:
	glm::vec3 center;
	glm::vec3 top;
	glm::vec3 bottom;
	glm::vec3 dimensions;

	// For Spheres
	BoundingBox(glm::vec3 ceneter_, float r_) : center(ceneter_), dimensions(glm::vec3(r_, r_, r_)) {
		top = ceneter_ + dimensions;
		bottom = ceneter_ - dimensions;
	}

	// For cubes
	BoundingBox(glm::vec3 bottom_, glm::vec3 top_) : top(top_), bottom(bottom_) {
		dimensions = glm::abs(top - bottom);
	}

	bool inBounds(glm::vec3 pos) {
		return (pos.x >= bottom.x && pos.y >= bottom.y && pos.z >= bottom.z) && (pos.x <= top.x && pos.y <= top.y && pos.z <= top.z);
	}

	bool intersect(BoundingBox other) {
		if (bottom.x > other.top.x || bottom.y > other.top.y || bottom.z > other.top.z) {
			return false;
		}
		if (top.x < other.bottom.x || top.y < other.bottom.y || top.z < other.bottom.z) {
			return false;
		}
		return true;
	}


	bool operator == (const BoundingBox& other) const {
		return bottom.x == other.bottom.x && bottom.y == other.bottom.y && bottom.z == other.bottom.z
		&& top.x == other.top.x && top.y == other.top.y && top.z == other.top.z;
	}
};

#endif // BOUNDBOX_H