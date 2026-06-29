#include "process.h"
#include "render.h"
#include "board_and_pieces.h"
#include <math.h>

static Transform card_transform = { 0.5, 0, 1, 1, 0, 0, 0 };
static Texture test_card;
static Texture test_text;

static float t;

void logical_init() {

    initialize_board_and_pieces();

	load_texture("./res/piece_card_pawn.png", &test_card);

    TTF_Font *font = load_font("./res/font/CreatoDisplay-Regular.otf", 20);

	create_texture_from_string(font, "Pawn (1 HP, 1 ATK)\nBecomes a Queen (4 HP, 4 ATK) upon\nreaching the far side of the board", 255, 255, 255, 255, 0, &test_text);

    // testing
    add_piece_player(PIECE_TYPE_KING);
    add_piece_player(PIECE_TYPE_KING);
    add_piece_opponent(PIECE_TYPE_KING);
}

void process(float mouse_u, float mouse_v) {

    process_board_and_pieces(t, mouse_u, mouse_v);

    t += 0.02;

    if (texture_contains_point(mouse_u, mouse_v, &card_transform, &test_card)) {

        card_transform.a_y = 0.0;
        card_transform.a_z = 0.0;

        draw_texture(&test_card, &card_transform);
    
        float off = (texture_effective_height(&card_transform, &test_card) + texture_effective_height(&card_transform, &test_text)) / 2.0 + 0.05;

        card_transform.v -= off;
        draw_texture(&test_text, &card_transform);
        card_transform.v += off;

    } else {

        card_transform.a_y = sin(t) * 0.2;
        card_transform.a_z = sin(t) * 0.05;

        draw_texture(&test_card, &card_transform);
    }
}