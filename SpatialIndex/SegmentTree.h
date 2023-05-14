#pragma once
#include "SegmentNode.h"
#include <set>

class SegmentTree {

public:

	SegmentNode* root;

public:
	SegmentTree();/*

	SegmentTree(const SegmentTree& other);
	
	~SegmentTree();*/

	void Insert(const Segment& segment);

	std::set<Segment> GetSegmentsForStabbingPoint(const double point);

	std::set<Segment> GetSegmentsForRangeQuery(const Segment& range);

	//void Remove(const Segment& segment);

	//void RemoveAll();

};

SegmentTree::SegmentTree() {
	root = NULL;
}

void SegmentTree::Insert(const Segment& segment) {
	SegmentNode::Insert(root, segment);
}

std::set<Segment> SegmentTree::GetSegmentsForStabbingPoint(const double point) {
	SegmentNode* node = SegmentNode::getOverlappingNodeForPoint(root, point);
	if (!node)
		return std::set<Segment>();
	else return node->associatedSet;
}

std::set<Segment> SegmentTree::GetSegmentsForRangeQuery(const Segment& range) {
	std::set<Segment> answer = std::set<Segment>();
	SegmentNode* node = SegmentNode::getOverlappingNodeForPoint(root, range.a);
	if (node == NULL) {
		node = SegmentNode::findMin(root);
		if (node->range.a > range.b)
			return answer;
	}

	do {
		answer.insert(node->associatedSet.begin(), node->associatedSet.end());
		node = SegmentNode::inorderSuccessor(node);
	} while (node != NULL && !node->range.contains(range.b));
	return answer;
}

