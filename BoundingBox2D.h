#ifndef BOUNDING_BOX2D_H
#define BOUNDING_BOX2D_H

#include <glm\glm.hpp>

class BoundingBox2D {
public:
	glm::vec2 top;
	glm::vec2 bottom;
	glm::vec2 dimensions;

	BoundingBox2D(glm::vec2 bottom_, glm::vec2 top_);
	BoundingBox2D(glm::vec2 ceneter_, float r_);
	~BoundingBox2D();

	bool inBounds(glm::vec2 pos);

	bool intersect(BoundingBox2D other);
};

#endif // BOUNDING_BOX2D_H