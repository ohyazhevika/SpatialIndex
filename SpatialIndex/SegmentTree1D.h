#pragma once
#include "SegmentNode1D.h"


class SegmentTree1D {

private:
	SegmentNode1D* root;
	static SegmentNode1D* leftDummy;
	static SegmentNode1D* rightDummy;
	static SegmentNode1D* mergeableTail;
	static Segment tailSuccessorRange;

public:
	SegmentTree1D();

	~SegmentTree1D();

	void Insert(const Segment& segment, StoredObjectNode* objPtr);

	std::set<StoredObjectNode*> GetSegmentsForStabbingPoint(const double point);

	std::set<StoredObjectNode*> GetSegmentsForRangeQuery(const Segment& range);

	void Remove(const Segment& segment, StoredObjectNode* objPtr);


private:

	void Insert(SegmentNode1D*&, const Segment&, StoredObjectNode*);
	
	SegmentNode1D* Insert(SegmentNode1D*, Segment, std::set<StoredObjectNode*>);

	SegmentNode1D* Remove(SegmentNode1D* p, const Segment& segment, StoredObjectNode* objPtr, bool leftSon);

	SegmentNode1D* Remove(SegmentNode1D* p, const Segment& segment, bool leftSon);

	int height(const SegmentNode1D*);

	int balanceFactor(const SegmentNode1D*);

	void fixHeight(SegmentNode1D*);

	SegmentNode1D* rotateright(SegmentNode1D* p);

	SegmentNode1D* rotateleft(SegmentNode1D* q);

	SegmentNode1D* balance(SegmentNode1D* p);

	SegmentNode1D* getOverlappingNodeForPoint(SegmentNode1D* p, const double point);

	void Delete(SegmentNode1D*&);

	SegmentNode1D* inorderSuccessor(SegmentNode1D* p);

	SegmentNode1D* inorderPredecessor(SegmentNode1D* p);

	SegmentNode1D* findMin(SegmentNode1D* p);

	SegmentNode1D* removeMin(SegmentNode1D* p);
};

SegmentNode1D* SegmentTree1D::leftDummy = NULL;
SegmentNode1D* SegmentTree1D::rightDummy = NULL;
SegmentNode1D* SegmentTree1D::mergeableTail = NULL;
Segment SegmentTree1D::tailSuccessorRange = Segment();

SegmentTree1D::SegmentTree1D() {
	root = NULL;
}

SegmentTree1D::~SegmentTree1D() {
	Delete(root);
}

void SegmentTree1D::Insert(const Segment& segment, StoredObjectNode* objPtr) {
	Insert(root, segment, objPtr);
}

void SegmentTree1D::Remove(const Segment& segment, StoredObjectNode* objPtr) {
	root = Remove(root, segment, objPtr, false);
	if (mergeableTail) {
		Segment newRange = Segment(mergeableTail->range.a, tailSuccessorRange.b);
		root = Remove(root, tailSuccessorRange, false);
		mergeableTail->range = newRange;
		mergeableTail = NULL;
		tailSuccessorRange = Segment();
	}
}

std::set<StoredObjectNode*> SegmentTree1D::GetSegmentsForStabbingPoint(const double point) {
	SegmentNode1D* node = getOverlappingNodeForPoint(root, point);
	if (!node)
		return std::set<StoredObjectNode*>();
	else return node->associatedSet;
}

std::set<StoredObjectNode*> SegmentTree1D:: GetSegmentsForRangeQuery(const Segment& range) {
	std::set<StoredObjectNode*> answer;
	SegmentNode1D* node = getOverlappingNodeForPoint(root, range.a);
	if (node == NULL) {
		node = findMin(root);
		if (node->range.a > range.b)
			return answer;
	}

	do {
		answer.insert(node->associatedSet.begin(), node->associatedSet.end());
		node = inorderSuccessor(node);
	} while (node != NULL && node->range.a <= range.b);
	return answer;
}

void SegmentTree1D::Insert(SegmentNode1D*& p, const Segment& segment, StoredObjectNode* objPtr) {
	std::set<StoredObjectNode*> associatedSet;
	associatedSet.insert(objPtr);
	root = Insert(root, segment, associatedSet);
}

SegmentNode1D* SegmentTree1D::Insert(SegmentNode1D* p, Segment segment, std::set<StoredObjectNode*> set) {
	if (p == NULL) {
		SegmentNode1D* newNode = new SegmentNode1D(segment);
		newNode->associatedSet = set;
		return newNode;
	}

	Segment rootRange = p->range;
	Segment leftForInsertion, C, rightForInsertion;
	std::set<StoredObjectNode*> R = p->associatedSet;
	std::set<StoredObjectNode*> L = p->associatedSet;
	C = segment.overlap(rootRange);
	if (!C.isEmpty()) {
		p->range = C;
		p->associatedSet.insert(set.begin(), set.end());
		if (segment.a != rootRange.a) {
			if (segment.a < rootRange.a) {
				L = set;
			}
			leftForInsertion = Segment(std::min(segment.a, rootRange.a), std::max(segment.a, rootRange.a));
		}
		if (segment.b != rootRange.b) {
			if (segment.b > rootRange.b) {
				R = set;
			}
			rightForInsertion = Segment(std::min(segment.b, rootRange.b), std::max(segment.b, rootRange.b));
		}
	}
	else {
		if (segment.isToTheLeftFrom(rootRange)) {
			leftForInsertion = segment;
			L = set;
		}
		else {
			rightForInsertion = segment;
			R = set;
		}
	}
	if (!leftForInsertion.isEmpty()) {
		SegmentNode1D* q;
		if (p->isLThread) {
			SegmentNode1D* pred = p->lLink;
			q = new SegmentNode1D(leftForInsertion);
			q->associatedSet = L;
			q->isLThread = true;
			q->lLink = pred;
			p->isLThread = false;
		}
		else {
			q = Insert(p->lLink, leftForInsertion, L);
		}
		p->lLink = q;
		if (q->rLink == rightDummy)
		{
			q->rLink = p;
			q->isRThread = true;
		}
	}
	if (!rightForInsertion.isEmpty()) {
		SegmentNode1D* q;
		if (p->isRThread) {
			SegmentNode1D* succ = p->rLink;
			q = new SegmentNode1D(rightForInsertion);
			q->associatedSet = R;
			q->isRThread = true;
			q->rLink = succ;
			root->isRThread = false;
		}
		else {
			q = Insert(p->rLink, rightForInsertion, R);
		}
		p->rLink = q;
		if (q->lLink == leftDummy)
		{
			q->lLink = p;
			q->isLThread = true;
		}
	}
	return balance(p);
}

SegmentNode1D* SegmentTree1D::Remove(SegmentNode1D* p, const Segment& segment, StoredObjectNode* objPtr, bool leftSon) {
	if (!p) return NULL;

	if (segment.a < p->range.a && !p->isLThread) {
		p->lLink = Remove(p->lLink, segment, objPtr, true);
	}

	Segment C = p->range.overlap(segment);
	SegmentNode1D* replacementNode(p);
	if (!C.isEmpty() && p->associatedSet.erase(objPtr)) {
		SegmentNode1D* pred = inorderPredecessor(p);
		bool mergeableWithPred = (pred && p->associatedSet == pred->associatedSet);
		bool isEmptyNode = p->associatedSet.empty();
		if (isEmptyNode || mergeableWithPred) {
			if (!isEmptyNode) {
				pred->range = Segment(pred->range.a, p->range.b);
			}
			if (p->lLink == leftDummy && p->rLink == rightDummy) {
				delete p;
				p = NULL;
				return p;
			}
			bool pLThread = p->isLThread;
			SegmentNode1D* l = p->lLink;
			bool pRThread = p->isRThread;
			SegmentNode1D* r = p->rLink;
			delete p;
			if (pRThread) {
				if (!pLThread) {
					if (l != leftDummy) {
						l->rLink = r;
						replacementNode = l;
					}
					else {
						return l;
					}
				}
				else {
					if (leftSon) {
						r->isLThread = true;
						return l;
					}
					else {
						l->isRThread = true;
						return r;
					}
				}
			}
			else if (r == rightDummy)
			{
				if (pLThread)
					return r;
				else {
					l->isRThread = false;
					l->rLink = r;
					replacementNode = l;
				}
			}
			else {
				SegmentNode1D* min = findMin(r);
				min->associatedSet.erase(objPtr);
				if (min != r) {
					min->rLink = removeMin(r);
					min->isRThread = pRThread;
				}
				min->lLink = l;
				min->isLThread = pLThread;
				if (pred != leftDummy)
					pred->rLink = min;
				replacementNode = min;
			}
			p = replacementNode;
		}
		if (p && segment.b == p->range.b) {
			SegmentNode1D* succ = inorderSuccessor(p);
			if (succ) {
				mergeableTail = p;
				tailSuccessorRange = succ->range;
			}
		}
	}

	if (segment.b > p->range.b && !p->isRThread) {
		p->rLink = Remove(p->rLink, segment, objPtr, false);
	}
	return balance(p);
}

SegmentNode1D* SegmentTree1D::Remove(SegmentNode1D* p, const Segment& segment, bool leftSon) {
	if (p == leftDummy || p == rightDummy)
		return p;
	if (segment < p->range)
	{
		if (p->isLThread)
			return p;
		else
			p->lLink = Remove(p->lLink, segment, true);
	}
	else if (segment > p->range)
	{
		if (p->isRThread)
			return p;
		else
			p->rLink = Remove(p->rLink, segment, false);
	}
	else
	{
		if (p->lLink == leftDummy && p->rLink == rightDummy) {
			delete p;
			p = NULL;
			return p;
		}
		bool pLThread = p->isLThread;
		SegmentNode1D* l = p->lLink;
		bool pRThread = p->isRThread;
		SegmentNode1D* r = p->rLink;
		SegmentNode1D* predForDeletedNode = inorderPredecessor(p);
		delete p;
		if (pRThread)
		{
			if (!pLThread) {
				if (l != leftDummy) {
					l->rLink = r;
				}
				return l;
			}
			else {
				if (leftSon) {
					r->isLThread = true;
					return l;
				}
				else {
					l->isRThread = true;
					return r;
				}
			}
		}
		if (r == rightDummy)
		{
			if (pLThread)
				return r;
			else {
				l->isRThread = false;
				l->rLink = r;
				return l;
			}
		}
		SegmentNode1D* min = findMin(r);
		if (min != r) {
			min->rLink = removeMin(r);
			min->isRThread = pRThread;
		}
		min->lLink = l;
		min->isLThread = pLThread;
		if (predForDeletedNode != leftDummy)
			predForDeletedNode->rLink = min;
		return balance(min);
	}
	balance(p);
}

int SegmentTree1D::height(const SegmentNode1D* node) {
	return (node == NULL) ? 0 : (node->height);
}

int SegmentTree1D:: balanceFactor(const SegmentNode1D* node) {
	if (node == NULL)
		return 0;
	return ((node->isLThread == false) ? height(node->lLink) : 0) - ((node->isRThread == false) ? height(node->rLink) : 0);
}

void SegmentTree1D::fixHeight(SegmentNode1D* node)
{
	int heightL = (node->isLThread == false) ? height(node->lLink) : 0;
	int heightR = (node->isRThread == false) ? height(node->rLink) : 0;
	node->height = ((heightL > heightR) ? heightL : heightR) + 1;
}

SegmentNode1D* SegmentTree1D::rotateright(SegmentNode1D* p) // правый поворот вокруг p
{
	SegmentNode1D* q = p->lLink;
	if (q->isRThread == false) {
		p->lLink = q->rLink;
	}
	else {
		p->isLThread = true;
	}
	q->rLink = p;
	q->isRThread = false;
	fixHeight(p);
	fixHeight(q);
	return q;
}

SegmentNode1D* SegmentTree1D::rotateleft(SegmentNode1D* q) // левый поворот вокруг q
{
	SegmentNode1D* p = q->rLink;
	if (p->isLThread == false) {
		q->rLink = p->lLink;
	}
	else {
		q->isRThread = true;
	}
	p->lLink = q;
	p->isLThread = false;
	fixHeight(q);
	fixHeight(p);
	return p;
}

SegmentNode1D* SegmentTree1D::balance(SegmentNode1D* p) // балансировка узла p
{
	if (!p) return NULL;
	fixHeight(p);
	int bf = balanceFactor(p);
	if (bf == -2)
	{
		if (balanceFactor(p->rLink) > 0)
			p->rLink = rotateright(p->rLink);
		return rotateleft(p);
	}
	if (bf == 2)
	{
		if (balanceFactor(p->lLink) < 0)
			p->lLink = rotateleft(p->lLink);
		return rotateright(p);
	}
	return p;
}

SegmentNode1D* SegmentTree1D::getOverlappingNodeForPoint(SegmentNode1D* p, const double point) {
	if (p == NULL || p->range.isEmpty())
		return NULL;
	if (p->range.contains(point))
		return p;
	if (p->range.b < point)
		return getOverlappingNodeForPoint(p->rLink, point);
	return getOverlappingNodeForPoint(p->lLink, point);
}

void SegmentTree1D::Delete(SegmentNode1D*& p) {
	if (p != NULL) {
		if (!p->isLThread && !p->lLink) Delete(p->lLink);
		if (!p->isRThread && !p->rLink) Delete(p->rLink);
		delete p;
		p = NULL;
	}
}

SegmentNode1D* SegmentTree1D::inorderSuccessor(SegmentNode1D* p)
{
	if (p == NULL)
		return NULL;
	SegmentNode1D* q = p->rLink;
	if (p->isRThread)
		return q;
	while (q != rightDummy && !q->isLThread)
	{
		q = q->lLink;
	}
	return q;
}

SegmentNode1D* SegmentTree1D::inorderPredecessor(SegmentNode1D* p)
{
	if (p == NULL)
		return NULL;
	SegmentNode1D* q = p->lLink;
	if (p->isLThread)
		return q;
	while (q != leftDummy && !q->isRThread)
	{
		q = q->rLink;
	}
	return q;
}

SegmentNode1D* SegmentTree1D::findMin(SegmentNode1D* p)
{
	return (p->isLThread || p->lLink == leftDummy) ? p : findMin(p->lLink);
}

SegmentNode1D* SegmentTree1D::removeMin(SegmentNode1D* p) {
	if (p->lLink == leftDummy)
	{
		if (p->isRThread)
			return p->lLink;
		else return p->rLink;
	}
	else if (p->isLThread)
	{
		if (p->isRThread)
		{
			p->rLink->isLThread = true;
			return p;
		}
		else return p->rLink;
	}
	p->lLink = removeMin(p->lLink);
	balance(p);
}