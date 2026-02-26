#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include<vector>

#include "Common.h"


namespace Tuples
{

	struct Tuple
	{

		float x, y, z, w;;

		Tuple()
			: x(0.0), y(0.0), z(0.0), w(0.0)
		{
		}

		Tuple(float x, float y, float z, float w)
			:x(x), y(y), z(z), w(w)
		{
		}

		void negate()
		{
			this->x = -this->x;
			this->y = -this->y;
			this->z = -this->z;
			this->w = -this->w;
		}

		bool isPoint() const { return w == 1.0f; }
		bool isVector() const { return w == 0.0f; }
	};

	Tuple Point(float x, float y, float z);
	Tuple Vector(float x, float y, float z);

	//Vector Operations:
	inline Tuple operator+(const Tuple& t1, const Tuple& t2) noexcept
	{
		return Tuple(
			t1.x + t2.x,
			t1.y + t2.y,
			t1.z + t2.z,
			t1.w + t2.w
		);
	}

	inline Tuple operator-(const Tuple& t1, const Tuple& t2) noexcept
	{
		return Tuple(
			t1.x - t2.x,
			t1.y - t2.y,
			t1.z - t2.z,
			t1.w - t2.w
		);
	}

	inline Tuple operator-(const Tuple& t) noexcept
	{
		return Tuple(-t.x, -t.y, -t.z, -t.w);
	}

	inline Tuple operator*(const Tuple& t, float f) noexcept
	{
		return Tuple(f * t.x, f * t.y, f * t.z, f * t.w);
	}

	inline Tuple operator*(float f, const Tuple& t) noexcept
	{
		return t * f;
	}

	inline Tuple operator/(const Tuple& t, float f) noexcept
	{
		return Tuple{ t.x / f, t.y / f, t.z / f, t.w / f };
	}

	inline bool operator==(const Tuple& t1, const Tuple& t2) noexcept
	{
		return
			std::abs(t1.x - t2.x) < EPSILON &&
			std::abs(t1.y - t2.y) < EPSILON &&
			std::abs(t1.z - t2.z) < EPSILON &&
			std::abs(t1.w - t2.w) < EPSILON;
	}

	inline float magnitude(const Tuple& t) noexcept
	{
		return std::sqrt(
			(t.x * t.x) +
			(t.y * t.y) +
			(t.z * t.z) +
			(t.w * t.w)
		);
	}

	inline Tuple normalize(const Tuple& t) noexcept
	{
		float m = magnitude(t);
		if (m < EPSILON) return t;
		return Tuple{ t.x / m, t.y / m, t.z / m, t.w / m };
	}

	inline float dot(const Tuple& t1, const Tuple& t2) noexcept
	{
		return
			(t1.x * t2.x) +
			(t1.y * t2.y) +
			(t1.z * t2.z) +
			(t1.w * t2.w);
	}

	inline Tuple cross(const Tuple& a, const Tuple& b) noexcept
	{
		return Vector(
			(a.y * b.z) - (a.z * b.y),
			(a.z * b.x) - (a.x * b.z),
			(a.x * b.y) - (a.y * b.x)
		);
	}

	//general helper method:
	bool compare(float f1, float f2);

}