#pragma once

#include "includes.h"

struct vector2
{
    float x, y;
};

struct vector3
{
	vector3(
		const float x = 0.f,
		const float y = 0.f,
		const float z = 0.f) noexcept :
		x(x), y(y), z(z) {};


	const vector3 operator-(const vector3& v) const noexcept
	{
		return vector3{ x - v.x, y - v.y, z - v.z };
	}
	const vector3 operator+(const vector3& v) const noexcept
	{
		return vector3{ x + v.x, y + v.y, z + v.z };
	}
	const vector3 operator/(const float v) const noexcept
	{
		return vector3{ x / v, y / v, z / v };
	}

	const vector3 operator*(const float v) const noexcept
	{
		return vector3{ x * v, y * v, z * v };
	}


	const bool isZero() const noexcept
	{
		return x == 0.f && y == 0.f && z == 0.f;
	}

	float x, y, z;
};

struct view_matrix_t 
{
	float* operator[ ](int index)
	{
		return matrix[index];
	}

	float matrix[4][4];
};

bool world_to_screen(const vector2& screen_size, const vector3& pos, vector2& out, view_matrix_t matrix);