#include "Ray.h"

Ray::Ray()
	: origin{}, direction{}
{}

Ray::Ray(Tuples::Tuple origin, Tuples::Tuple direction)
	: origin{origin}, direction{direction}
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

Ray transformRay(const Ray& ray, const Matrix4& matrix)
{
	
	Ray newRay{ matrix * ray.origin, matrix * ray.direction };

	return newRay;

}

std::vector<Intersection> intersect(Shape& shape, const Ray& ray)
{
	auto localRay = transformRay(ray, shape.transform.inverse());
	std::vector<float> ts = shape.localIntersect(localRay);
	std::vector<Intersection> is{};

	for (auto& t : ts)
	{
		is.emplace_back(t, &shape);
	}

	return is;
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


Intersection::Intersection(float t_, Shape *object_)
	: t{t_}, object{object_}
{}



PointLight::PointLight()
	: position(Tuples::Point(0,0,0))
	, intensity(Colors::Color(1,1,1))
{}

PointLight::PointLight(const Tuples::Tuple& position, const Colors::Color& intensity)
	: position(position)
	, intensity(intensity)
{}

World::World()
	: objects{}
	, light{std::nullopt}
{
}

World::World(std::initializer_list<Shape*> objects)
	: objects{objects}
{
}

World::World(std::initializer_list<Shape*> objects, const PointLight& light_)
	: objects{objects}
	, light{light_}
	, areaLight{std::nullopt}
{
}

World::World(std::initializer_list<Shape*> objects, const AreaLight& light_)
	: objects{objects}
	, light{std::nullopt}
	, areaLight{light_}
{
}



std::optional<PointLight> World::getLight() const
{
	return this->light;
}

std::optional<AreaLight> World::getAreaLight() const
{
	return areaLight;
}

void World::setLight(const PointLight& light)
{
	this->light = std::optional<PointLight>(light);
	this->areaLight = std::nullopt;
}

void World::setLight(const AreaLight& light)
{
	this->light = std::nullopt;
	this->areaLight.emplace(light);
}

void World::addObject(Shape& shape)
{
	objects.push_back(&shape);
}

bool World::contains(Shape& shape)
{
	for (auto s : this->objects)
	{
		if (s->hash() == shape.hash()) {
			return true;
		}
	}

	return false;
}

World& World::defaultWorld()
{
	static World w{};
	static Sphere s1{}, s2{};

	static std::once_flag flag;

	std::call_once(flag, [&]() {
		Material m{};
		PointLight light{ Tuples::Point(-10, 10, -10), Colors::Color(1, 1, 1) };
		m.color = Colors::Color(0.8, 1.0, 0.6);
		m.diffuse = 0.7;
		m.specular = 0.2;

		s1.material = m;
		s2.setTransform(Matrix4::scale(0.5, 0.5, 0.5));

		w = World{ {&s1, &s2}, light };
		});

	return w;
}

std::vector<Intersection> intersectWorld(const World& world, const Ray& ray)
{
	std::vector<Intersection> intersections_{};
	for (const auto& shape : world.objects)
	{
		auto newVec = intersect(*shape, ray);
		intersections_.insert(intersections_.end(), newVec.begin(), newVec.end());
	}

	std::sort(intersections_.begin(), intersections_.end()
		, [](const Intersection& a, const Intersection& b) 
		{
			return a.t < b.t;
		});
	return intersections_;
}


