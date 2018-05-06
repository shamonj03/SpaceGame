#ifndef QUAD_TREE_HPP
#define QUAD_TREE_HPP

#include <vector>

#include "BoundingBox2D.h"
#include "Node.h"

#define MAX_CAPACITY 32
#define CHILD_COUNT 4

class QuadTree {
public:
	BoundingBox2D box;

	int count;
	Node* nodes[MAX_CAPACITY];
	QuadTree** children;

	QuadTree(BoundingBox2D box_) : box(box_), count(0) {
	}

	~QuadTree() {
		delete[] children;
	}

	inline bool withinArea(BoundingBox2D box2, std::vector<Node*>& nodez);

	inline bool insert(Node* data);
	inline bool insertToChildren(Node* data);

	inline void split();

	bool isLeaf() {
		return !children;
	}

	int size() {
		return count;
	}
};

inline bool QuadTree::insert(Node* data) {
	if (isLeaf()) { // This is a leaf
		if (box.inBounds(data->position)) {
			if (count < MAX_CAPACITY) {
				nodes[count++] = data;
				//		std::cout << "INSERTED" << std::endl;
				return true;
			}
			else {
				split();
				//std::cout << "SPLIT" << std::endl;

				for (int x = 0; x < count; x++) {
					for (int i = 0; i < CHILD_COUNT; i++) {
						if (children[i]->insert(nodes[x])) {
							return true;
						}
					}
				}
				return insertToChildren(data);
			}
		}
	} else {
		return insertToChildren(data);
	}
	return false;
}


inline bool QuadTree::insertToChildren(Node* data) {
	for (int i = 0; i < CHILD_COUNT; i++) {
		if (children[i]->insert(data)) {
			return true;
		}
	}
	return false;
}

inline void QuadTree::split() {
	glm::vec2 dimensions = box.dimensions / 2.0f;
	glm::vec2 bottom = box.bottom;
	glm::vec2 top = box.top;

	children = new QuadTree*[CHILD_COUNT];

	children[0] = new QuadTree(BoundingBox2D(bottom, bottom + dimensions));
	children[1] = new QuadTree(BoundingBox2D(bottom + glm::vec2(dimensions.x, 0), bottom + glm::vec2(dimensions.x, 0) + dimensions));

	children[2] = new QuadTree(BoundingBox2D(bottom + glm::vec2(0, dimensions.y), (bottom + dimensions) + glm::vec2(0, dimensions.y)));
	children[3] = new QuadTree(BoundingBox2D(bottom + glm::vec2(dimensions.x, 0) + glm::vec2(0, dimensions.y), (bottom + dimensions) + glm::vec2(dimensions.x, 0) + glm::vec2(0, dimensions.y)));
}

inline bool QuadTree::withinArea(BoundingBox2D range, std::vector<Node*>& nodez) {
	if (!box.intersect(range)) {
		return false;
	}
	if (isLeaf()) {
		// Get all nodes in leaf that are within the box.
		for (int i = 0; i < count; i++) {
			if (range.inBounds(nodes[i]->position)) {
				nodez.push_back(nodes[i]);
			}
		}
		return true;
	} else {
		for (int i = 0; i < CHILD_COUNT; i++) {
			if (children[i]->withinArea(range, nodez)) {
				return true;
			}
		}
		return false;
	}
	return false;
}

#endif // QUAD_TREE_HPP