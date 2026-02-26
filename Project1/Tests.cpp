#include "catch_amalgamated.hpp"
#include "Tuple.h"
#include "Color.h"
#include "Matrix.h"
#include "Canvas.h"
#include "Ray.h"
#include "Demo.h"
#include "Camera.h"


#include <cmath>
#include <numbers>

using namespace Tuples;
using namespace Colors;

TEST_CASE("Tuple creation and operations.", "[Tuple]")
{
	Tuple tuple1;
	Tuple tuple2;
	Tuple tuple3;
	Tuple tuple4;

	SECTION("Tuple constructor", "[Constructor]")
	{
		tuple1 = Tuple(1.0f, 2.0f, 3.0f, 4.0f);

		CHECK(tuple1.x == 1.0f);
		CHECK(tuple1.y == 2.0f);
		CHECK(tuple1.z == 3.0f);
		CHECK(tuple1.w == 4.0f);
	}

	SECTION("Creating a point", "[Constructor]")
	{
		tuple2 = Point(3.0f, 3.5f, 4.0f);

		CHECK(tuple2.x == 3.0f);
		CHECK(tuple2.y == 3.5f);
		CHECK(tuple2.z == 4.0f);
		CHECK(tuple2.w == 1.0f);

		CHECK(tuple2.isPoint() == true);
	}

	SECTION("Creating a vector", "[Constructor]")
	{
		tuple3 = Vector(3.0f, 3.5f, 4.0f);

		CHECK(tuple3.x == 3.0f);
		CHECK(tuple3.y == 3.5f);
		CHECK(tuple3.z == 4.0f);
		CHECK(tuple3.w == 0.0f);

		CHECK(tuple3.isVector() == true);
	}

	SECTION("Tuple addition", "[Operations]")
	{
		tuple1 = Tuple(1.0f, 2.0f, 3.0f, 4.0f);
		tuple3 = Vector(3.0f, 3.5f, 4.0f);

		tuple4 = tuple1 + tuple3;

		CHECK(tuple4.x == 4.0f);
		CHECK(tuple4.y == 5.5f);
		CHECK(tuple4.z == 7.0f);
		CHECK(tuple4.w == 4.0f);
	}

	SECTION("Tuple subtraction", "[Operations]")
	{
		tuple1 = Tuple(1.0f, 2.0f, 3.0f, 4.0f);
		tuple3 = Vector(3.0f, 3.5f, 4.0f);

		tuple4 = tuple3 - tuple1;

		CHECK(compare(tuple4.x, 2.0f));
		CHECK(compare(tuple4.y, 1.5f));
		CHECK(compare(tuple4.z, 1.0f));
		CHECK(compare(tuple4.w, -4.0f));
	}

	SECTION("Subtracting a vector from the 0 vector", "[Tuple Operations]")
	{
		tuple2 = Vector(0.0f, 0.0f, 0.0f);
		tuple1 = Vector(1.0f, -2.0f, 3.0f);

		tuple3 = tuple2 - tuple1;

		CHECK(compare(tuple3.x, -1.0f));
		CHECK(compare(tuple3.y, 2.0f));
		CHECK(compare(tuple3.z, -3.0f));
	}

	SECTION("Negating a tuple", "[Tuple Operations]")
	{
		tuple1 = Tuple(1.0f, -2.0f, 3.0f, -4.0f);

		tuple1 = -tuple1;

		CHECK(tuple1.x == -1.0f);
		CHECK(tuple1.y == 2.0f);
		CHECK(tuple1.z == -3.0f);
		CHECK(tuple1.w == 4.0f);
	}

	SECTION("Scalar multiplication", "[Tuple operations]")
	{
		Tuple a = Tuple(1.0f, -2.0f, 3.0f, -4.0f);
		Tuple b = Tuple(3.5f, -7.0f, 10.5f, -14.0f);

		CHECK((a * 3.5f) == b);
		CHECK((3.5f * a) == b);
	}

	SECTION("Scalar Multiplication 2", "[Tuple Operations]")
	{
		Tuple a{ 1.0f, -2.0f, 3.0f, -4.0f };
		Tuple b{ 0.5f, -1.0f, 1.5f, -2.0f };

		CHECK((a * 0.5f) == b);
		CHECK((0.5f * a) == b);
	}

	SECTION("Scalar Division", "[Tuple operations]")
	{
		Tuple a{ 1.0f,-2.0f,3.0f,-4.0f };
		Tuple b{ 0.5f, -1.0f, 1.5f, -2.0f };
		Tuple c = a / 2.0f;

		CHECK(c == b);
	}
}

TEST_CASE("Vector Math", "[Tuple]")
{
	SECTION("Magnitude of a vector test1", "[Vector operations]")
	{
		Tuple v = Vector(1.0f, 0.0f, 0.0f);
		CHECK(magnitude(v) == 1.0f);
	}

	SECTION("Magnitude of a vector test2", "[Vector operations]")
	{
		Tuple v = Vector(0.0f, 1.0f, 0.0f);
		CHECK(magnitude(v) == 1.0f);
	}

	SECTION("Magnitude of a vector test3", "[Vector operations]")
	{
		Tuple v = Vector(0.0f, 0.0f, 1.0f);
		CHECK(magnitude(v) == 1.0f);
	}

	SECTION("Magnitude of a vector test4", "[Vector operations]")
	{
		Tuple v = Vector(1.0f, 2.0f, 3.0f);
		float f = (float)sqrt(14.0);
		CHECK(magnitude(v) == f);
	}

	SECTION("Magnitude of a vector test5", "[Vector operations]")
	{
		Tuple v = Vector(-1.0f, -2.0f, -3.0f);
		float f = (float)sqrt(14.0);
		CHECK(magnitude(v) == f);
	}

	SECTION("Normalized tuple test1", "[Tuple normalization]")
	{
		Tuple v = Vector(4.0f, 0.0f, 0.0f);
		Tuple u = Vector(1.0f, 0.0f, 0.0f);

		CHECK(normalize(v) == u);
	}

	SECTION("Normalized tuple test2", "[Tuple normalization]")
	{
		Tuple v = Vector(1.0f, 2.0f, 3.0f);
		Tuple u = Vector(1.0f / (float)sqrt(14),
			2.0f / (float)sqrt(14),
			3.0f / (float)sqrt(14));

		CHECK(normalize(v) == u);
	}

	SECTION("Vector Multiplication: Dot Product", "[Tuple vector multiplication]")
	{
		Tuple a = Vector(1.0f, 2.0f, 3.0f);
		Tuple b = Vector(2.0f, 3.0f, 4.0f);

		CHECK(dot(a, b) == 20.0f);
	}

	SECTION("Vector Multiplication: Cross Product", "[Tuple vector multiplication]")
	{
		Tuple a = Vector(1.0f, 2.0f, 3.0f);
		Tuple b = Vector(2.0f, 3.0f, 4.0f);

		Tuple expectedab = Vector(-1.0f, 2.0f, -1.0f);
		Tuple expectedba = Vector(1.0f, -2.0f, 1.0f);

		CHECK(cross(a, b) == expectedab);
		CHECK(cross(b, a) == expectedba);
	}
}

TEST_CASE("Testing Colors", "[Color]")
{
	using namespace Colors;

	SECTION("Testing color addition", "[Colors]")
	{
		Color c1{ 0.9f,0.6f,0.75f };
		Color c2{ 0.7f, 0.1f, 0.25f };

		Color result{ 1.6f,0.7f,1.0f };
		CHECK(c1 + c2 == result);
	}

	SECTION("Testing color subtraction", "[Colors]")
	{
		Color c1{ 0.9f,0.6f,0.75f };
		Color c2{ 0.7f, 0.1f, 0.25f };

		Color result{ 0.2f,0.5f, 0.5f };
		CHECK(c1 - c2 == result);
	}

	SECTION("Testing Multiplication of a color by a scalar", "[Colors]")
	{
		Color c{ 0.2f, 0.3f, 0.4f };
		Color result{ 0.4f, 0.6f, 0.8f };
		CHECK(c * 2 == result);
	}

	SECTION("Multiplying two colors together", "[Colors]")
	{
		Color c1{ 1.0f, 0.2f, 0.4f };
		Color c2{ 0.9f, 1.0f, 0.1f };

		Color result{ 0.9f, 0.2f, 0.04f };

		CHECK(c1 * c2 == result);
		CHECK(hadamardProduct(c1, c2) == result);
	}
}

TEST_CASE("Testing Canvas", "[Canvas]")
{
	SECTION("Canvas Constructor", "[Canvas]")
	{
		Canvas c{ 10, 10, Colors::Color {0.0f,0.0f,0.0f} };

		for (int i {0}; i < 10; i++)
			for (int j{ 0 }; j < 10; j++)
			{
				CHECK(c.pixelAt(i, j) == Colors::Color(0.0f, 0.0f, 0.0f));
			}

	}

	SECTION("Canvas write pixel", "[Canvas]")
	{
		Colors::Color red{ 1.0f,0.0f,0.0f };
		Canvas c{ 10,20, Colors::Color(0.0f,0.0f,0.0f) };
		c.writePixel(2, 3, red);
		CHECK(c.pixelAt(2, 3) == red);
	}

	SECTION("Testing ppm string", "[Canvas]")
	{
		Canvas c{ 5,3, Colors::Color {0.0f,0.0f,0.0f} };


		Colors::Color c1{ 1.5f, 0.0f, 0.0f };
		Colors::Color c2{ 0.0f, 0.5f, 0.0f };
		Colors::Color c3{ -0.5f,0.0f, 1.0f };

		c.writePixel(0, 0, c1);
		c.writePixel(2, 1, c2);
		c.writePixel(4, 2, c3);

		std::string ppm = c.toPpm();
		std::stringstream ss(ppm);
		std::string line1, line2, line3, line4, line5, line6;

		std::getline(ss, line1);
		std::getline(ss, line2);
		std::getline(ss, line3);
		std::getline(ss, line4);
		std::getline(ss, line5);
		std::getline(ss, line6);

		CHECK(line1 == "P3");
		CHECK(line2 == "5 3");
		CHECK(line3 == "255");
		CHECK(line4 == "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ");
		CHECK(line5 == "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0 ");
		CHECK(line6 == "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255 ");
	}

	SECTION("Splitting long lines in ppm files", "[Canvas]")
	{
		Canvas c{ 10,2,Colors::Color{1.0f,0.8f,0.6f} };
		std::string ppm= c.toPpm();
		std::string expectedValue =
			"P3\n10 2\n255\n255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 \n255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 \n";
		CHECK(ppm == expectedValue);
	}

	SECTION("Running projectile demo", "[Canvas]")
	{
		Demo::Demo demo(
			Tuples::Vector(-0.01f,0.0f,0.0f),
			Tuples::Vector(0.0f,-0.1f,0.0f),
			Tuples::Point(0.0f,1.0f,0.0f),
			normalize(Tuples::Vector(1.0f,1.8f,0.0f)) * 11.25f,
			900,500,
			Colors::Color(0.0f,0.0f,0.0f)
		);

		demo.run();
	}

	
}

TEST_CASE("Features/matrices", "[Matrix]")
{
	SECTION("Constructing and inspecting a 4x4 matrix", "[Matrix]")
	{
		Matrix4 mat
		{
			1.0f,2.0f,3.0f,4.0f,
			5.5f,6.5f,7.5f,8.5f,
			9.0f,10.0f,11.0f,12.0f,
			13.5f,14.5f,15.5f,16.5f
		};

		CHECK(mat(0,0) == 1.0f);
		CHECK(mat(0,3) == 4.0f);
		CHECK(mat(1,0) == 5.5f);
		CHECK(mat(1,2) == 7.5f);
		CHECK(mat(2,2) == 11.0f);
		CHECK(mat(3,0) == 13.5f);
		CHECK(mat(3,2) == 15.5f);
	}

	SECTION("A 2x2 matrix", "[Matrix]")
	{
		Matrix2 m22
		{
			-3.0f,5.0f,
			1.0f,-2.0f
		};

		CHECK(m22(0,0) == -3.0f);
		CHECK(m22(0,1) == 5.0f);
		CHECK(m22(1,0) == 1.0f);
		CHECK(m22(1,1) == -2.0f);
	}

	SECTION("Matrix equality with identical matrices", "Matrix")
	{
		Matrix4 A
		{
			1.0f,2.0f,3.0f,4.0f,
			5.0f,6.0f,7.0f,8.0f,
			9.0f,8.0f,7.0f,6.0f,
			5.0f,4.0f,3.0f,2.0f
		};

		Matrix4 B
		{
			1.0f,2.0f,3.0f,4.0f,
			5.0f,6.0f,7.0f,8.0f,
			9.0f,8.0f,7.0f,6.0f,
			5.0f,4.0f,3.0f,2.0f
		};

		REQUIRE(A == B);
	}

	SECTION("Matrix equality with identical matrices", "Matrix")
	{
		Matrix4 A
		{
			1.0f,2.0f,3.0f,4.0f,
			5.0f,6.0f,7.0f,8.0f,
			9.0f,8.0f,7.0f,6.0f,
			5.0f,4.0f,3.0f,2.0f
		};

		Matrix4 B
		{
			2.0f,3.0f,4.0f,5.0f,
			6.0f,7.0f,8.0f,9.0f,
			8.0f,7.0f,6.0f,5.0f,
			4.0f,3.0f,2.0f,1.0f
		};

		REQUIRE(A != B);
	}

	SECTION("Matrix multiplication", "[Matrix]")
	{
		Matrix4 A
		{
			1.0f,2.0f,3.0f,4.0f,
			5.0f,6.0f,7.0f,8.0f,
			9.0f,8.0f,7.0f,6.0f,
			5.0f,4.0f,3.0f,2.0f
		};

		Matrix4 B
		{
			-2.0f,1.0f,2.0f,3.0f,
			3.0f,2.0f,1.0f,-1.0f,
			4.0f,3.0f,6.0f,5.0f,
			1.0f,2.0f,7.0f,8.0f
		};

		Matrix4 result
		{
			20.0f,22.0f,50.0f,48.0f,
			44.0f,54.0f,114.0f,108.0f,
			40.0f,58.0f,110.0f,102.0f,
			16.0f,26.0f,46.0f,42.0f
		};

		CHECK(A * B == result);
	}

	SECTION("Multiplication by a tuple", "[Matrix]")
	{
		Matrix4 A{
			1.0f,2.0f,3.0f,4.0f,
			2.0f,4.0f,4.0f,2.0f,
			8.0f,6.0f,4.0f,1.0f,
			0.0f,0.0f,0.0f,1.0f
		};
		Tuples::Tuple b{ 1.0f,2.0f,3.0f,1.0f };

		Tuples::Tuple result{ 18.0f,24.0f,33.0f,1.0f };

		CHECK(A * b == result);
	}

	SECTION("Multiplying a matrix by the identity matrix", "[Matrix]")
	{
		Matrix4 A
		{
			0.0f,1.0f,2.0f,4.0f,
			1.0f,2.0f,4.0f,8.0f,
			2.0f,4.0f,8.0f,16.0f,
			4.0f,8.0f,16.0f,32.0f
		};
		
		CHECK(A * Matrix4::identity() == A);
	}

	SECTION("Multiplying the identity matrix by a tuple", "[Matrix]")
	{
		Matrix4 m = Matrix4::identity();
		Tuples::Tuple t{ 1.0f,2.0f,3.0f,4.0f };
		CHECK(m * t == t);
	}

	SECTION("Transpose", "[Matrix]")
	{
		Matrix4 A
		{
			0.0f,9.0f,3.0f,0.0f,
			9.0f,8.0f,0.0f,8.0f,
			1.0f,8.0f,5.0f,3.0f,
			0.0f,0.0f,5.0f,8.0f
		};

		Matrix4 T
		{
			0.0f, 9.0f, 1.0f, 0.0f,
			9.0f, 8.0f, 8.0f, 0.0f,
			3.0f, 0.0f, 5.0f, 5.0f,
			0.0f, 8.0f, 3.0f, 8.0f
		};

		CHECK(A.transpose() == T);
	}

	SECTION("Transposing the identity matrix", "[Matrix]")
	{
		Matrix4 i = Matrix4::identity();
		CHECK(i == i.transpose());
	}

	SECTION("Calculating the determinant of a 2x2 matrix", "[Matrix]")
	{
		Matrix2 A
		{
			1.0f,5.0f,
			-3.0f,2.0f
		};

		CHECK(A.determinant() == 17.0f);
	}

	SECTION("Submatrix of a 3x3 matrix is a 2x2 matrix", "[Matrix]")
	{
		Matrix3 A
		{
			1.0f,5.0f,0.0f,
			-3.0f,2.0f,7.0f,
			0.0f,6.0f,-3.0f
		};

		Matrix2 S 
		{
			-3.0f,2.0f,
			0.0f,6.0f
		};

		Matrix2 expected = A.submatrix(0,2);

		CHECK(S == expected);
	}

	SECTION("Submatrix of a 4x4 matrix is a 3x3 matrix", "[Matrix]")
	{
		Matrix4 A
		{
			-6.0f,  1.0f,  1.0f,  6.0f,
			-8.0f,  5.0f,  8.0f,  6.0f,
			-1.0f,  0.0f,  8.0f,  2.0f,
			-7.0f,  1.0f, -1.0f,  1.0f
		};

		Matrix3 S
		{
			-6.0f,  1.0f,  6.0f,
			-8.0f,  8.0f,  6.0f,
			-7.0f, -1.0f,  1.0f
		};

		Matrix3 expected = A.submatrix(2, 1);

		CHECK(S == expected);
	}

	SECTION("Calculating a minor of a 3x3 matrix", "[Matrix]")
	{
		Matrix3 A
		{
			3.0f,5.0f,0.0f,
			2.0f,-1.0f,-7.0f,
			6.0f,-1.0f,5.0f
		};

		Matrix2 B = A.submatrix(1, 0);
		float determinant = B.determinant();
		float minor = A.minor(1, 0);
		CHECK(minor == 25.0f);
		CHECK(determinant == 25.0f);
	}

	SECTION("Computing cofactors", "[Matrix]")
	{
		Matrix3 A
		{
			3.0f,5.0f,0.0f,
			2.0f,-1.0f,-7.0f,
			6.0f,-1.0f,5.0f
		};
		float minor1 = A.minor(0, 0);
		float cofactor1 = A.cofactor(0, 0);
		float minor2 = A.minor(1, 0);
		float cofactor2 = A.cofactor(1, 0);
		CHECK(minor1 == -12.0f);
		CHECK(cofactor1 == -12.0f);
		CHECK(minor2 == 25.0f);
		CHECK(cofactor2 == -25.0f);


	}

	SECTION("Calculating the determinant of a 3x3 matrix")
	{
		Matrix3 A
		{
			1.0f,2.0f,6.0f,
			-5.0f,8.0f,-4.0f,
			2.0f,6.0f,4.0f
		};

		CHECK(A.cofactor(0, 0) == 56);
		CHECK(A.cofactor(0, 1) == 12);
		CHECK(A.cofactor(0, 2) == -46);
		CHECK(A.determinant() == -196);
	}

	SECTION("Calculating the determinant of a 4x4 matrix")
	{
		Matrix4 A
		{
			-2.0f, -8.0f,  3.0f,  5.0f,
			-3.0f,  1.0f,  7.0f,  3.0f,
			 1.0f,  2.0f, -9.0f,  6.0f,
			-6.0f,  7.0f,  7.0f, -9.0f
		};

		CHECK(A.cofactor(0, 0) == 690);
		CHECK(A.cofactor(0, 1) == 447);
		CHECK(A.cofactor(0, 2) == 210);
		CHECK(A.cofactor(0, 3) == 51);
		CHECK(A.determinant() == -4071);
	}

	SECTION("Calculating the inverse of a 4x4 matrix")
	{
		Matrix4 A(
			-5.0f, 2.0f, 6.0f, -8.0f,
			1.0f, -5.0f, 1.0f, 8.0f,
			7.0f, 7.0f, -6.0f, -7.0f,
			1.0f, -3.0f, 7.0f, 4.0f
		);

		Matrix4 expected(
			0.21805f, 0.45113f, 0.24060f, -0.04511f,
			-0.80827f, -1.45677f, -0.44361f, 0.52068f,
			-0.07895f, -0.22368f, -0.05263f, 0.19737f,
			-0.52256f, -0.81391f, -0.30075f, 0.30639f
		);

		Matrix4 inv = A.inverse();

		// Compare each element (with some epsilon)
		for (size_t r = 0; r < 4; ++r)
			for (size_t c = 0; c < 4; ++c)
				CHECK(std::abs(inv(r, c) - expected(r, c)) < 0.0001f);
	}

	SECTION("Multiplying a 4x4 matrix by its inverse gives identity")
	{
		Matrix4 A(
			3.0f, -9.0f, 7.0f, 3.0f,
			3.0f, -8.0f, 2.0f, -9.0f,
			-4.0f, 4.0f, 4.0f, 1.0f,
			-6.0f, 5.0f, -1.0f, 1.0f
		);

		Matrix4 inv = A.inverse();
		Matrix4 identity = Matrix4::identity();

		Matrix4 result = A * inv;

		// Check each element
		for (size_t r = 0; r < 4; ++r)
			for (size_t c = 0; c < 4; ++c)
				CHECK(std::abs(result(r, c) - identity(r, c)) < 0.0001f);
	}

	SECTION("Inverse of identity matrix is identity")
	{
		Matrix4 I = Matrix4::identity();
		Matrix4 inv = I.inverse();

		for (size_t r = 0; r < 4; ++r)
			for (size_t c = 0; c < 4; ++c)
				CHECK(inv(r, c) == I(r, c));
	}
}

TEST_CASE("Transformation matrices", "[Matrix]")
{
	SECTION("Multiplying by a translation matrix", "[Matrix]")
	{
		Matrix4 transform = Matrix4::translation(5.0f, -3.0f, 2.0f);
		Tuples::Tuple p = Tuples::Point(-3.0f, 4.0f, 5.0f);
		CHECK(transform * p == Tuples::Point(2.0f, 1.0f, 7.0f));
	}

	SECTION("Mulyiplying by the inverse of a trasnslation matrix", "[Matirx]")
	{
		Matrix4 transform = Matrix4::translation(5, -3, 2);
		Matrix4 inv = transform.inverse();
		Tuples::Tuple p = Tuples::Point(-3, 4, 5);
		CHECK(inv * p == Tuples::Point(-8, 7, 3));
	}

	SECTION("Translation does not affect vectors", "[Matrix]")
	{
		Matrix4 transform = Matrix4::translation(5.0f, -3.0f, 2.0f);
		Tuples::Tuple v = Tuples::Vector(-3, 4, 5);
		CHECK(transform * v == v);
	}

	SECTION("A scale matrix applied to a point", "[Matrix]")
	{
		Matrix4 transform = Matrix4::scale(2, 3, 4);
		Tuples::Tuple p = Tuples::Point(-4, 6, 8);
		CHECK(transform * p == Tuples::Point(-8, 18, 32));
	}

	SECTION("A scaling matrix applied to a vector")
	{
		Matrix4 transform = Matrix4::scale(2, 3, 4);
		Tuples::Tuple v = Tuples::Vector(-4, 6, 8);
		CHECK(transform * v == Tuples::Vector(-8, 18, 32));
	}

	SECTION("Multiplying by the inverse of a scaling matrix")
	{
		Matrix4 transform = Matrix4::scale(2, 3, 4);
		Matrix4 inv = transform.inverse();
		Tuples::Tuple v = Tuples::Vector(-4, 6, 8);
		CHECK(inv * v == Tuples::Vector(-2, 2, 2));
	}

	SECTION("Reflection is scaling by a negative value", "[Matrix]")
	{
		Matrix4 transform = Matrix4::scale(-1, 1, 1);
		Tuples::Tuple p = Tuples::Point(2, 3, 4);
		CHECK(transform * p == Tuples::Point(-2, 3, 4));
	}

	SECTION("Rotating a point around the x axis", "[Matrix]")
	{
		Tuples::Tuple p = Tuples::Point(0.0f, 1.0f, 0.0f);
		Matrix4 halfQuarter = Matrix4::rotationX(static_cast<float>(std::numbers::pi / 4));
		Matrix4 fullQuarter = Matrix4::rotationX(static_cast<float>(std::numbers::pi / 2));

		CHECK(halfQuarter * p == Tuples::Point(0, sqrt(2.0) / 2, sqrt(2.0) / 2));
		CHECK(fullQuarter * p == Tuples::Point(0, 0, 1));
	}

	SECTION("Rotating a point around the y axis", "[Matrix]")
	{
		Tuples::Tuple p = Tuples::Point(0.0f, 0.0f, 1.0f);
		Matrix4 halfQuarter = Matrix4::rotationY(std::numbers::pi / 4);
		Matrix4 fullQuarter = Matrix4::rotationY(std::numbers::pi / 2);

		CHECK(halfQuarter * p == Tuples::Point(sqrt(2.0) / 2, 0, sqrt(2.0) / 2));
		CHECK(fullQuarter * p == Tuples::Point(1, 0, 0));
	}

	SECTION("Rotating a point around the z axis", "[Matrix]")
	{
		Tuples::Tuple p = Tuples::Point(0.0f, 1.0f, 0.0f);
		Matrix4 halfQuarter = Matrix4::rotationZ(std::numbers::pi / 4);
		Matrix4 fullQuarter = Matrix4::rotationZ(std::numbers::pi / 2);

		CHECK(halfQuarter * p == Tuples::Point(-sqrt(2.0) / 2, sqrt(2.0) / 2, 0 ));
		CHECK(fullQuarter * p == Tuples::Point(-1, 0, 0));
	}

	SECTION("A shearing transformation moves x in proportion to y", "[Matrix]")
	{
		Matrix4 transform = Matrix4::shearing(1, 0, 0, 0, 0, 0);
		Tuples::Tuple p = Tuples::Point(2, 3, 4);
		CHECK(transform * p == Tuples::Point(5, 3, 4));
	}

	SECTION("Shearing moves x in proportion to y")
	{
		Tuples::Tuple p{ 2, 3, 4, 1 };
		Matrix4 shear = Matrix4::shearing(1, 0, 0, 0, 0, 0); // xy = 1
		Tuples::Tuple expected{ 5, 3, 4, 1 }; // x = 2 + 3

		CHECK(shear * p == expected);
	}

	SECTION("Shearing moves x in proportion to z")
	{
		Tuples::Tuple p{ 2,3,4,1 };
		Matrix4 shear = Matrix4::shearing(0, 1, 0, 0, 0, 0); // xz = 1
		Tuples::Tuple expected{ 6,3,4,1 };

		CHECK(shear * p == expected);
	}

	SECTION("Shearing moves y in proportion to x")
	{
		Tuples::Tuple p{ 2,3,4,1 };
		Matrix4 shear = Matrix4::shearing(0, 0, 1, 0, 0, 0); // yx = 1
		Tuples::Tuple expected{ 2,5,4,1 };

		CHECK(shear * p == expected);
	}

	SECTION("Shearing moves y in proportion to z")
	{
		Tuples::Tuple p{ 2,3,4,1 };
		Matrix4 shear = Matrix4::shearing(0, 0, 0, 1, 0, 0); // yz = 1
		Tuples::Tuple expected{ 2,7,4,1 };

		CHECK(shear * p == expected);
	}

	SECTION("Shearing moves z in proportion to x")
	{
		Tuples::Tuple p{ 2,3,4,1 };
		Matrix4 shear = Matrix4::shearing(0, 0, 0, 0, 1, 0); // zx = 1
		Tuples::Tuple expected{ 2,3,6,1 };

		CHECK(shear * p == expected);
	}

	SECTION("Shearing moves z in proportion to y")
	{
		Tuples::Tuple p{ 2,3,4,1 };
		Matrix4 shear = Matrix4::shearing(0, 0, 0, 0, 0, 1); // zy = 1
		Tuples::Tuple expected{ 2,3,7,1 };

		CHECK(shear * p == expected);
	}

	SECTION("Individual transformations applied in sequence")
	{
		Tuples::Tuple p{ 1,0,1,1 };

		Matrix4 A = Matrix4::rotationX(std::numbers::pi / 2);
		Matrix4 B = Matrix4::scale(5, 5, 5);
		Matrix4 C = Matrix4::translation(10, 5, 7);

		// Apply transformations in order
		Tuples::Tuple p2 = A * p;
		Tuples::Tuple p3 = B * p2;
		Tuples::Tuple p4 = C * p3;

		Tuples::Tuple expected{ 15,0,7,1 };
		CHECK(p4 == expected);
	}

	SECTION("Chained transformations via matrix multiplication")
	{
		Tuples::Tuple p{ 1,0,1,1 };

		Matrix4 A = Matrix4::rotationX(std::numbers::pi / 2);
		Matrix4 B = Matrix4::scale(5, 5, 5);
		Matrix4 C = Matrix4::translation(10, 5, 7);

		// Compose into a single transformation matrix (right-to-left order)
		Matrix4 T = C * B * A;
		Tuples::Tuple result = T * p;

		Tuples::Tuple expected{ 15,0,7,1 };
		CHECK(result == expected);
	}

}


TEST_CASE("Creating and querying a ray", "[ray]")
{
	auto origin = Tuples::Point(1, 2, 3);
	auto direction = Tuples::Vector(4, 5, 6);

	Ray r(origin, direction);

	REQUIRE(r.origin == origin);
	REQUIRE(r.direction == direction);
}

TEST_CASE("Computing a point from a distance", "[ray]")
{
	auto r = Ray(Tuples::Point(2, 3, 4), Tuples::Vector(1, 0, 0));

	SECTION("Position at t = 0")
	{
		REQUIRE(r.position(0) == Tuples::Point(2, 3, 4));
	}

	SECTION("Position at t = 1")
	{
		REQUIRE(r.position(1) == Tuples::Point(3, 3, 4));
	}

	SECTION("Position at t = -1")
	{
		REQUIRE(r.position(-1) == Tuples::Point(1, 3, 4));
	}

	SECTION("Position at t = 2.5")
	{
		REQUIRE(r.position(2.5f) == Tuples::Point(4.5f, 3, 4));
	}
}

TEST_CASE("Ray with diagonal direction", "[ray]")
{
	auto r = Ray(Tuples::Point(0, 0, 0), Tuples::Vector(1, 1, 1));

	auto pos = r.position(1);
	REQUIRE(pos == Tuples::Point(1, 1, 1));

	auto pos2 = r.position(2);
	REQUIRE(pos2 == Tuples::Point(2, 2, 2));
}

TEST_CASE("Ray default constructor", "[ray]")
{
	Ray r;

	// Should have default-initialized tuples
	REQUIRE(r.origin.x == 0.0f);
	REQUIRE(r.origin.y == 0.0f);
	REQUIRE(r.origin.z == 0.0f);
	REQUIRE(r.direction.x == 0.0f);
	REQUIRE(r.direction.y == 0.0f);
	REQUIRE(r.direction.z == 0.0f);
}

TEST_CASE("Ray with negative direction", "[ray]")
{
	auto r = Ray(Tuples::Point(0, 0, 0), Tuples::Vector(-1, -1, -1));

	auto pos = r.position(1);
	REQUIRE(pos == Tuples::Point(-1, -1, -1));

	auto pos2 = r.position(2);
	REQUIRE(pos2 == Tuples::Point(-2, -2, -2));
}

TEST_CASE("Ray position with fractional t values", "[ray]")
{
	auto r = Ray(Tuples::Point(1, 2, 3), Tuples::Vector(2, 3, 4));

	SECTION("t = 0.5")
	{
		auto pos = r.position(0.5f);
		REQUIRE(pos == Tuples::Point(2, 3.5f, 5));
	}

	SECTION("t = 0.25")
	{
		auto pos = r.position(0.25f);
		REQUIRE(pos == Tuples::Point(1.5f, 2.75f, 4));
	}
}

TEST_CASE("Ray mathematical properties", "[ray]")
{
	SECTION("Position is linear")
	{
		auto r = Ray(Tuples::Point(1, 1, 1), Tuples::Vector(2, 2, 2));

		// position(t1 + t2) should equal position(t1) + direction * t2
		auto pos1 = r.position(1);
		auto pos2 = r.position(2);
		auto pos3 = r.position(3);

		// Verify linearity: pos3 - pos1 == 2 * (pos2 - pos1)
		auto diff1 = pos2 - pos1;
		auto diff2 = pos3 - pos1;

		REQUIRE(diff2 == diff1 * 2);
	}
}

TEST_CASE("Ray with normalized direction", "[ray]")
{
	// Common case: direction is a unit vector
	auto direction = Tuples::Vector(1, 0, 0);  // Already normalized
	auto r = Ray(Tuples::Point(0, 0, 0), direction);

	SECTION("Distance equals t when direction is normalized")
	{
		auto pos = r.position(5);
		REQUIRE(pos == Tuples::Point(5, 0, 0));

		// Distance from origin to pos should be approximately 5
		auto diff = pos - r.origin;
		float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
		REQUIRE(distance ==(5.0f));
	}
}

TEST_CASE("Ray going backwards in time", "[ray]")
{
	auto r = Ray(Tuples::Point(5, 5, 5), Tuples::Vector(1, 1, 1));

	// Negative t means going backwards from origin
	auto pos = r.position(-5);
	REQUIRE(pos == Tuples::Point(0, 0, 0));
}

TEST_CASE("Ray intersects a sphere at a tangent", "[Ray]")
{
	Ray r{ Tuples::Point(0,1,-5), Tuples::Vector(0,0,1) };
	Sphere s{ Tuples::Point(0,0,0), 1.0f };

	auto xs = intersect(s,r);

	REQUIRE(xs.size() == 2);
	CHECK(xs[0].t == 5.0f);
	CHECK(xs[1].t == 5.0f);
}

TEST_CASE("Ray misses a sphere", "[Ray]")
{
	Ray r{ Tuples::Point(0,2,-5), Tuples::Vector(0,0,1) };
	Sphere s{ Tuples::Point(0,0,0), 1.0f };

	auto xs = intersect(s,r);

	REQUIRE(xs.size() == 0);
}

TEST_CASE("A ray originates inside of a sphere", "[Ray]")
{
	Ray r{Tuples::Point(0,0,0),Tuples::Vector(0,0,1)};
	Sphere s(Tuples::Point(0, 0, 0), 1);

	auto xs = intersect(s,r);

	REQUIRE(xs.size() == 2);
	CHECK(xs[0].t == -1.0f);
	CHECK(xs[1].t == 1.0f);
}

TEST_CASE("Intersect sets the object on the intersection", "[Ray]")
{

	Ray r{ Tuples::Point(0,0,-5), Tuples::Vector(0,0,1) };
	Sphere s{ Tuples::Point(0,0,0),1.0f };
	auto xs = intersect(s, r);
	REQUIRE(xs.size() == 2);
	REQUIRE(xs[0].object == &s);
	REQUIRE(xs[1].object == &s);

}

TEST_CASE("Introducing a function called hit. Returning the hit from a collection of intersecrion records.", "[Hit]")
{
	SECTION("The hit when all intersections have a positive t.", "[Hit]")
	{
		Sphere s{};
		Intersection i1{ 1,&s };
		Intersection i2{ 2,&s };
		auto xs = intersections({ i2,i1 });
		auto i = hit(xs);
		//std::cout << "i expected: " << i1.t << "i real: " << i.value().t;
		REQUIRE((i.value().t) == i1.t);
	}

	SECTION("The hit when some intersections have a negative t.", "[Hit]")
	{
		Sphere s{};
		Intersection i1{ -1,&s };
		Intersection i2{ 1,&s };
		auto xs = intersections({ i2,i1 });
		auto i = hit(xs);
		REQUIRE((i.value().t) == i2.t);
	}

	SECTION("The hit when all intersections have a negative t.", "[Hit]")
	{
		Sphere s{};
		Intersection i1{ -2,&s };
		Intersection i2{ -1,&s };
		auto xs = intersections({ i2,i1 });
		auto i = hit(xs);
		//std::cout << "i expected: " << i1.t << "i real: " << i.value().t;

		REQUIRE(i == std::nullopt);
	}

	SECTION("The hit is always the lowest nonnegative intersection.", "[Hit]")
	{
		Sphere s{};
		Intersection i1{ 5,&s };
		Intersection i2{ 7,&s };
		Intersection i3{ -3,&s };
		Intersection i4{ 2,&s };
		auto xs = intersections({ i1,i2,i3,i4 });
		auto i = hit(xs);
		REQUIRE((i.value().t) == i4.t);
	}
}

TEST_CASE("Object transformations")
{
	SECTION("Translating a ray", "[Ray]")
	{
		Ray r{ Tuples::Point(1,2,3), Tuples::Vector(0,1,0) };
		Matrix4 m = Matrix4::translation(3, 4, 5);
		Ray r2 = transformRay(r, m);
		Tuples::Tuple resultP = Tuples::Point(4, 6, 8);
		Tuples::Tuple resultD = Tuples::Vector(0, 1, 0);
		REQUIRE(resultP == r2.origin);
		REQUIRE(resultD == r2.direction);
	}

	SECTION("Scaling a ray", "[Ray]")
	{
		Ray r{ Tuples::Point(1,2,3), Tuples::Vector(0,1,0) };
		Matrix4 m = Matrix4::scale(2, 3, 4);
		Ray r2 = transformRay(r, m);
		Tuples::Tuple resultP = Tuples::Point(2, 6, 12);
		Tuples::Tuple resultD = Tuples::Vector(0, 3, 0);
		REQUIRE(resultP == r2.origin);
		REQUIRE(resultD == r2.direction);
	}

	SECTION("A sphere's default transformation", "[Ray]")
	{
		Sphere s{};
		REQUIRE(s.transform == Matrix4::identity());


	}

	SECTION("Changing a ray's transformation", "[Ray]")
	{
		Sphere s{};
		Matrix4 t{ Matrix4::translation(2,3,4) };
		s.setTransform(t);
		REQUIRE(s.transform == t);


	}

	SECTION("Intersectiong a scaled sphere with a ray.", "[Ray]")
	{
		Ray r{ Tuples::Point(0,0,-5), Tuples::Vector(0,0,1) };
		Sphere s{};
		s.setTransform(Matrix4::scale(2, 2, 2));
		auto xs = intersect(s, r);
		REQUIRE(xs.size() == 2);
		CHECK(xs[0].t == 3);
		CHECK(xs[1].t == 7);
	}

	SECTION("Intersectiong a translated sphere with a ray", "[Ray]")
	{
		Ray r{ Tuples::Point(0,0,-5), Tuples::Vector(0,0,1) };
		Sphere s{};
		s.setTransform(Matrix4::translation(5, 0, 0));
		auto xs = intersect(s, r);
		REQUIRE(xs.size() == 0);

	}
}

TEST_CASE("Computing normals")
{
	SECTION("The normal on a sphere at a point on the x axis", "[Sphere]")
	{
		Sphere s{};
		auto n = s.normalAt(Tuples::Point(1, 0, 0));
		REQUIRE(n == Tuples::Vector(1, 0, 0));
	}

	SECTION("The normal on a sphere at a point on the y axis", "[Sphere]")
	{
		Sphere s{};
		auto n = s.normalAt(Tuples::Point(0, 0, 1));
		REQUIRE(n == Tuples::Vector(0, 0, 1));
	}

	SECTION("The normal on a sphere at a point on the z axis", "[Sphere]")
	{
		Sphere s{};
		auto n = s.normalAt(Tuples::Point(0, 0, 1));
		REQUIRE(n == Tuples::Vector(0, 0, 1));
	}

	SECTION("The normal on a sphere at a nonaxial point", "[Sphere]")
	{
		Sphere s{};
		auto n = s.normalAt(Tuples::Point(std::sqrt(3) / 3, std::sqrt(3) / 3, std::sqrt(3) / 3));
		REQUIRE(n == normalize(n));
	}

	SECTION("Comupting normals on a translated Sphere", "[Sphere]")
	{
		Sphere s{};
		s.setTransform(Matrix4::translation(0, 1, 0));
		auto n = s.normalAt(Tuples::Point(0, 1.70711, -0.70711));
		REQUIRE(n == Tuples::Vector(0, 0.70711, -0.70711));
	}

	SECTION("Computing normals on a transformed sphere", "[Sphere]")
	{
		Sphere s{};
		Matrix4 m{ Matrix4::scale(1,0.5,1) * Matrix4::rotationZ(std::numbers::pi/5) };
		s.setTransform(m);
		auto n = s.normalAt(Tuples::Point(0, std::sqrt(2) / 2, -std::sqrt(2) / 2));
		REQUIRE(n == Tuples::Vector(0, 0.97014, -0.24254));
	}
}

TEST_CASE("Reflecting vectors", "[Sphere]")
{
	SECTION("Reflecting a vector approaching at 45 degrees.", "[Sphere]")
	{
		auto v = Tuples::Vector(1, -1, 0);
		auto n = Tuples::Vector(0, 1, 0);
		auto r = reflect(v, n);
		REQUIRE(r == Tuples::Vector(1, 1, 0));
	}

	SECTION("Reflecting a vector off a slanted surface", "[Sphere]")
	{
		auto v = Tuples::Vector(0, -1, 0);
		auto n = Tuples::Vector(std::sqrt(2) / 2, std::sqrt(2) / 2, 0);
		auto r = reflect(v, n);
		REQUIRE(r == Tuples::Vector(1, 0, 0));
	}
}

TEST_CASE("Phong Reflection Model", "[Ray]")
{
	SECTION("A point light has a position and an intensity", "[Light]")
	{
		Colors::Color intensity(1, 1, 1);
		Tuples::Tuple position = Tuples::Point(0, 0, 0);
		auto light = PointLight(position, intensity);
		REQUIRE(light.position == position);
		REQUIRE(light.intensity == intensity);
	}

	SECTION("The default material", "[Material]")
	{
		Material m{};
		REQUIRE(m.color == Colors::Color(1, 1, 1));
		REQUIRE(m.ambient == 0.1f);
		REQUIRE(m.diffuse == 0.9f);
		REQUIRE(m.specular == 0.9f);
		REQUIRE(m.shininess == 200.0f);
	}

	SECTION("A sphere has a default marerial", "[Material]")
	{
		Sphere s{};
		REQUIRE(s.material == Material{});
	}

	SECTION("A sphere may be assigned a material", "[Meterial]")
	{
		Sphere s{};
		Material m{};
		m.ambient = 1;
		s.material = m;
		REQUIRE(s.material == m);
	}
}

TEST_CASE("Lighting")
{
	SECTION("Lighting wiyh the eye between the light and the surface")
	{
		Material m{};
		auto position = Tuples::Point(0, 0, 0);
		auto eyev = Tuples::Vector(0, 0, -1);
		auto normalv = Tuples::Vector(0, 0, -1);
		auto light = PointLight(Tuples::Point(0,0,-10), Colors::Color(1,1,1));
		auto result = lighting(m, light, position, eyev, normalv, false);
		REQUIRE(result == Colors::Color(1.9, 1.9, 1.9));
	}

	SECTION("Lighting with the eye between light and surface, eye offset 45")
	{
		Material m{};
		auto position = Tuples::Point(0, 0, 0);
		auto eyev = Tuples::Vector(0, sqrt(2) / 2, -sqrt(2) / 2);
		auto normalv = Tuples::Vector(0, 0, -1);
		auto light = PointLight(Tuples::Point(0, 0, -10), Colors::Color(1, 1, 1));
		auto result = lighting(m, light, position, eyev, normalv, false);
		REQUIRE(result == Colors::Color(1.0, 1.0, 1.0));

	}

	SECTION("Lighting with the eye opposite surface, eye offset 45")
	{
		Material m{};
		auto position = Tuples::Point(0, 0, 0);
		auto eyev = Tuples::Vector(0, 0, -1 );
		auto normalv = Tuples::Vector(0, 0, -1);
		auto light = PointLight(Tuples::Point(0, 10, -10), Colors::Color(1, 1, 1));
		auto result = lighting(m, light, position, eyev, normalv, false);
		REQUIRE(result == Colors::Color(0.7364, 0.7364, 0.7364));

	}

	SECTION("Lighting with the eye in the path of the reflection vector")
	{
		Material m{};
		auto position = Tuples::Point(0, 0, 0);
		auto eyev = Tuples::Vector(0, -sqrt(2) / 2, -sqrt(2) / 2);
		auto normalv = Tuples::Vector(0, 0, -1);
		auto light = PointLight(Tuples::Point(0, 10, -10), Colors::Color(1, 1, 1));
		auto result = lighting(m, light, position, eyev, normalv, false);
		REQUIRE(result == Colors::Color(1.6364, 1.6364, 1.6364));

	}

	SECTION("Lighting with the light behind the surface")
	{
		Material m{};
		auto position = Tuples::Point(0, 0, 0);
		auto eyev = Tuples::Vector(0, 0, -1);
		auto normalv = Tuples::Vector(0, 0, -1);
		auto light = PointLight(Tuples::Point(0, 0, 10), Colors::Color(1, 1, 1));
		auto result = lighting(m, light, position, eyev, normalv, false);
		REQUIRE(result == Colors::Color(0.1, 0.1, 0.1));
	}
}

TEST_CASE("Features/World")
{
	SECTION("Creating a world", "[World]")
	{
		World w{};
		REQUIRE(w.objectCount() == 0);
		REQUIRE(w.getLight() == std::nullopt);

	}

	SECTION("The default world", "[World]")
	{
		PointLight light{ Tuples::Point(-10,10,-10), Colors::Color(1,1,1) };
		Sphere s1{};
		Material m{};
		m.color = Colors::Color(0.8, 1.0, 0.6);
		m.diffuse = 0.7;
		m.specular = 0.2;
		s1.material = m;
		Sphere s2{};
		s2.transform = Matrix4::scale(0.5, 0.5, 0.5);

		World w = World::defaultWorld();

		REQUIRE(w.getLight() == light);
		REQUIRE(w.contains(s1));
		REQUIRE(w.contains(s2));
	}

	SECTION("Intersecting a world with a ray", "[World]")
	{
		//TODO: Fix default world, make it a pointer so that it can be accessed anywhere
		auto w = World::defaultWorld();
		Ray r{ Tuples::Point(0,0,-5), Tuples::Vector(0,0,1) };
		auto xs = intersectWorld(w, r);
		REQUIRE(xs.size() == 4);
		CHECK(xs[0].t == 4);
		CHECK(xs[1].t == 4.5);
		CHECK(xs[2].t == 5.5);
		CHECK(xs[3].t == 6);
	}

	SECTION("Precomputing the state of an intersection", "[World]")
	{
		Ray r{ Tuples::Point(0,0,-5), Tuples::Vector(0,0,1) };
		Sphere shape{};
		Intersection i{ 4, &shape };
		auto comps = prepareComputations(i, r);
		REQUIRE(comps.t == i.t);
		CHECK((comps.object == i.object));
		CHECK(comps.point == Tuples::Point(0, 0, -1));
		CHECK(comps.eyev == Tuples::Vector(0, 0, -1));
		CHECK(comps.normalv == Tuples::Vector(0, 0, -1));
	}

	SECTION("The hit, when an intersection occurs on the outside", "[Intersection]")
	{
		Ray r{ Tuples::Point(0,0,-5), Tuples::Vector(0,0,1) };
		Sphere shape{};
		auto i = Intersection(4, &shape);
		auto comps = prepareComputations(i, r);
		CHECK(comps.inside == false);
	}

	SECTION("The hit, when an intersection occurs on the inside", "[Intersection]")
	{
		Ray r{ Tuples::Point(0,0,0), Tuples::Vector(0,0,1) };
		Sphere shape{};
		auto i = Intersection( 1, &shape );
		auto comps = prepareComputations(i, r);
		CHECK(comps.point == Tuples::Point(0, 0, 1));
		CHECK(comps.eyev == Tuples::Vector(0, 0, -1));
		CHECK(comps.inside == true);
		CHECK(comps.normalv == Tuples::Vector(0, 0, -1));
	}

	SECTION("Shading an intersection", "[Intersection]")
	{
		World w = World::defaultWorld();
		Ray r{ Tuples::Point(0,0,-5), Tuples::Vector(0,0,1) };
		Shape *shape = w.objects[0];
		//std::cout << shape.transform.matrixElements.at(0) << std::endl;
		Intersection i(4.0f, shape);
		auto comps = prepareComputations(i, r);
		auto c = shadeHit(w, comps);
		//std::cout << c.r << "," << c.g << "," << c.b << std::endl;
		CHECK(c == Colors::Color(0.38066, 0.47583, 0.2855));
	}

	SECTION("Shading an intersectio from the inside", "[Intersection]")
	{
		World w = World::defaultWorld();
		w.setLight( PointLight( Tuples::Point(0, 0.25, 0), Colors::Color(1,1,1)));
		Ray r{ Tuples::Point(0,0,0), Tuples::Vector(0,0,1) };
		Shape *shape = w.objects[1];
		//std::cout << shape.transform.matrixElements.at(0) << std::endl;
		Intersection i(0.5f, shape);
		auto comps = prepareComputations(i, r);
		auto c = shadeHit(w, comps);
		//std::cout << c.r << "," << c.g << "," << c.b << std::endl;
		CHECK(c == Colors::Color(0.90498, 0.90498, 0.90498));
	}

	SECTION(" A color when a ray misses", "[colorAt]")
	{
		World w = World::defaultWorld();
		Ray r{ Tuples::Point(0,0,-5), Tuples::Vector(0,1,0) };
		Colors::Color c = colorAt(w, r);
		CHECK(c == Colors::Color(0, 0, 0));
	}

	SECTION(" A color when a ray hits", "[colorAt]")
	{
		World w = World::defaultWorld();
		Ray r{ Tuples::Point(0,0,-5), Tuples::Vector(0,0,1) };
		Colors::Color c = colorAt(w, r);
		CHECK(c == Colors::Color(0.38066, 0.47583, 0.2855));
	}

	SECTION("The color with an intersection behind the ray", "[colorAt]")
	{
		World w = World::defaultWorld();
		Shape* outer = w.objects[0];
		outer->material.ambient = 1;
		Shape* inner = w.objects[1];
		inner->material.ambient = 1;
		Ray r{ Tuples::Point(0,0,0.75), Tuples::Vector(0,0,-1) };
		Colors::Color c = colorAt(w, r);
		CHECK(c == inner->material.color);
	}
}

TEST_CASE("Testing view transformation", "[viewTransformation]")
{
	SECTION("The transformation matrix for the default orientation", "[viewTransformation]")
	{
		auto from = Tuples::Point(0, 0, 0);
		auto to = Tuples::Point(0, 0, -1);
		auto up = Tuples::Vector(0, 1, 0);

		auto t = Matrix4::viewTransform(from, to, up);
		CHECK(t == Matrix4::identity());
	}

	SECTION("The transformation matrix looking in positive z direction", "[viewTransformation]")
	{
		auto from = Tuples::Point(0, 0, 0);
		auto to = Tuples::Point(0, 0, 1);
		auto up = Tuples::Vector(0, 1, 0);
		auto t = Matrix4::viewTransform(from, to, up);
		CHECK(t == Matrix4::scale(-1, 1, -1));
	}

	SECTION("The view transformation moves the world", "[viewTransformation]")
	{
		auto from = Tuples::Point(0, 0, 8);
		auto to = Tuples::Point(0, 0, 0);
		auto up = Tuples::Vector(0, 1, 0);
		auto t = Matrix4::viewTransform(from, to, up);
		CHECK(t == Matrix4::translation(0, 0, -8));
	}

	SECTION("An arbitrary view transformation", "[viewTransformation]")
	{
		auto from = Tuples::Point(1, 3, 2);
		auto to = Tuples::Point(4, -2, 8);
		auto up = Tuples::Vector(1, 1, 0);
		auto t = Matrix4::viewTransform(from, to, up);
		auto expected = Matrix4(
			-0.50709f, 0.50709f, 0.67612f, -2.36643f,
			0.76772f, 0.60609f, 0.12122f, -2.82843f,
			-0.35857f, 0.59761f, -0.71714f, 0.00000f,
			0.00000f, 0.00000f, 0.00000f, 1.00000f
		);
		CHECK(t == expected);
	}
}

TEST_CASE("Implementing a camera", "[camera]")
{
	SECTION("Constructing a camera", "[camera]")
	{
		size_t hsize = 160;
		size_t vsize = 120;
		float fieldOfView = radians(90);
		Camera c(hsize, vsize, fieldOfView);

		CHECK(c.hSize == 160);
		CHECK(c.vSize == 120);
		CHECK(c.fieldOfView == radians(90));
		CHECK(c.transform == Matrix4::identity());
	}

	SECTION("The pixel size for a horizontal canvas", "[camera]")
	{
		Camera c(200, 125, radians(90));
		CHECK(std::abs(c.pixelSize - 0.01f) < 0.0001f);
	}

	SECTION("The pixel size for a vertical canvas", "[camera]")
	{
		Camera c(125, 200, radians(90));
		CHECK(std::abs(c.pixelSize - 0.01f) < 0.0001f);
	}

	SECTION("Constructing a ray through the center of the canvas", "[camera]")
	{
		Camera c(201, 101, radians(90));
		Ray r = c.rayForPixel(100, 50);
		CHECK(r.origin == Tuples::Point(0, 0, 0));
		CHECK(r.direction == Tuples::Vector(0, 0, -1));
	}

	SECTION("Constructing a ray through a corner of the canvas", "[camera]")
	{
		Camera c(201, 101, radians(90));
		Ray r = c.rayForPixel(0, 0);
		CHECK(r.origin == Tuples::Point(0, 0, 0));
		CHECK(r.direction == Tuples::Vector(0.66519f, 0.33259f, -0.66851f));
	}

	SECTION("Constructing a ray when the camera is transformed", "[camera]")
	{
		Camera c(201, 101, radians(90));
		c.transform = Matrix4::rotationY(radians(45)) * Matrix4::translation(0, -2, 5);
		Ray r = c.rayForPixel(100, 50);
		CHECK(r.origin == Tuples::Point(0, 2, -5));
		CHECK(r.direction == Tuples::Vector(std::sqrt(2.0f) / 2.0f, 0, -std::sqrt(2.0f) / 2.0f));
	}



	SECTION("Rendering a world with a camera", "[camera]")
	{
		World w = World::defaultWorld();
		Camera c(11, 11, radians(90));
		auto from = Tuples::Point(0, 0, -5);
		auto to = Tuples::Point(0, 0, 0);
		auto up = Tuples::Vector(0, 1, 0);
		c.transform = Matrix4::viewTransform(from, to, up);

		Canvas image = render(c, w);

		CHECK(image.pixelAt(5, 5) == Colors::Color(0.38066f, 0.47583f, 0.2855f));
	}

}

TEST_CASE("Chapter 11 - implementing shadows", "[Shadows]")
{
	SECTION("Lighing the surface in shadow", "[Shadows]")
	{
		auto eyev = Tuples::Vector(0, 0, -1);
		auto normalv = Tuples::Vector(0, 0, -1);
		auto light = PointLight(Tuples::Point(0, 0, -10), Colors::Color(1, 1, 1));
		auto inShadow = true;

		auto result = lighting(Material{}, light, Tuples::Point(0, 0, 0), eyev, normalv, inShadow);

		CHECK(result == Colors::Color(0.1, 0.1, 0.1));

	}

	SECTION("There is no shadow when nothing is collinear with point and light", "[Shadows]")
	{
		auto w = World::defaultWorld();
		auto p = Tuples::Point(0, 10, 0);
		CHECK(isShadowed(w, p) == false);
	}

	SECTION("The shadow when an object is between the point and the light", "[Shadows]")
	{
		auto w = World::defaultWorld();
		auto p = Tuples::Point(10, -10, 10);

		CHECK(isShadowed(w, p) == true);


	}

	SECTION("The light lies between the point and the sphere", "[Shadows]")
	{
		auto w = World::defaultWorld();
		auto p = Tuples::Point(-20, 20, -20);
		CHECK(isShadowed(w, p) == false);
	}

	SECTION("The object is behind the point", "[Shadows]")
	{
		auto w = World::defaultWorld();
		auto p = Tuples::Point(-2, 2, -2);
		CHECK(isShadowed(w, p) == false);
	}

	SECTION("shadeHit is given an intersection in shadow", "[Shadows]") {
		World w{};
		w.setLight(PointLight(Tuples::Point(0, 0, -10), Colors::Color(1, 1, 1)));

		Sphere s1{};
		w.addObject(s1);

		Sphere s2{};
		s2.setTransform(Matrix4::translation(0, 0, 10));
		w.addObject(s2);

		Ray r(Tuples::Point(0, 0, 5), Tuples::Vector(0, 0, 1));
		Intersection i(4, &s2);

		Comps comps = prepareComputations(i, r);
		Colors::Color c = shadeHit(w, comps);

		REQUIRE(c == Colors::Color(0.1, 0.1, 0.1));  // Only ambient light
	}

}

TEST_CASE("Test Shape - Chapter 9", "[Shape]")
{
	// Helper test shape that tracks method calls
	struct TestShape : public Shape
	{
		mutable Ray saved_ray{};

		std::vector<float> localIntersect(const Ray& ray) const override
		{
			saved_ray = ray;
			return std::vector<float>{};
		}

		Tuples::Tuple localNormalAt(const Tuples::Tuple& localPoint) const override
		{
			return Tuples::Vector(localPoint.x, localPoint.y, localPoint.z);
		}

		unsigned int hash() const override
		{
			return 999;  // Arbitrary hash for test shape
		}
	};

	SECTION("The default transformation")
	{
		TestShape s{};
		REQUIRE(s.transform == Matrix4::identity());
	}

	SECTION("Assigning a transformation")
	{
		TestShape s{};
		s.transform = Matrix4::translation(2, 3, 4);
		REQUIRE(s.transform == Matrix4::translation(2, 3, 4));
	}

	SECTION("The default material")
	{
		TestShape s{};
		Material m = s.material;
		REQUIRE(m == Material{});
	}

	SECTION("Assigning a material")
	{
		TestShape s{};
		Material m{};
		m.ambient = 1;
		s.material = m;
		REQUIRE(s.material == m);
	}

	SECTION("Intersecting a scaled shape with a ray")
	{
		Ray r{ Tuples::Point(0, 0, -5), Tuples::Vector(0, 0, 1) };
		TestShape s{};
		s.transform = Matrix4::scale(2, 2, 2);
		auto xs = intersect(s, r);

		REQUIRE(s.saved_ray.origin == Tuples::Point(0, 0, -2.5));
		REQUIRE(s.saved_ray.direction == Tuples::Vector(0, 0, 0.5));
	}

	SECTION("Intersecting a translated shape with a ray")
	{
		Ray r{ Tuples::Point(0, 0, -5), Tuples::Vector(0, 0, 1) };
		TestShape s{};
		s.transform = Matrix4::translation(5, 0, 0);
		auto xs = intersect(s, r);

		REQUIRE(s.saved_ray.origin == Tuples::Point(-5, 0, -5));
		REQUIRE(s.saved_ray.direction == Tuples::Vector(0, 0, 1));
	}

	SECTION("Computing the normal on a translated shape")
	{
		TestShape s{};
		s.transform = Matrix4::translation(0, 1, 0);
		auto n = s.normalAt(Tuples::Point(0, 1.70711, -0.70711));

		REQUIRE(n == Tuples::Vector(0, 0.70711, -0.70711));
	}

	SECTION("Computing the normal on a transformed shape")
	{
		TestShape s{};
		Matrix4 m = Matrix4::scale(1, 0.5, 1) * Matrix4::rotationZ(std::numbers::pi / 5);
		s.transform = m;
		auto n = s.normalAt(Tuples::Point(0, std::sqrt(2.0f) / 2, -std::sqrt(2.0f) / 2));

		REQUIRE(n == Tuples::Vector(0, 0.97014, -0.24254));
	}
}

TEST_CASE("Implementing a Plane", "[Plane]") 
{
	SECTION("The normal of a plane is constant everywhere")
	{
		auto p = Plane{};
		auto n1 = p.normalAt(Tuples::Point(0, 0, 0));
		auto n2 = p.normalAt(Tuples::Point(10, 0, -10));
		auto n3 = p.normalAt(Tuples::Point(-5, 0, 150));

		CHECK(n1 == Tuples::Vector(0, 1, 0));
		CHECK(n2 == Tuples::Vector(0, 1, 0));
		CHECK(n3 == Tuples::Vector(0, 1, 0));
	}

	SECTION(" intersect with a ray parallel to the plane ")
	{
		auto p = Plane();
		auto r = Ray{ Tuples::Point(0,10,0), Tuples::Vector(0,0,1) };
		auto xs = p.localIntersect(r);
		CHECK(xs.size() == 0 );
	}

	SECTION("Intersect with a coplanar ray")
	{
		Plane p{};
		Ray r{ Tuples::Point(0,0,0), Tuples::Vector(0,0,1) };
		auto xs = p.localIntersect(r);
		CHECK(xs.size() == 0);
	}

	SECTION("A ray intersecting a plane from above")
	{
		Plane p{};
		Ray r{ Tuples::Point(0,1,0), Tuples::Vector(0,-1,0) };
		auto xs = p.localIntersect(r);

		REQUIRE(xs.size() == 1);
		CHECK(xs[0] == 1.0f);
	}

	SECTION("A ray intersecting a plane from below")
	{
		Plane p{};
		Ray r{ Tuples::Point(0,-1,0), Tuples::Vector(0,1,0) };
		auto xs = p.localIntersect(r);
		REQUIRE(xs.size() == 1);
		CHECK(xs[0] == 1);
	}

}

TEST_CASE("Implementing patterns", "[Pattern]")
{

	SECTION("Creating a stripe pattern")
	{
		auto pattern = stripePattern(Colors::white(), Colors::black());
		CHECK(pattern.a == Colors::white());
		CHECK(pattern.b == Colors::black());
	}

	SECTION("A stripe pattern is constant in y")
	{
		auto pattern = stripePattern(Colors::white(), Colors::black());

		CHECK(stripeAt(pattern, Tuples::Point(0, 0, 0)) == Colors::white());
		CHECK(stripeAt(pattern, Tuples::Point(0, 1, 0)) == Colors::white());
		CHECK(stripeAt(pattern, Tuples::Point(0, 2, 0)) == Colors::white());

	}

	SECTION("A pattern is constant in z")
	{
		auto pattern = stripePattern(Colors::white(), Colors::black());

		CHECK(stripeAt(pattern, Tuples::Point(0, 0, 0)) == Colors::white());
		CHECK(stripeAt(pattern, Tuples::Point(0, 0, 1)) == Colors::white());
		CHECK(stripeAt(pattern, Tuples::Point(0, 0, 2)) == Colors::white());
	}

	SECTION("A stripe pattern alternates in x")
	{
		auto pattern = stripePattern(Colors::white(), Colors::black());

		CHECK(stripeAt(pattern, Tuples::Point(0, 0, 0)) == Colors::white());
		CHECK(stripeAt(pattern, Tuples::Point(0.9, 0, 0)) == Colors::white());
		CHECK(stripeAt(pattern, Tuples::Point(1, 0, 0)) == Colors::black());
		CHECK(stripeAt(pattern, Tuples::Point(-0.1, 0, 0)) == Colors::black());
		CHECK(stripeAt(pattern, Tuples::Point(-1, 0, 0)) == Colors::black());
		CHECK(stripeAt(pattern, Tuples::Point(-1.1, 0, 0)) == Colors::white());
	}

	SECTION("lighting with a pattern applied")
	{
		auto p = stripePattern(Colors::white(), Colors::black());

		Material m{};

		m.pattern = p;
		m.ambient = 1;
		m.diffuse = 0;
		m.specular = 0;
		auto eyev = Tuples::Vector(0, 0, -1);
		auto normalv = Tuples::Vector(0, 0, -1);

		PointLight light{ Tuples::Point(0,0,-10), Colors::Color(1,1,1) };
		auto c1 = lighting(m, light, Tuples::Point(0.9, 0, 0), eyev, normalv, false);
		auto c2 = lighting(m, light, Tuples::Point(1.1, 0, 0), eyev, normalv, false);

		CHECK(c1 == Colors::Color(1, 1, 1));
		CHECK(c2 == Colors::Color(0, 0, 0));
	}
}

TEST_CASE("Reflections ", "[Reflection]")
{
	SECTION("Reflectivity for the default material")
	{
		Material m{};
		CHECK(m.reflective == 0.0f);
	}

	SECTION("Precomputing the reflection vector")
	{
		Plane shape{};
		Ray r{ Tuples::Point(0,1,-1), Tuples::Vector(0,-sqrt(2) / 2, sqrt(2) / 2 )};
		Intersection i{ (float)sqrt(2), &shape };
		auto comps = prepareComputations(i, r);
		CHECK(comps.reflectv == Tuples::Vector(0, sqrt(2) / 2, sqrt(2) / 2));
	}

	SECTION("Strike a nonreflective surface")
	{
		auto w = World::defaultWorld();
		Ray r{ Tuples::Point(0,0,0), Tuples::Vector(0,0,1) };
		auto shape = w.objects[1];
		shape->material.ambient = 1;
		Intersection i{ 1.0f, shape };
		auto comps = prepareComputations(i, r);
		auto color = reflectedColor(w, comps);
		CHECK(color == Colors::Color(0, 0, 0));
	}

	SECTION("Strike a reflective surface")
	{
		auto w = World::defaultWorld();
		auto plane = Plane();
		plane.material.reflective = 0.5f;
		plane.transform = Matrix4::translation(0, -1, 0);
		w.addObject(plane);
		Ray r(Tuples::Point(0, 0, -3), Tuples::Vector(0, -sqrt(2) / 2, sqrt(2) / 2));
		Intersection i(sqrt(2), &plane);
		auto comps = prepareComputations(i, r);
		auto color = reflectedColor(w, comps);

		std::cout << color.r << ", " << color.g << ", " << color.b;

		CHECK(color == Colors::Color(0.19032, 0.2379, 0.14274));
	}

	SECTION("The reflected color at the maximum recursive depth")
	{
		auto w = World::defaultWorld();
		auto plane = Plane();
		plane.material.reflective = 0.5f;
		plane.transform = Matrix4::translation(0, -1, 0);
		w.addObject(plane);
		Ray r(Tuples::Point(0, 0, -3), Tuples::Vector(0, -sqrt(2) / 2, sqrt(2) / 2));
		Intersection i(sqrt(2), &plane);
		auto comps = prepareComputations(i, r);
		auto color = reflectedColor(w, comps, 0);  // remaining = 0
		CHECK(color == Colors::Color(0, 0, 0));
	}

	SECTION("The reflected color for a reflective material")
	{
		auto w = World::defaultWorld();
		auto plane = Plane();
		plane.material.reflective = 0.5f;
		plane.transform = Matrix4::translation(0, -1, 0);
		w.addObject(plane);
		Ray r(Tuples::Point(0, 0, -3), Tuples::Vector(0, -sqrt(2) / 2, sqrt(2) / 2));
		Intersection i(sqrt(2), &plane);
		auto comps = prepareComputations(i, r);
		auto color = shadeHit(w, comps, 1);  // remaining = 1
		CHECK(color == Colors::Color(0.87677, 0.92436, 0.82918));
	}
}

TEST_CASE("Soft Shadows", "[SoftShadows]")
{
	SECTION("is_shadow tests for occlusion between two points")
	{
		auto w = World::defaultWorld();
		auto lightPosition = Tuples::Point(-10, -10, -10);

		auto test = [](const World& world
			, const Tuples::Tuple& lightPos
			, const Tuples::Tuple& point
			, bool result) {
				CHECK(isShadowed(world, lightPos, point) == result);
			};

		test(w, lightPosition
			, Tuples::Point(-10, -10, -10)
			, false);
		
		test(w, lightPosition
			, Tuples::Point(10, 10, 10)
			, true);


		test(w, lightPosition
			, Tuples::Point(-20, -20, -20)
			, false);


		test(w, lightPosition
			, Tuples::Point(-5, -5, -5)
			, false);
	}

	SECTION("Point lights evaluate the light intensity at a given point")
	{
		auto test = [](const Tuples::Tuple& point, float result)
			{
				auto w = World::defaultWorld();
				auto light = w.getLight().value();
				auto pt = point;
				auto intensity = intensityAt(light, pt, w);
				CHECK(intensity == result);
			};

		test(Tuples::Point(0.0f, 1.0001f, 0.0f), 1.0f);
		test(Tuples::Point(-1.0001f, 0.0f, 0.0f), 1.0f);
		test(Tuples::Point(0.0f, 0.0f, -1.0001f), 1.0f);
		test(Tuples::Point(0.0f, 0.0f, 1.0001f), 0.0f);
		test(Tuples::Point(1.0001f, 0.0f, 0.0f), 0.0f);
		test(Tuples::Point(0.0f, -1.0001f, 0.0f), 0.0f);
		test(Tuples::Point(0.0f, 0.0f, 0.0f), 0.0f);
	}

	SECTION("lighting() uses light intensity to attenuate color")
	{
		auto test = [](float intensity, const Colors::Color& result)
			{
				auto w = World::defaultWorld();
				w.setLight(PointLight(Tuples::Point(0, 0, -10)
					, Colors::Color(1, 1, 1)));
				auto shape = w.objects[0];

				shape->material.ambient = 0.1f;
				shape->material.diffuse = 0.9f;
				shape->material.specular = 0.0f;
				shape->material.color = Colors::Color(1, 1, 1);

				auto pt = Tuples::Point(0, 0, -1);
				auto eyev = Tuples::Vector(0, 0, -1);
				auto normalv = Tuples::Vector(0, 0, -1);

				auto actual = lighting(shape->material
					, w.getLight().value()
					, pt, eyev, normalv, intensity);

				CHECK(result == actual);
			};
	}

	//SECTION("Creating an area light")
	//{
	//	auto corner = Tuples::Point(0, 0, 0);
	//	auto v1 = Tuples::Vector(2, 0, 0);
	//	auto v2 = Tuples::Vector(0, 0, 1);
	//	auto light = AreaLight(corner, v1, 4, v2, 2, Colors::Color(1, 1, 1));

	//	CHECK(light.corner == corner);
	//	CHECK(light.uvec == Tuples::Vector(0.5, 0, 0));
	//	CHECK(light.usteps == 4);
	//	CHECK(light.vvec == Tuples::Vector(0, 0, 0.5));
	//	CHECK(light.vsteps == 2);
	//	CHECK(light.samples == 8);
	//	CHECK(light.position == Tuples::Point(1, 0, 0.5));
	//}

	//SECTION("Finding a single point on an area light")
	//{
	//	auto corner = Tuples::Point(0, 0, 0);
	//	auto v1 = Tuples::Vector(2, 0, 0);
	//	auto v2 = Tuples::Vector(0, 0, 1);
	//	auto light = AreaLight(corner, v1, 4, v2, 2, Colors::Color(1, 1, 1));

	//	// Test case: u=0, v=0
	//	auto pt = light.pointOnLight(0, 0);
	//	CHECK(pt == Tuples::Point(0.25, 0, 0.25));

	//	// Test case: u=1, v=0
	//	pt = light.pointOnLight(1, 0);
	//	CHECK(pt == Tuples::Point(0.75, 0, 0.25));

	//	// Test case: u=0, v=1
	//	pt = light.pointOnLight(0, 1);
	//	CHECK(pt == Tuples::Point(0.25, 0, 0.75));

	//	// Test case: u=2, v=0
	//	pt = light.pointOnLight(2, 0);
	//	CHECK(pt == Tuples::Point(1.25, 0, 0.25));

	//	// Test case: u=3, v=1
	//	pt = light.pointOnLight(3, 1);
	//	CHECK(pt == Tuples::Point(1.75, 0, 0.75));
	//}

	//SECTION("The area light intensity function")
	//{
	//	auto w = World::defaultWorld();
	//	auto corner = Tuples::Point(-0.5, -0.5, -5);
	//	auto v1 = Tuples::Vector(1, 0, 0);
	//	auto v2 = Tuples::Vector(0, 1, 0);
	//	auto light = AreaLight(corner, v1, 2, v2, 2, Colors::Color(1, 1, 1));

	//	// Test case: point(0, 0, 2) - fully shadowed
	//	auto pt = Tuples::Point(0, 0, 2);
	//	auto intensity = intensityAt(light, pt, w);
	//	CHECK(intensity == 0.0f);

	//	// Test case: point(1, -1, 2) - 1 of 4 samples visible
	//	pt = Tuples::Point(1, -1, 2);
	//	intensity = intensityAt(light, pt, w);
	//	CHECK(intensity == 0.25f);

	//	// Test case: point(1.5, 0, 2) - 2 of 4 samples visible
	//	pt = Tuples::Point(1.5, 0, 2);
	//	intensity = intensityAt(light, pt, w);
	//	CHECK(intensity == 0.5f);

	//	// Test case: point(1.25, 1.25, 3) - 3 of 4 samples visible
	//	pt = Tuples::Point(1.25, 1.25, 3);
	//	intensity = intensityAt(light, pt, w);
	//	CHECK(intensity == 0.75f);

	//	// Test case: point(0, 0, -2) - fully lit (all 4 samples visible)
	//	pt = Tuples::Point(0, 0, -2);
	//	intensity = intensityAt(light, pt, w);
	//	CHECK(intensity == 1.0f);
	//}

	SECTION("lighting() samples the area light")
	{
		auto corner = Tuples::Point(-0.5, -0.5, -5);
		auto v1 = Tuples::Vector(1, 0, 0);
		auto v2 = Tuples::Vector(0, 1, 0);
		auto light = AreaLight(corner, v1, 2, v2, 2, Colors::Color(1, 1, 1), JitterRNG(0));

		auto shape = Sphere();
		shape.material.ambient = 0.1f;
		shape.material.diffuse = 0.9f;
		shape.material.specular = 0.0f;
		shape.material.color = Colors::Color(1, 1, 1);

		auto eye = Tuples::Point(0, 0, -5);

		// Test case 1: point(0, 0, -1)
		auto pt = Tuples::Point(0, 0, -1);
		auto eyev = Tuples::normalize(eye - pt);
		auto normalv = Tuples::Vector(pt.x, pt.y, pt.z);
		auto result = lighting(shape.material, light, pt, eyev, normalv, 1.0f);
		CHECK(result == Colors::Color(0.9965, 0.9965, 0.9965));

		// Test case 2: point(0, 0.7071, -0.7071)
		pt = Tuples::Point(0, 0.7071, -0.7071);
		eyev = Tuples::normalize(eye - pt);
		normalv = Tuples::Vector(pt.x, pt.y, pt.z);
		result = lighting(shape.material, light, pt, eyev, normalv, 1.0f);
		CHECK(result == Colors::Color(0.6232, 0.6232, 0.6232));
	}
}

//TEST_CASE("Triangles", "[Triangles]")
//{
//	SECTION("Constructing a triangle")
//	{
//
//		auto p1 = Tuples::Point(0, 1, 0);
//		auto p2 = Tuples::Point(-1, 0, 0);
//		auto p3 = Tuples::Point(1, 0, 0);
//		auto t = Triangle(p1, p2, p3);
//
//		CHECK(t.p1 == p1);
//		CHECK(t.p2 == p2);
//		CHECK(t.p3 == p3);
//		
//		CHECK(t.e1 == Tuples::Vector(-1, -1, 0));
//		CHECK(t.e2 == Tuples::Vector(1, -1, 0));
//		CHECK(t.normal == Tuples::Vector(0, 0, -1));
//	}
//
//	SECTION("Normal vector on a triangle")
//	{
//		auto t = Triangle(Tuples::Point(0, 1, 0), Tuples::Point(-1, 0, 0), Tuples::Point(1, 0, 0));
//
//	}
//}

#include "YamlParser.h"

TEST_CASE("testing yaml parser", "[GayParse]")
{
	SECTION("testing splitLine")
	{
		std::string line = "                Scale: {x: 2, y: 2, z: 2} ";
		auto strings = Parser::splitLine(line);
		CHECK(std::get<0>(strings) == "Scale");
		CHECK(std::get<1>(strings) == " {x: 2, y: 2, z: 2} ");
		CHECK(std::get<2>(strings) == 16);
	}

	SECTION("Testing splitline with float")
	{
		std::string line = "                Ambient: 0.1 ";

		auto strings = Parser::splitLine(line);

		CHECK(std::get<0>(strings) == "Ambient");
		CHECK(std::get<1>(strings) == " 0.1 ");
		CHECK(std::get<2>(strings) == 16);

	}

	SECTION("Testing splitline with only key")
	{
		std::string line = "    Objects: ";

		auto strings = Parser::splitLine(line);
		
		CHECK(std::get<0>(strings) == "Objects");
		CHECK(std::get<1>(strings) == " ");
		CHECK(std::get<2>(strings) == 4);

	}
}