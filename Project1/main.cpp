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
void poolBallsScene();
void scene1();
void softShadowsExampleScene();
void desertDuskScene();
void neonShowroomScene();

int main()
{
	//poolBallsScene();
	desertDuskScene();
	neonShowroomScene();
	return 0;
}

void scene1()
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
		, 5
		, vvec
		, 5
		, Colors::Color(1, 1, 1)
		, jitterBy
	);

	//World world{ {&floor, &middle, &right, &left }
	//, PointLight(Tuples::Point(-10,10,-10), Colors::Color(0.9,0.9,0.9)) };

	World world{ {&floor, &middle, &right, &left }
	, light };


	Camera camera{ 256, 256, std::numbers::pi / 3 };
	camera.transform = Matrix4::viewTransform(Tuples::Point(0, 1.5, -5)
		, Tuples::Point(0, 1, 0)
		, Tuples::Vector(0, 1, 0));



	ImageRenderer renderer(camera, world);
	auto canvas = renderer.render();

	std::cout << world.objects.back()->material.color.r << " , " << world.objects.back()->material.color.b << "\n";

	//Canvas canvas = render(camera, world);

	canvas.toFile("outputFile");
}

void softShadowsExampleScene()
{
	Camera camera(400, 160, 0.7854);
	camera.transform = Matrix4::viewTransform(Tuples::Point(-3, 1, 2.5), Tuples::Point(0, 0.5, 0), Tuples::Vector(0, 1, 0));
	
	auto corner = Tuples::Point(-1, 2, 4);
	auto uvec = Tuples::Vector(2, 0, 0);
	auto vvec = Tuples::Vector(0, 2, 0);
	int usteps = 10;
	int vsteps = 10;
	bool jitter = true;
	JitterRNG jitterBy(5);
	Colors::Color intensity(1.5, 1.5, 1.5);

	AreaLight light(corner
		, uvec, usteps
		, vvec, vsteps
		, intensity, jitterBy);

	Material planeMaterial{};
	planeMaterial.color = Colors::Color(1, 1, 1);
	planeMaterial.ambient = 0.1f;
	planeMaterial.specular = 0;
	planeMaterial.reflective = 0.3f;

	Plane plane{};
	plane.material = planeMaterial;
	
	Material sphereMaterial{};
	sphereMaterial.color = Colors::Color(1, 0, 0);
	sphereMaterial.ambient = 0.1f;
	sphereMaterial.specular = 0.0f;
	sphereMaterial.diffuse = 0.6f;
	sphereMaterial.reflective = 0.3f;

	Sphere sphere{};
	sphere.transform =
		Matrix4::translation(0.5f, 0.5, 0.5f)
		* Matrix4::scale(0.5f, 0.5f, 0.5f);
	sphere.material = sphereMaterial;

	Material sphere2Material{};
	sphere2Material.color = Colors::Color(0.5f, 0.5f, 1.0f);
	sphere2Material.ambient = 0.1f;
	sphere2Material.specular = 0.0f;
	sphere2Material.diffuse = 0.6f;
	sphere2Material.reflective = 0.3f;

	Sphere sphere2{};
	sphere2.transform =
		Matrix4::translation(-0.25f,0.33f,0.0f)
		* Matrix4::scale(0.33f,0.33f,0.33f);

	sphere2.material = sphere2Material;
	
	World world({ &plane, &sphere, &sphere2 }, light);

	ImageRenderer renderer(camera, world);

	auto canvas = renderer.render();
	canvas.toFile("softShadowsExample");
}

void poolBallsScene()
{
	Camera camera(720, 720, 0.8);
	camera.transform = Matrix4::viewTransform(
		Tuples::Point(0, 3.5f, -6),
		Tuples::Point(0, 0.5f, 0),
		Tuples::Vector(0, 1, 0));

	// Overhead area light — soft, square
	auto corner = Tuples::Point(-2, 5, -2);
	auto uvec = Tuples::Vector(4, 0, 0);
	auto vvec = Tuples::Vector(0, 0, 4);
	JitterRNG jitterBy(100);
	Colors::Color intensity(1.4f, 1.4f, 1.4f);
	AreaLight light(corner, uvec, 10, vvec, 10, intensity, jitterBy);

	// Green felt floor
	Material feltMat{};
	feltMat.color = Colors::Color(0.1f, 0.45f, 0.15f);
	feltMat.ambient = 0.15f;
	feltMat.diffuse = 0.9f;
	feltMat.specular = 0.05f;
	feltMat.reflective = 0.0f;
	Plane plane{};
	plane.material = feltMat;

	// Helper lambda for a shiny ball
	auto makeBall = [](Colors::Color c, float x, float z) -> Sphere {
		Material m{};
		m.color = c;
		m.ambient = 0.1f;
		m.diffuse = 0.6f;
		m.specular = 0.9f;
		m.shininess = 200;
		m.reflective = 0.3f;
		Sphere s{};
		s.transform = Matrix4::translation(x, 0.5f, z) * Matrix4::scale(0.5f, 0.5f, 0.5f);
		s.material = m;
		return s;
		};

	// Classic triangle rack arrangement
	Sphere cue = makeBall(Colors::Color(0.95f, 0.95f, 0.9f), 0.0f, -2.0f); // cue ball
	Sphere b1 = makeBall(Colors::Color(0.95f, 0.7f, 0.0f), 0.0f, 0.0f); // 1 yellow
	Sphere b2 = makeBall(Colors::Color(0.1f, 0.1f, 0.85f), -0.55f, 0.95f); // 2 blue
	Sphere b3 = makeBall(Colors::Color(0.8f, 0.1f, 0.1f), 0.55f, 0.95f); // 3 red
	Sphere b4 = makeBall(Colors::Color(0.55f, 0.1f, 0.55f), -1.1f, 1.9f); // 4 purple
	Sphere b5 = makeBall(Colors::Color(0.95f, 0.45f, 0.0f), 0.0f, 1.9f); // 5 orange
	Sphere b6 = makeBall(Colors::Color(0.1f, 0.6f, 0.15f), 1.1f, 1.9f); // 6 green

	World world({ &plane, &cue, &b1, &b2, &b3, &b4, &b5, &b6 }, light);
	ImageRenderer renderer(camera, world);
	auto canvas = renderer.render();
	canvas.toFile("poolBalls");
	std::cout << "Rendered to poolBalls.ppm\n";
}

void desertDuskScene()
{
	Camera camera(720, 720, 1.0f);
	camera.transform = Matrix4::viewTransform(
		Tuples::Point(-4, 2.5f, -8),
		Tuples::Point(0, 0.8f, 0),
		Tuples::Vector(0, 1, 0));

	auto corner = Tuples::Point(4, 3, -5);
	auto uvec = Tuples::Vector(0, 0, 4);
	auto vvec = Tuples::Vector(0, 3, 0);
	JitterRNG jitterBy(200);
	Colors::Color intensity(2.0f, 1.1f, 0.4f); 
	AreaLight light(corner, uvec, 14, vvec, 14, intensity, jitterBy);

	Material sandMat{};
	sandMat.color = Colors::Color(0.76f, 0.6f, 0.4f);
	sandMat.ambient = 0.25f;
	sandMat.diffuse = 0.9f;
	sandMat.specular = 0.02f;
	sandMat.reflective = 0.0f;
	Plane floor{};
	floor.material = sandMat;

	auto makeDune = [](Colors::Color c, float x, float z, float r,
		float amb, float refl) -> Sphere {
			Material m{};
			m.color = c;
			m.ambient = amb;
			m.diffuse = 0.8f;
			m.specular = 0.05f;
			m.shininess = 20;
			m.reflective = refl;
			Sphere s{};
			s.transform = Matrix4::translation(x, r, z) * Matrix4::scale(r, r, r);
			s.material = m;
			return s;
		};


	Sphere boulder = makeDune(Colors::Color(0.72f, 0.3f, 0.15f), 0.0f, 0.0f, 1.1f, 0.2f, 0.05f);

	Sphere rock1 = makeDune(Colors::Color(0.8f, 0.5f, 0.2f), -2.5f, 0.8f, 0.55f, 0.2f, 0.02f);
	Sphere rock2 = makeDune(Colors::Color(0.65f, 0.35f, 0.2f), 2.3f, 1.0f, 0.45f, 0.2f, 0.02f);

	Sphere obsidian = makeDune(Colors::Color(0.05f, 0.03f, 0.04f), 1.0f, 2.5f, 0.3f, 0.05f, 0.6f);

	Sphere pebble1 = makeDune(Colors::Color(0.7f, 0.55f, 0.5f), -0.8f, 3.5f, 0.18f, 0.25f, 0.0f);
	Sphere pebble2 = makeDune(Colors::Color(0.75f, 0.58f, 0.48f), 0.4f, 4.2f, 0.14f, 0.25f, 0.0f);
	Sphere pebble3 = makeDune(Colors::Color(0.68f, 0.52f, 0.44f), -1.8f, 5.0f, 0.12f, 0.25f, 0.0f);

	World world({ &floor, &boulder, &rock1, &rock2, &obsidian, &pebble1, &pebble2, &pebble3 }, light);
	ImageRenderer renderer(camera, world);
	renderer.render().toFile("desertDusk");
	std::cout << "Rendered desertDusk\n";
}

void neonShowroomScene()
{
	Camera camera(720, 720, 0.75f);
	camera.transform = Matrix4::viewTransform(
		Tuples::Point(0, 5, -8),
		Tuples::Point(0, 1, 0),
		Tuples::Vector(0, 1, 0));

	// Small intense overhead spotlight
	auto corner = Tuples::Point(-1, 8, -1);
	auto uvec = Tuples::Vector(2, 0, 0);
	auto vvec = Tuples::Vector(0, 0, 2);
	JitterRNG jitterBy(200);
	Colors::Color intensity(2.5f, 2.5f, 2.5f);
	AreaLight light(corner, uvec, 16, vvec, 16, intensity, jitterBy);

	// Glossy white showroom floor
	Material floorMat{};
	floorMat.color = Colors::Color(0.9f, 0.9f, 0.92f);
	floorMat.ambient = 0.05f;
	floorMat.diffuse = 0.3f;
	floorMat.specular = 1.0f;
	floorMat.shininess = 500;
	floorMat.reflective = 0.8f;
	Plane floor{};
	floor.material = floorMat;

	auto makeShowpiece = [](Colors::Color c, float x, float z, float r,
		float shin, float refl) -> Sphere {
			Material m{};
			m.color = c;
			m.ambient = 0.08f;
			m.diffuse = 0.4f;
			m.specular = 1.2f;
			m.shininess = shin;
			m.reflective = refl;
			Sphere s{};
			s.transform = Matrix4::translation(x, r, z) * Matrix4::scale(r, r, r);
			s.material = m;
			return s;
		};

	
	Sphere chrome = makeShowpiece(Colors::Color(0.8f, 0.8f, 0.85f), 0.0f, 0.0f, 1.2f, 600, 0.9f);
	
	Sphere neonRed = makeShowpiece(Colors::Color(1.0f, 0.05f, 0.1f), -2.8f, 0.5f, 0.65f, 400, 0.5f);
	
	Sphere neonCyan = makeShowpiece(Colors::Color(0.0f, 0.85f, 0.9f), 2.8f, 0.8f, 0.55f, 400, 0.5f);
	
	Sphere yellow = makeShowpiece(Colors::Color(0.95f, 0.9f, 0.0f), -1.4f, 2.2f, 0.3f, 300, 0.4f);
	
	Sphere pink = makeShowpiece(Colors::Color(0.95f, 0.1f, 0.6f), 1.5f, 2.5f, 0.25f, 300, 0.4f);
	
	Sphere violet = makeShowpiece(Colors::Color(0.4f, 0.0f, 0.9f), 0.5f, 3.2f, 0.18f, 250, 0.35f);

	World world({ &floor, &chrome, &neonRed, &neonCyan, &yellow, &pink, &violet }, light);
	ImageRenderer renderer(camera, world);
	renderer.render().toFile("neonShowroom");
	std::cout << "Rendered neonShowroom\n";
}

#endif