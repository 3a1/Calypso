#pragma once

#include <cmath>
#include <numbers>
#include "includes.h"

struct PlayerPosition {
	float screenPosition[2];
};

struct view_matrix_t {
    float* operator[ ](int index) 
	{
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

	Vector3 WTS(view_matrix_t matrix) const
	{
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		float _x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z + matrix[0][3];
		float _y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z + matrix[1][3];

		float w = matrix[3][0] * x + matrix[3][1] * y + matrix[3][2] * z + matrix[3][3];

		if (w < 0.01f) 
			return false;
		

		float inv_w = 1.f / w;
		_x *= inv_w;
		_y *= inv_w;

		float x = screenWidth / 2;
		float y = screenHeight / 2;

		x += 0.5f * _x * screenWidth + 0.5f;
		y -= 0.5f * _y * screenHeight + 0.5f;

		if (y > screenHeight)
			return false;
		if (x > screenWidth)
			return false;
		if (x < 0)
			return false;
		if (y < 0)
			return false;

		return { x, y, w };
	}

	float x, y, z;
};

class Vec2
{
public:
	float x, y;

	Vec2() {};

	Vec2(float x, float y)
	{
		this->x;
		this->y;
	}

	Vec2 operator + (Vec2 v)
	{
		return Vec2(x + v.x, y + v.y);
	}
	Vec2 operator - (Vec2 v)
	{
		return Vec2(x - v.x, y - v.y);
	}
};