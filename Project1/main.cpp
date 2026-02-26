// main.cpp
#define RUN_TESTS
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
#include <cmath>
#include <numbers>



int main()
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
	
	return 0;
}

#endif