#pragma once

#include <cmath>
#include <numbers>
#include "includes.h"

struct view_matrix_t {
    float* operator[ ](int index) {
        return matrix[index];
    }

    float matrix[4][4];
};


struct Vector3
{
	Vector3(
		const float x = 0.f,
		const float y = 0.f,
		const float z = 0.f) noexcept :
		x(x), y(y), z(z) {};


	const Vector3 operator-(const Vector3& v) const noexcept
	{
		return Vector3{ x - v.x, y - v.y, z - v.z };
	}
	const Vector3 operator+(const Vector3& v) const noexcept
	{
		return Vector3{ x + v.x, y + v.y, z + v.z };
	}
	const Vector3 operator/(const float v) const noexcept
	{
		return Vector3{ x / v, y / v, z / v };
	}

	const Vector3 operator*(const float v) const noexcept
	{
		return Vector3{ x * v, y * v, z * v };
	}


	const bool isZero() const noexcept
	{
		return x == 0.f && y == 0.f && z == 0.f;
	}

	float x, y, z;
};