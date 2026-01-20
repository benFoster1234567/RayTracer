#pragma once
#include "Color.h"

struct Material
{
	float ambient{};
	float diffuse{};
	float specular{};
	float shininess{};
	Colors::Color color{};

	Material();
};