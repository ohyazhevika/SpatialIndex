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
	int k;								// вместо этого у нас есть поле range
	int height;							// высота поддерева с корнем в данном узле
	//Segment range;						// атомарный сегмент
	//std::set<Segment> associatedSet;	// ассоциированный список (список сегментов, включающих в себя данный атомарный сегмент)
	Node* rLink;						// указатель на правого сына
	Node* lLink;						// указатель на левого сына
	bool isRThread;						// 1, если rLink является прямым указателем на successor-а данного узла
	bool isLThread;						// 1, если lLink является прямым указателем на predecessor-а данного узла

	// этот конструктор нам не нужен в таком виде, его нужно поменять  - у нас вместо k есть range
	Node(int k);

	static Node* leftDummy;
	static Node* rightDummy;

	// вычисление высоты дерева с корнем в узле node
	static int getHeight(const Node* node);

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

	// вставка ключа k в дерево с корнем p
	static Node* insert(Node* p, int k);

	// непосредственный inorder-последователь узла p 
	static Node* inorderSuccessor(Node* p);

	// непосредственный inorder-предшественник узла p
	static Node* inorderPredecessor(Node* p); 


	/*static int calculateHeight(const Node* p)
	{
		if (p->lLink && p->isLThread == 0 && p->rLink && p->isRThread == 0) {
			if (p->lLink->height < p->rLink->height)
				return p->rLink->height + 1;
			else return  p->lLink->height + 1;
		}
		else if (p->lLink && p->isLThread == 0 && p->isRThread == 1) {
			return p->lLink->height + 1;
		}
		else if (p->isLThread == 1 && p->rLink && p->isRThread == 0) {
			return p->rLink->height + 1;
		}
		return (p == NULL) ? 0 : 1;
	}*/
};
