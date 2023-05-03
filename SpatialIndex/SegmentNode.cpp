#include "SegmentNode.h"


SegmentNode* SegmentNode::leftDummy = NULL;
SegmentNode* SegmentNode::rightDummy = NULL;

SegmentNode::SegmentNode(Segment range) :	range(range),
									rLink(rightDummy),
									lLink(leftDummy),
									height(1),
									isLThread(false),
									isRThread(false) {}


int SegmentNode::getHeight(const SegmentNode* node)
{
	return (node == NULL) ? 0 : (node->height);
}

int SegmentNode::balanceFactor(const SegmentNode* node)
{
	if (node == NULL)
		return 0;
	return ((node->isLThread == false) ? getHeight(node->lLink) : 0) - ((node->isRThread == false) ? getHeight(node->rLink) : 0);
}

void SegmentNode::fixHeight(SegmentNode* node)
{
	int heightL = (node->isLThread == false) ? getHeight(node->lLink) : 0;
	int heightR = (node->isRThread == false) ? getHeight(node->rLink) : 0;
	node->height = ((heightL > heightR) ? heightL : heightR) + 1;
}

SegmentNode* SegmentNode::rotateright(SegmentNode* p) // правый поворот вокруг p
{
	SegmentNode* q = p->lLink;
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

SegmentNode* SegmentNode::rotateleft(SegmentNode* q) // левый поворот вокруг q
{
	SegmentNode* p = q->rLink;
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

SegmentNode* SegmentNode::balance(SegmentNode* p) // балансировка узла p
{
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
	return p; // балансировка не нужна
}

SegmentNode* SegmentNode::insert(SegmentNode* p, Segment range) // вставка отрезка в дерево с корнем p
{
	if (p == NULL) return new SegmentNode(range);
	if (range < p->range)
	{
		SegmentNode* q;
		if (p->isLThread)
		{
			SegmentNode* pred = p->lLink;
			q = new SegmentNode(range);
			q->isLThread = true;
			q->lLink = pred;
			p->isLThread = false;
		}
		else //if (!p->isLThread)
		{
			q = insert(p->lLink, range);
		}
		p->lLink = q;
		if (q->rLink == rightDummy)
		{
			q->rLink = p;
			q->isRThread = true;
		}
	}
	else if (range > p->range)
	{
		SegmentNode* q;
		if (p->isRThread) {
			SegmentNode* succ = p->rLink;
			q = new SegmentNode(range);
			q->isRThread = true;
			q->rLink = succ;
			p->isRThread = false;
		}
		else
		{
			q = insert(p->rLink, range);
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


void SegmentNode:: INSERT(SegmentNode*& root, Segment range, std::set<Segment> segmentsIds) // вставка отрезка в дерево с корнем p
{
	// вставка в пустое поддерево
	if (root == NULL) { 
		root = new SegmentNode(range); 
		root->associatedSet = segmentsIds;
	}

	SegmentNode* p = NULL;
	SegmentNode* rootPtr = root;
	Segment C;
	Segment rootRange;

	// пытаемся найти узел, с диапазоном которого у нас есть пересечение
	while (C.isEmpty() && rootPtr != leftDummy && rootPtr != rightDummy) {
		rootRange = rootPtr->range;
		C = rootRange.overlap(range);
		p = rootPtr;
		if (C.isEmpty()) {
			if (range < rootRange)
			{
				rootPtr = rootPtr->lLink;
			}
			else {
				rootPtr = rootPtr->rLink;
			}
		}
	}
	// есть пересечение с диапазоном некоторого узла
	if (!C.isEmpty()) {
		rootPtr->range = C;
		std::set<Segment> R = rootPtr->associatedSet;
		rootPtr->associatedSet.insert(range);
		// образовался отрезок слева от пересечения range и диапазона корня - L 
		if (range.a != rootRange.a) {
			if (range.a < rootRange.a) {
				R.insert(range);
			}
			Segment leftToOverlap = Segment(std::min(range.a, rootRange.a), std::max(range.a, rootRange.a));
			SegmentNode* newNode;
			SegmentNode* predecessor = rootPtr->lLink;
			if (rootPtr->isLThread/* || rootPtr->lLink == leftDummy*/) {
				newNode = new SegmentNode(leftToOverlap);
				newNode->associatedSet.insert(range);
				newNode->lLink = predecessor;
				newNode->isLThread = true;
				rootPtr->isLThread = false;
			}
			else {
				INSERT(predecessor, leftToOverlap, R);
				newNode = predecessor;
			}
			rootPtr->lLink = newNode;
			if (newNode->rLink == rightDummy)
			{
				newNode->rLink = rootPtr;
				newNode->isRThread = true;
			}
		}

		// образовался отрезок справа от пересечения range и диапазона корня - R
		if (range.b != rootRange.b) {
			if (range.b > rootRange.b) {
				R.insert(range);
			}
			Segment rightToOverlap = Segment(std::min(range.b, rootRange.b), std::max(range.b, rootRange.b));
			SegmentNode* newNode;
			SegmentNode* successor = rootPtr->rLink;
			if (rootPtr->isRThread/* || rootPtr->rLink == rightDummy*/) {
				newNode = new SegmentNode(rightToOverlap);
				newNode->associatedSet.insert(range);
				newNode->rLink = successor;
				newNode->isRThread = true;
				rootPtr->isRThread = false;

			}
			else {
				INSERT(successor, rightToOverlap, R);
				newNode = successor;
			}
			rootPtr->rLink = newNode;
			if (newNode->lLink == leftDummy)
			{
				newNode->lLink = rootPtr;
				newNode->isLThread = true;
			}
		}

	}

	if (rootPtr == leftDummy || rootPtr == rightDummy) {
		SegmentNode* newNode = new SegmentNode(range);
		newNode->associatedSet = segmentsIds;
		if (p != NULL) {
			if (p->range < newNode->range) {
				p->rLink = newNode;
				newNode->isLThread = true;
				newNode->lLink = p;
			}
			else {
				p->lLink = newNode;
				newNode->isRThread = true;
				newNode->rLink = p;
			}
		}
	}
	root = balance(root);
}

SegmentNode* SegmentNode::Insert(SegmentNode* root, Segment range) {
	std::set<Segment> segmentsIds = std::set<Segment>();
	segmentsIds.insert(range);
	return SegmentNode::Insert(root, range, segmentsIds);
}

SegmentNode* SegmentNode::Insert(SegmentNode* root, Segment range, std::set<Segment> segmentsIds) {
	SegmentNode* p = NULL;
	SegmentNode* rootPtr = root;
	Segment rangeToInsert = range;
	if (rootPtr != NULL && rootPtr != leftDummy && rootPtr != rightDummy) {
		Segment C;
		Segment rootRange;
		while (C.isEmpty() && rootPtr != leftDummy && rootPtr != rightDummy) {
			rootRange = rootPtr->range;
			C = rootRange.overlap(rangeToInsert);
			p = rootPtr;
			if (C.isEmpty()) {
				if (rangeToInsert < rootRange)
				{
					rootPtr = rootPtr->lLink;
				}
				else {
					rootPtr = rootPtr->rLink;
				}
			}
		}
		if (!C.isEmpty()) {
			rootPtr->range = C;
			std::set<Segment> R = rootPtr->associatedSet;
			rootPtr->associatedSet.insert(rangeToInsert);
			if (rangeToInsert.a != rootRange.a) {
				if (rangeToInsert.a < rootRange.a) {
					R.insert(rangeToInsert);
				}
				SegmentNode* predecessor = inorderPredecessor(rootPtr);
				Segment newRangeToInsert = Segment(std::min(rangeToInsert.a, rootRange.a), std::max(rangeToInsert.a, rootRange.a));
				predecessor = Insert(predecessor, newRangeToInsert, R);
			}
			if (rangeToInsert.b != rootRange.b) {
				if (rangeToInsert.b > rootRange.b) {
					R.insert(rangeToInsert);
				}
				SegmentNode* successor = inorderSuccessor(rootPtr);
				Segment newRangeToInsert = Segment(std::min(rangeToInsert.b, rootRange.b), std::max(rangeToInsert.b, rootRange.b));
				successor = Insert(successor, newRangeToInsert, R);
			}
		}
	}
	if (rootPtr == NULL || rootPtr == leftDummy || rootPtr == rightDummy) {
		SegmentNode* newNode = new SegmentNode(rangeToInsert);
		newNode->associatedSet = segmentsIds;
		if (p != NULL) {
			if (p->range < newNode->range) {
				p->rLink = newNode;
			}
			else {
				p->lLink = newNode;
			}
		}
	}
	return balance(rootPtr);
}


SegmentNode* SegmentNode::inorderSuccessor(SegmentNode* p)
{
	if (p == NULL)
		return NULL;
	SegmentNode* q = p->rLink;
	if (p->isRThread)
		return q;
	while (q != rightDummy && !q->isLThread)
	{
		q = q->lLink;
	}
	return q;
}

SegmentNode* SegmentNode::inorderPredecessor(SegmentNode* p)
{
	if (p == NULL)
		return NULL;
	SegmentNode* q = p->lLink;
	if (p->isLThread)
		return q;
	while (q != leftDummy && !q->isRThread)
	{
		q = q->rLink;
	}
	return q;
}

SegmentNode* SegmentNode::findMin(SegmentNode* p)
{
	return (p->isLThread || p->lLink == leftDummy) ? p : findMin(p->lLink);
}

SegmentNode* SegmentNode::removeMin(SegmentNode* p)
{
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
	return balance(p);
}

SegmentNode* SegmentNode::remove(SegmentNode* p, Segment range, bool leftSon)
{
	if (p == leftDummy || p == rightDummy)
		return p;
	if (range < p->range)
	{
		if (p->isLThread)
			return p;
		else
			p->lLink = remove(p->lLink, range, true);
	}
	else if (range > p->range)
	{
		if (p->isRThread)
			return p;
		else
			p->rLink = remove(p->rLink, range, false);
	}
	else
	{
		if (p->lLink == leftDummy && p->rLink == rightDummy) {
			delete p;
			p = NULL;
			return p;
		}
		bool pLThread = p->isLThread;
		SegmentNode* l = p->lLink;
		bool pRThread = p->isRThread;
		SegmentNode* r = p->rLink;
		SegmentNode* predForDeletedNode = inorderPredecessor(p);
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
		SegmentNode* min = findMin(r);
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