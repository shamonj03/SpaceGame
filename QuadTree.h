#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include <vector>

#define MAX_CAPACITY 32
#define CHILD_COUNT 4

class QuadTree {
public:
	class BoundingBox2D* box;

	int count;
	class Node* nodes[MAX_CAPACITY];
	QuadTree** children;

	QuadTree(class BoundingBox2D* box_);

	~QuadTree();

	bool withinArea(class BoundingBox2D* box2, std::vector<class Node*>& nodez);

	bool insert(class Node* data);
	bool insertToChildren(class Node* data);

	inline void split();

	bool isLeaf();

	int size();
};

#endif // QUAD_TREE_H