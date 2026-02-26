#pragma once
#include "Common.h"
#include "Matrix.h"
#include "Tuple.h"
#include <algorithm>
#include <optional>
struct Ray
{

	Tuples::Tuple origin{};
	Tuples::Tuple direction{};

	Ray();
	Ray(Tuples::Tuple origin, Tuples::Tuple direction);

	inline Tuples::Tuple position(float t) const
	{
		return origin + direction * t;
	}

};


struct Sphere
{

	Tuples::Tuple position{};
	float radius{};

	Sphere();
	Sphere(Tuples::Tuple position, float radius);

};


struct Intersection
{
	
	float t{};

	const Sphere *object;

	Intersection(float t_, const Sphere *object_);

};

std::vector<Intersection> intersections(std::initializer_list<Intersection> list);


const std::vector<Intersection> intersect(const Sphere& sphere, const Ray& ray);
std::optional<Intersection> hit(const std::vector<Intersection>& ints);