// main.cpp
//#define RUN_TESTS
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
#include "Camera.h"

#include <optional>
#include <cmath>
#include <numbers>

int main()
{
	Plane floor{};
	floor.transform = Matrix4::scale(10, 1, 10);
	floor.material = Material();
	floor.material.color = Colors::Color(1, 0.9, 0.9);
	floor.material.specular = 0;
	floor.material.reflective = 0.3f;
	floor.material.pattern = stripePattern(Colors::white(), Colors::Color(0.7, 0.7, 0.7));

	Plane backWall = Plane{};
	backWall.material = floor.material;
	backWall.transform = Matrix4::translation(0, 0, -5) * Matrix4::rotationX(-std::numbers::pi / 2);

	Sphere middle{};
	middle.setTransform(Matrix4::translation(-0.5, 1, 0.5));
	middle.material = floor.material;
	middle.material.specular = 0.7;

	middle.material = Material();
	middle.material.color = Colors::Color(0.1, 1, 0.5);
	middle.material.diffuse = 0.7;
	middle.material.specular = 0.1;
	middle.material.shininess = 20;

	Sphere right{};
	right.setTransform(
		Matrix4::translation(1.5, 0.5, 0.5) *
		Matrix4::scale(0.5, 0.5, 0.5)
	);
	right.material = Material{};
	right.material.color = Colors::Color(0.5, 1, 0.1);
	right.material.diffuse = 0.7;
	right.material.specular = 0.3;
	right.material.reflective = 0.7f;

	Sphere left{};
	left.setTransform
	(
		Matrix4::translation(-1.5, 0.33, -0.75) *
		Matrix4::scale(0.33, 0.33, 0.33)
	);
	left.material = Material{};
	left.material.color = Colors::Color(1, 0.8, 0.1);
	left.material.diffuse = 0.7;
	left.material.specular = 0.3;

	Tuples::Tuple lightPoint = Tuples::Point(-2, 8, -2);  // Above scene, closer
	Tuples::Tuple uvec = Tuples::Vector(4, 0, 0);   // 4 units wide (don't normalize!)
	Tuples::Tuple vvec = Tuples::Vector(0, 0, 4);
	JitterRNG jitterBy(100);
	AreaLight light(
		lightPoint
		, uvec
		, 10
		, vvec
		, 10
		, Colors::Color(1, 1, 1)
		, jitterBy
	);

	//World world{ {&floor, &middle, &right, &left }
	//, PointLight(Tuples::Point(-10,10,-10), Colors::Color(0.9,0.9,0.9)) };

	World world{ {&floor, &middle, &right, &left }
	, light };


	Camera camera{ 512, 512, std::numbers::pi / 3 };
	camera.transform = Matrix4::viewTransform(Tuples::Point(0, 1.5, -5)
		, Tuples::Point(0, 1, 0)
		, Tuples::Vector(0, 1, 0));
	


	ImageRenderer renderer(camera, world);
	auto canvas = renderer.render();

	std::cout << world.objects.back()->material.color.r << " , " << world.objects.back()->material.color.b << "\n";

	//Canvas canvas = render(camera, world);

	canvas.toFile("outputFile");
	
	return 0;
}



#endif