#include "SegmentNode.h"
#include <stack>


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

void SegmentNode::rotateright(SegmentNode*& p) // правый поворот вокруг p
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
	p = q;
}

void SegmentNode::rotateleft(SegmentNode*& q) // левый поворот вокруг q
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
	q = p;
}

void SegmentNode::balance(SegmentNode*& p) // балансировка узла p
{
	if (p == NULL)
		return;
	fixHeight(p);
	int bf = balanceFactor(p);
	if (bf == -2)
	{
		if (balanceFactor(p->rLink) > 0)
			rotateright(p->rLink);
		rotateleft(p);
	}
	if (bf == 2)
	{
		if (balanceFactor(p->lLink) < 0)
			rotateleft(p->lLink);
		rotateright(p);
	}
}

void SegmentNode::INSERT(SegmentNode*& root, Segment range) {
	std::set<Segment> associatedSet = std::set<Segment>();
	associatedSet.insert(range);
	INSERT(root, range, associatedSet);
}

void SegmentNode:: INSERT(SegmentNode*& root, Segment range, std::set<Segment>& segmentsIds) // вставка отрезка в дерево с корнем p
{
	// вставка в пустое поддерево
	if (root == NULL) { 
		root = new SegmentNode(range); 
		root->associatedSet = segmentsIds;
		return;
	}

	std::stack<SegmentNode*> nodesPath;
	std::stack<bool> isReachedFromLeft;
	nodesPath.push(root);
	SegmentNode* p = root;
	SegmentNode* rootPtr = root;
	Segment C;
	Segment rootRange;
	bool reachedThread = false;
	// пытаемся найти узел, с диапазоном которого у нас есть пересечение
	do {
		rootRange = rootPtr->range;
		C = rootRange.overlap(range);
		p = rootPtr;
		if (C.isEmpty()) {
			if (range.isToTheLeftFrom(rootRange))
			{
				if (rootPtr->isLThread) {
					reachedThread = true;
				}
				else {
					rootPtr = rootPtr->lLink;
					isReachedFromLeft.push(true);
					nodesPath.push(rootPtr);
				}
			}
			else {
				if (rootPtr->isRThread) {
					reachedThread = true;
				}
				else {
					rootPtr = rootPtr->rLink;
					isReachedFromLeft.push(false);
					nodesPath.push(rootPtr);
				}
			}
		}
	} while (C.isEmpty() && rootPtr != leftDummy && rootPtr != rightDummy && !reachedThread);
	// есть пересечение с диапазоном некоторого узла
	if (!C.isEmpty()) {
		rootPtr->range = C;
		std::set<Segment> R = rootPtr->associatedSet;
		rootPtr->associatedSet.insert(segmentsIds.begin(), segmentsIds.end());
		// образовался отрезок слева от пересечения range и диапазона корня - L 
		if (range.a != rootRange.a) {
			if (range.a < rootRange.a) {
				R = segmentsIds;
			}
			Segment leftToOverlap = Segment(std::min(range.a, rootRange.a), std::max(range.a, rootRange.a));
			SegmentNode* newNode;
			SegmentNode* predecessor = rootPtr->lLink;
			if (rootPtr->isLThread) {
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
				R = segmentsIds;
			}
			Segment rightToOverlap = Segment(std::min(range.b, rootRange.b), std::max(range.b, rootRange.b));
			SegmentNode* newNode;
			SegmentNode* successor = rootPtr->rLink;
			if (rootPtr->isRThread) {
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

	if (reachedThread) {
		SegmentNode* newNode = new SegmentNode(range);
		newNode->associatedSet = segmentsIds;
		if (newNode->range.isToTheLeftFrom(rootPtr->range)) {
			newNode->isLThread = true;
			newNode->lLink = rootPtr->lLink;
			rootPtr->isLThread = false;
			rootPtr->lLink = newNode;
			newNode->isRThread = true;
			newNode->rLink = rootPtr;
			isReachedFromLeft.push(true);
		}
		else {
			newNode->isRThread = true;
			newNode->rLink = rootPtr->rLink;
			rootPtr->isRThread = false;
			rootPtr->rLink = newNode;
			newNode->isLThread = true;
			newNode->lLink = rootPtr;
			isReachedFromLeft.push(false);
		}
		nodesPath.push(newNode);
	}

	if (rootPtr == leftDummy || rootPtr == rightDummy) {
		SegmentNode* newNode = new SegmentNode(range);
		newNode->associatedSet = segmentsIds;
		if (p->range.isToTheLeftFrom(newNode->range)) {
			p->rLink = newNode;
			newNode->isLThread = true;
			newNode->lLink = p;
		}
		else {
			p->lLink = newNode;
			newNode->isRThread = true;
			newNode->rLink = p;
		}
		rootPtr = p;
		nodesPath.pop();
		nodesPath.push(newNode);
	}
	
	SegmentNode* kid;
	SegmentNode* parent = NULL;
	bool isReachedFromLeftVar;

	if (!nodesPath.empty()) {
		parent = nodesPath.top();
		nodesPath.pop();
	}

	
	/*while (!nodesPath.empty()) {

		kid = parent;
		balance(kid);

		if (!nodesPath.empty()) {
			parent = nodesPath.top();
			nodesPath.pop();
			
			isReachedFromLeftVar = isReachedFromLeft.top();
			isReachedFromLeft.pop();
			if (isReachedFromLeftVar) {
				parent->lLink = kid;
			}
			else {
				parent->rLink = kid;
			}
		}
	}

	balance(parent);*/
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

//SegmentNode* SegmentNode::removeMin(SegmentNode* p)
//{
//	if (p->lLink == leftDummy)
//	{
//		if (p->isRThread)
//			return p->lLink;
//		else return p->rLink;
//	}
//	else if (p->isLThread)
//	{
//		if (p->isRThread)
//		{
//			p->rLink->isLThread = true;
//			return p;
//		}
//		else return p->rLink;
//	}
//	p->lLink = removeMin(p->lLink);
//	balance(p);
//}
//
//SegmentNode* SegmentNode::remove(SegmentNode* p, Segment range, bool leftSon)
//{
//	if (p == leftDummy || p == rightDummy)
//		return p;
//	if (range < p->range)
//	{
//		if (p->isLThread)
//			return p;
//		else
//			p->lLink = remove(p->lLink, range, true);
//	}
//	else if (range > p->range)
//	{
//		if (p->isRThread)
//			return p;
//		else
//			p->rLink = remove(p->rLink, range, false);
//	}
//	else
//	{
//		if (p->lLink == leftDummy && p->rLink == rightDummy) {
//			delete p;
//			p = NULL;
//			return p;
//		}
//		bool pLThread = p->isLThread;
//		SegmentNode* l = p->lLink;
//		bool pRThread = p->isRThread;
//		SegmentNode* r = p->rLink;
//		SegmentNode* predForDeletedNode = inorderPredecessor(p);
//		delete p;
//		if (pRThread)
//		{
//			if (!pLThread) {
//				if (l != leftDummy) {
//					l->rLink = r;
//				}
//				return l;
//			}
//			else {
//				if (leftSon) {
//					r->isLThread = true;
//					return l;
//				}
//				else {
//					l->isRThread = true;
//					return r;
//				}
//			}
//		}
//		if (r == rightDummy)
//		{
//			if (pLThread)
//				return r;
//			else {
//				l->isRThread = false;
//				l->rLink = r;
//				return l;
//			}
//		}
//		SegmentNode* min = findMin(r);
//		if (min != r) {
//			min->rLink = removeMin(r);
//			min->isRThread = pRThread;
//		}
//		min->lLink = l;
//		min->isLThread = pLThread;
//		if (predForDeletedNode != leftDummy)
//			predForDeletedNode->rLink = min;
//		return balance(min);
//	}
//	balance(p);
//}