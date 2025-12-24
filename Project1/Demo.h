#pragma once
#include <iostream>
#include <ranges>
#include <fstream>
#include <string>
#include <algorithm>
#include "Tuple.h"
#include "Color.h"
#include "Matrix.h"
#include "Canvas.h"

using vec3 = Tuples::Tuple;
using point = Tuples::Tuple;

namespace Demo
{
	struct Environment
	{
		vec3 wind{};
		vec3 gravity{};

		Environment()
			: wind{ Tuples::Tuple{} },
			gravity{ Tuples::Tuple{} }
		{}

		Environment(const vec3& wind, const vec3& gravity)
			: wind{ wind }, gravity{ gravity }
		{}
	};

	struct Projectile
	{
		point position;
		vec3 velocity;

		Projectile()
			: position{ Tuples::Point(0.0f, 0.0f, 0.0f) },
			velocity{ Tuples::Tuple{} }
		{}

		Projectile(const point& position, const vec3& velocity)
			: position{ position }, velocity{ velocity }
		{}

		void printPosition() const
		{
			std::cout
				<< "Position: x: " << position.x
				<< " y: " << position.y
				<< " z: " << position.z
				<< " w: " << position.w  // Fixed: was "z:" again
				<< std::endl;
		}
	};

	class Demo
	{
	public:
		Demo(const Environment& env, Projectile& proj, Canvas canvas)
			: canvas{ canvas },
			env{ env },
			proj{ proj },
			width{ canvas.getWidth() },
			height{ canvas.getHeight() },
			ppm{ "" }
		{}

		Demo(
			Tuples::Tuple wind,
			Tuples::Tuple grav,
			Tuples::Tuple startPos,
			Tuples::Tuple startVelocity,
			size_t width,
			size_t height,
			Colors::Color bgColor
		)
			: canvas{ width, height, bgColor },
			env{ wind, grav },
			proj{ startPos, startVelocity },
			width{ width },
			height{ height },
			ppm{ "" }
		{}

		void run();

	private:
		Canvas canvas{};
		Environment env{};
		Projectile proj{};
		size_t width{};
		size_t height{};
		std::string ppm{};

		void toFile(std::string filename);

		inline Projectile tick(const Environment& env,
			const Projectile& proj) const
		{
			return Projectile{
				proj.position + proj.velocity,
				proj.velocity + env.gravity + env.wind
			};
		}
	};
}