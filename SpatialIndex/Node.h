#pragma once
#include "Segment.h"
#include <iostream>
#include <set>
#include <string>
#include <vector>

//typedef double Numeric;
//typedef std::string ListElement;
//typedef std::set<ListElement> set;


// узел BITS-дерева
// 
//template<typename Numeric, typename ListElement>
struct Node
{
public:
	int k;								// вместо этого у нас есть поле range
	int height;							// высота поддерева с корнем в данном узле
	//Segment range;						// атомарный сегмент
	//std::set<Segment> associatedSet;	// ассоциированный список (список сегментов, включающих в себя данный атомарный сегмент)
	Node* rLink;						// указатель на правого сына
	Node* lLink;						// указатель на левого сына
	bool isRThread;						// 1, если rLink является прямым указателем на successor-а данного узла
	bool isLThread;						// 1, если lLink является прямым указателем на predecessor-а данного узла

public:
	// этот конструктор нам не нужен в таком виде, его нужно поменять  - у нас вместо k есть range
	Node(int k);

	static Node* leftDummy;
	static Node* rightDummy;
	// вставка ключа k в дерево с корнем p
	static Node* insert(Node* p, int k);

	// удаление ключа k из дерева с корнем в узле p
	static Node* remove(Node* p, int k, bool leftSon = false);

	// непосредственный inorder-последователь узла p 
	static Node* inorderSuccessor(Node* p);

	// непосредственный inorder-предшественник узла p
	static Node* inorderPredecessor(Node* p);

	// вычисление высоты дерева с корнем в узле node
	static int getHeight(const Node* node);

private:

	// разница высот левого и правого поддеревьев дерева с корнем в узле node
	static int balanceFactor(const Node* node);

	// пересчет высоты поддерева с корнем в узле node
	static void fixHeight(Node* node);

	// правый поворот вокруг p
	static Node* rotateright(Node* p);

	// левый поворот вокруг q
	static Node* rotateleft(Node* q);

	// балансировка узла p
	static Node* balance(Node* p); 

	// самый левый узел (узел с минимальным значением ключа) в данном поддереве
	static Node* findMin(Node* p);

	// удаление узла с минимальным значением ключа в данном поддереве
	static Node* removeMin(Node* p);
};
