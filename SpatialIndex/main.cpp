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

	TreePrinter printer = TreePrinter();
	SegmentNode* root = NULL;
	int cnt = 0;
	while (cnt < 10) {
		cnt++;
		double
			left = std::rand() % 100,
			length = std::rand() % 100 + 1,
			right = left + length;
		SegmentNode::INSERT(root, Segment(left, right));
		printer.printToConsole(root);
	}


	/*auto start = std::chrono::high_resolution_clock::now();

	auto finish = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);

	std::cout << duration.count() << std::endl << std::endl;
	printer.printToConsole(root);*/
}