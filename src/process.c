#include "process.h"
#include "render.h"
#include <math.h>

static Transform test_transform = { 0, 0, 1, 1, 0, 0, 0 };
static Texture card_water, card_fire, card_air, card_earth;
static Texture test_text1, test_text2;

static float t;

void logical_init() {

	load_texture("./res/card_water.png", &card_water);
    load_texture("./res/card_fire.png", &card_fire);
    load_texture("./res/card_air.png", &card_air);
    load_texture("./res/card_earth.png", &card_earth);

    TTF_Font *font = load_font("./res/font/CreatoDisplay-Regular.otf", 20);

	create_texture_from_string(font, "Deal 2 DMG", 255, 255, 255, 255, 0, &test_text1);
    create_texture_from_string(font, "Heal 1 HP", 255, 255, 255, 255, 0, &test_text2);
}

void process(float mouse_u, float mouse_v) {

    test_transform.a_y = sin(t) * 0.1;
    test_transform.a_z = sin(t) * 0.025;

    t += 0.02;

    test_transform.x = -0.5;
    test_transform.y = 0.5;
    draw_texture(&card_water, &test_transform);
    draw_texture(&test_text1, &test_transform);

    test_transform.x = 0.5;
    test_transform.y = 0.5;
    draw_texture(&card_fire, &test_transform);
    draw_texture(&test_text1, &test_transform);

    test_transform.x = -0.5;
    test_transform.y = -0.5;
    draw_texture(&card_air, &test_transform);
    draw_texture(&test_text2, &test_transform);

    test_transform.x = 0.5;
    test_transform.y = -0.5;
    draw_texture(&card_earth, &test_transform);
    draw_texture(&test_text2, &test_transform);
}