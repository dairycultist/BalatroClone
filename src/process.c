#include "process.h"
#include "render.h"
#include <math.h>

static Transform card_transform_idle = { 0, 0, 1, 1, 0, 0, 0 };
static Transform card_transform_hover = { 0, 0, 1, 1, 0, 0, 0 };
static Texture test_card;
static Texture test_text;

static Transform board_transform = { 0, 0, 1, 1, 0, 0, 0 };
static Texture board_texture;

static float t;

void logical_init() {

    load_texture("./res/board.png", &board_texture);
	load_texture("./res/piece_card_pawn.png", &test_card);

    TTF_Font *font = load_font("./res/font/CreatoDisplay-Regular.otf", 20);

	create_texture_from_string(font, "Pawn\n1 HP, 1 ATK", 255, 255, 255, 255, 0, &test_text);
}

void process(float mouse_u, float mouse_v) {

    draw_texture(&board_texture, &board_transform);

    card_transform_idle.a_y = sin(t) * 0.2;
    card_transform_idle.a_z = sin(t) * 0.05;

    t += 0.02;

    if (texture_contains_point(mouse_u, mouse_v, &card_transform_idle, &test_card)) {

        draw_texture(&test_card, &card_transform_hover);
    
        card_transform_hover.v -= 0.5;
        draw_texture(&test_text, &card_transform_hover);
        card_transform_hover.v += 0.5;

    } else {

        draw_texture(&test_card, &card_transform_idle);
    }
}