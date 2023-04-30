#include <set>
#include "Segment.h"

struct SegmentAssociatedList {
private: 
	std::set<long long> items;
public: 
	SegmentAssociatedList() {
		items = std::set<long long>();
	}
	~SegmentAssociatedList() {
		items.clear();
	}
};