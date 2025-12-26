// main.cpp
//#define RUN_TESTS
#ifdef RUN_TESTS
#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#else

#include <iostream>
#include <ranges>
#include <fstream>
#include <string>
#include <algorithm>

#include "catch_amalgamated.hpp"
#include "Tuple.h"
#include "Color.h"
#include "Matrix.h"
#include "Canvas.h"
#include "Demo.h"
#include "Ray.h"

#include <optional>
#include <cmath>
#include <numbers>

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

void sillouetteTest()
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

void timeTest()
{
	int c = 0;
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			c++;
		}
	}
}

int main()
{
	//sillouetteTest();
	timeTest();

	return 0;
}

#endif