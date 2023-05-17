#pragma once
#include "Segment.h"

//typedef double Numeric;
//typedef std::string ListElement;
//typedef std::set<ListElement> set;


// ���� BITS-������
// 
//template<typename Numeric, typename ListElement>
struct StoredObjectNode
{
public:
	int k;								// ������ ����� � ��� ���� ���� range
	int height;							// ������ ��������� � ������ � ������ ����
	//Segment range;						// ��������� �������
	//std::set<Segment> associatedSet;	// ��������������� ������ (������ ���������, ���������� � ���� ������ ��������� �������)
	StoredObjectNode* rLink;						// ��������� �� ������� ����
	StoredObjectNode* lLink;						// ��������� �� ������ ����
	bool isRThread;						// 1, ���� rLink �������� ������ ���������� �� successor-� ������� ����
	bool isLThread;						// 1, ���� lLink �������� ������ ���������� �� predecessor-� ������� ����

public:
	// ���� ����������� ��� �� ����� � ����� ����, ��� ����� ��������  - � ��� ������ k ���� range
	StoredObjectNode(int k);

	static StoredObjectNode* leftDummy;
	static StoredObjectNode* rightDummy;
	// ������� ����� k � ������ � ������ p
	static StoredObjectNode* insert(StoredObjectNode* p, int k);

	// �������� ����� k �� ������ � ������ � ���� p
	static StoredObjectNode* remove(StoredObjectNode* p, int k, bool leftSon = false);

	// ���������������� inorder-������������� ���� p 
	static StoredObjectNode* inorderSuccessor(StoredObjectNode* p);

	// ���������������� inorder-�������������� ���� p
	static StoredObjectNode* inorderPredecessor(StoredObjectNode* p);

	// ���������� ������ ������ � ������ � ���� node
	static int getHeight(const StoredObjectNode* node);

private:

	// ������� ����� ������ � ������� ����������� ������ � ������ � ���� node
	static int balanceFactor(const StoredObjectNode* node);

	// �������� ������ ��������� � ������ � ���� node
	static void fixHeight(StoredObjectNode* node);

	// ������ ������� ������ p
	static StoredObjectNode* rotateright(StoredObjectNode* p);

	// ����� ������� ������ q
	static StoredObjectNode* rotateleft(StoredObjectNode* q);

	// ������������ ���� p
	static StoredObjectNode* balance(StoredObjectNode* p); 

	// ����� ����� ���� (���� � ����������� ��������� �����) � ������ ���������
	static StoredObjectNode* findMin(StoredObjectNode* p);

	// �������� ���� � ����������� ��������� ����� � ������ ���������
	static StoredObjectNode* removeMin(StoredObjectNode* p);
};
