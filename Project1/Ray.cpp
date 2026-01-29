#include "Ray.h"

Ray::Ray()
	: origin{}, direction{}
{}

Ray::Ray(Tuples::Tuple origin, Tuples::Tuple direction)
	: origin{origin}, direction{direction}
{}

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
	: position{Tuples::Point(0,0,0)}
	, radius(1.0f)
{}


Sphere::Sphere(Tuples::Tuple position, float radius)
	: position{position}
	, radius(radius)
{}

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

Tuples::Tuple reflect(const Tuples::Tuple& in, const Tuples::Tuple& normal)
{
	return in - normal * 2 * dot(in, normal);
}

Colors::Color lighting(const Material& material
	, const AreaLight& light
	, const Tuples::Tuple& point
	, const Tuples::Tuple& eyev
	, const Tuples::Tuple& normalv
	, float intensity)
{
	Colors::Color color{};

	if (material.pattern != std::nullopt)
		color = checkerAt(material.pattern.value(), point);

	else
		color = material.color;

	Colors::Color diffuse{}, specular{};

	auto effectiveColor = color * light.intensity;
	auto ambient = effectiveColor * material.ambient;
	for (int v{ 0 }; v < light.vsteps; v++)
		for (int u{ 0 }; u < light.usteps; u++)
		{
			float factor{};
			auto lightv = Tuples::normalize(light.pointOnLight(u,v) - point);

			auto lightDotNormal = Tuples::dot(lightv, normalv);

			if (lightDotNormal < 0)
			{
				diffuse += Colors::Color(0, 0, 0);
				specular += Colors::Color(0, 0, 0);
			}

			else
			{
				diffuse = effectiveColor * material.diffuse * lightDotNormal;
				auto reflectv = reflect(-lightv, normalv);
				auto reflectDotEye = Tuples::dot(reflectv, eyev);
				if (reflectDotEye <= 0)
				{
					specular += Colors::Color(0, 0, 0);
				}
				else
				{
					factor = pow(reflectDotEye, material.shininess);
					specular += light.intensity * material.specular * factor;
				}
			}
		}
	return ambient + intensity * (diffuse + specular)/light.samples;

	//Colors::Color color{};

	//if (material.pattern != std::nullopt)
	//	color = checkerAt(material.pattern.value(), point);

	//else
	//	color = material.color;

	//Colors::Color diffuse{}, specular{};
	//float factor{};
	//auto effectiveColor = color * light.intensity;
	//auto lightv = Tuples::normalize(light.position - point);
	//auto ambient = effectiveColor * material.ambient;

	//auto lightDotNormal = Tuples::dot(lightv, normalv);

	//if (lightDotNormal < 0)
	//{
	//	diffuse = Colors::Color(0, 0, 0);
	//	specular = Colors::Color(0, 0, 0);
	//}

	//else
	//{
	//	diffuse = effectiveColor * material.diffuse * lightDotNormal;
	//	auto reflectv = reflect(-lightv, normalv);
	//	auto reflectDotEye = Tuples::dot(reflectv, eyev);
	//	if (reflectDotEye <= 0)
	//	{
	//		specular = Colors::Color(0, 0, 0);
	//	}
	//	else
	//	{
	//		factor = pow(reflectDotEye, material.shininess);
	//		specular = light.intensity * material.specular * factor;
	//	}
	//}
	//return ambient + intensity * (diffuse + specular);
}

Colors::Color lighting(const Material& material
	, const PointLight& light
	, const Tuples::Tuple& point
	, const Tuples::Tuple& eyev
	, const Tuples::Tuple& normalv
	, float intensity)
{
	Colors::Color color{};

	if (material.pattern != std::nullopt)
		color = checkerAt(material.pattern.value(), point);

	else
		color = material.color;

	Colors::Color diffuse{}, specular{};
	float factor{};
	auto effectiveColor = color * light.intensity;
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
	return ambient + intensity * (diffuse + specular);
}


Colors::Color lighting(const Material& material
	, const PointLight& light
	, const Tuples::Tuple& point
	, const Tuples::Tuple& eyev
	, const Tuples::Tuple& normalv
	, bool inShadow)
{
	Colors::Color color{};

	if (material.pattern != std::nullopt)
		color = checkerAt(material.pattern.value(), point);

	else
		color = material.color;

	Colors::Color diffuse{}, specular{};
	float factor{};
	auto effectiveColor = color * light.intensity;
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

	comps.reflectv = reflect(ray.direction, comps.normalv);
	comps.overPoint = comps.point + comps.normalv * 0.0001;

	return comps;
}

Colors::Color shadeHit(const World& world, const Comps& comps, int remaining)
{
	Colors::Color surface;

	if (world.getAreaLight().has_value())
	{
		auto areaLight = world.getAreaLight().value();
		float intensity = intensityAt(areaLight, comps.overPoint, world);
		surface = lighting(comps.object->material, areaLight,
			comps.overPoint, comps.eyev, comps.normalv, intensity);
	}
	else if (world.getLight().has_value())
	{
		auto pointLight = world.getLight().value();
		float intensity = intensityAt(pointLight, comps.overPoint, world);
		surface = lighting(comps.object->material, pointLight,
			comps.overPoint, comps.eyev, comps.normalv, intensity);
	}
	else
	{
		surface = Colors::Color(0, 0, 0);
	}

	Colors::Color reflected = reflectedColor(world, comps, remaining);
	return surface + reflected;
}

Colors::Color colorAt(const World& world, const Ray& ray, int remaining)
{
	auto ix = intersectWorld(world, ray);
	if (ix.empty()) return Colors::Color(0, 0, 0);
	auto i = hit(ix);
	if (i == std::nullopt) return Colors::Color(0, 0, 0);
	auto comps = prepareComputations(i.value(), ray);
	return shadeHit(world, comps, remaining);
}

bool isShadowed(const World& world
	, const Tuples::Tuple& lightPosition
	, const Tuples::Tuple& point)
{
	//auto v = world.getLight().value().position - point;
	auto v = lightPosition - point;
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

bool isShadowed(const World& world, const Tuples::Tuple& point)
{
	auto v = world.getLight().value().position - point; //light position
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

Colors::Color reflectedColor(const World& world
	, const Comps& comps, int remaining)
{
	if (comps.object->material.reflective == 0 || remaining <= 0)
		return Colors::Color(0, 0, 0);

	Ray reflectedRay(comps.overPoint, comps.reflectv);
	Colors::Color color = colorAt(world, reflectedRay, remaining - 1);

	return color * comps.object->material.reflective;
}

float intensityAt(const PointLight& light
	, const Tuples::Tuple& point
	, const World& world)
{
	if (isShadowed(world, light.position, point))
		return 0.0f;

	return 1.0f;
}

float intensityAt(
	const AreaLight& light
	, const Tuples::Tuple& point
	, const World& world
)
{
	float total = 0.0f;
	for (int v{ 0 }; v < light.vsteps; v++)
	{
		for (int u{ 0 }; u < light.usteps; u++)
		{
			auto lightPos = light.pointOnLight(u, v);
			if (!isShadowed(world, lightPos, point))
				total += 1;
		}
	}

	return static_cast<float>(total) / static_cast<float>(light.samples);
}



Comps::Comps()
	: t{}
	, point{}
	, eyev{}
	, normalv{}
	, object{}
	, overPoint{}
	, inside{false}
	, reflectv{}
{ }

AreaLight::AreaLight(const Tuples::Tuple& corner
	, const Tuples::Tuple& fullUvec
	, int usteps
	, const Tuples::Tuple& fullVvec
	, int vSteps
	, const Colors::Color& intensity
	, JitterRNG jitterBy
	)
	
	:corner(corner)
	, uvec(fullUvec / usteps)
	, usteps(usteps)
	, vvec(fullVvec / vSteps)
	, vsteps(vSteps)
	, intensity(intensity)
	, samples(usteps* vSteps)
	, position(corner + (fullVvec + fullUvec) / 2)
	, jitterBy(jitterBy)
{
	samples = usteps * vsteps;
}

Tuples::Tuple AreaLight::pointOnLight(int u, int v) const
{
	int index = (v * usteps + u);
	return corner + uvec * (u + jitterBy.get(index))
		+ vvec * (v + jitterBy.get(index * 2 + 1));
}

JitterRNG::JitterRNG()
	: size(5)
	, values{}
{
	values.reserve(size);
	values.assign(size, 0.0f);

	std::random_device rd{};
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist{ 0.0f,1.0f };


	for (int i{ 0 }; i < size; i++)
		values[i] = dist(gen);
}

JitterRNG::JitterRNG(unsigned int size)
	: size(size)
{
	values.reserve(size);
	values.assign(size, 0.0f);
	
	std::random_device rd{};
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist{ 0.0f,1.0f };

	for (int i{ 0 }; i < size; i++)
		values[i] = dist(gen);

	std::cout << "random values generated: ";
	for (const auto& i : values)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
}
