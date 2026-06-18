#include "render.h"
#include "error.h"

static GLuint sprite_shader;
static GLuint sprite_vao; // vertex array

static void create_texture_from_surface(SDL_Surface *surface, Texture *const out) {

	// create texture object
	GLuint texture;
	glGenTextures(1, &texture);

	// bind texture (to active texture 2D)
	glBindTexture(GL_TEXTURE_2D, texture);

	// wrap clamp
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// write texture data to GL_TEXTURE_2D buffer
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	out->texture = texture;
	out->w = surface->w;
	out->h = surface->h;
}

void load_texture(const char *path, Texture *const out) {

	SDL_Surface *surface = IMG_Load(path);

	if (!surface)
		THROW("Could not load \"%s\"\n%s\n", path, SDL_GetError());

	create_texture_from_surface(surface, out);
	SDL_FreeSurface(surface);
}

void create_texture_from_string(TTF_Font *font, const char *text, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint32 wrapLength, Texture *const out) {

	if (!font)
		THROW("Font cannot be null\n");

	if (!text)
		THROW("Text cannot be null\n");

	SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(font, text, (SDL_Color) { r, g, b, a }, wrapLength);

	if (!surface)
		THROW("Error with TTF_RenderUTF8_Blended_Wrapped\n%s\n", TTF_GetError());

	// fonts aren't loaded in the format we use, so we have to convert it
	SDL_Surface *new_surface = SDL_ConvertSurface(surface, &((SDL_PixelFormat) { 376840196, NULL, 32, 4, {0, 0}, 0xFF, 0xFF00, 0xFF0000, 0xFF000000 }), 8);

	if (!new_surface)
		THROW("Error with SDL_ConvertSurface\n%s\n", SDL_GetError());
	
	create_texture_from_surface(new_surface, out);

	SDL_FreeSurface(surface);
	SDL_FreeSurface(new_surface);
}

void draw_texture(const Texture *texture, const Transform *transform) {

	// scale in world
	GLfloat model_matrix[4][4] = {
		{  transform->s_x * texture->w / 128.0,                                    0, 0, 0 },
		{                                    0,  transform->s_y * texture->h / 128.0, 0, 0 },
		{                                    0,                                    0, 1, 0 },
		{                                    0,                                    0, 0, 1 }
	};

	// rotate
	GLfloat pitch_matrix[4][4];
	GLfloat yaw_matrix[4][4];
	GLfloat roll_matrix[4][4];

	generate_rotation_matrices(
		pitch_matrix, transform->a_x,
		yaw_matrix, transform->a_y,
		roll_matrix, transform->a_z
	);

	mat4_mult(pitch_matrix, model_matrix, model_matrix);
	mat4_mult(yaw_matrix,   model_matrix, model_matrix);
	mat4_mult(roll_matrix,  model_matrix, model_matrix);

	// translate
	model_matrix[3][0] += transform->x;
	model_matrix[3][1] += transform->y;
	model_matrix[3][2] -= 5.0;

	// perspective projection (hardcoded)
	// https://www.songho.ca/opengl/gl_projectionmatrix.html#fov
	static GLfloat proj_matrix[4][4] = {
		{ 1.0 / ASPECT,   0,    0,    0 },
		{            0, 1.0,    0,    0 },
		{            0,   0, -1.0, -1.0 },
		{            0,   0, -0.2,    0 },
	};
	mat4_mult(proj_matrix, model_matrix, model_matrix);

	// bind the mesh and its texture
	glBindVertexArray(sprite_vao);
	glBindTexture(GL_TEXTURE_2D, texture->texture);

	// load the uniform we just calculated
	glUniformMatrix4fv(glGetUniformLocation(sprite_shader, "model_matrix"), 1, GL_FALSE, &model_matrix[0][0]);

	// draw
	glDrawArrays(GL_TRIANGLES, 0, SPRITE_MESH_VERTEX_COUNT);
}

void destroy_texture(Texture *texture) {

	glDeleteTextures(1, &texture->texture);
	texture->texture = 0;
	texture->w = 0;
	texture->h = 0;
}

static inline GLuint load_shader(const char *shadercode, const GLenum shader_type) {

	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shadercode, NULL);
	glCompileShader(shader);

	return shader;
}

void init_renderer() {

	// initialize shader
	sprite_shader = glCreateProgram();
	
	glAttachShader(sprite_shader, load_shader(VERTEX_SHADERCODE, GL_VERTEX_SHADER));
	glAttachShader(sprite_shader, load_shader(FRAGMENT_SHADERCODE, GL_FRAGMENT_SHADER));

	glLinkProgram(sprite_shader);	// apply changes to shader program
	glUseProgram(sprite_shader);	// since this is the only shader, we can just load it right now

	// initialize the sprite mesh's vertex array
	glGenVertexArrays(1, &sprite_vao);
	glBindVertexArray(sprite_vao);

	// make vertex buffer (stored by sprite_vao)
	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);

	// make it the active buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	// copy vertex data into the active buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * SPRITE_MESH_VERTEX_COUNT, SPRITE_MESH_DATA, GL_STATIC_DRAW);

	// link active vertex data and shader attributes
	GLint pos_attrib = glGetAttribLocation(sprite_shader, "position");
	glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glEnableVertexAttribArray(pos_attrib);

	GLint uv_attrib = glGetAttribLocation(sprite_shader, "UV");
	glVertexAttribPointer(uv_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (GLvoid *) (sizeof(float) * 2));
	glEnableVertexAttribArray(uv_attrib);

	// debind vertex array
	glBindVertexArray(0);
}