#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include <vector>
#include <algorithm>
#include <fstream>
#include "Common.h"
#include "Color.h"
#include "Material.h"
class Canvas
{
public:

	Canvas()
		: width{ 0 }, height{ 0 }
	{}

	Canvas(
		size_t width,
		size_t height,
		const Colors::Color& initialColor
	)
		: width(width),
		height(height),
		pixels(height* width, initialColor)
	{}

	std::string toPpm();

	size_t getWidth() const { return this->width; }
	size_t getHeight() const { return this->height; }


	inline void writePixel(int x, int y, const Colors::Color& color)
	{
		pixels.at(y * width + x) = color;
	}



	inline const Colors::Color& pixelAt(int x, int y) const
	{
		return pixels.at(y * width + x);
	}


	inline void printPixelMap() const
	{
		for (size_t i{ 0 }; i < width * height; i++)
		{
			const auto& c{ pixels[i] };
			if ((i + 1) % width == 0)
			{
				std::cout << "\n" << std::endl;
			}
			std::cout << "[" << c.r << ","
				<< c.g << "," << c.b << "]";
		}
	}

	void toFile(std::string filename);

private:
	std::vector<Colors::Color> pixels;
	size_t width{};
	size_t height{};
};

