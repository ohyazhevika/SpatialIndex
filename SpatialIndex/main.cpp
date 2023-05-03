#include <string>
#include <ctime>

#include <stdio.h>
#include "SegmentNode.h"
#include "TreePrinter.h"
#include <random>
#include <Windows.h>
#include <chrono>
#include <set>



int main()
{
	SegmentNode* root = new SegmentNode(Segment(20, 25));
	TreePrinter printer = TreePrinter();
	printer.printToConsole(root);
	std::set<Segment> sett;
	Segment segs[] = {Segment()};
	SegmentNode::INSERT(root, Segment(10, 11), sett);
	printer.printToConsole(root);

	/*Node* root = new Node(0);
	TreePrinter printer = TreePrinter();
	printer.printToConsole(root);
	int cnt = 100000;


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

	std::cout << duration.count() << std::endl << std::endl;
	printer.printToConsole(root);*/
}