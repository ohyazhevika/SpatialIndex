#pragma once

#include <string>
#include "Segment.h"
typedef std::string string;


template <std::size_t dimensions>
struct BoundingBox {
	Segment edges[dimensions];
	BoundingBox() {}

	inline double area() {
		double area = 1;
		for (std::size_t axis = 0; axis < dimensions; axis++) {
			area *= (edges[axis].a - edges[axis].b);
		}
		return area;
	}

	inline bool encloses(const BoundingBox& bb) {
		for (std::size_t axis = 0; axis < dimensions; axis++)
			if (bb.edges[axis].a < edges[axis].a || edges[axis].b < bb.edges[axis].b)
				return false;

		return true;
	}

	inline bool overlaps(const BoundingBox& bb) {
		for (std::size_t axis = 0; axis < dimensions; axis++)
		{
			if (!(edges[axis].a < bb.edges[axis].b) || !(bb.edges[axis].a < edges[axis].b))
				return false;
		}

		return true;
	}

	bool operator == (const BoundingBox& bb) {
		for (std::size_t axis = 0; axis < dimensions; axis++)
			if (edges[axis].a != bb.edges[axis].a || edges[axis].b != bb.edges[axis].b)
				return false;

		return true;
	}
};

template<std::size_t dimensions>
struct StoredObject {

	typedef BoundingBox<dimensions> BoundingBox;

	string name;

	BoundingBox bound;

	StoredObject() {}

	StoredObject(const string& name, const BoundingBox& bound): name(name), bound(bound) {}

	StoredObject(const StoredObject& other) {
		name = other.name;
		bound = other.bound;
	}

	friend bool operator < (const StoredObject& obj1, const StoredObject& obj2) {
		return obj1.name < obj2.name;
	}
};

template<std::size_t dimensions>
struct StoredObjectNode {

	typedef StoredObject<dimensions> StoredObject;

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

template<std::size_t dimensions>

class AVLTree {

	typedef BoundingBox<dimensions>			BoundingBox;
	typedef StoredObject<dimensions>		StoredObject;
	typedef StoredObjectNode<dimensions>	StoredObjectNode;

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

	StoredObjectNode* find(const string& objectName) {
		return find(root, objectName);
	}

	~AVLTree() {
		Delete(root);
	}

private:
	void Delete(StoredObjectNode*& p) {
		if (p != NULL) {
			Delete(p->left);
			Delete(p->right);
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
		if (obj.name < p->object.name)
			p->left = Insert(p->left, obj, nodeForObject);
		else if (obj.name > p->object.name)
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
		if (obj.name < p->object.name)
			p->left = remove(p->left, obj);
		else if (obj.name > p->object.name)
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

	StoredObjectNode* find(StoredObjectNode* p, const string& name) {
		if (!p) return NULL;
		if (name < p->object.name) {
			return find(p->left, name);
		}
		else if (p->object.name < name) {
			return find(p->right, name);
		}
		else {
			return p;
		}
	}
};