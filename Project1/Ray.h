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

struct Shape
{
	Matrix4 transform{};
	Material material{};

	virtual std::vector<float> localIntersect(const Ray& ray) const = 0;
	virtual Tuples::Tuple normalAt(const Tuples::Tuple& worldPoint) const = 0;
	virtual unsigned int hash() const = 0;


};

	
struct Sphere : public Shape
{

	Tuples::Tuple position{};
	Matrix4 transform{};
	float radius{};
	Material material{};

	Sphere();
	Sphere(Tuples::Tuple position, float radius);

	unsigned int hash() const override
	{
		return 1;
	}

	Tuples::Tuple normalAt(const Tuples::Tuple& worldPoint) const override;
	std::vector<float> localIntersect(const Ray& ray) const override;
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

	Shape *object;

	Intersection(float t_, Shape *object_);

};

std::vector<Intersection> intersections(std::initializer_list<Intersection> list);

Ray transformRay(const Ray& ray, const Matrix4& matrix);

std::vector<Intersection> intersect(Shape& Shape, const Ray& ray);

std::optional<Intersection> hit(const std::vector<Intersection>& ints);

Tuples::Tuple reflect(const Tuples::Tuple& in, const Tuples::Tuple& normal);

Colors::Color lighting(const Material& material
	, const PointLight& light
	, const Tuples::Tuple& point
	, const Tuples::Tuple& eyev
	, const Tuples::Tuple& normalv
	, bool inShadow);

class World
{
public:

	World();
	World(std::initializer_list<Shape*> objects);
	World(std::initializer_list<Shape*> objects, const PointLight& light);

	std::optional<PointLight> getLight() const;
	void setLight(const PointLight& light);

	void addObject(Shape& shape);
	bool contains(Shape& shape);

	size_t objectCount() const
	{
		return objects.size();
	}

	static World& defaultWorld();

	std::vector<Shape*> objects{};
private:
	std::optional<PointLight> light{};

};


std::vector<Intersection> intersectWorld(const World& world, const Ray& ray);

struct Comps
{
	float t{};
	Tuples::Tuple point{};
	Tuples::Tuple eyev{};
	Tuples::Tuple normalv{};
	Shape *object;
	bool inside{};
	Tuples::Tuple overPoint{};

	Comps();

};

Comps prepareComputations(Intersection intersection, Ray ray);
Colors::Color shadeHit(const World& world, const Comps& comps);
Colors::Color colorAt(const World& world, const Ray& ray);

bool isShadowed(const World& world, const Tuples::Tuple& point);