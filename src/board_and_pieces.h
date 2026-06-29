#ifndef BOARD_AND_PIECES_H
#define BOARD_AND_PIECES_H

#include "render.h"

#define MAX_PIECE_INSTANCES 8

#define PIECE_TYPE_DNE 0
#define PIECE_TYPE_KING 1

typedef struct {

    int type;
    int hp, atk;
    int board_x, board_y;
    Texture text_texture;

} PieceInstance;

void initialize_board_and_pieces();
void add_piece_player(int piece_type);
void add_piece_opponent(int piece_type);
void draw_board_and_pieces(float t);

#endif