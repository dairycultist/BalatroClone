#include "render.h"

static GLuint sprite_shader;
static GLuint sprite_vao; // vertex array

void load_texture(const char *path, Texture *const out) {

	SDL_Surface *surface = IMG_Load(path);

	// flip surface vertically to match OpenGL spec
	unsigned char *pixels = (unsigned char *) surface->pixels;
	int pixel_bytes = surface->format->BytesPerPixel;

	for (int y = 0; y < surface->h / 2; y++) {

		for (int x = 0; x < surface->w; x++) {

			int top = x + y * surface->h;
			int bottom = x + (surface->w * surface->h - (y + 1) * surface->h);

			for (int i=0; i<pixel_bytes; i++) {

				unsigned char hold = pixels[top * pixel_bytes + i];
				pixels[top * pixel_bytes + i] = pixels[bottom * pixel_bytes + i];
				pixels[bottom * pixel_bytes + i] = hold;
			}
		}
	}

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	out->texture = texture;
	out->w = surface->w;
	out->h = surface->h;
}

void draw_texture(const Texture *texture, const Transform *transform) {

	GLfloat position_matrix[4][4] = { // TODO rename to model matrix once we care
		{ transform->s_x / ASPECT,              0, 0, 0 },
		{                       0, transform->s_y, 0, 0 },
		{                       0,              0, 1, 0 },
		{                       0,              0, 0, 1 }
	};

	// scale

	// TODO rotate
	// GLfloat pitch_matrix[4][4];
	// GLfloat yaw_matrix[4][4];

	// // model matrix (converts from model space to world space)
	// generate_rotation_matrices(
	// 	pitch_matrix, transform->pitch,
	// 	yaw_matrix, transform->yaw
	// );

	// GLfloat model_matrix[4][4];

	// mat4_mult(yaw_matrix, pitch_matrix, model_matrix); // rotation

	// model_matrix[3][0] = transform->x; // translation
	// model_matrix[3][1] = transform->y;
	// model_matrix[3][2] = transform->z;

	// // final position matrix (proj_matrix * view_matrix * model_matrix)
	// mat4_mult(proj_matrix, model_matrix, position_matrix);

	// translate
	position_matrix[3][0] += transform->x;
	position_matrix[3][1] += transform->y;

	// bind the mesh and its texture
	glBindVertexArray(sprite_vao);
	glBindTexture(GL_TEXTURE_2D, texture->texture);

	// load the uniform we just calculated
	glUniformMatrix4fv(glGetUniformLocation(sprite_shader, "position_matrix"), 1, GL_FALSE, &position_matrix[0][0]);

	// draw
	glDrawArrays(GL_TRIANGLES, 0, SPRITE_MESH_VERTEX_COUNT);
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