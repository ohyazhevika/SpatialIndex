#include <string>
#include <ctime>

#include <stdio.h>
#include "TreePrinter.h"
#include "SegmentTree.h"
#include "AVL-Tree.h"
#include <random>
#include <Windows.h>
#include <chrono>
#include <set>


StoredObject storedObject(string name, double x1, double x2, double y1, double y2) {
	StoredObject o;
	o.label = name;
	o.xRange = Segment(x1, x2);
	o.yRange = Segment(y1, y2);
	return o;
}

int main()
{

	TreePrinter printer;
	SegmentTree tree;
	std::set<Segment> segs;
	int count = 0;
	while (count < 100) {
		count++;
		double
			left = std::rand() % 1000,
			length = std::rand() % 100 + 1,
			right = left + length;
		Segment seg(left, right);
		segs.insert(seg);
		tree.Insert(seg);
		std::cout << "Insert " << "[" << seg.a << ", " << seg.b << "]\n\n";
		printer.printToConsole(tree.root);
		std::cout << "\n\n";
	}
	while (!segs.empty()) {
		Segment seg = *(segs.begin());
		segs.erase(seg);
		tree.Remove(seg);
		std::cout << "Remove " << "[" << seg.a << ", " << seg.b << "]\n\n";
		printer.printToConsole(tree.root);
		std::cout << "\n\n";
	}

	/*SegmentTree2D tree;
	tree.Insert(storedObject("R1", 1, 2, 3, 4));
	tree.Insert(storedObject("R2", -1, 0.499, 3, 4));
	tree.Insert(storedObject("R3", 0.69990, 0.69999, 20, 24));
	tree.Insert(storedObject("R4", 91, 92, 3, 4));
	tree.Insert(storedObject("R5", -12, 0.5, 19, 20));
	std::set<StoredObject> windowQ = tree.getOverlappedByRectangle(Segment(0.5, 0.7), Segment(0.7, 19));*/
	/*auto start = std::chrono::high_resolution_clock::now();
	auto finish = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
	std::cout << duration.count() << std::endl << std::endl;
	printer.printToConsole(root);*/
}