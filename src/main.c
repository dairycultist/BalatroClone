#include "render.h"
#include "error.h"

typedef char bool;
#define TRUE 1
#define FALSE 0

int main() {

	printf("Starting game\n");

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		THROW("Could not initialize SDL\n%s\n", SDL_GetError());

	if (TTF_Init() != 0)
		THROW("Could not initialize SDL TTF\n%s\n", TTF_GetError());

	// init OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// create the window
	int window_w = 400, window_h = window_w / ASPECT;

	SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_w, window_h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if (!window)
		THROW("Could not create window\n%s\n", SDL_GetError());

	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	glewInit();

	init_renderer();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// test initialization
	Transform test_transform = { 0, 0, 10, 10, 0, 0, 0 };
	Texture test_texture, test_text;

	load_texture("./res/test_sprite.png", &test_texture);

	TTF_Font *font = TTF_OpenFont("./res/font/CreatoDisplay-Regular.otf", 28); // void TTF_CloseFont(TTF_Font *font);

	if (!font)
		THROW("Could not open font \"%s\"\n%s\n", "./res/font/CreatoDisplay-Regular.otf", TTF_GetError());

	create_texture_from_string(font, "Hello world!", 255, 255, 255, 255, 0, &test_text);

	// process events until window is closed
	SDL_Event event;
	bool running = TRUE;

	int viewport_w = window_w, viewport_h = window_h, viewport_x = 0, viewport_y = 0;
	float mouse_u = 0, mouse_v = 0;

	while (running) {

		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {

				running = FALSE;

			} else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {

				window_w = event.window.data1;
				window_h = event.window.data2;

				if (ASPECT > event.window.data1 / (float) event.window.data2) {

					viewport_w = event.window.data1;
					viewport_h = event.window.data1 / ASPECT;

					viewport_x = 0;
					viewport_y = (event.window.data2 - viewport_h) / 2;

				} else {

					viewport_w = event.window.data2 * ASPECT;
					viewport_h = event.window.data2;

					viewport_x = (event.window.data1 - viewport_w) / 2;
					viewport_y = 0;
				}

				glViewport(viewport_x, viewport_y, viewport_w, viewport_h);

			} else if (event.type == SDL_MOUSEMOTION) {

				// transform mouse position to be relative to viewport
				mouse_u = event.motion.x - viewport_x;
				mouse_v = event.motion.y - viewport_y;

				mouse_u = mouse_u * 2 - viewport_w;
				mouse_v = mouse_v * 2 - viewport_h;

				mouse_u /= viewport_w;
				mouse_v /= viewport_h;

				printf("%f \t%f\n", mouse_u, mouse_v);
			}
		}

		// process/draw
		draw_texture(&test_texture, &test_transform);

		// test_transform.a_x += 0.01;
		// test_transform.a_y += 0.023;
		// test_transform.a_z += 0.017;

		SDL_GL_SwapWindow(window);
		SDL_Delay(1000 / 60);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	return 0;
}