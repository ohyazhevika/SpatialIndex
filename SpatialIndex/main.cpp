#include <string>
#include <ctime>

#include <stdio.h>
#include "TreePrinter.h"
#include "SegmentTree.h"
#include <random>
#include <Windows.h>
#include <chrono>
#include <set>



int main()
{
	SegmentTree tree;
	TreePrinter printer = TreePrinter();
	std::set<Segment> deletables;
	int cnt = 0;
	while (cnt < 100) {
		cnt++;
		double
			left = std::rand() % 1000,
			length = std::rand() % 100 + 1,
			right = left + length;
		tree.Insert(Segment(left, right));
		deletables.insert(Segment(left, right));
		printer.printToConsole(tree.root);
	}

	std::cout << "-------------------------------------------------------------------------------------\n";
	
	for (auto it = deletables.begin(); it != deletables.end(); it++) {
		tree.root = SegmentNode::rem(tree.root, *it, true);
		printer.printToConsole(tree.root);
	}
	tree.root = SegmentNode::rem(tree.root, Segment(41, 109), true);


	/*auto start = std::chrono::high_resolution_clock::now();

	auto finish = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);

	std::cout << duration.count() << std::endl << std::endl;
	printer.printToConsole(root);*/
}