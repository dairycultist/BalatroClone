#ifndef RENDER_H
#define RENDER_H

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct {

	float x;
	float y;
    float s_x; // scale
    float s_y;
	float a_x; // pitch
	float a_y; // yaw
	float a_z; // roll

} Transform;

typedef struct {

	GLuint texture;
	float w, h;

} Texture;

void load_texture(const char *path, Texture *const out);
void draw_texture(const Texture *texture, const Transform *transform);
void init_renderer();

void mat4_mult(const GLfloat b[4][4], const GLfloat a[4][4], GLfloat out[4][4]);
void generate_rotation_matrices(GLfloat pitch_matrix[4][4], float pitch, GLfloat yaw_matrix[4][4], float yaw);

#endif