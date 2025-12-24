#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include<vector>

#include "Common.h"

namespace Colors
{
	struct Color
	{
		float r, g, b;

		Color()
			: r(0.0f), b(0.0f), g(0.0f)
		{
		}

		Color(float r, float g, float b)
			: r{ r }, g{ g }, b{ b }
		{
		}
	};

	inline Color hadamardProduct(const Color& c1, const Color& c2) noexcept
	{
		return Color{
			c1.r * c2.r,
			c1.g * c2.g,
			c1.b * c2.b
		};
	}

	inline Color operator+(const Color& c1, const Color& c2) noexcept
	{
		return Color{
			c1.r + c2.r,
			c1.g + c2.g,
			c1.b + c2.b
		};
	}

	inline Color operator-(const Color& c1, const Color& c2) noexcept
	{
		return Color{
			c1.r - c2.r,
			c1.g - c2.g,
			c1.b - c2.b
		};
	}

	inline Color operator*(const Color& c1, const Color& c2) noexcept
	{
		return hadamardProduct(c1, c2);
	}

	inline Color operator*(const Color& c, float f) noexcept
	{
		return Color{
			c.r * f,
			c.g * f,
			c.b * f
		};
	}

	inline Color operator*(float f, const Color& c) noexcept
	{
		return Color{
			c.r * f,
			c.g * f,
			c.b * f
		};
	}

	inline Color operator/(const Color& c, float f) noexcept
	{
		return Color{
			c.r / f,
			c.g / f,
			c.b / f
		};
	}

	inline Color& operator+=(Color& c1, const Color& c2) noexcept
	{
		c1.r += c2.r;
		c1.g += c2.g;
		c1.b += c2.b;
		return c1;
	}

	inline Color& operator-=(Color& c1, const Color& c2) noexcept
	{
		c1.r -= c2.r;
		c1.g -= c2.g;
		c1.b -= c2.b;
		return c1;
	}

	inline Color& operator*=(Color& c1, const Color& c2) noexcept
	{
		c1.r *= c2.r;
		c1.g *= c2.g;
		c1.b *= c2.b;
		return c1;
	}

	inline bool operator==(const Color& c1, const Color& c2) noexcept
	{
		return
			std::abs(c1.r - c2.r) < EPSILON &&
			std::abs(c1.g - c2.g) < EPSILON &&
			std::abs(c1.b - c2.b) < EPSILON;
	}

	inline float dot(const Color& t1, const Color& t2) noexcept
	{
		return
			(t1.r * t2.r) +
			(t1.g * t2.g) +
			(t1.b * t2.b);
	}


}