#include "render.h"

typedef char bool;
#define TRUE 1
#define FALSE 0

int main() {

	printf("Starting game\n");

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	// init OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// create the window
	SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 240, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if (!window) {
		fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
		return 1;
    }

	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	glewInit();

	init_renderer();
	glFrontFace(GL_CCW);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	Transform test_transform = { 0, 0, 1, 1, 0, 0, 0 };
	Texture test_texture;
	load_texture("./test_sprite.png", &test_texture);

	// process events until window is closed
	SDL_Event event;
	bool running = TRUE;
	bool use_original_res = TRUE;

	bool up       = FALSE;
	bool down     = FALSE;
	bool left     = FALSE;
	bool right    = FALSE;
	bool action_1 = FALSE;
	bool action_2 = FALSE;

	int window_w = 400, window_h = 240, window_x = 0, window_y = 0;

	while (running) {

		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {

				running = FALSE;

			} else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {

				if (ASPECT > event.window.data1 / (float) event.window.data2) {

					window_w = event.window.data1;
					window_h = event.window.data1 / ASPECT;

					window_x = 0;
					window_y = (event.window.data2 - window_h) / 2;

				} else {

					window_w = event.window.data2 * ASPECT;
					window_h = event.window.data2;

					window_x = (event.window.data1 - window_w) / 2;
					window_y = 0;
				}

				glViewport(window_x, window_y, window_w, window_h);

			} else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {

				if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
					left = TRUE;
				} else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
					right = TRUE;
				} else if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
					up = TRUE;
				} else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
					down = TRUE;
				} else if (event.key.keysym.scancode == SDL_SCANCODE_Z) {
					action_1 = TRUE;
				} else if (event.key.keysym.scancode == SDL_SCANCODE_X) {
					action_2 = TRUE;
				} else if (event.key.keysym.scancode == SDL_SCANCODE_O) {
					use_original_res = !use_original_res;
				}

			} else if (event.type == SDL_KEYUP) {

				if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
					left = FALSE;
				} else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
					right = FALSE;
				} else if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
					up = FALSE;
				} else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
					down = FALSE;
				} else if (event.key.keysym.scancode == SDL_SCANCODE_Z) {
					action_1 = FALSE;
				} else if (event.key.keysym.scancode == SDL_SCANCODE_X) {
					action_2 = FALSE;
				}
			}
		}

		// process/draw
		draw_texture(&test_texture, &test_transform);
		test_transform.a_x += 0.01;
		test_transform.a_y += 0.023;

		SDL_GL_SwapWindow(window);
		SDL_Delay(1000 / 60);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	return 0;
}