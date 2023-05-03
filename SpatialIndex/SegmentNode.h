#pragma once
#include "Segment.h"
#include <set>

struct SegmentNode {
public:
	Segment range;						// атомарный сегмент
	std::set<Segment> associatedSet;	// ассоциированный список (список сегментов, включающих в себя данный атомарный сегмент)

	int height;							// высота поддерева с корнем в данном узле
	SegmentNode* rLink;					// указатель на правого сына
	SegmentNode* lLink;					// указатель на левого сына
	bool isRThread;						// 1, если rLink является прямым указателем на successor-а данного узла
	bool isLThread;						// 1, если lLink является прямым указателем на predecessor-а данного узла

public:

	SegmentNode(Segment range);

	static SegmentNode* leftDummy;
	static SegmentNode* rightDummy;
	// вставка ключа k в дерево с корнем p
	static SegmentNode* insert(SegmentNode* p, Segment range);

	// вставка отрезка range в дерево с корнем root
	static SegmentNode* Insert(SegmentNode* root, Segment range, std::set<Segment> segmentsIds);

	// вставка отрезка range в дерево с корнем root
	static SegmentNode* Insert(SegmentNode* root, Segment range);

	static void INSERT(SegmentNode*& root, Segment range, std::set<Segment> segmentsIds);
	
	// удаление ключа k из дерева с корнем в узле p
	static SegmentNode* remove(SegmentNode* p, Segment range, bool leftSon = false);

	// непосредственный inorder-последователь узла p 
	static SegmentNode* inorderSuccessor(SegmentNode* p);

	// непосредственный inorder-предшественник узла p
	static SegmentNode* inorderPredecessor(SegmentNode* p);

	// вычисление высоты дерева с корнем в узле node
	static int getHeight(const SegmentNode* node);

private:

	// разница высот левого и правого поддеревьев дерева с корнем в узле node
	static int balanceFactor(const SegmentNode* node);

	// пересчет высоты поддерева с корнем в узле node
	static void fixHeight(SegmentNode* node);

	// правый поворот вокруг p
	static SegmentNode* rotateright(SegmentNode* p);

	// левый поворот вокруг q
	static SegmentNode* rotateleft(SegmentNode* q);

	// балансировка узла p
	static SegmentNode* balance(SegmentNode* p);

	// самый левый узел (узел с минимальным значением ключа) в данном поддереве
	static SegmentNode* findMin(SegmentNode* p);

	// удаление узла с минимальным значением ключа в данном поддереве
	static SegmentNode* removeMin(SegmentNode* p);
};