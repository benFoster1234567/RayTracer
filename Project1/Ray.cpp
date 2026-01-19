#include "Ray.h"

Ray::Ray()
	: origin{}, direction{}
{}

Ray::Ray(Tuples::Tuple origin, Tuples::Tuple direction)
	: origin{origin}, direction{direction}
{}

Sphere::Sphere()
	: position{Tuples::Point(0,0,0)}
	, radius(1.0f)
	, transform{ Matrix4::identity()}
	, material{}
{}


Sphere::Sphere(Tuples::Tuple position, float radius)
	: position{position}
	, radius(radius)
	, transform{Matrix4::identity()}
	, material{}
{}

Tuples::Tuple Sphere::normalAt(const Tuples::Tuple& worldPoint) const
{
	Tuples::Tuple objectPoint{ this->transform.inverse() * worldPoint };
	auto objectNormal = objectPoint - Tuples::Point(0, 0, 0);
	auto worldNormal = this->transform.inverse().transpose() * objectNormal;
	worldNormal.w = 0;
	return normalize(worldNormal);
}

std::vector<float> Sphere::localIntersect(const Ray& ray) const
{
	Ray ray2 = transformRay(ray, this->transform.inverse());
	Tuples::Tuple sphereToRay{ ray2.origin - Tuples::Point(0,0,0) };
	float a = Tuples::dot(ray2.direction, ray2.direction);
	float b = 2 * Tuples::dot(ray2.direction, sphereToRay);
	float c = Tuples::dot(sphereToRay, sphereToRay) - 1;
	float discriminant = (b * b) - (4 * a * c);

	//neg discriminant, return an empty vector
	if (discriminant < 0)
		return std::vector<float>();

	float t1 = (-b - sqrt(discriminant)) / (2 * a);
	float t2 = (-b + sqrt(discriminant)) / (2 * a);

	return std::vector<float>{t1, t2};
}

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
	std::vector<float> ts = shape.localIntersect(ray);
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

Tuples::Tuple reflect(const Tuples::Tuple& in, const Tuples::Tuple& normal)
{
	return in - normal * 2 * dot(in, normal);
}

Colors::Color lighting(const Material& material
	, const PointLight& light
	, const Tuples::Tuple& point
	, const Tuples::Tuple& eyev
	, const Tuples::Tuple& normalv
	, bool inShadow)
{
	Colors::Color diffuse{}, specular{};
	float factor{};
	auto effectiveColor = material.color * light.intensity;
	auto lightv = Tuples::normalize(light.position - point);
	auto ambient = effectiveColor * material.ambient;

	auto lightDotNormal = Tuples::dot(lightv, normalv);

	if (lightDotNormal < 0)
	{
		diffuse = Colors::Color(0, 0, 0);
		specular = Colors::Color(0, 0, 0);
	}

	else
	{
		diffuse = effectiveColor * material.diffuse * lightDotNormal;
		auto reflectv = reflect(-lightv, normalv);
		auto reflectDotEye = Tuples::dot(reflectv, eyev);
		if (reflectDotEye <= 0)
		{
			specular = Colors::Color(0, 0, 0);
		}
		else
		{
			factor = pow(reflectDotEye, material.shininess);

			specular = light.intensity * material.specular * factor;
		}
	}
	return (inShadow? ambient : ambient + diffuse + specular);
}

Intersection::Intersection(float t_, Shape *object_)
	: t{t_}, object{object_}
{}

Material::Material()
	: ambient(0.1)
	, diffuse(0.9)
	, specular(0.9)
	, shininess(200)
	, color(Colors::Color(1, 1, 1))
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

World::World(std::initializer_list<Shape*> objects, const PointLight& light)
	: objects{objects}
	, light{light}
{
}

std::optional<PointLight> World::getLight() const
{
	return this->light;
}

void World::setLight(const PointLight& light)
{
	this->light = std::optional<PointLight>(light);
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

Comps prepareComputations(Intersection intersection, Ray ray)
{
	Comps comps{};
	comps.t = intersection.t;
	comps.object = intersection.object;
	comps.point = ray.position(comps.t);
	comps.normalv = comps.object->normalAt(comps.point);
	comps.eyev = -ray.direction;

	if (Tuples::dot(comps.normalv, comps.eyev) < 0)
	{
		comps.inside = true;
		comps.normalv = -comps.normalv;
	}
	else
		comps.inside = false;

	comps.overPoint = comps.point + comps.normalv * 0.01;

	return comps;
}

Colors::Color shadeHit(const World& world, const Comps& comps)
{
	auto shadowed = isShadowed(world, comps.overPoint);

	return lighting(comps.object->material
		, world.getLight().value()
		, comps.overPoint
		, comps.eyev
		, comps.normalv
		, shadowed);
}

Colors::Color colorAt(const World& world, const Ray& ray)
{


	auto ix = intersectWorld(world, ray);
	if (ix.empty()) return Colors::Color(0, 0, 0);
	auto i = hit(ix);
	if (i == std::nullopt) return Colors::Color(0, 0, 0);
	auto comps = prepareComputations(i.value(), ray);
	return shadeHit(world, comps);
}

bool isShadowed(const World& world, const Tuples::Tuple& point)
{
	auto v = world.getLight().value().position - point;
	auto distance = Tuples::magnitude(v);
	auto direction = Tuples::normalize(v);
	Ray r{ point, direction };
	auto intersections = intersectWorld(world, r);
	auto h = hit(intersections);

	if (h.has_value() && h.value().t < distance)
	{
		return true;
	}

	return false;
}


Comps::Comps()
	: t{}
	, point{}
	, eyev{}
	, normalv{}
	, object{}
	, overPoint{}
	, inside{false}
{ }
