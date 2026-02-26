#pragma once
#include "Tuple.h"
#include "Color.h"
#include <optional>

struct Pattern
{
	Colors::Color a{};
	Colors::Color b{};

	Pattern();
};

struct Material
{
	float ambient{};
	float diffuse{};
	float specular{};
	float shininess{};
	std::optional<Pattern> pattern{std::nullopt};

	float reflective{ 0.0f };

	Colors::Color color{};

	Material();
};

Pattern stripePattern(Colors::Color a, Colors::Color b);
Colors::Color stripeAt(const Pattern& pattern, const Tuples::Tuple& point);
Colors::Color checkerAt(const Pattern& pattern, const Tuples::Tuple& point);

inline bool operator==(const Material& a, const Material& b)
{
	return
		a.ambient == b.ambient &&
		a.specular == b.specular &&
		a.diffuse == b.diffuse &&
		a.shininess == b.shininess &&
		a.color == b.color;
}

