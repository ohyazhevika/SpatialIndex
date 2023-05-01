#pragma once
#include "SegmentNode.h"
#include <set>

class SegmentTree {

protected:

	SegmentNode* root;

public:
	SegmentTree();

	SegmentTree(const SegmentTree& other);
	
	~SegmentTree();

	void Insert(const Segment& segment);

	void Remove(const Segment& segment);

	void RemoveAll();

};

SegmentTree::SegmentTree() {
	root = NULL;
}

SegmentTree::SegmentTree(const SegmentTree& other) {
	assert(NULL);
}

SegmentTree::~SegmentTree() {
	assert(NULL);
}

void SegmentTree::Insert(const Segment& segment) {

}
