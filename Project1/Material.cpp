#include "Material.h"

Material::Material()
	: ambient(0.1)
	, diffuse(0.9)
	, specular(0.9)
	, shininess(200)
	, color(Colors::Color(1, 1, 1))
{
}