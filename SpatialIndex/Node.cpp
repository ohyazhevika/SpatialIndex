#include "Node.h"

StoredObjectNode* StoredObjectNode::leftDummy = NULL;
StoredObjectNode* StoredObjectNode::rightDummy = NULL;

StoredObjectNode::StoredObjectNode(int k) : k(k), rLink(rightDummy), lLink(leftDummy), height(1), isLThread(false), isRThread(false) {}


int StoredObjectNode::getHeight(const StoredObjectNode* node)
{
	return (node == NULL) ? 0 : (node->height);
}

int StoredObjectNode::balanceFactor(const StoredObjectNode* node)
{
	if (node == NULL)
		return 0;
	return ((node->isLThread == false) ? getHeight(node->lLink) : 0) - ((node->isRThread == false) ? getHeight(node->rLink) : 0);
}

void StoredObjectNode::fixHeight(StoredObjectNode* node)
{
	int heightL = (node->isLThread == false) ? getHeight(node->lLink) : 0;
	int heightR = (node->isRThread == false) ? getHeight(node->rLink) : 0;
	node->height = ((heightL > heightR) ? heightL : heightR) + 1;
}

StoredObjectNode* StoredObjectNode::rotateright(StoredObjectNode* p) // правый поворот вокруг p
{
	StoredObjectNode* q = p->lLink;
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

StoredObjectNode* StoredObjectNode::rotateleft(StoredObjectNode* q) // левый поворот вокруг q
{
	StoredObjectNode* p = q->rLink;
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

StoredObjectNode* StoredObjectNode::balance(StoredObjectNode* p) // балансировка узла p
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

StoredObjectNode* StoredObjectNode::insert(StoredObjectNode* p, int k) // вставка ключа k в дерево с корнем p
{
	if (p == NULL) return new StoredObjectNode(k);
	if (k < p->k)
	{
		StoredObjectNode* q;
		if (p->isLThread)
		{
			StoredObjectNode* pred = p->lLink;
			q = new StoredObjectNode(k);
			q->isLThread = true;
			q->lLink = pred;
			p->isLThread = false;
		}
		else //if (!p->isLThread)
		{
			q = insert(p->lLink, k);
		}
		p->lLink = q;
		if (q->rLink == rightDummy)
		{
			q->rLink = p;
			q->isRThread = true;
		}
	}
	else if (k > p->k)
	{
		StoredObjectNode* q;
		if (p->isRThread) {
			StoredObjectNode* succ = p->rLink;
			q = new StoredObjectNode(k);
			q->isRThread = true;
			q->rLink = succ;
			p->isRThread = false;
		}
		else
		{
			q = insert(p->rLink, k);
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


StoredObjectNode* StoredObjectNode::inorderSuccessor(StoredObjectNode* p)
{
	if (p == NULL)
		return NULL;
	StoredObjectNode* q = p->rLink;
	if (p->isRThread)
		return q;
	while (q != rightDummy && !q->isLThread)
	{
		q = q->lLink;
	}
	return q;
}

StoredObjectNode* StoredObjectNode::inorderPredecessor(StoredObjectNode* p)
{
	if (p == NULL)
		return NULL;
	StoredObjectNode* q = p->lLink;
	if (p->isLThread)
		return q;
	while (q != leftDummy && !q->isRThread)
	{
		q = q->rLink;
	}
	return q;
}

StoredObjectNode* StoredObjectNode:: findMin(StoredObjectNode* p)
{
	return (p->isLThread || p->lLink == leftDummy) ? p : findMin(p->lLink);
}

StoredObjectNode* StoredObjectNode::removeMin(StoredObjectNode* p)
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

StoredObjectNode* StoredObjectNode::remove(StoredObjectNode* p, int k, bool leftSon)
{
	if (p == leftDummy || p == rightDummy)
		return p;
	if (k < p->k)
	{
		if (p->isLThread)
			return p;
		else
			p->lLink = remove(p->lLink, k, true);
	}
	else if (k > p->k)
	{
		if (p->isRThread)
			return p;
		else
			p->rLink = remove(p->rLink, k, false);
	}
	else
	{
		if (p ->lLink == leftDummy && p->rLink == rightDummy) {
			delete p;
			p = NULL;
			return p;
		}
		bool pLThread = p->isLThread;
		StoredObjectNode* l = p->lLink;
		bool pRThread = p->isRThread;
		StoredObjectNode* r = p->rLink;
		StoredObjectNode* predForDeletedNode = inorderPredecessor(p);
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
		StoredObjectNode* min = findMin(r);
		if (min != r) {
			min->rLink = removeMin(r);
			min->isRThread = pRThread;
		}
		min->lLink = l;
		min->isLThread = pLThread;
		if(predForDeletedNode != leftDummy)
			predForDeletedNode->rLink = min;
		return balance(min);
	}
	return balance(p);
}