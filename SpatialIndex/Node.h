#pragma once
#include "Segment.h"
#include <iostream>
#include <set>
#include <string>
#include <vector>

//typedef double Numeric;
//typedef std::string ListElement;
//typedef std::set<ListElement> set;


// ���� BITS-������
// 
//template<typename Numeric, typename ListElement>
struct Node
{
	int k;								// ������ ����� � ��� ���� ���� range
	int height;							// ������ ��������� � ������ � ������ ����
	//Segment range;						// ��������� �������
	//std::set<Segment> associatedSet;	// ��������������� ������ (������ ���������, ���������� � ���� ������ ��������� �������)
	Node* rLink;						// ��������� �� ������� ����
	Node* lLink;						// ��������� �� ������ ����
	bool isRThread;						// 1, ���� rLink �������� ������ ���������� �� successor-� ������� ����
	bool isLThread;						// 1, ���� lLink �������� ������ ���������� �� predecessor-� ������� ����

	Node();

	// ���� ����������� ��� �� ����� � ����� ����, ��� ����� ��������  - � ��� ������ k ���� range
	Node(int k);

	static Node* leftDummy;
	static Node* rightDummy;

	static int getHeight(const Node* node);

	static int balanceFactor(const Node* node);

	static void fixHeight(Node* node);

	static Node* rotateright(Node* p); // ������ ������� ������ p

	static Node* rotateleft(Node* q); // ����� ������� ������ q

	static Node* balance(Node* p); // ������������ ���� p

	static Node* insert(Node* p, int k); // ������� ����� k � ������ � ������ p

	static void inorderTraverse(Node* p, std::vector<int>& inorderSequence);

	static Node* inorderSuccessor(Node* p); // ���������������� inorder-������������� ���� p 

	static Node* inorderPredecessor(Node* p);	// ���������������� inorder-�������������� ���� p 


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
