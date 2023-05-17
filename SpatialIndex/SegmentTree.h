#pragma once
#include "SegmentNode.h"
#include <set>

class SegmentTree {

public:

	SegmentNode* root;

public:
	SegmentTree();

	//SegmentTree(const SegmentTree& other);
	
	/*~SegmentTree();*/

	void Insert(const Segment& segment);

	std::set<Segment> GetSegmentsForStabbingPoint(const double point);

	std::set<Segment> GetSegmentsForRangeQuery(const Segment& range);

	void Remove(const Segment& segment);

	//void Delete(SegmentNode*& p);

};

SegmentTree::SegmentTree() {
	root = NULL;
}

//SegmentTree:: ~SegmentTree() {
//	Delete(root);
//}

//void SegmentTree::Delete(SegmentNode*& p) {
//	if (p) {
//		Delete(p->lLink);
//		Delete(p->rLink);
//		delete p;
//		p = NULL;
//	}
//}

void SegmentTree::Insert(const Segment& segment) {
	SegmentNode::Insert(root, segment);
}

void SegmentTree::Remove(const Segment& segment) {
	std::pair<SegmentNode*, bool> pair = SegmentNode::rem(root, segment);
	root = pair.first;

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

