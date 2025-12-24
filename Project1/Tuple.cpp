#include "Tuple.h"

namespace Tuples
{

	Tuple Point(float x, float y, float z)
	{
		return Tuple(x, y, z, 1.0f);
	}

	Tuple Vector(float x, float y, float z)
	{
		return Tuple(x, y, z, 0.0f);
	}

	bool compare(float f1, float f2)
	{
		return std::abs(f1 - f2) < EPSILON;
	}

}