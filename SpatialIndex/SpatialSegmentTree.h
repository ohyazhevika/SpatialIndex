#pragma once
# include "SegmentTree1D.h"
# include "AVL-Tree.h"

template<std::size_t dimensions>
struct Point {
	double coordinates[dimensions];

	double& operator[] (std::size_t index) {
		assert(index >= 0 && index < dimensions);
		return coordinates[index];
	}
};

template<std::size_t dimensions=2>
class SpatialSegmentTree {

	typedef Point<dimensions>				Point;
	typedef BoundingBox<dimensions>			BoundingBox;
	typedef StoredObject<dimensions>		StoredObject;
	typedef StoredObjectNode<dimensions>	StoredObjectNode;
	typedef AVLTree<dimensions>				AVLTree;
	typedef SegmentTree1D<dimensions>		SegmentTree1D;

private: 
	SegmentTree1D segmentTrees[dimensions];
	AVLTree cTree;
	int capacity = 0;

public:
	SpatialSegmentTree();

	~SpatialSegmentTree();

	int getCapacity();

	void Insert(const StoredObject& obj);

	void Remove(const string& objectName);

	std::set<StoredObject> getOverlappedByRectangle(const BoundingBox& bound);

	std::set<StoredObject> getStabbedByPoint(const Point& point);
};

template<std::size_t dimensions>
SpatialSegmentTree<dimensions>::SpatialSegmentTree(): capacity(0) {}

template<std::size_t dimensions>
SpatialSegmentTree<dimensions>::~SpatialSegmentTree() {}

template<std::size_t dimensions>
int SpatialSegmentTree<dimensions>::getCapacity() {
	return capacity;
}

template<std::size_t dimensions>
void SpatialSegmentTree<dimensions>:: Insert(const StoredObject& obj) {
	StoredObjectNode* newStoredObject = cTree.Insert(obj);
	if (newStoredObject) {
		for (std::size_t axis = 0; axis < dimensions; axis++) {
			segmentTrees[axis].Insert(obj.bound.edges[axis], newStoredObject);
		}
		capacity++;
	}
}

template<std::size_t dimensions>
void SpatialSegmentTree<dimensions>::Remove(const string& objectName) {
	StoredObjectNode* removableNode = cTree.find(objectName);
	if (removableNode) {
		for (std::size_t axis = 0; axis < dimensions; axis++) {
			segmentTrees[axis].Remove(removableNode->object.bound.edges[axis], removableNode);
		}
		cTree.Remove(removableNode->object);
		capacity--;
	}
}
template<std::size_t dimensions>
std::set<StoredObject<dimensions>> SpatialSegmentTree<dimensions>::getOverlappedByRectangle(const BoundingBox& bound) {
	std::set<StoredObject> answer;
	/*std::set<StoredObjectNode*> markedNodes;*/
	std::size_t lastAxis = dimensions - 1;
	for (std::size_t axis = 0; axis < lastAxis; axis++) {
		std::set<StoredObjectNode*> axisOverlappedNodes = segmentTrees[axis].GetOverlappedObjects(bound.edges[axis]);
		for (auto itr = axisOverlappedNodes.begin(); itr != axisOverlappedNodes.end(); itr++) {
			(*itr)->color++;
			/*markedNodes.insert(itr);*/
		}
	}
	std::set<StoredObjectNode*> lastAxisOverLappedNodes = segmentTrees[lastAxis].GetOverlappedObjects(bound.edges[lastAxis]);
	for (auto itr = lastAxisOverLappedNodes.begin(); itr != lastAxisOverLappedNodes.end(); itr++) {
		if ((*itr)->color == lastAxis) {
			answer.insert((*itr)->object);
		}
	}
	/*for (auto itr = markedNodes.begin(); itr != markedNodes.end(); itr++) {
		(*itr)->color = 0;
	}*/
	cTree.ResetNodeColors();
	return answer;
}

template<std::size_t dimensions>
std::set<StoredObject<dimensions>> SpatialSegmentTree<dimensions>::getStabbedByPoint(const Point& point) {
	std::set<StoredObject> answer;
	/*std::set<StoredObjectNode*> markedNodes;*/
	std::size_t lastAxis = dimensions - 1;
	for (std::size_t axis = 0; axis < lastAxis; axis++) {
		std::set<StoredObjectNode*> axisStabbedNodes = segmentTrees[axis].GetSegmentsForStabbingPoint(point[axis]);
		for (auto itr = axisStabbedNodes.begin(); itr != axisStabbedNodes.end(); itr++) {
			(*itr)->color++;
			/*markedNodes.insert(itr);*/
		}
	}
	std::set<StoredObjectNode*> lastAxisStabbedNodes = segmentTrees[lastAxis].GetSegmentsForStabbingPoint(point[lastAxis]);
	for (auto itr = lastAxisStabbedNodes.begin(); itr != lastAxisStabbedNodes.end(); itr++) {
		if ((*itr)->color == lastAxis) {
			answer.insert((*itr)->object);
		}
	}
	/*for (auto itr = markedNodes.begin(); itr != markedNodes.end(); itr++) {
		(*itr)->color = 0;
	}*/
	cTree.ResetNodeColors();
	return answer;
}

