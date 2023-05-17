#pragma once
#include "Segment.h"

//typedef double Numeric;
//typedef std::string ListElement;
//typedef std::set<ListElement> set;


// узел BITS-дерева
// 
//template<typename Numeric, typename ListElement>
struct StoredObjectNode
{
public:
	int k;								// вместо этого у нас есть поле range
	int height;							// высота поддерева с корнем в данном узле
	//Segment range;						// атомарный сегмент
	//std::set<Segment> associatedSet;	// ассоциированный список (список сегментов, включающих в себя данный атомарный сегмент)
	StoredObjectNode* rLink;						// указатель на правого сына
	StoredObjectNode* lLink;						// указатель на левого сына
	bool isRThread;						// 1, если rLink является прямым указателем на successor-а данного узла
	bool isLThread;						// 1, если lLink является прямым указателем на predecessor-а данного узла

public:
	// этот конструктор нам не нужен в таком виде, его нужно поменять  - у нас вместо k есть range
	StoredObjectNode(int k);

	static StoredObjectNode* leftDummy;
	static StoredObjectNode* rightDummy;
	// вставка ключа k в дерево с корнем p
	static StoredObjectNode* insert(StoredObjectNode* p, int k);

	// удаление ключа k из дерева с корнем в узле p
	static StoredObjectNode* remove(StoredObjectNode* p, int k, bool leftSon = false);

	// непосредственный inorder-последователь узла p 
	static StoredObjectNode* inorderSuccessor(StoredObjectNode* p);

	// непосредственный inorder-предшественник узла p
	static StoredObjectNode* inorderPredecessor(StoredObjectNode* p);

	// вычисление высоты дерева с корнем в узле node
	static int getHeight(const StoredObjectNode* node);

private:

	// разница высот левого и правого поддеревьев дерева с корнем в узле node
	static int balanceFactor(const StoredObjectNode* node);

	// пересчет высоты поддерева с корнем в узле node
	static void fixHeight(StoredObjectNode* node);

	// правый поворот вокруг p
	static StoredObjectNode* rotateright(StoredObjectNode* p);

	// левый поворот вокруг q
	static StoredObjectNode* rotateleft(StoredObjectNode* q);

	// балансировка узла p
	static StoredObjectNode* balance(StoredObjectNode* p); 

	// самый левый узел (узел с минимальным значением ключа) в данном поддереве
	static StoredObjectNode* findMin(StoredObjectNode* p);

	// удаление узла с минимальным значением ключа в данном поддереве
	static StoredObjectNode* removeMin(StoredObjectNode* p);
};
