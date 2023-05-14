#pragma once
#include <cassert>
#include <iostream>
#include <math.h>

//typedef double Numeric;

//template <typename Numeric>
struct Segment {

	// левый конец отрезка
	double a;
	// правый конец отрезка
	double b;

	Segment()
	{
		*this = emptySegment();
	}
	Segment(double a, double b)
	{
		this->a = a;
		this->b = b;
	}

	Segment emptySegment()
	{
		return Segment(1, -1);
	}

	bool isEmpty()
	{
		return a == 1 && b == -1;
	}

	bool isPoint()
	{
		return a == b;
	}

	bool contains(const double p) {
		return a <= p && p <= b;
	}

	// проверка отрезков на взаимопересечение
	bool overlaps(const Segment& s)
	{
		return !this->overlap(s).isEmpty();
	}

	// пересечение двух отрезков
	Segment overlap(const Segment& s)
	{
		if (a <= s.a && s.a < b && b <= s.b)
			return Segment(s.a, b);
		if (a <= s.a && s.a <= s.b && s.b <= b)
			return Segment(s.a, s.b);
		if (s.a <= a && a < s.b && s.b <= b)
			return Segment(a, s.b);
		if (s.a <= a && a <= b && b <= s.b)
			return Segment(a, b);
		else
			return emptySegment();
	}

	//объединение
	Segment unionForOverlapping(const Segment& s)
	{
		//отрезки должны пересекаться хотя бы в одной точке
		if (!overlaps(s))
			return emptySegment();
		return Segment(std::min(a, s.a), std::max(b, s.b));
	}

	// сегмент слева от пересечения 
	Segment leftFromOverlap(const Segment& s)
	{
		if (!overlaps(s))
			return emptySegment();
		int union_a = unionForOverlapping(s).a;
		int overlap_a = overlap(s).a;
		return (union_a == overlap_a) ? emptySegment() : Segment(union_a, overlap_a);
	}

	// сегмент справа от пересечения
	Segment rightFromOverlap(const Segment& s)
	{
		if (!overlaps(s))
			return emptySegment();
		int union_b = unionForOverlapping(s).b;
		int overlap_b = overlap(s).a;
		return (union_b == overlap_b) ? emptySegment() : Segment(union_b, overlap_b);
	}


	bool isToTheLeftFrom(const Segment& s) {
		return a <= b && b <= s.a && s.a <= s.b;
	}

	friend bool operator < (const Segment& s1, const Segment& s2)
	{
		if (s1.a < s2.a) {
			return true;
		}
		else {
			if (s2.a < s1.a)
				return false;
			else {
				if (s1.b < s2.b)
					return true;
				else return false;
			}
		}
	}

	friend bool operator <= (const Segment& s1, const Segment& s2)
	{
		return !(s2 < s1);
	}

	friend bool operator > (const Segment& s1, const Segment& s2)
	{
		return s2 < s1;;
	}

	friend bool operator >= (const Segment& s1, const Segment& s2)
	{
		return !(s1 < s2);
	}

	friend bool operator==(const Segment& s1, const Segment& s2) {
		return s1.a == s2.a && s1.b == s2.b;
	}

	friend bool operator!=(const Segment& s1, const Segment& s2) {
		return s1.a != s2.a || s1.b != s2.b;
	}

	Segment& operator = (const Segment& s) {
		this->a = s.a;
		this->b = s.b;
		return *this;
	}
};