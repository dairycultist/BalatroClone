#ifndef RENDER_H
#define RENDER_H

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// free aspect ratio is outside of scope (involves updating UI meshes and their positions, no thanks)
#define ASPECT 1.666

#define VERTEX_SHADERCODE "#version 150 core\n"\
	"uniform mat4 position_matrix;"\
	"in vec2 position;"\
	"in vec2 UV;"\
	"out vec2 frag_UV;"\
	"void main() {"\
		"gl_Position = position_matrix * vec4(position.xy, 0.0, 1.0);"\
		"frag_UV = UV;"\
	"}"

#define FRAGMENT_SHADERCODE "#version 150 core\n"\
	"uniform sampler2D tex;"\
	"in vec2 frag_UV;"\
	"out vec4 outColor;"\
	"void main() {"\
		"outColor = texture(tex, frag_UV);"\
	"}"

// hardcoded unit square (every sprite is drawn with the same mesh but transformed)
#define SPRITE_MESH_DATA (float[]) { \
	 1,  1, 1, 1,\
	-1, -1, 0, 0,\
	-1,  1, 0, 1,\
	 1,  1, 1, 1,\
	 1, -1, 1, 0,\
	-1, -1, 0, 0\
}
#define SPRITE_MESH_VERTEX_COUNT 6

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