#ifndef BOARD_AND_PIECES_H
#define BOARD_AND_PIECES_H

#include "render.h"

#define MAX_PIECE_INSTANCES 8

#define PIECE_TYPE_KING 0

void initialize_board_and_pieces();
void add_piece_player(int piece_type); // probably temp; replace with start_round_board_and_pieces() that polls cards.c for starting pieces
void add_piece_opponent(int piece_type);
void process_board_and_pieces(float t, float mouse_u, float mouse_v, float mouse_clicked);

#endif