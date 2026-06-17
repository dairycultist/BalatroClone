#include "render.h"

static GLuint sprite_shader;

// hardcoded unit square (every sprite is drawn with the same mesh but transformed)
static const float data[] = {
	 1,  1, 1, 1,
	-1, -1, 0, 0,
	-1,  1, 0, 1,
	 1,  1, 1, 1,
	 1, -1, 1, 0,
	-1, -1, 0, 0
};
static const uint vertex_count = 6;
static GLuint vertex_array; // "VAO"

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

	printf("%d\n", pixel_bytes);

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

	GLfloat position_matrix[4][4]; // rename to model matrix once we care

	// TODO scale

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
	memset(position_matrix, 0, sizeof(GLfloat) * 16);
	position_matrix[0][0] = 1;
	position_matrix[1][1] = 1;
	position_matrix[2][2] = 1;
	position_matrix[3][3] = 1;

	position_matrix[3][0] = transform->x;
	position_matrix[3][1] = transform->y;

	// bind the mesh and its texture
	glBindVertexArray(vertex_array);
	glBindTexture(GL_TEXTURE_2D, texture->texture);

	// load the uniform we just calculated
	glUniformMatrix4fv(glGetUniformLocation(sprite_shader, "position_matrix"), 1, GL_FALSE, &position_matrix[0][0]);

	// draw
	glDrawArrays(GL_TRIANGLES, 0, vertex_count);
}

static GLuint load_shader(const char *shadercode, GLenum shader_type) {

	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shadercode, NULL);
	glCompileShader(shader);

	return shader;
}

void init_renderer() {

	// initialize shader
	sprite_shader = glCreateProgram();
	
	glAttachShader(sprite_shader, load_shader(
		"#version 150 core\n"
		"uniform mat4 position_matrix;"
		"in vec2 position;"
		"in vec2 UV;"
		"out vec2 frag_UV;"
		"void main() {"
			"gl_Position = position_matrix * vec4(position.xy, 0.0, 1.0);" // get final position
			"frag_UV = UV;" // pass along UV
		"}"
	, GL_VERTEX_SHADER));

	glAttachShader(sprite_shader, load_shader(
		"#version 150 core\n"
		"uniform sampler2D tex;"
		"in vec2 frag_UV;"
		"out vec4 outColor;"
		"void main() {"
			"outColor = texture(tex, frag_UV);"
			//"if (outColor == vec4(0, 0, 0, 1)) { discard; }" // texture clip transparency (discard any fully black pixels in unshaded)
		"}"
	, GL_FRAGMENT_SHADER));

	glLinkProgram(sprite_shader);	// apply changes to shader program
	glUseProgram(sprite_shader);	// since this is the only shader, we can just load it right now

	// now create the sprite mesh

	// initialize vertex array
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	// make vertex buffer (stored by vertex_array)
	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);											// make it the active buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * vertex_count, data, GL_STATIC_DRAW);	// copy vertex data into the active buffer

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