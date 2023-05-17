#pragma once
#include "Segment.h"
#include <set>

struct SegmentNode {
public:

	Segment range;						// ��������� �������
	std::set<Segment> associatedSet;	// ��������������� ������ (������ ���������, ���������� � ���� ������ ��������� �������)

	int height;							// ������ ��������� � ������ � ������ ����
	SegmentNode* rLink;					// ��������� �� ������� ����
	SegmentNode* lLink;					// ��������� �� ������ ����
	bool isRThread;						// 1, ���� rLink �������� ������� �� ������ ��������
	bool isLThread;						// 1, ���� lLink �������� ������� �� ����� �������� ����

	SegmentNode(Segment range);

	static SegmentNode* leftDummy;
	static SegmentNode* rightDummy;
	static Segment deletableTail;

	static void Insert(SegmentNode*& root, Segment range);

	static SegmentNode* Insert(SegmentNode* root, Segment range, std::set<Segment>& segmentaIds);

	static std::pair<SegmentNode*, bool> rem(SegmentNode* p, const Segment& segment);
	
	// �������� ����� k �� ������ � ������ � ���� p
	static SegmentNode* remove(SegmentNode* p, Segment range, bool leftSon = false);

	// ���������������� inorder-������������� ���� p 
	static SegmentNode* inorderSuccessor(SegmentNode* p);

	// ���������������� inorder-�������������� ���� p
	static SegmentNode* inorderPredecessor(SegmentNode* p);

	// ���������� ������ ������ � ������ � ���� node
	static int getHeight(const SegmentNode* node);
	
	// ������� ������ ��� ������
	static SegmentNode* getOverlappingNodeForPoint(SegmentNode* root, const double point);

public:

	// ������� ����� ������ � ������� ����������� ������ � ������ � ���� node
	static int balanceFactor(const SegmentNode* node);

	// �������� ������ ��������� � ������ � ���� node
	static void fixHeight(SegmentNode* node);

	// ������ ������� ������ p
	static SegmentNode* rotateright(SegmentNode* p);

	// ����� ������� ������ q
	static SegmentNode* rotateleft(SegmentNode* q);

	// ������������ ���� p
	static SegmentNode* balance(SegmentNode* p);

	// ����� ����� ���� (���� � ����������� ��������� �����) � ������ ���������
	static SegmentNode* findMin(SegmentNode* p);

	// �������� ���� � ����������� ��������� ����� � ������ ���������
	static SegmentNode* removeMin(SegmentNode* p);
};