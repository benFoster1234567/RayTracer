#pragma once
#include "Common.h"
#include "Color.h"
#include "Matrix.h"
#include "Tuple.h"
#include "Material.h"
#include <algorithm>
#include <optional>

#include <random>

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
	Matrix4 transform{Matrix4::identity()};
	Material material{};

	virtual ~Shape() = default;
	
	virtual std::vector<float> localIntersect(const Ray& ray) const = 0;
	virtual Tuples::Tuple localNormalAt(const Tuples::Tuple& localPoint) const = 0;

	Tuples::Tuple normalAt(const Tuples::Tuple& worldPoint) const
	{
		auto inv = this->transform.inverse();

		Tuples::Tuple localPoint = inv * worldPoint;
		Tuples::Tuple localNormal = this->localNormalAt(localPoint);

		Tuples::Tuple worldNormal = inv.transpose() * localNormal;
		worldNormal.w = 0;

		return Tuples::normalize(worldNormal);
	}

	virtual unsigned int hash() const = 0;

};

	
struct Plane : public Shape
{

	Plane();

	std::vector<float> localIntersect(const Ray& ray) const override;

	Tuples::Tuple localNormalAt(const Tuples::Tuple& localPoint) const override;

	unsigned int hash() const override
	{
		return 1;
	}
};

struct Sphere : public Shape
{

	Tuples::Tuple position{};
	float radius{};

	Sphere();

	Sphere(Tuples::Tuple position, float radius);

	unsigned int hash() const override;

	std::vector<float> localIntersect(const Ray& ray) const override;

	Tuples::Tuple localNormalAt(const Tuples::Tuple& localPoint) const override;

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

struct JitterRNG
{
	std::vector<float> values{};

	unsigned int size{};

	JitterRNG();
	JitterRNG(unsigned int size);

	inline float get(unsigned int index) const
	{
		return values[index % size];
	}

};

struct AreaLight
{
	Tuples::Tuple uvec{};
	Tuples::Tuple corner{};
	float uedge{};
	int usteps{};
	int vsteps{};
	int cellNum{};
	int samples{};
	Tuples::Tuple position{};
	Tuples::Tuple vvec{};
	Colors::Color intensity{};
	JitterRNG jitterBy{};

	AreaLight(const Tuples::Tuple& corner
		, const Tuples::Tuple& fullUvec
		, int usteps
		, const Tuples::Tuple& fullVvec
		, int vSteps
		, const Colors::Color& intensity
		, JitterRNG jitterBy);

	Tuples::Tuple pointOnLight(int u, int v) const;
};

Colors::Color lighting(const Material& material
	, const PointLight& light
	, const Tuples::Tuple& point
	, const Tuples::Tuple& eyev
	, const Tuples::Tuple& normalv
	, bool inShadow);

Colors::Color lighting(const Material& material
	, const AreaLight& light
	, const Tuples::Tuple& point
	, const Tuples::Tuple& eyev
	, const Tuples::Tuple& normalv
	, float intensity);

Colors::Color lighting(const Material& material
	, const PointLight& light
	, const Tuples::Tuple& point
	, const Tuples::Tuple& eyev
	, const Tuples::Tuple& normalv
	, float intensity);


class World
{
public:

	World();
	World(std::initializer_list<Shape*> objects);
	World(std::initializer_list<Shape*> objects, const PointLight& light_);
	World(std::initializer_list<Shape*> objects, const AreaLight& light_);

	std::optional<PointLight> getLight() const;
	std::optional<AreaLight> getAreaLight() const;

	void setLight(const PointLight& light);
	void setLight(const AreaLight& light);

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
	std::optional<AreaLight> areaLight{};
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
	
	Tuples::Tuple reflectv{};

	Comps();

};

Comps prepareComputations(Intersection intersection, Ray ray);
Colors::Color shadeHit(const World& world, const Comps& comps, int remaining = 5);
Colors::Color colorAt(const World& world, const Ray& ray, int remaining = 5);


bool isShadowed(const World& world, const Tuples::Tuple& point);
bool isShadowed(const World& world
	, const Tuples::Tuple& lightPosition
	, const Tuples::Tuple& point);

Colors::Color reflectedColor(const World& world, const Comps& comps, int remaining = 5);

float intensityAt(const PointLight& light
	, const Tuples::Tuple& point
	, const World& world);

float intensityAt(
	const AreaLight& light
	, const Tuples::Tuple& point
	, const World& world
);
