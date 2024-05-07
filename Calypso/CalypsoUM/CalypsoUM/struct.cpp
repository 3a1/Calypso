#include "struct.h"

bool world_to_screen(const vector2& screen_size, const vector3& pos, vector2& out, view_matrix_t matrix) {
	out.x = matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3];
	out.y = matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3];

	float w = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3];

	if (w < 0.01f)
		return false;

	float inv_w = 1.f / w;
	out.x *= inv_w;
	out.y *= inv_w;

	float x = screen_size.x * .5f;
	float y = screen_size.y * .5f;

	x += 0.5f * out.x * screen_size.x + 0.5f;
	y -= 0.5f * out.y * screen_size.y + 0.5f;

	out.x = x;
	out.y = y;

	return true;
}