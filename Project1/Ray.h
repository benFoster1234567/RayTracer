#pragma once
#include "Common.h"
#include "Color.h"
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

struct Material
{
	float ambient{};
	float diffuse{};
	float specular{};
	float shininess{};
	Colors::Color color{};

	Material();
};

inline bool operator==(const Material& a, const Material& b)
{
	return
		a.ambient == b.ambient &&
		a.specular == b.specular &&
		a.diffuse == b.diffuse &&
		a.shininess == b.shininess &&
		a.color == b.color;
}

struct PointLight
{
	Tuples::Tuple position{};
	Colors::Color intensity{};

	PointLight();
	PointLight(const Tuples::Tuple& position, const Colors::Color& intensity);

};

inline bool operator==(const PointLight& a, const PointLight& b)
{
	return a.intensity == b.intensity && a.position == b.position;
}
	
struct Sphere
{

	Tuples::Tuple position{};
	Matrix4 transform{};
	float radius{};
	Material material{};

	Sphere();
	Sphere(Tuples::Tuple position, float radius);
	Tuples::Tuple normalAt(const Tuples::Tuple& worldPoint) const;
	inline void setTransform(Matrix4 transform_)
	{
		transform = transform_;
	}

};

inline bool operator==(const Sphere& a, const Sphere& b)
{
	return a.material == b.material
		&& a.position == b.position
		&& a.radius == b.radius
		&& a.transform == b.transform;
}


struct Intersection
{
	
	float t{};

	const Sphere *object;

	Intersection(float t_, const Sphere *object_);

};

std::vector<Intersection> intersections(std::initializer_list<Intersection> list);
Ray transform(const Ray& ray, const Matrix4& matrix);

const std::vector<Intersection> intersect(const Sphere& sphere, const Ray& ray);
std::optional<Intersection> hit(const std::vector<Intersection>& ints);

Tuples::Tuple reflect(const Tuples::Tuple& in, const Tuples::Tuple& normal);

Colors::Color lighting(const Material& material
	, const PointLight& light
	, const Tuples::Tuple& point
	, const Tuples::Tuple& eyev
	, const Tuples::Tuple& normalv);

class World
{
public:

	World();
	World(std::initializer_list<Sphere> objects);
	World(std::initializer_list<Sphere> objects, const PointLight& light);

	std::optional<PointLight> getLight();
	void setLight(const PointLight& light);

	void addObject(const Sphere& sphere);
	bool contains(const Sphere& sphere) const;

	std::vector<Sphere> getObjects() const
	{
		return objects;
	}

	inline size_t objectCount() const
	{
		return objects.size();
	}

	static World defaultWorld();

private:
	std::vector<Sphere> objects{};
	std::optional<PointLight> light{};

};


std::vector<Intersection> intersectWorld(const World& world, const Ray& ray);

struct Comps
{
	float t{};
	Tuples::Tuple point{};
	Tuples::Tuple eyev{};
	Tuples::Tuple normalv{};
	const Sphere *object;
	bool inside{};

	Comps();

};

Comps prepareComputations(Intersection intersection, Ray ray);