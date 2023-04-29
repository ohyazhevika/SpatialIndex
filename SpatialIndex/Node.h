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

	// ���� ����������� ��� �� ����� � ����� ����, ��� ����� ��������  - � ��� ������ k ���� range
	Node(int k);

	static Node* leftDummy;
	static Node* rightDummy;

	// ���������� ������ ������ � ������ � ���� node
	static int getHeight(const Node* node);

	// ������� ����� ������ � ������� ����������� ������ � ������ � ���� node
	static int balanceFactor(const Node* node);

	// �������� ������ ��������� � ������ � ���� node
	static void fixHeight(Node* node);

	// ������ ������� ������ p
	static Node* rotateright(Node* p);

	// ����� ������� ������ q
	static Node* rotateleft(Node* q);

	// ������������ ���� p
	static Node* balance(Node* p); 

	// ������� ����� k � ������ � ������ p
	static Node* insert(Node* p, int k);

	// ���������������� inorder-������������� ���� p 
	static Node* inorderSuccessor(Node* p);

	// ���������������� inorder-�������������� ���� p
	static Node* inorderPredecessor(Node* p);

	// ����� ����� ���� (���� � ����������� ��������� �����) � ������ ���������
	static Node* findMin(Node* p);

	// �������� ���� � ����������� ��������� ����� � ������ ���������
	static Node* removeMin(Node* p);

	// �������� ����� k �� ������ � ������ � ���� p
	static Node* remove(Node* p, int k, bool leftSon = false);

};
