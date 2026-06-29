#ifndef RENDER_H
#define RENDER_H

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// free aspect ratio is outside of scope (involves updating UI meshes and their positions, no thanks)
#define ASPECT 1.666

// how much to scale each texture, I mean it's pretty self explanatory
#define SCALE 0.005

// how must perspective-ness should happen during texture rotation
#define DEPTH 0.5

#define VERTEX_SHADERCODE "#version 150 core\n"\
	"uniform mat4 model_matrix;"\
	"in vec2 position;"\
	"in vec2 UV;"\
	"out vec2 frag_UV;"\
	"void main() {"\
		"gl_Position = model_matrix * vec4(position.xy, 0.0, 1.0);"\
		"frag_UV = UV;"\
	"}"

#define FRAGMENT_SHADERCODE "#version 150 core\n"\
	"uniform sampler2D tex;"\
	"in vec2 frag_UV;"\
	"out vec4 outColor;"\
	"void main() {"\
		"outColor = texture(tex, frag_UV);"\
	"}"

// hardcoded unit square (every texture is drawn with the same mesh but transformed)
// technically the UVs are flipped vertically because the SDL_Surface spec is different
// from the OpenGL spec and I don't feel like bothering converting from one to another
#define TEXTURE_MESH_DATA (float[]) { \
	 1,  1, 1, 0,\
	-1, -1, 0, 1,\
	-1,  1, 0, 0,\
	 1,  1, 1, 0,\
	 1, -1, 1, 1,\
	-1, -1, 0, 1\
}
#define TEXTURE_MESH_VERTEX_COUNT 6

typedef struct {

	float u; // center in window-uv space
	float v;
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
TTF_Font *load_font(const char *path, int pt_size);
void create_texture_from_string(TTF_Font *font, const char *text, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint32 wrapLength, Texture *const out);
void draw_texture(const Texture *texture, const Transform *transform);
void destroy_texture(Texture *texture);
void destroy_font(TTF_Font *font);

int aabb_contains_point(float u, float v, float aabb_u, float aabb_v, float aabb_wd2, float aabb_hd2); // aabb width/height divided by 2
int texture_contains_point(float u, float v, Transform *transform, Texture *texture); // doesn't take into account rotation, yes scale tho

float texture_effective_width(Transform *transform, Texture *texture);
float texture_effective_height(Transform *transform, Texture *texture);

void init_renderer();

void mat4_mult(const GLfloat b[4][4], const GLfloat a[4][4], GLfloat out[4][4]);
void generate_rotation_matrices(GLfloat pitch_matrix[4][4], float pitch, GLfloat yaw_matrix[4][4], float yaw, GLfloat roll_matrix[4][4], float roll);

#endif