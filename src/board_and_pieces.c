#include "board_and_pieces.h"
#include <math.h>

typedef struct {

    int base_hp, base_atk;
    Texture texture_player, texture_opponent;

} PieceType;

typedef struct {

    PieceType *type;
    int hp, atk;
    int board_x, board_y;
    Texture text_texture;

} PieceInstance;

static PieceType piece_types[] = {
    [ PIECE_TYPE_KING ] = { 1, 0 }
};

static PieceInstance pieces_player[MAX_PIECE_INSTANCES]; // if .type is NULL, that instance does not exist
static PieceInstance pieces_opponent[MAX_PIECE_INSTANCES];

static PieceInstance *selected_piece;

static Transform board_transform = { 0, 0, 1, 1, 0, 0, 0 };
static Texture board_texture;
static float tile_width;
static float tile_height;

static Texture move_pos_textures[2];

static TTF_Font *font;

void initialize_board_and_pieces() {

    load_texture("./res/piece_king_player.png", &piece_types[PIECE_TYPE_KING].texture_player);
    load_texture("./res/piece_king_opponent.png", &piece_types[PIECE_TYPE_KING].texture_opponent);

    load_texture("./res/move_position.png", &move_pos_textures[0]);
    load_texture("./res/move_position_hovered.png", &move_pos_textures[1]);

    load_texture("./res/board.png", &board_texture);
    tile_width = texture_effective_width(&board_transform, &board_texture) / 5.0;
    tile_height = texture_effective_height(&board_transform, &board_texture) / 5.0;

    font = load_font("./res/font/gothica/bold.ttf", 24);
}

static void refresh_piece_instance_text(PieceInstance *instance, int is_opponent) {

    char string[24];

    snprintf(string, sizeof(string), "%d HP\n%d ATK", instance->hp, instance->atk);
    if (is_opponent)
        create_texture_from_string(font, string, 255, 255, 255, 255, 0, &instance->text_texture);
    else
        create_texture_from_string(font, string, 0, 0, 0, 255, 0, &instance->text_texture);
}

void add_piece_player(int piece_type) {

    for (int i = 0; i < MAX_PIECE_INSTANCES; i++) {
        
        if (pieces_player[i].type == NULL) {

            pieces_player[i].type = &piece_types[piece_type];
            pieces_player[i].hp = pieces_player[i].type->base_hp;
            pieces_player[i].atk = pieces_player[i].type->base_atk;
            pieces_player[i].board_x = 4 - i % 5;
            pieces_player[i].board_y = i / 5;

            refresh_piece_instance_text(&pieces_player[i], 0);

            return;
        }
    }
}

void add_piece_opponent(int piece_type) {

    for (int i = 0; i < MAX_PIECE_INSTANCES; i++) {
        
        if (!pieces_opponent[i].type) {

            pieces_opponent[i].type = &piece_types[piece_type];
            pieces_opponent[i].hp = pieces_opponent[i].type->base_hp;
            pieces_opponent[i].atk = pieces_opponent[i].type->base_atk;
            pieces_opponent[i].board_x = i % 5;
            pieces_opponent[i].board_y = 4 - i / 5;
            
            refresh_piece_instance_text(&pieces_opponent[i], 1);

            return;
        }
    }
}

static int is_piece_player_occupying(int board_x, int board_y) {

    for (int i = 0; i < MAX_PIECE_INSTANCES; i++) {
        
        if (pieces_player[i].type != NULL && pieces_player[i].board_x == board_x && pieces_player[i].board_y == board_y)
            return 1;
    }

    return 0;
}

static void get_piece_legal_moves(PieceInstance *instance, int *up, int *down, int *left, int *right) {

    *up = instance->board_y != 4 && !is_piece_player_occupying(instance->board_x, instance->board_y + 1);
    *down = instance->board_y != 0 && !is_piece_player_occupying(instance->board_x, instance->board_y - 1);
    *left = instance->board_x != 0 && !is_piece_player_occupying(instance->board_x - 1, instance->board_y);
    *right = instance->board_x != 4 && !is_piece_player_occupying(instance->board_x + 1, instance->board_y);
}

void process_board_and_pieces(float t, float mouse_u, float mouse_v, float mouse_clicked) {

    draw_texture(&board_texture, &board_transform);

    Transform piece_transform = { 0, 0, 1, 1, 0, 0, 0 };

    for (int i = 0; i < MAX_PIECE_INSTANCES; i++) {
        
        // process player piece (if it exists)
        if (pieces_player[i].type) {

            piece_transform.u = tile_width  * (pieces_player[i].board_x - 2);
            piece_transform.v = tile_height * (pieces_player[i].board_y - 2);

            // selected piece is lifted a bit
            if (&pieces_player[i] == selected_piece)
                piece_transform.v += tile_height / 4.0;

            // logic for hovering/selecting a piece
            if (texture_contains_point(mouse_u, mouse_v, &piece_transform, &pieces_player[i].type->texture_player)) {

                piece_transform.a_z = sin(t * 5.0) * 0.1;

                if (mouse_clicked) {

                    if (&pieces_player[i] == selected_piece) {

                        // deselect this piece
                        selected_piece = NULL;

                    } else {

                        // select this piece
                        selected_piece = &pieces_player[i];

                        // shoddy attempt at animation
                        piece_transform.v += tile_height / 8.0;
                    }
                }
            }

            // draw the piece and its stats
            draw_texture(&pieces_player[i].type->texture_player, &piece_transform);
            piece_transform.a_z = 0.0;
            draw_texture(&pieces_player[i].text_texture, &piece_transform);
        }

        // process opponent piece (if it exists)
        if (pieces_opponent[i].type) {

            piece_transform.u = tile_width  * (pieces_opponent[i].board_x - 2);
            piece_transform.v = tile_height * (pieces_opponent[i].board_y - 2);

            draw_texture(&pieces_opponent[i].type->texture_opponent, &piece_transform);
            draw_texture(&pieces_opponent[i].text_texture, &piece_transform);
        }
    }

    // draw legal move spots for the selected piece
    if (selected_piece) {

        piece_transform.u = tile_width  * (selected_piece->board_x - 2);
        piece_transform.v = tile_height * (selected_piece->board_y - 2);
        
        int up, down, left, right;
        get_piece_legal_moves(selected_piece, &up, &down, &left, &right);

        if (up) {
            piece_transform.v += tile_height;
            if (texture_contains_point(mouse_u, mouse_v, &piece_transform, &move_pos_textures[0])) {
                draw_texture(&move_pos_textures[1], &piece_transform);
                if (mouse_clicked) {
                    selected_piece->board_y++;
                    selected_piece = NULL;
                }
            } else {
                draw_texture(&move_pos_textures[0], &piece_transform);
            }
            piece_transform.v -= tile_height;
        }

        if (down) {
            piece_transform.v -= tile_height;
            if (texture_contains_point(mouse_u, mouse_v, &piece_transform, &move_pos_textures[0])) {
                draw_texture(&move_pos_textures[1], &piece_transform);
                if (mouse_clicked) {
                    selected_piece->board_y--;
                    selected_piece = NULL;
                }
            } else {
                draw_texture(&move_pos_textures[0], &piece_transform);
            }
            piece_transform.v += tile_height;
        }

        if (left) {
            piece_transform.u -= tile_width;
            if (texture_contains_point(mouse_u, mouse_v, &piece_transform, &move_pos_textures[0])) {
                draw_texture(&move_pos_textures[1], &piece_transform);
                if (mouse_clicked) {
                    selected_piece->board_x--;
                    selected_piece = NULL;
                }
            } else {
                draw_texture(&move_pos_textures[0], &piece_transform);
            }
            piece_transform.u += tile_width;
        }

        if (right) {
            piece_transform.u += tile_width;
            if (texture_contains_point(mouse_u, mouse_v, &piece_transform, &move_pos_textures[0])) {
                draw_texture(&move_pos_textures[1], &piece_transform);
                if (mouse_clicked) {
                    selected_piece->board_x++;
                    selected_piece = NULL;
                }
            } else {
                draw_texture(&move_pos_textures[0], &piece_transform);
            }
            piece_transform.u -= tile_width;
        }
    }
}