#include "Material.h"

Material::Material()
	: ambient(0.1)
	, diffuse(0.9)
	, specular(0.9)
	, shininess(200)
	, color(Colors::Color(1, 1, 1))
{
}


Pattern::Pattern()
	: a(Colors::black())
	, b(Colors::white())
{
}

Pattern stripePattern(Colors::Color a, Colors::Color b)
{
	Pattern p{};
	p.a = a;
	p.b = b;
	return p;
}

Colors::Color stripeAt(const Pattern& pattern, const Tuples::Tuple& point)
{
	if (static_cast<int>(floor(point.x)) % 2 == 0)
	{
		return pattern.a;
	}
	return pattern.b;
}

Colors::Color checkerAt(const Pattern& pattern, const Tuples::Tuple& point)
{
	int x = static_cast<int>(floor(point.x));
	int z = static_cast<int>(floor(point.z));
	bool xStripe = x % 2 == 0;
	bool zStripe = z % 2 == 0;

	if (xStripe == zStripe)
		return pattern.a;
	else
		return pattern.b;
}

