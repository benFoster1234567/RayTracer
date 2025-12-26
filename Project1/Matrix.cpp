#include "Matrix.h"
#include <numbers>

bool MatrixBase::equals(const MatrixBase& other, float epsilon) const
{
	if (m != other.m || n != other.n)
	{
		return false;
	}

	for (size_t i{}; i < matrixElements.size(); ++i)
	{
		if (std::abs(matrixElements[i] - other.matrixElements[i]) > epsilon)
		{
			return false;
		}
	}

	return true;
}

void MatrixBase::multiply(const MatrixBase& other, MatrixBase& result, size_t matrixSideLen)
{
	for (size_t row{}; row < m; ++row)
		for (size_t col{}; col < n; ++col)
		{
			float results{ 0.0f };
			for (size_t i{}; i < matrixSideLen; ++i)
			{
				results += (*this)(row, i) * other(i, col);
			}
			result(row, col) = results;
		}
}

Tuples::Tuple Matrix4::operator*(const Tuples::Tuple& other) const
{
	Tuples::Tuple result{ 0, 0, 0, 0 };

	result.x =
		(*this)(0, 0) * other.x +
		(*this)(0, 1) * other.y +
		(*this)(0, 2) * other.z +
		(*this)(0, 3) * other.w;

	result.y =
		(*this)(1, 0) * other.x +
		(*this)(1, 1) * other.y +
		(*this)(1, 2) * other.z +
		(*this)(1, 3) * other.w;

	result.z =
		(*this)(2, 0) * other.x +
		(*this)(2, 1) * other.y +
		(*this)(2, 2) * other.z +
		(*this)(2, 3) * other.w;

	result.w =
		(*this)(3, 0) * other.x +
		(*this)(3, 1) * other.y +
		(*this)(3, 2) * other.z +
		(*this)(3, 3) * other.w;

	return result;
}

Matrix4 Matrix4::inverse() const
{
	Matrix4 M2{};
	if (!this->isInvertable())
	{
		return M2;
	}

	for (size_t row{}; row < M2.m; row++)
		for (size_t col{}; col < M2.n; col++)
		{
			float c = (*this).cofactor(row, col);
			M2(col, row) = c / (*this).determinant();
		}

	return M2;
}

Matrix4 Matrix4::translation(float x, float y, float z)
{
	return Matrix4
	{
		1.0f,0.0f,0.0f,x,
		0.0f,1.0f,0.0f,y,
		0.0f,0.0f,1.0f,z,
		0.0f,0.0f,0.0f,1.0f
	};
}

Matrix4 Matrix4::scale(float x, float y, float z)
{
	return Matrix4
	{
		x,0.0f,0.0f,0.0f,
		0.0f,y,0.0f,0.0f,
		0.0f,0.0f,z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
}

Matrix4 Matrix4::rotationX(float radians)
{

	return Matrix4
	(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cos(radians),-sin(radians), 0.0f,
		0.0f,sin(radians),cos(radians), 0.0f,
		0.0f,0.0f,0.0f,1.0f
	);
}

Matrix4 Matrix4::rotationY(float radians)
{
	return Matrix4
	(
		cos(radians),  0.0f, sin(radians), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sin(radians), 0.0f,  cos(radians), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4 Matrix4::rotationZ(float radians)
{
	return Matrix4
	(
		cos(radians), -sin(radians), 0.0f, 0.0f,
		sin(radians), cos(radians), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4 Matrix4::shearing(float xy, float xz, float yx, float yz, float zx, float zy)
{
	return Matrix4
	{
		1.0f,xy,xz,0.0f,
		yx,1.0f,yz,0.0f,
		zx,zy,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
}

Matrix4 Matrix4::identity()
{
	return Matrix4
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
}

Matrix2 Matrix3::submatrix(size_t row, size_t col) const
{
	Matrix2 result{};

	for (int row3{}, row2{}; row3 < 3 && row2 < 2; row3++)
	{
		if (row3 != row) {
			for (int col3{}, col2{}; col3 < 3 && col2 < 2; col3++)
			{
				if (col3 != col) {
					result(row2, col2) = (*this)(row3, col3);
					col2++;
				}
			}
			row2++;
		}
	}

	return result;
}

Matrix3 Matrix4::submatrix(size_t row, size_t col) const
{
	Matrix3 result{};
	for (size_t row4{}, row3{}; row4 < 4 && row3 < 3; row4++)
	{
		if (row4 != row) {
			for (size_t col4{}, col3{}; col4 < 4 && col3 < 3; col4++)
			{
				if (col4 != col) {
					result(row3, col3) = (*this)(row4, col4);
					col3++;
				}
			}
			row3++;
		}
	}

	return result;
}

//helper function for converting degrees to radians.
float radians(float degrees)
{
	return degrees * static_cast<float>(std::numbers::pi) / 180.0f;
}
