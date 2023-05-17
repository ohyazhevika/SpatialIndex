#include "SegmentNode.h"

SegmentNode* SegmentNode::leftDummy = NULL;
SegmentNode* SegmentNode::rightDummy = NULL;

Segment SegmentNode::deletableTail = Segment();


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

void SegmentNode::Insert(SegmentNode*& root, Segment range) {
	std::set<Segment> associatedSet = std::set<Segment>();
	associatedSet.insert(range);
	root = Insert (root, range, associatedSet);
}

// из узла p удаляет сегмент segment, если узел теперь пуст или может слиться с предыдущим - сливаем;
// возвращаем указатель на себя же; если смержили - то возвращается указатель на предшественника прямого
std::pair<SegmentNode*, bool> SegmentNode::rem(SegmentNode* p, const Segment& segment) {
	if (!p) return std::make_pair(nullptr, false);

	if (segment.a < p->range.a && !p->isLThread) {

		std::pair<SegmentNode*, bool> pair(rem(p->lLink, segment));
		p->lLink = pair.first;
		p->isLThread = pair.second;
	}
	Segment C = p->range.overlap(segment);
	if (!C.isEmpty()) {
		if (p->associatedSet.erase(segment))
		{
			SegmentNode* pred = inorderPredecessor(p);
			bool mergeableWithPred = (pred && p->associatedSet == pred->associatedSet);
			bool isEmptyNode = p->associatedSet.empty();
			if (isEmptyNode || mergeableWithPred) {
				if (p->isLThread) {
					std::pair<SegmentNode*, bool> pair = std::make_pair(p->rLink, p->isRThread);
					if (segment.b > p->range.b && !p->isRThread) {
						pair = rem(p->rLink, segment);
						p->rLink = pair.first;
						p->isRThread = pair.second;
					}
					if (!isEmptyNode) pred->range = Segment(pred->range.a, p->range.b);
					if (pred->rLink == p) {
						pred = remove(pred, p->range);
						return std::make_pair(pred->rLink, pair.second);
					}
					pred = remove(pred, p->range);
					SegmentNode* returnedNode = !pair.second ? pair.first : pred;
					return std::make_pair(returnedNode, pair.second);
				}
				else if (!p->lLink) {
					if (segment.b > p->range.b && !p->isRThread) {
						std::pair<SegmentNode*, bool> pair(rem(p->rLink, segment));
						p->rLink = pair.first;
						p->isRThread = pair.second;
					}
					SegmentNode* returnedNode;
					if (p->isRThread) {
						returnedNode = leftDummy;
					}
					else {
						if (p->rLink) {
							p->rLink->lLink = leftDummy;
							p->rLink->isLThread = false;
						}
						returnedNode = p->rLink;
					} 
					delete p;
					return std::make_pair(returnedNode, false);
				}
				else {
					if (!isEmptyNode) p->range = Segment(pred->range.a, p->range.b);
					p = remove(p, pred->range);
				}
			}
			if (segment.b == p->range.b) {

				SegmentNode* succ = inorderSuccessor(p);
				bool mergeableWithSucc = (succ && p->associatedSet == succ->associatedSet);
				bool isEmptyNode = p->associatedSet.empty();
				if (isEmptyNode || mergeableWithSucc) {
					if (p->isRThread) {
						std::pair<SegmentNode*, bool> pair = std::make_pair(p->lLink, p->isLThread);
						if (!isEmptyNode) succ->range = Segment(p->range.a,succ->range.b);
						if (succ->lLink == p) {
							succ = remove(succ, p->range);
							return std::make_pair(succ->rLink, pair.second);
						}
						succ = remove(succ, p->range);
						SegmentNode* returnedNode = !pair.second ? pair.first : succ;
						return std::make_pair(returnedNode, pair.second);
					}
					else if (!p->rLink) {
						if (segment.b > p->range.b && !p->isRThread) {
							std::pair<SegmentNode*, bool> pair(rem(p->rLink, segment));
							p->rLink = pair.first;
							p->isRThread = pair.second;
						}
						SegmentNode* returnedNode;
						if (p->isLThread) {
							returnedNode = rightDummy;
						}
						else {
							if (p->lLink) {
								p->lLink->rLink = leftDummy;
								p->lLink->isRThread = false;
							}
							returnedNode = p->rLink;
						}
						delete p;
						return std::make_pair(returnedNode, false);
					}
					else {
						if (!isEmptyNode) p->range = Segment(p->range.a, succ->range.b);
						p = remove(p, succ->range);
					}
				}
			}
		}
	}
	if (segment.b > p->range.b && !p->isRThread) {
		std::pair<SegmentNode*, bool> pair(rem(p->rLink, segment));
		p->rLink = pair.first;
		p->isRThread = pair.second;
	}
	return std::make_pair(balance(p), false);
}

SegmentNode* SegmentNode::Insert(SegmentNode* root, Segment range, std::set<Segment>& segmentsIds) {
	if (root == NULL) {
		SegmentNode* newNode = new SegmentNode(range);
		newNode->associatedSet = segmentsIds;
		return newNode;
	}
	
	Segment rootRange = root->range;
	Segment leftForInsertion, C, rightForInsertion;
	std::set<Segment> R = root->associatedSet;
	std::set<Segment> L = root->associatedSet;
	C = range.overlap(rootRange);
	if (!C.isEmpty()) {
		root->range = C;
		root->associatedSet.insert(segmentsIds.begin(), segmentsIds.end());
		if (range.a != rootRange.a) {
			if (range.a < rootRange.a) {
				L = segmentsIds;
			}
			leftForInsertion = Segment(std::min(range.a, rootRange.a), std::max(range.a, rootRange.a));
		}
		if (range.b != rootRange.b) {
			if (range.b > rootRange.b) {
				R = segmentsIds;
			}
			rightForInsertion = Segment(std::min(range.b, rootRange.b), std::max(range.b, rootRange.b));
		}
	}
	else {
		if (range.isToTheLeftFrom(rootRange)) {
			leftForInsertion = range;
			L = segmentsIds;
		}
		else {
			rightForInsertion = range;
			R = segmentsIds;
		}
	}
	if (!leftForInsertion.isEmpty()) {
		SegmentNode* q;
		if (root->isLThread) {
			SegmentNode* pred = root->lLink;
			q = new SegmentNode(leftForInsertion);
			q->associatedSet = L;
			q->isLThread = true;
			q->lLink = pred;
			root->isLThread = false;
		}
		else {
			q = Insert(root->lLink, leftForInsertion, L);
		}
		root->lLink = q;
		if (q->rLink == rightDummy)
		{
			q->rLink = root;
			q->isRThread = true;
		}
	}
	if (!rightForInsertion.isEmpty()) {
		SegmentNode* q;
		if (root->isRThread) {
			SegmentNode* succ = root->rLink;
			q = new SegmentNode(rightForInsertion);
			q->associatedSet = R;
			q->isRThread = true;
			q->rLink = succ;
			root->isRThread = false;
		}
		else {
			q = Insert(root->rLink, rightForInsertion, R);
		}
		root->rLink = q;
		if (q->lLink == leftDummy)
		{
			q->lLink = root;
			q->isLThread = true;
		}
	}
	return balance(root);
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
	balance(p);
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

SegmentNode* SegmentNode::getOverlappingNodeForPoint(SegmentNode* root, const double point) {
	if (root == NULL || root->range.isEmpty())
		return NULL;
	if (root->range.contains(point))
		return root;
	if (root->range.b < point)
		return getOverlappingNodeForPoint(root->rLink, point);
	 return getOverlappingNodeForPoint(root->lLink, point);
}