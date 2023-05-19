#pragma once
#include "Segment.h"
#include "AVL-Tree.h"
#include <set>

template<std::size_t dimensions>
struct SegmentNode1D {
	typedef StoredObjectNode<dimensions>	StoredObjectNode;

public:
	Segment range;								// атомарный сегмент
	std::set<StoredObjectNode*> associatedSet;	// ассоциированный список (список указателей на объекты, включающих в себя данный атомарный сегмент)

	int height;									// высота поддерева с корнем в данном узле
	SegmentNode1D* rLink;						// указатель на правого сына
	SegmentNode1D* lLink;						// указатель на левого сына
	bool isRThread;								// 0, если rLink является ссылкой на правый дочерний узел
	bool isLThread;								// 0, если lLink является ссылкой на левый дочерний узел

	SegmentNode1D(Segment range) :
		range(range),
		height(1),
		rLink(NULL),
		lLink(NULL),
		isLThread(false),
		isRThread(false) {}
};

