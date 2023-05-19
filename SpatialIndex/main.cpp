#include <string>
#include <ctime>

#include <stdio.h>
#include "TreePrinter.h"
#include "SegmentTree2D.h"
#include <random>
#include <Windows.h>
#include <chrono>
#include <set>

//typedef SpatialSegmentTree<2> SpatialSegmentTree;

BoundingBox<2> bound(double x1, double x2, double y1, double y2) {
	BoundingBox<2> bounds;
	bounds.edges[0] = Segment(x1, x2);
	bounds.edges[1] = Segment(y1, y2);
	return bounds;
}


StoredObject<2> storedObject(string name, double x1, double x2, double y1, double y2) {
	BoundingBox<2> bounds = bound(x1, x2, y1, y2);
	return StoredObject<2>(name, bounds);
}

int main()
{
	SpatialSegmentTree<2> tree;
	for (int i = 0; i < 1000; i++) {
		string objectName = std::to_string(i);
		double
			x1 = rand() % 10000,
			x2 = x1 + rand() % 1000 + 1,
			y1 = rand() % 10000,
			y2 = y1 + rand() % 1000 + 1;
		tree.Insert(storedObject(objectName, x1, x2, y1, y2));
	}

	//std::set<StoredObject<2>> windowQ = tree.getOverlappedByRectangle(bound( 0, 787,	999, 1020 ));
	int i = 0;
	while (tree.getCapacity() > 0) {
		string objectName = std::to_string(i);
		tree.Remove(objectName);
		i++;
	}
	tree.getCapacity();
	/*tree.Insert(storedObject("R1", 1, 2, 3, 4));
	tree.Insert(storedObject("R2", -1, 0.499, 3, 4));
	tree.Insert(storedObject("R3", 0.69990, 0.69999, 20, 24));
	tree.Insert(storedObject("R4", 91, 92, 3, 4));
	tree.Insert(storedObject("R5", -12, 0.5, 19, 20));
	std::set<StoredObject<2>> windowQ = tree.getOverlappedByRectangle(bound(0.5, 0.7,	0.7, 19));
	tree.Remove("R1");
	tree.Remove("R2");
	tree.Remove("R3");
	tree.Remove("R4");
	tree.Remove("R5");
	std::cout << "!";*/


	/*auto start = std::chrono::high_resolution_clock::now();
	auto finish = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
	std::cout << duration.count() << std::endl << std::endl;
	printer.printToConsole(root);*/
}