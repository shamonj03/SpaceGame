
#include "QuadTree.h"
#include "BoundingBox2D.h"
#include "Node.h"

QuadTree::QuadTree(BoundingBox2D* box_) : box(box_), count(0), leaf(true) {
}

QuadTree::~QuadTree() {
	delete[] children;
}
bool QuadTree::isLeaf() {
	return leaf;
}

int QuadTree::size() {
	return count;
}

void QuadTree::insert(Node* data) {
	if (!data) { // Null data
		return;
	}

	if (!box->contains(data->position)) { // Not in bounds
		return;
	}

	if (isLeaf()) {
		if (count < MAX_CAPACITY) {
			nodes[count++] = data;
		} else {
			split();
			for (int i = 0; i < CHILD_COUNT; i++) {
				children[i]->insert(data);

				for (int i2 = 0; i2 < count; i2++) {
					children[i]->insert(nodes[i2]);
				}
			}
		}
	} else {
		for (int i = 0; i < CHILD_COUNT; i++) {
			children[i]->insert(data);
		}
	}
}



void QuadTree::split() {
	glm::vec2 dimensions = box->dimensions / 2.0f;
	glm::vec2 bottom = box->bottom;
	glm::vec2 top = box->top;

	leaf = false;
	children = new QuadTree*[CHILD_COUNT];

	children[0] = new QuadTree(new BoundingBox2D(bottom, bottom + dimensions));
	children[1] = new QuadTree(new BoundingBox2D(bottom + glm::vec2(dimensions.x, 0), bottom + glm::vec2(dimensions.x, 0) + dimensions));

	children[2] = new QuadTree(new BoundingBox2D(bottom + glm::vec2(0, dimensions.y), (bottom + dimensions) + glm::vec2(0, dimensions.y)));
	children[3] = new QuadTree(new BoundingBox2D(bottom + glm::vec2(dimensions.x, 0) + glm::vec2(0, dimensions.y), (bottom + dimensions) + glm::vec2(dimensions.x, 0) + glm::vec2(0, dimensions.y)));
}

 void QuadTree::withinArea(BoundingBox2D* range, std::vector<Node*>& nodez) {
	if (!box->intersect(*range)) {
		return;
	}
	if (isLeaf()) {
		// Get all nodes in leaf that are within the box->
		for (int i = 0; i < count; i++) {
			if (range->contains(nodes[i]->position)) {
				nodez.push_back(nodes[i]);
			}
		}
		return;
	} 
	for (int i = 0; i < CHILD_COUNT; i++) {
		children[i]->withinArea(range, nodez);
	}
}