#include "Ray.h"


Plane::Plane()
{
}

std::vector<float> Plane::localIntersect(const Ray& ray) const
{
	if (std::abs(ray.direction.y) < EPSILON)
	{
		return std::vector<float>();
	}

	float t = -ray.origin.y / ray.direction.y;

	return std::vector<float>{t};
}

Tuples::Tuple Plane::localNormalAt(const Tuples::Tuple& localPoint) const
{
	return Tuples::Vector(0, 1, 0);
}

Sphere::Sphere()
	: position{ Tuples::Point(0,0,0) }
	, radius(1.0f)
{
}


Sphere::Sphere(Tuples::Tuple position, float radius)
	: position{ position }
	, radius(radius)
{
}

unsigned int Sphere::hash() const
{
	return 0;
}

std::vector<float> Sphere::localIntersect(const Ray& ray) const
{
	Tuples::Tuple sphereToRay{ ray.origin - Tuples::Point(0,0,0) };
	float a = Tuples::dot(ray.direction, ray.direction);
	float b = 2 * Tuples::dot(ray.direction, sphereToRay);
	float c = Tuples::dot(sphereToRay, sphereToRay) - 1;
	float discriminant = (b * b) - (4 * a * c);

	//neg discriminant, return an empty vector
	if (discriminant < 0)
		return std::vector<float>();

	float t1 = (-b - sqrt(discriminant)) / (2 * a);
	float t2 = (-b + sqrt(discriminant)) / (2 * a);

	return std::vector<float>{t1, t2};
}

Tuples::Tuple Sphere::localNormalAt(const Tuples::Tuple& localPoint) const
{
	return localPoint - Tuples::Point(0, 0, 0);
}