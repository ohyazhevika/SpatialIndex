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

Node* Node::rotateright(Node* p) // ������ ������� ������ p
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

Node* Node::rotateleft(Node* q) // ����� ������� ������ q
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

Node* Node::balance(Node* p) // ������������ ���� p
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
	return p; // ������������ �� �����
}

Node* Node::insert(Node* p, int k) // ������� ����� k � ������ � ������ p
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