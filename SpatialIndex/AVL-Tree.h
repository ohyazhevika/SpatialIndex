#pragma once

#include <string>
#include "Segment.h"
typedef std::string string;


struct StoredObject {

	string label;
	Segment xRange;
	Segment yRange;

	StoredObject() {}

	StoredObject(const StoredObject& other) {
		label = other.label;
		xRange = other.xRange;
		yRange = other.xRange;
	}

	friend bool operator < (const StoredObject& obj1, const StoredObject& obj2) {
		return obj1.label < obj2.label;
	}
};
struct StoredObjectNode {
public:
	StoredObject object;
	char color;
	int height;
	StoredObjectNode* right;
	StoredObjectNode* left;						

	StoredObjectNode(const StoredObject& obj) {
		object = obj;
		color = 0;
		height = 1;
		left = NULL;
		right = NULL;
	}
};

class AVLTree {

private:
	StoredObjectNode* root;

public:
	AVLTree() {
		root = nullptr;
	}

	StoredObjectNode* Insert(StoredObject obj) {
		StoredObjectNode* nodeForObject;
		root = Insert(root, obj, nodeForObject);
		return nodeForObject;
	}

	void Remove(StoredObject obj) {
		root = remove(root, obj);
	}

	void ResetNodeColors() {
		ResetSubtreeColors(root);
	}

	~AVLTree() {
		Delete(root);
	}

private:
	void Delete(StoredObjectNode*& p) {
		if (p != NULL) {
			Delete(p->right);
			Delete(p->left);
			delete p;
			p = NULL;
		}
	}

	void ResetSubtreeColors(StoredObjectNode* ptr) {
		if (ptr) {
			ResetSubtreeColors(ptr->left);
			ResetSubtreeColors(ptr->right);
			ptr->color = 0;
		}
	}

	int height(StoredObjectNode* p) {
		return (p) ? p->height : 0;
	}

	int balanceFactor(StoredObjectNode* p) {
		return (p) ? height(p->left) - height(p->right) : 0;
	}

	void fixHeight(StoredObjectNode* p) {
		int hl = height(p->left);
		int hr = height(p->right);
		p->height = (hl > hr ? hl : hr) + 1;
	}

	StoredObjectNode* rotateRight(StoredObjectNode* p)
	{
		StoredObjectNode* q = p->left;
		p->left = q->right;
		q->right = p;
		fixHeight(p);
		fixHeight(q);
		return q;
	}

	StoredObjectNode* rotateLeft(StoredObjectNode* q) // левый поворот вокруг q
	{
		StoredObjectNode* p = q->right;
		q->right = p->left;
		p->left = q;
		fixHeight(q);
		fixHeight(p);
		return p;
	}

	StoredObjectNode* balance(StoredObjectNode* p)
	{
		fixHeight(p);
		int bf = balanceFactor(p);
		if (bf == -2)
		{
			if (balanceFactor(p->right) > 0)
				p->right = rotateRight(p->right);
			return rotateLeft(p);
		}
		if (bf == 2)
		{
			if (balanceFactor(p->left) < 0)
				p->left = rotateLeft(p->left);
			return rotateRight(p);
		}
		return p;
	}

	StoredObjectNode* Insert(StoredObjectNode* p, StoredObject obj, StoredObjectNode*& nodeForObject) // вставка ключа k в дерево с корнем p
	{
		if (!p) {
			nodeForObject = new StoredObjectNode(obj);
			return nodeForObject;
		};
		if (obj.label < p->object.label)
			p->left = Insert(p->left, obj, nodeForObject);
		else if (obj.label > p->object.label)
			p->right = Insert(p->right, obj, nodeForObject);
		else {
			nodeForObject = NULL;
			return p;
		}

		return balance(p);
	}

	StoredObjectNode* findMin(StoredObjectNode* p)
	{
		return p->left ? findMin(p->left) : p;
	}

	StoredObjectNode* removeMin(StoredObjectNode* p) // удаление узла с минимальным ключом из дерева p
	{
		if (p->left == NULL)
			return p->right;
		p->left = removeMin(p->left);
		return balance(p);
	}

	StoredObjectNode* remove(StoredObjectNode* p, StoredObject obj) // удаление ключа k из дерева p
	{
		if (!p) return NULL;
		if (obj.label < p->object.label)
			p->left = remove(p->left, obj);
		else if (obj.label > p->object.label)
			p->right = remove(p->right, obj);
		else
		{
			StoredObjectNode* q = p->left;
			StoredObjectNode* r = p->right;
			delete p;
			if (!r) return q;
			StoredObjectNode* min = findMin(r);
			min->right = removeMin(r);
			min->left = q;
			return balance(min);
		}
		return balance(p);
	}
};