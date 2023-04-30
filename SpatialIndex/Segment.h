#pragma once
#include <cassert>
#include <iostream>
#include <math.h>

//typedef double Numeric;

//template <typename Numeric>
struct Segment {

	// ����� ����� �������
	double a;
	// ������ ����� �������
	double b;

	Segment()
	{
		*this = emptySegment();
	}
	Segment(double a, double b)
	{

		if (a > b)
			std::swap(a, b);
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

	// �������� �������� �� �����������������
	bool overlaps(const Segment& s)
	{
		return !((a <= b < s.a <= s.b) || (s.a <= s.b < a <= b));
	}

	// ����������� ���� ��������
	Segment overlap(const Segment& s)
	{
		if (!overlaps(s))
			return emptySegment();
		if (a <= s.a <= b <= s.b)
			return Segment(s.a, b);
		if (a <= s.a <= s.b <= b)
			return Segment(s.a, s.b);
		if (s.a <= a <= s.b <= b)
			return Segment(a, s.b);
		if (s.a <= a <= b <= s.b)
			return Segment(a, b);

	}

	//�����������
	Segment unionForOverlapping(const Segment& s)
	{
		//������� ������ ������������ ���� �� � ����� �����
		if (!overlaps(s))
			return emptySegment();
		return Segment(std::min(a, s.a), std::max(b, s.b));
	}

	// ������� ����� �� ����������� 
	Segment leftFromOverlap(const Segment& s)
	{
		if (!overlaps(s))
			return emptySegment();
		int union_a = unionForOverlapping(s).a;
		int overlap_a = overlap(s).a;
		return (union_a == overlap_a) ? emptySegment() : Segment(union_a, overlap_a);
	}

	// ������� ������ �� �����������
	Segment rightFromOverlap(const Segment& s)
	{
		if (!overlaps(s))
			return emptySegment();
		int union_b = unionForOverlapping(s).b;
		int overlap_b = overlap(s).a;
		return (union_b == overlap_b) ? emptySegment() : Segment(union_b, overlap_b);
	}

	// �������� ����, ��� ������ ������� ����� ������ ����� ������� s
	bool operator < (const Segment& s)
	{
		return b < s.a;
	}

	// �������� ����, ��� ������ ������� ����� ����� ������� s
	bool operator <= (const Segment& s)
	{
		return b <= s.a;
	}

	// �������� ����, ��� ������ ������� ����� ������ ������ ������� s
	bool operator > (const Segment& s)
	{
		return a > s.b;

	}

	// �������� ����, ��� ������ ������� ����� ������ ������� s
	bool operator >= (const Segment& s)
	{
		return a >= s.b;
	}

	Segment& operator = (const Segment& s) {
		this->a = s.a;
		this->b = s.b;
		return *this;
	}
};