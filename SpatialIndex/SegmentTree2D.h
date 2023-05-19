#pragma once
# include "SegmentTree1D.h"
# include "AVL-Tree.h"

class SegmentTree2D {

private: 
	SegmentTree1D xTree;
	SegmentTree1D yTree;
	AVLTree cTree;

	int capacity = 0;

public:
	SegmentTree2D();

	~SegmentTree2D();

	void Insert(const StoredObject& obj);

	void Remove(const string& objectName);

	std::set<StoredObject> getOverlappedByRectangle(const Segment& xrange, const Segment& yrange);

	std::set<StoredObject> getStabbedByPoint(const double& x, const double& y);
};

SegmentTree2D::SegmentTree2D(): capacity(0) {}

SegmentTree2D::~SegmentTree2D() {}

void SegmentTree2D:: Insert(const StoredObject& obj) {
	StoredObjectNode* newStoredObject = cTree.Insert(obj);
	if (newStoredObject) {
		xTree.Insert(obj.bound.xRange, newStoredObject);
		yTree.Insert(obj.bound.yRange, newStoredObject);
		capacity++;
	}
}

void SegmentTree2D::Remove(const string& objectName) {
	StoredObjectNode* removableNode = cTree.find(objectName);
	if (removableNode) {
		xTree.Remove(removableNode->object.bound.xRange, removableNode);
		yTree.Remove(removableNode->object.bound.yRange, removableNode);
		capacity--;
	}
}

std::set<StoredObject> SegmentTree2D::getOverlappedByRectangle(const Segment& xrange, const Segment& yrange) {
	std::set<StoredObject> answer;
	std::set<StoredObjectNode*> xRangeNodes = xTree.GetSegmentsForRangeQuery(xrange);
	for (auto itr = xRangeNodes.begin(); itr != xRangeNodes.end(); itr++)
	{
		(*itr)->color++;
	}
	std::set<StoredObjectNode*> yRangeNodes = yTree.GetSegmentsForRangeQuery(yrange);
	for (auto itr = yRangeNodes.begin(); itr != yRangeNodes.end(); itr++) {
		if ((*itr)->color == 1) {
			answer.insert((*itr)->object);
		}
	}
	for (auto itr = xRangeNodes.begin(); itr != xRangeNodes.end(); itr++) {
		(*itr)->color = 0;
	}
	return answer;
}

std::set<StoredObject> SegmentTree2D::getStabbedByPoint(const double& x, const double& y) {
	std::set<StoredObject> answer;
	std::set<StoredObjectNode*> xRangeNodes = xTree.GetSegmentsForStabbingPoint(x);
	for (auto itr = xRangeNodes.begin(); itr != xRangeNodes.end(); itr++)
	{
		(*itr)->color++;
	}
	std::set<StoredObjectNode*> yRangeNodes = yTree.GetSegmentsForStabbingPoint(y);
	for (auto itr = yRangeNodes.begin(); itr != yRangeNodes.end(); itr++) {
		if ((*itr)->color == 1) {
			answer.insert((*itr)->object);
		}
	}
	for (auto itr = xRangeNodes.begin(); itr != xRangeNodes.end(); itr++) {
		(*itr)->color = 0;
	}
	return answer;
}

