#include "Ray.h"

Triangle::Triangle()
	: p1(Tuples::Point(0,0,0))
	, p2(Tuples::Point(0,0,0))
	, p3(Tuples::Point(0,0,0))
{
}

Triangle::Triangle(Tuples::Tuple p1, Tuples::Tuple p2, Tuples::Tuple p3)
	: p1(p1)
	, p2(p2)
	, p3(p3)
{
}