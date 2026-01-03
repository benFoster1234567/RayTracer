// main.cpp
#define RUN_TESTS
#ifdef RUN_TESTS
#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#else

#include <windows.h>
#include <iostream>
#include <ranges>
#include <fstream>
#include <string>
#include <algorithm>

//#include "catch_amalgamated.hpp"
#include "Tuple.h"
#include "Color.h"
#include "Matrix.h"
#include "Canvas.h"
#include "Demo.h"
#include "Ray.h"

#include <optional>
#include <cmath>
#include <numbers>

void dotTest();
void sphereSillouette();
void sphereTest();

int main()
{
	sphereTest();

	return 0;
}

void dotTest() 
{
	Canvas canvas{ 500,500,Colors::Color{0.0f,0.0f,0.0f} };
	Colors::Color white{ 1.0f,1.0f,1.0f };

	Tuples::Tuple drawPosition = Tuples::Point(1.0f, 0.0f, 0.0f);

	for (int i = 0; i < 12; i++)
	{
		Matrix4 transform = Matrix4::rotationZ(std::numbers::pi / 6);
		drawPosition = transform * drawPosition;
		auto x = static_cast<int>(std::round(100 * drawPosition.x));
		auto y = static_cast<int>(std::round(100 * drawPosition.y));
		auto z = static_cast<int>(std::round(100 * drawPosition.z));

		canvas.writePixel(x + 250, y + 250, white);
	}

	canvas.toFile("outfile");

}

void sphereSillouette()
{
	
	size_t widthPixels = 300, heightPixels = 300;
	Canvas canvas(widthPixels, heightPixels, Colors::Color(0, 0, 0));
	Sphere sphere{};

	Tuples::Tuple rayOrigin = Tuples::Point(0, 0, -5);

	float viewWidth = 7.0f;
	float viewHeight = 7.0f;
	float wallZ = 10.0f;

	float pixelWidth{ viewWidth / widthPixels };
	float pixelHeight{ viewHeight / heightPixels };

	auto isHit = [](const Sphere& s, const Ray& r) -> bool
		{
			return !(hit(intersect(s, r)) == std::nullopt);
		};

	for (size_t r{}; r < heightPixels; ++r)
		for (size_t c{}; c < widthPixels; ++c)
		{
			//find correct pixels?
			float x = (c + 0.5f) * pixelWidth - (viewWidth / 2); // Adjust to center the view
			float y = (r + 0.5f) * pixelHeight - (viewHeight / 2); // Adjust to center the view
			//create a ray
			Ray ray{ rayOrigin,Tuples::normalize(Tuples::Point(x,y,wallZ) - rayOrigin) };
			//fire ray
			if (isHit(sphere, ray))
			{
				auto h = hit(intersect(sphere, ray));
				canvas.writePixel(int(c), int(r), Colors::Color(1, 0, 1));
			}
		}

	canvas.toFile("outfile");

}

void sphereTest()
{

	size_t widthPixels = 300, heightPixels = 300;
	Canvas canvas(widthPixels, heightPixels, Colors::Color(0, 0, 0));
	
	Sphere sphere{};
	sphere.material.color = Colors::Color(1, 0.2, 1);
	
	auto lightPosition = Tuples::Point(-10, 10, -10);
	Colors::Color lightColor(1, 1, 1);

	PointLight light(lightPosition, lightColor);

	Tuples::Tuple rayOrigin = Tuples::Point(0, 0, -5);

	float viewWidth = 7.0f;
	float viewHeight = 7.0f;
	float wallZ = 10.0f;

	float pixelWidth{ viewWidth / widthPixels };
	float pixelHeight{ viewHeight / heightPixels };

	auto isHit = [](const Sphere& s, const Ray& r) -> bool
		{
			return !(hit(intersect(s, r)) == std::nullopt);
		};

	auto scanlinesPerCheckpoint = static_cast<int>(heightPixels / 100);
	if (heightPixels < 100) scanlinesPerCheckpoint = 1;

	for (size_t r{}; r < heightPixels; ++r)
	{
		std::clog << "\033c" << std::flush;
		std::clog << "scan lines remaining: " << heightPixels - r << "/" << heightPixels << ' ' << std::flush;
		//if (r % scanlinesPerCheckpoint == 0)
		//	std::clog << '*' << std::flush;
		
		for (size_t c{}; c < widthPixels; ++c)
		{
			//find correct pixels?
			float x = (c + 0.5f) * pixelWidth - (viewWidth / 2); // Adjust to center the view
			float y = (viewHeight / 2) - (r + 0.5f) * pixelHeight;
			//create a ray
			Ray ray{ rayOrigin,Tuples::normalize(Tuples::Point(x,y,wallZ) - rayOrigin) };
			//fire ray

			auto h = hit(intersect(sphere, ray));
			
			if (h != std::nullopt) 
			{
				Intersection hitIntersection = h.value();
				Tuples::Tuple point = ray.position(hitIntersection.t);
				auto normal = hitIntersection.object->normalAt(point);
				Tuples::Tuple eye = -ray.direction;
				Colors::Color  color = lighting(hitIntersection.object->material, light, point, eye, normal);
				canvas.writePixel(int(c), int(r), color);
			}
		}
	}
	canvas.toFile("outfile");

}

#endif