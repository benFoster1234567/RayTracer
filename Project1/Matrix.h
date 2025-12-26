#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include<vector>

#include "Tuple.h"

//default m by n matrix base class
class MatrixBase
{
protected:
	std::vector<float> matrixElements;
	size_t m{};//rows
	size_t n{};//cols

	inline MatrixBase(size_t rows, size_t cols)
		: matrixElements(rows* cols, 0.0f), m(rows), n(cols)
	{}


public:
	virtual ~MatrixBase() = default;
	virtual float determinant() const = 0;

	inline float& operator()(size_t row, size_t col)
	{
		return matrixElements[row * n + col];
	}

	inline const float& operator()(size_t row, size_t col) const
	{
		return matrixElements[row * n + col];
	}

	size_t rows() const { return m; }
	size_t cols() const { return n; }

	bool equals(const MatrixBase& other, float epsilon = 0.00001f) const;
	void multiply(const MatrixBase& other, MatrixBase& result, size_t matrixSideLen);

	inline bool isInvertable() const { return (*this).determinant() != 0.0f; };

};

class Matrix2 : public MatrixBase
{
public:
	inline Matrix2() : MatrixBase(2, 2) {}

	inline Matrix2(float m00, float m01,
		float m10, float m11)
		: MatrixBase(2, 2)
	{
		matrixElements = { m00,m01,m10,m11 };
	}

	inline bool operator==(const Matrix2& other) const
	{
		return equals(other);
	}

	inline bool operator!=(const Matrix2& other) const
	{
		return !(*this == other);
	}

	inline float determinant() const
	{
		return (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
	}

};

class Matrix3 : public MatrixBase
{
public:
	inline Matrix3() : MatrixBase(3, 3) {}
	inline Matrix3(float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22)
		: MatrixBase(3, 3)
	{
		matrixElements = { m00,m01,m02,
						m10,m11,m12,
						m20,m21,m22 };
	}

	inline bool operator==(const Matrix3& other)  const
	{
		return equals(other);
	}

	inline bool operator!=(const Matrix3& other) const
	{
		return !(*this == other);
	}

	Matrix2 submatrix(size_t row, size_t col) const;
	inline float minor(size_t row, size_t col) const
	{
		return (*this).submatrix(row, col).determinant();
	}

	inline float cofactor(size_t row, size_t col) const
	{
		return ((row + col) % 2 == 0 ? 1.0f : -1.0f) * (*this).minor(row, col);
	}

	inline float determinant() const
	{
		return (*this)(0, 0) * (*this).cofactor(0, 0) +
			(*this)(0, 1) * (*this).cofactor(0, 1) +
			(*this)(0, 2) * (*this).cofactor(0, 2);
	}


};

class Matrix4 : public MatrixBase
{
public:
	inline Matrix4() : MatrixBase(4, 4) {}

	inline Matrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33
	) : MatrixBase(4, 4)
	{
		matrixElements = {
			 m00, m01, m02, m03,
			 m10, m11, m12, m13,
			 m20, m21, m22, m23,
			 m30, m31, m32, m33 };
	}

	inline bool operator==(const Matrix4& other)  const
	{
		return equals(other);
	}

	inline bool operator!=(const Matrix4& other) const
	{
		return !(*this == other);
	}

	inline Matrix4 operator*(const Matrix4& other)
	{
		Matrix4 M{};
		size_t sideLen = 4;
		for (size_t row{}; row < m; ++row)
			for (size_t col{}; col < n; ++col)
			{
				float result{ 0.0f };
				for (size_t i{}; i < sideLen; ++i)
				{
					result += (*this)(row, i) * other(i, col);
				}
				M(row, col) = result;
			}

		return M;
	}

	inline Matrix4 transpose() const
	{
		return Matrix4
		{
			(*this)(0,0),(*this)(1,0),(*this)(2,0),(*this)(3,0),
			(*this)(0,1),(*this)(1,1),(*this)(2,1),(*this)(3,1),
			(*this)(0,2),(*this)(1,2),(*this)(2,2),(*this)(3,2),
			(*this)(0,3),(*this)(1,3),(*this)(2,3),(*this)(3,3)
		};
	}
	Tuples::Tuple operator*(const Tuples::Tuple& other) const;
	static Matrix4 identity();

	Matrix3 submatrix(size_t row, size_t col) const;
	inline float minor(size_t row, size_t col) const
	{
		return (*this).submatrix(row, col).determinant();
	}

	inline float cofactor(size_t row, size_t col) const
	{
		return ((row + col) % 2 == 0 ? 1.0f : -1.0f) * (*this).minor(row, col);
	}

	inline float determinant() const
	{
		return  (*this)(0, 0) * (*this).cofactor(0, 0) +
				(*this)(0, 1) * (*this).cofactor(0, 1) +
				(*this)(0, 2) * (*this).cofactor(0, 2) +
				(*this)(0, 3) * (*this).cofactor(0, 3);
	}

	Matrix4 inverse() const;
	static Matrix4 translation(float x, float y, float z);
	static Matrix4 scale(float x, float y, float z);
	static Matrix4 rotationX(float radians);
	static Matrix4 rotationY(float radians);
	static Matrix4 rotationZ(float radians);
	static Matrix4 shearing(float xy, float xz, float yx, float yz, float zx, float zy);


};

//inline Tuples::Tuple operator*(const Matrix4& m, const Tuples::Tuple& t)
//{
//	return Tuples::Tuple{
//		(m(0,0) * t.x) + (m(0,1) * t.y) + (m(0,2) * t.z) + (m(0,3) * t.w),
//		(m(1,0) * t.x) + (m(1,1) * t.y) + (m(1,2) * t.z) + (m(1,3) * t.w),
//		(m(2,0) * t.x) + (m(2,1) * t.y) + (m(2,2) * t.z) + (m(2,3) * t.w),
//		(m(3,0) * t.x) + (m(3,1) * t.y) + (m(3,2) * t.z) + (m(3,3) * t.w)
//	};
//}

float radians(float degrees);