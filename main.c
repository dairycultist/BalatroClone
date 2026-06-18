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
	SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 240, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if (!window)
		THROW("Could not create window\n%s\n", SDL_GetError());

	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	glewInit();

	init_renderer();
	glFrontFace(GL_CCW);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// test initialization
	Transform test_transform = { 0, 0, 1, 1, 0, 0, 0 };
	Texture test_texture, test_text;

	load_texture("./test_sprite.png", &test_texture);

	TTF_Font *font = TTF_OpenFont("./font/CreatoDisplay-Regular.otf", 28); // void TTF_CloseFont(TTF_Font *font);

	if (!font)
		THROW("Could not open font \"%s\"\n%s\n", "./font/CreatoDisplay-Regular.otf", TTF_GetError());

	create_texture_from_string(font, "Hello", 255, 255, 255, 255, 0, &test_text);

	// process events until window is closed
	SDL_Event event;
	bool running = TRUE;

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
			}
		}

		// process/draw
		draw_texture(&test_texture, &test_transform);
		test_transform.y += 2.0;
		draw_texture(&test_text, &test_transform);
		test_transform.y -= 2.0;
		test_transform.a_x += 0.01;
		test_transform.a_y += 0.023;
		test_transform.a_z += 0.017;

		SDL_GL_SwapWindow(window);
		SDL_Delay(1000 / 60);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	return 0;
}