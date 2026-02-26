#include "Ray.h"


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

	auto effectiveColor = color * light.intensity;
	auto ambient = effectiveColor * material.ambient;

	auto sum = Colors::black();

	for (int v{ 0 }; v < light.vsteps; v++)
	{
		for (int u{ 0 }; u < light.usteps; u++)
		{

			//TODO: maybe refactor this into separate functions?
			Colors::Color curDiffuse = Colors::black();
			Colors::Color curSpecular = Colors::black();
			auto lightv = Tuples::normalize(
				light.pointOnLight(u, v) - point
			);
			auto lightDotNormal = Tuples::dot(lightv, normalv);
			float specFactor = 0.0f;
			if (lightDotNormal >= 0.0f)
			{
				curDiffuse = effectiveColor * material.diffuse * lightDotNormal;
				auto reflectv = reflect(-lightv, normalv);
				auto reflectDotEye = Tuples::dot(reflectv, eyev);
				if (reflectDotEye <= 0)
				{
					curSpecular = Colors::Color(0, 0, 0);
				}
				else
				{
					specFactor = pow(reflectDotEye, material.shininess);
					curSpecular = light.intensity * material.specular * specFactor;
				}
			}

			sum += curDiffuse;
			sum += curSpecular;

		}
	}

	return ambient + (sum / light.samples) * intensity;

	/*Colors::Color color{};

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
	return ambient + intensity * (diffuse + specular);*/
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
	return (inShadow ? ambient : ambient + diffuse + specular);
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
	comps.overPoint = comps.point + comps.normalv * 0.001;

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
	, inside{ false }
	, reflectv{}
{
}

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
	static thread_local std::mt19937 rng(std::random_device{}());
	static thread_local std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	return corner + uvec * (u + dist(rng))
		+ vvec * (v + dist(rng));
}

JitterRNG::JitterRNG()
	: size(5)
	, values{}
{
	if (size != 0)
	{
		values.reserve(size);
		values.assign(size, 0.0f);

		std::random_device rd{};
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist{ 0.0f,1.0f };


		for (int i{ 0 }; i < size; i++)
			values[i] = dist(gen);
	}
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
