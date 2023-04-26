#include <string>
#include <ctime>

#include <stdio.h>
#include "Node.h"



int main()
{
	Node* root = new Node(99);
	for (int i = 0; i <= 10; i++)
	{
		root = Node::insert(root, i);
	}
	/*std::vector<int> inorderSequence;
	Node::inorderTraverse(root, inorderSequence);
	for (int i = 0; i < inorderSequence.size(); i++)
		std::cout << inorderSequence[i] << " ";*/
	Node* p = root;
	root = Node::remove(root, 8);

}