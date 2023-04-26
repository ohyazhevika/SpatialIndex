#include "Node.h"

Node* Node::leftDummy = NULL;
Node* Node::rightDummy = NULL;

Node::Node(int k) : k(k), rLink(rightDummy), lLink(leftDummy), height(1), isLThread(false), isRThread(false) {}


int Node::getHeight(const Node* node)
{
	return (node == NULL) ? 0 : (node->height);
}

int Node::balanceFactor(const Node* node)
{
	if (node == NULL)
		return 0;
	return ((node->isLThread == false) ? getHeight(node->lLink) : 0) - ((node->isRThread == false) ? getHeight(node->rLink) : 0);
}

void Node::fixHeight(Node* node)
{
	int heightL = (node->isLThread == false) ? getHeight(node->lLink) : 0;
	int heightR = (node->isRThread == false) ? getHeight(node->rLink) : 0;
	node->height = ((heightL > heightR) ? heightL : heightR) + 1;
}

Node* Node::rotateright(Node* p) // правый поворот вокруг p
{
	Node* q = p->lLink;
	if (q->isRThread == false)
		p->lLink = q->rLink;
	else {
		p->isLThread = true;
		p->lLink = q;
	}
	q->rLink = p;
	q->isRThread = false;
	fixHeight(p);
	fixHeight(q);
	return q;
}

Node* Node::rotateleft(Node* q) // левый поворот вокруг q
{
	Node* p = q->rLink;
	if (p->isLThread == false)
		q->rLink = p->lLink;
	else {
		q->isRThread = true;
		q->rLink = p;
	}
	p->lLink = q;
	p->isLThread = false;
	fixHeight(q);
	fixHeight(p);
	return p;
}

Node* Node::balance(Node* p) // балансировка узла p
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

Node* Node::insert(Node* p, int k) // вставка ключа k в дерево с корнем p
{
	if (p == NULL) return new Node(k);
	if (k < p->k)
	{
		Node* q;
		if (p->isLThread)
		{
			Node* pred = p->lLink;
			q = new Node(k);
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
		Node* q;
		if (p->isRThread) {
			Node* succ = p->rLink;
			q = new Node(k);
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


Node* Node::inorderSuccessor(Node* p)
{
	if (p == NULL)
		return NULL;
	Node* q = p->rLink;
	if (p->isRThread)
		return q;
	while (q != rightDummy && !q->isLThread)
	{
		q = q->lLink;
	}
	return q;
}

Node* Node::inorderPredecessor(Node* p)
{
	if (p == NULL)
		return NULL;
	Node* q = p->lLink;
	if (p->isLThread)
		return q;
	while (q != leftDummy && !q->isRThread)
	{
		q = q->rLink;
	}
	return q;
}

Node* Node:: findMin(Node* p)
{
	return (p->isLThread || p->lLink == leftDummy) ? p : findMin(p->lLink);
}

Node* Node::removeMin(Node* p)
{
	//if (p->isLThread || p->lLink == leftDummy)
		//return p->rLink;
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

Node* Node::remove(Node* p, int k)
{
	if (p == leftDummy || p == rightDummy)
		return p;
	if (k < p->k)
	{
		if (p->isLThread)
			return p;
		else
			p->lLink = remove(p->lLink, k);
	}
	else if (k > p->k)
	{
		if (p->isRThread)
			return p;
		else
			p->rLink = remove(p->rLink, k);
	}
	else
	{
		bool pLThread = p->isLThread;
		Node* l = p->lLink;
		bool pRThread = p->isRThread;
		Node* r = (p->isRThread) ? NULL : p->rLink;
		Node* predForDeletedNode = inorderPredecessor(p);
		delete p;
		if (pRThread)
		{
			return l;
		}
		Node* min = findMin(r);
		min->rLink = removeMin(r);
		min->isRThread = false;
		min->lLink = l;
		min->isLThread = false;
		if(predForDeletedNode != leftDummy)
			predForDeletedNode->rLink = min;
		return balance(min);
	}
	balance(p);
}