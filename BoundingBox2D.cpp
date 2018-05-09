#include "BoundingBox2D.h"

BoundingBox2D::~BoundingBox2D() {}

BoundingBox2D::BoundingBox2D(glm::vec2 ceneter_, float r_) : 
	dimensions(glm::vec2(r_ * 2, r_ * 2)),
	top(ceneter_ + r_),
	bottom(ceneter_ - r_) {
}

BoundingBox2D::BoundingBox2D(glm::vec2 bottom_, glm::vec2 top_) : 
	top(top_), 
	bottom(bottom_),
	dimensions(glm::abs(top_ - bottom_)) {
}

bool BoundingBox2D::contains(glm::vec2 pos) {
	return (pos.x >= bottom.x && pos.x <= top.x) && (pos.y >= bottom.y && pos.y <= top.y);
}


bool BoundingBox2D::intersect(BoundingBox2D other) {
	if (bottom.x > other.top.x || bottom.y > other.top.y) {
		return false;
	}
	if (top.x < other.bottom.x || top.y < other.bottom.y) {
		return false;
	}
	return true;
}