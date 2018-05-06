#ifndef BOUNDING_BOX2D_H
#define BOUNDING_BOX2D_H

#include <glm\glm.hpp>

class BoundingBox2D {
public:
	glm::vec2 top;
	glm::vec2 bottom;
	glm::vec2 dimensions;
	glm::vec2 center;

	BoundingBox2D(glm::vec2 bottom_, glm::vec2 top_) : top(top_), bottom(bottom_) {
		dimensions = glm::abs(top - bottom);
	}

	BoundingBox2D(glm::vec2 ceneter_, float r_) : center(ceneter_), dimensions(glm::vec2(r_ * 2, r_ * 2)) {
		top = ceneter_ + dimensions / 2.0f;
		bottom = ceneter_ - dimensions / 2.0f;
	}

	bool inBounds(glm::vec2 pos) {
		return (pos.x >= bottom.x && pos.y >= bottom.y) && (pos.x <= top.x && pos.y <= top.y);
	}

	bool inBounds(glm::vec3 pos) {
		return (pos.x >= bottom.x && pos.y >= bottom.y) && (pos.x <= top.x && pos.y <= top.y);
	}

	bool intersect(BoundingBox2D other) {
		if (bottom.x > other.top.x || bottom.y > other.top.y) {
			return false;
		}
		if (top.x < other.bottom.x || top.y < other.bottom.y) {
			return false;
		}
		return true;
	}

	bool operator == (const BoundingBox2D& other) const {
		return bottom.x == other.bottom.x && bottom.y == other.bottom.y
			&& top.x == other.top.x && top.y == other.top.y;
	}
};

#endif // BOUNDING_BOX2D_H