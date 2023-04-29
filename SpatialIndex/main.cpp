#include <string>
#include <ctime>

#include <stdio.h>
#include "Node.h"
#include "TreePrinter.h"
#include <random>
#include <Windows.h>
#include <chrono>



int main()
{
	Node* root = new Node(0);
	TreePrinter printer = TreePrinter();
	printer.printToConsole(root);
	int cnt = 100000;
	/*while (cnt >=0) {
		cnt--;
		std::cout 
			<< std::endl 
			<< "---------------------------------------------------------------" 
			<< std::endl << "i/r + num: ";
		char c;
		int num;
		std::cin >> c >> num;
		switch (c) {
		case 'i':
			root = Node::insert(root, num);
			break;
		case 'r':
			root = Node::remove(root, num);
			break;
		}
		TreePrinter::printToConsole(root);
	}*/

	while (cnt > 0) {
		cnt--;
		bool insert = true;
		if (insert) {
			root = Node::insert(root, cnt);
		}
	}
	auto start = std::chrono::high_resolution_clock::now();
	root = Node::insert(root, 1000000);
	auto finish = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);

	// To get the value of duration use the count()
	// member function on the duration object
	std::cout << duration.count() << std::endl << std::endl;
	printer.printToConsole(root);
}