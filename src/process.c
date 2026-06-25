#include "process.h"
#include "render.h"
#include <math.h>

static Transform card_transform_idle = { 0, 0, 1, 1, 0, 0, 0 };
static Transform card_transform_hover = { 0, 0, 1, 1, 0, 0, 0 };
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

    card_transform_idle.a_y = sin(t) * 0.2;
    card_transform_idle.a_z = sin(t) * 0.05;

    t += 0.02;

    card_transform_idle.u = -0.5;
    card_transform_idle.v = 0.5;
    card_transform_hover.u = -0.5;
    card_transform_hover.v = 0.5;
    draw_texture(&card_water, aabb_contains(mouse_u, mouse_v, &card_transform_idle, &card_water) ? &card_transform_hover : &card_transform_idle);
    draw_texture(&test_text1, aabb_contains(mouse_u, mouse_v, &card_transform_idle, &card_water) ? &card_transform_hover : &card_transform_idle);

    card_transform_idle.u = 0.5;
    card_transform_idle.v = 0.5;
    card_transform_hover.u = 0.5;
    card_transform_hover.v = 0.5;
    draw_texture(&card_fire, aabb_contains(mouse_u, mouse_v, &card_transform_idle, &card_fire) ? &card_transform_hover : &card_transform_idle);
    draw_texture(&test_text1, aabb_contains(mouse_u, mouse_v, &card_transform_idle, &card_fire) ? &card_transform_hover : &card_transform_idle);

    card_transform_idle.u = -0.5;
    card_transform_idle.v = -0.5;
    card_transform_hover.u = -0.5;
    card_transform_hover.v = -0.5;
    draw_texture(&card_air, aabb_contains(mouse_u, mouse_v, &card_transform_idle, &card_air) ? &card_transform_hover : &card_transform_idle);
    draw_texture(&test_text2, aabb_contains(mouse_u, mouse_v, &card_transform_idle, &card_air) ? &card_transform_hover : &card_transform_idle);

    card_transform_idle.u = 0.5;
    card_transform_idle.v = -0.5;
    card_transform_hover.u = 0.5;
    card_transform_hover.v = -0.5;
    draw_texture(&card_earth, aabb_contains(mouse_u, mouse_v, &card_transform_idle, &card_earth) ? &card_transform_hover : &card_transform_idle);
    draw_texture(&test_text2, aabb_contains(mouse_u, mouse_v, &card_transform_idle, &card_earth) ? &card_transform_hover : &card_transform_idle);
}