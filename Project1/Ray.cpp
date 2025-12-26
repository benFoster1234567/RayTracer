#include "Ray.h"

Ray::Ray()
	: origin{}, direction{}
{}

Ray::Ray(Tuples::Tuple origin, Tuples::Tuple direction)
	: origin{origin}, direction{direction}
{}

Sphere::Sphere()
	: position{Tuples::Point(10,10,0)}, radius(1.0f)
{}

Sphere::Sphere(Tuples::Tuple position, float radius)
{}

std::vector<Intersection> intersections(std::initializer_list<Intersection> list)
{
	auto v = std::vector<Intersection>(list);
	std::sort(v.begin(), v.end(), [](const Intersection& a, const Intersection& b)
		{
			return a.t < b.t;
		});
	return v;
}

const std::vector<Intersection> intersect(const Sphere& sphere, const Ray& ray)
{
	Tuples::Tuple sphereToRay{ ray.origin - Tuples::Point(0,0,0) };
	float a = Tuples::dot(ray.direction, ray.direction);
	float b = 2 * Tuples::dot(ray.direction, sphereToRay);
	float c = Tuples::dot(sphereToRay, sphereToRay) - 1;
	float discriminant = (b * b) - (4 * a * c);
	
	//neg discriminant, return an empty vector
	if (discriminant < 0)
		return std::vector<Intersection>();

	float t1 = (-b - sqrt(discriminant)) / (2 * a);
	float t2 = (-b + sqrt(discriminant)) / (2 * a);

	return std::vector<Intersection>{Intersection{ t1,&sphere }, Intersection{ t2, &sphere }};
}

std::optional<Intersection> hit(const std::vector<Intersection>& ints)
{
	std::optional<Intersection> result{std::nullopt};
	float minT = std::numeric_limits<float>::infinity();
	for (const auto& i : ints)
	{
		if (i.t >= 0 && i.t < minT)
		{
			minT = i.t;
			result = i;
		}
	}
	return result;
}

Intersection::Intersection(float t_, const Sphere *object_)
	: t{t_}, object{object_}
{}
