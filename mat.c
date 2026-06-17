#include "render.h"

void mat4_mult(const GLfloat b[4][4], const GLfloat a[4][4], GLfloat out[4][4]) {

	// a (rightmost) is applied first, then b

	GLfloat matrix[4][4] = {
		{
			a[0][0] * b[0][0] + a[0][1] * b[1][0] + a[0][2] * b[2][0] + a[0][3] * b[3][0],
			a[0][0] * b[0][1] + a[0][1] * b[1][1] + a[0][2] * b[2][1] + a[0][3] * b[3][1],
			a[0][0] * b[0][2] + a[0][1] * b[1][2] + a[0][2] * b[2][2] + a[0][3] * b[3][2],
			a[0][0] * b[0][3] + a[0][1] * b[1][3] + a[0][2] * b[2][3] + a[0][3] * b[3][3],
		},
		{
			a[1][0] * b[0][0] + a[1][1] * b[1][0] + a[1][2] * b[2][0] + a[1][3] * b[3][0],
			a[1][0] * b[0][1] + a[1][1] * b[1][1] + a[1][2] * b[2][1] + a[1][3] * b[3][1],
			a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2] * b[2][2] + a[1][3] * b[3][2],
			a[1][0] * b[0][3] + a[1][1] * b[1][3] + a[1][2] * b[2][3] + a[1][3] * b[3][3],
		},
		{
			a[2][0] * b[0][0] + a[2][1] * b[1][0] + a[2][2] * b[2][0] + a[2][3] * b[3][0],
			a[2][0] * b[0][1] + a[2][1] * b[1][1] + a[2][2] * b[2][1] + a[2][3] * b[3][1],
			a[2][0] * b[0][2] + a[2][1] * b[1][2] + a[2][2] * b[2][2] + a[2][3] * b[3][2],
			a[2][0] * b[0][3] + a[2][1] * b[1][3] + a[2][2] * b[2][3] + a[2][3] * b[3][3],
		},
		{
			a[3][0] * b[0][0] + a[3][1] * b[1][0] + a[3][2] * b[2][0] + a[3][3] * b[3][0],
			a[3][0] * b[0][1] + a[3][1] * b[1][1] + a[3][2] * b[2][1] + a[3][3] * b[3][1],
			a[3][0] * b[0][2] + a[3][1] * b[1][2] + a[3][2] * b[2][2] + a[3][3] * b[3][2],
			a[3][0] * b[0][3] + a[3][1] * b[1][3] + a[3][2] * b[2][3] + a[3][3] * b[3][3],
		},
	};

	int x, y;

	for (x = 0; x < 4; x++) {
		for (y = 0; y < 4; y++) {
			out[x][y] = matrix[x][y];
		}
	}
}

void inline generate_rotation_matrices(
		GLfloat pitch_matrix[4][4], float pitch,
		GLfloat   yaw_matrix[4][4], float yaw,
		GLfloat  roll_matrix[4][4], float roll
	) {

	float cos_a = cos(pitch);
	float sin_a = sin(pitch);

	pitch_matrix[0][0] = 1;
	pitch_matrix[0][1] = 0;
	pitch_matrix[0][2] = 0;
	pitch_matrix[0][3] = 0;

	pitch_matrix[1][0] = 0;
	pitch_matrix[1][1] = cos_a;
	pitch_matrix[1][2] = -sin_a;
	pitch_matrix[1][3] = 0;

	pitch_matrix[2][0] = 0;
	pitch_matrix[2][1] = sin_a;
	pitch_matrix[2][2] = cos_a;
	pitch_matrix[2][3] = 0;

	pitch_matrix[3][0] = 0;
	pitch_matrix[3][1] = 0;
	pitch_matrix[3][2] = 0;
	pitch_matrix[3][3] = 1;

	cos_a = cos(yaw);
	sin_a = sin(yaw);

	yaw_matrix[0][0] = cos_a;
	yaw_matrix[0][1] = 0;
	yaw_matrix[0][2] = sin_a;
	yaw_matrix[0][3] = 0;

	yaw_matrix[1][0] = 0;
	yaw_matrix[1][1] = 1;
	yaw_matrix[1][2] = 0;
	yaw_matrix[1][3] = 0;

	yaw_matrix[2][0] = -sin_a;
	yaw_matrix[2][1] = 0;
	yaw_matrix[2][2] = cos_a;
	yaw_matrix[2][3] = 0;

	yaw_matrix[3][0] = 0;
	yaw_matrix[3][1] = 0;
	yaw_matrix[3][2] = 0;
	yaw_matrix[3][3] = 1;

	cos_a = cos(roll);
	sin_a = sin(roll);

	roll_matrix[0][0] = cos_a;
	roll_matrix[0][1] = -sin_a;
	roll_matrix[0][2] = 0;
	roll_matrix[0][3] = 0;

	roll_matrix[1][0] = sin_a;
	roll_matrix[1][1] = cos_a;
	roll_matrix[1][2] = 0;
	roll_matrix[1][3] = 0;

	roll_matrix[2][0] = 0;
	roll_matrix[2][1] = 0;
	roll_matrix[2][2] = 1;
	roll_matrix[2][3] = 0;

	roll_matrix[3][0] = 0;
	roll_matrix[3][1] = 0;
	roll_matrix[3][2] = 0;
	roll_matrix[3][3] = 1;
}