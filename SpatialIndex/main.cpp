#include <string>
#include <ctime>

#include <stdio.h>
#include "TreePrinter.h"
#include "SegmentTree2D.h"
#include "AVL-Tree.h"
#include <random>
#include <Windows.h>
#include <chrono>
#include <set>


StoredObject storedObject(string name, double x1, double x2, double y1, double y2) {
	BoundingBox bound(Segment(x1, x2), Segment(y1, y2));
	return StoredObject(name, bound);
}

int main()
{
	SegmentTree2D tree;
	tree.Insert(storedObject("R1", 1, 2, 3, 4));
	tree.Insert(storedObject("R2", -1, 0.499, 3, 4));
	tree.Insert(storedObject("R3", 0.69990, 0.69999, 20, 24));
	tree.Insert(storedObject("R4", 91, 92, 3, 4));
	tree.Insert(storedObject("R5", -12, 0.5, 19, 20));
	std::set<StoredObject> windowQ = tree.getOverlappedByRectangle(Segment(0.5, 0.7), Segment(0.7, 19));
	tree.Remove(storedObject("R1", 1, 2, 3, 4));
	tree.Remove(storedObject("R2", -1, 0.499, 3, 4));
	tree.Remove(storedObject("R3", 0.69990, 0.69999, 20, 24));
	tree.Remove(storedObject("R4", 91, 92, 3, 4));
	tree.Remove(storedObject("R5", -12, 0.5, 19, 20));
	std::cout << "!";


	/*auto start = std::chrono::high_resolution_clock::now();
	auto finish = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
	std::cout << duration.count() << std::endl << std::endl;
	printer.printToConsole(root);*/
}