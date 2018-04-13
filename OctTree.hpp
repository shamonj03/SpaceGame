#ifndef QUAD_TREE_HPP
#define QUAD_TREE_HPP

#include <glm\glm.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include "Node.h"
#include "BoundingBox.h"

#define MAX_CAPACITY 64

class OctTree {
public:
	BoundingBox box;

	int count;
	Node* nodes[MAX_CAPACITY];
	OctTree** children;

	OctTree(BoundingBox box_) : box(box_),  count(0) {
	}

	~OctTree() {
		delete[] children;
	}

	inline bool withinArea(BoundingBox box2, std::vector<Node*>& nodez);

	inline bool insert(Node* data);
	inline bool insertToChildren (Node* data);

	inline void split();
	
	bool isLeaf() {
		return !children;
	}

	int size() {
		return count;
	}
};

inline bool OctTree::insert(Node* data) {
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
			
				for(int x = 0; x < count; x++) {
					for (int i = 0; i < 8; i++) {
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


inline bool OctTree::insertToChildren(Node* data) {
	for (int i = 0; i < 8; i++) {
		if (children[i]->insert(data)) {
			return true;
		}
	}
	return false;
}

inline void OctTree::split() {
	glm::vec3 dimensions = box.dimensions / 2.0f;
	glm::vec3 bottom = box.bottom;
	glm::vec3 top = box.top;

	children = new OctTree*[8];

	for (int i = 0; i < 8; i++) {
		// This is faster than 3 for loops
		children[i] = new OctTree(BoundingBox(bottom + (dimensions * glm::vec3((i & 1) ? 1 : -1, (i & 2) ? 1 : -1, (i & 4) ? 1 : -1)),
			bottom + (dimensions * glm::vec3((i & 1) ? 1 : -1, (i & 2) ? 1 : -1, (i & 4) ? 1 : -1)) + dimensions));
	}
}

inline bool OctTree::withinArea(BoundingBox box2, std::vector<Node*>& nodez) {
	if (!box.intersect(box2)) {
		return false;
	}
	if (isLeaf()) {
		// Get all nodes in leaf that are within the box.
		for (int i = 0; i < count; i++) {
			if (box2.inBounds(nodes[i]->position)) {
				nodez.push_back(nodes[i]);
			}
		}
		return true;
	} else {
		for (int i = 0; i < 8; i++) {
			if (children[i]->withinArea(box2, nodez)) {
				return true;
			}
		}
	}
	return false;
}

#endif // QUAD_TREE_HPP