#pragma once
#include "Segment.h"
#include "AVL-Tree.h"
#include <set>

struct SegmentNode1D {
public:
	Segment range;								// ��������� �������
	std::set<StoredObjectNode*> associatedSet;	// ��������������� ������ (������ ���������� �� �������, ���������� � ���� ������ ��������� �������)

	int height;									// ������ ��������� � ������ � ������ ����
	SegmentNode1D* rLink;						// ��������� �� ������� ����
	SegmentNode1D* lLink;						// ��������� �� ������ ����
	bool isRThread;								// 0, ���� rLink �������� ������� �� ������ �������� ����
	bool isLThread;								// 0, ���� lLink �������� ������� �� ����� �������� ����

	SegmentNode1D(Segment range) :
		range(range),
		height(1),
		rLink(NULL),
		lLink(NULL),
		isLThread(false),
		isRThread(false) {}
};

